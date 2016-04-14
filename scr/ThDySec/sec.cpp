/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*  https://www.fli.de/en/institutes/institut-fuer-neue-und-neuartige-tierseuchenerreger/wissenschaftlerinnen/prof-dr-m-h-groschup/
*  distributed under the GNU General Public License, see <http://www.gnu.org/licenses/>.
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDySec\sec.cpp
*
* @brief Implement fundamental sequence classes
*
*/

#ifdef WINDOWS_FORM_GUI
#include "stdafx.h"
#pragma unmanaged
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <math.h>
#include <list>
#include <stack>

using namespace std ; 

#include "ThDySec/sec_mult.h"
#include "ThDySec/common.h" 
using namespace DegCod;

CSec::CSec ( long l, std::shared_ptr<CSaltCorrNN> NNpar) 		
			:	CSecBasInfo ( l ) ,			
				_NNpar(NNpar)
{	
			_b  .reserve(l+3);
			_SdS.reserve(l+3);
			_SdH.reserve(l+3);

};  

bool		CSec::Selected(		) const //< User-editable  ??
{
	return CSecBasInfo::Selected() && (_parentMS ? _parentMS->Selected(): true)  ;
}					 

/// Fundamental class to manipulate sec.
/// Some variables have index base [1] while others have [0] in sec. 
CSec::CSec (    const std::string&  sec, 
                int                 id, 
                const std::string&  nam, 
                std::shared_ptr<CSaltCorrNN> NNpar, 
                LonSecPos           lmax,   // lamx - limita la cant de bases originales a leer despues de las primeras secBeg-1 bases 
                LonSecPos           secBeg, // base [1] in sec. The first letter in sec to be read. 
                const std::string&  clas, 
                float               conc        ) 
:	CSecBasInfo ( id, nam, clas) ,		
	_NNpar	    ( NNpar),
	_Conc	    ( conc )
{		
		if (secBeg<1) 
              secBeg=1; // from the very begginig

            //      Y_beg ,  Y_end,  Y_pos (current position) are coordenates in Y. This coordenates will be adjusted in the first (pre)read

            // sec_X - string seq. Original string TEXT of the seq.  (index in sec[0])     
        const LonSecPos sec_Len=static_cast<LonSecPos>( sec .length());
		if (sec_Len < 2)  return;  /// return if only 0 or 1 base to analize
        LonSecPos sec_beging =0,     
                  sec_end    =sec_Len,    
                  sec_pos    =0;   

            // orig_X - original "abstract" seq for which sec intent to be the representation, for example some gene: is the only intersting for a "biologist"
		    // index orig gene[1] 
		LonSecPos orig_beging = 0,  // secBeg,
                  orig_end    =  lmax ? secBeg + lmax -1 : secBeg + sec_Len -1,
                  orig_pos    = 0;   
                  
            // fltr - filtred seq, or what will be the resulting seq   (index in _c[1], _b[1], etc. becouse [0] is ? )
        LonSecPos fltr_pos =1;  
				/* fb, */           
                /* fe, */     
			
		// skip  non base and first secBeg bases or gaps in sec and set it in orig
		while ( !is_degbase[base(sec[sec_beging])] || ++orig_beging < secBeg ) // skip no-bases but not gaps "-"
			if (++sec_beging >= sec_Len) return;    // the sec had no more nt, but we dont reach the desired beg in the gene

        Base c, gap=basek[0];  // "-"

		// skip any further non base or gaps, and set orig beg to the first actual non gap nt in sec
		while (c = base(sec[sec_beging]), (c == gap || !is_degbase[c] ) )
		{
			if (c == gap)
				if (++orig_beging >= orig_end) return;  // we find no nt, but we reach the desired end of the gene

			if (++sec_beging >= sec_Len) return;    // the sec had no more nt, but we dont reach the desired beg in the gene
		}

		orig_pos = orig_beging ; /// where in alignment really beging the nts of this sequence, after all gaps
		sec_pos  = sec_beging ;  /// Now we are at the position were we beging to read the bases of the sequence

        if (sec_beging >= sec_Len-1 || orig_beging >= orig_end)   return; /// return if only 0 or 1 base to analize ???
           
        /// first we want to know how many bases are there making a pre - read.
		do
		{
			c = base(sec[sec_pos]);
			if (is_degbase[c] && c != gap) // a nt, n gap - count it
				if (++orig_pos >= orig_end) break;  // we find no nt, but we reach the desired end of the gene

			if (sec_pos == sec_Len - 1) break; // this was the last char in sec
			++sec_pos;
		} while ( true );

		sec_end = sec_pos  ;
		orig_end = orig_pos ;

        /// now go back to ignore terminal gaps and non bases in sec 
		while (c = base(sec[sec_end]), (c == gap || !is_degbase[c] ) )
		{
			if (c == gap)
				if ( orig_beging >= --orig_end) return;  // we find no nt 

			if ( sec_beging >= --sec_end) return;    // the sec had no nt
		}

        if (orig_beging > 1 || orig_pos > orig_end)    // there  are initial or terminal ----, posible the sec not beg at the beg of the aln
        {
            if (! _aln_fragment)
                _aln_fragment.reset(new Aligned_fragment);
            _aln_fragment->sq.SetMin(orig_beging);
            _aln_fragment->sq.SetMax(orig_end);
        }

        LonSecPos len = orig_end-orig_beging+1;   /// _len is the numer of "bases" to be readed (posible including gaps and deg-bases, but not external gaps)
			              /// as in:" TGCA$" . Dont count the 2 '$' - principio y fin de Kadelari and the final '\0'
        if (len < 2 )  return;  /// return if only 0 or 1 base to analize
           
		_c  .reserve(len+2)   ;
		_b  .reserve(len+2)   ;
		_SdS.reserve(len+1)   ;
		_SdH.reserve(len+1)   ;	

		register Base a_1, a;
		for (a=0; a<n_dgba; a++)	_Count[a]=0 ;

		//_c  .push_back( basek[n_basek-1] ); // '$' principio y fin de Kadelari.=" TGCA$"   in fltr_pos=0
		//_b  .push_back( n_basek-1) ; 	  	
		//_SdS.push_back( _NNpar->GetInitialEntropy()); // Solo dep de las conc, no de la sec. Ajustar primero la conc del target, y despues volverla a poner,?? 
		//_SdH.push_back(  0 );						  // y comprobar que se hacen los calculos necesarios

	    a_1=is_degbase	[ base(sec[sec_beging]) ] ;   // we read sepparate the first base, in fltr_pos=1 when sec_pos=sec_beging
		_GCp	+= is_GC	[a_1] ;                   // it will be always OK (we just tested that)
		_GrDeg	*= grad_deg	[a_1] ;
		_Count  [  db2nu	[a_1]]++ ;
		if (grad_deg[a_1] >1) _NDB++ ;
		_c.push_back( a_1  );
		_b.push_back( a_1 =bk2nu[is_base[a_1]]) ;						// que hacer si en la sec hay bases deg que Kadelari no considera?

		for (sec_pos=sec_beging+1, fltr_pos= 2; fltr_pos <= len && sec_pos <= sec_end  ; ++sec_pos)		// suficiente  fltr_pos <= len   ???
			if ( a=is_degbase	[ base(sec[sec_pos] )] ) 	
			{	
                if(sec[sec_pos]==gap)   // ignore gaps !!!!
                    continue;
                fltr_pos++ ;  
				_GCp	+= is_GC		[a] ;						// 1-G or C, 0-lo demas.      Y que con las bases deg ??????????????????
				_GrDeg	*= grad_deg		[a] ;
				_Count  [  db2nu		[a] ]++ ;
				if (grad_deg[a] >1) _NDB++ ;
				_c  .push_back( a                     );
				_b  .push_back( a =bk2nu[is_base[a]]  );				// que hacer si en la sec hay bases deg que Kadelari no considera?
				_SdS.push_back( _SdS.back() + NNpar->GetSelfEntr (	a_1 , a)  );
				_SdH.push_back( _SdH.back() + NNpar->GetSelfEnth (	a_1 , a)  );
				a_1 = a ;
			}	
		_c.push_back( basek[n_basek-1] ); // '$' principio y fin de Kadelari.=" TGCA$", + '\0'
		_b.push_back(       n_basek-1  ); 	  	
		_GCp	= _GCp*100/len ;	
		_Tm.Set( NNpar->CalcTM( _SdS.back(), _SdH.back()) ) ;      //_maxTm = _minTm =
}

//CSec  * CSec::CreateCopy(DNAstrand strnd) // strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
//{	Base *s=GetCopy_charSec(strnd); 
//	//char *n; 
//	CSec *newS=new CSec( (char*)s, 
//						NewS_ID(),				
//						_name + DNAstrandName[strnd],
//						_NNpar	,
//						0,1,
//						_Clas,
//						_Conc
//						);
//	newS->Selected(Selected());
//	newS->Filtered(Filtered());
//	delete []s;/*delete []n;*/
//	return newS;
//}
    
CSec* CSec::Clone   	(DNAstrand strnd 	 ) const  /// unique_ptr<ISec>   strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
{	
	
	string s; 
	unique_ptr<CSec> newS {new CSec( Copy_Seq(s,strnd), 
						             NewS_ID(),				
									_name + DNAstrandName[(int)strnd],
									_NNpar	,
									0,1,
									_Clas,
									_Conc
									)
	                       };
	newS->Selected(Selected());
	newS->Filtered(Filtered());
 	return newS.release();
}


float	CSec::Tm	(long pi, long pf)const
{	assert( 0< pi/* && pi<=pf*/); 		assert( /*0< pi && */pi<=pf+1);		// assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcTM(	_SdS[pf-1]					 ,		_SdH[pf-1]			 );
					return _NNpar->CalcTM(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2]);
}

	//float		G	(long pi, float Ta)const{return G(pi,_len, Ta);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
	//float		G	(float Ta )const	    {return G(1 ,_len, Ta);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
float	CSec::G	(long pi, long pf, float Ta)const				// G de la sonda con sec desde pi hasta pf, inclusive ambas!! 
{	assert( 0< pi/* && pi<=pf*/);		assert( /*0< pi && */pi<=pf+1);		// assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcG(	_SdS[pf-1]					 ,		_SdH[pf-1]			 , Ta);
					return _NNpar->CalcG(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2], Ta);
} 

//float		G	(long pi)const	{return G(pi,_len);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
//float		G	( )const		{return G(1,_len) ;}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
float	CSec::G	(long pi, long pf) const
{	assert( 0< pi/* && pi<=pf*/);		assert( /*0< pi && */pi<=pf+1);		// assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcG(	_SdS[pf-1]					 ,		_SdH[pf-1]			 );
					return _NNpar->CalcG(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2]);
} 

		CSec::~CSec () 
{	
	Remove();
}    

void	CSec::CorrectSaltOwczarzy() 
{	
	auto len=_SdS.size() ;
	for (decltype(len) j=2; j< len ; j++)
		_SdS[j-1] = _SdS[j-2] + _NNpar->GetCorrectSaltOwczarzySelfEntr ( _b[j-1] , _b[j] , _GCp );

	_Tm.Set( _NNpar->CalcTM( _SdS.back(), _SdH.back()) ) ; //_maxTm = _minTm =	_Tm  = _maxTm = _minTm =  ;
} 

CMultSec *CSec::CreateNonDegSet()
{
	if (! _NDB )					// La sec no tiene bases deg 
	{	if (_NonDegSet)				// pero existia un ndgs !! no debe ser...pero por si...
			{	delete _NonDegSet ;	// lo borramos 
				_NonDegSet=nullptr;	// y ponemos en 0. Porque esto se usa para saber si existe.
			}
		return _NonDegSet ;			// O sea, nullptr
	}
	else 	
		{	if (_NonDegSet)			// La sec tiene bases deg y ya existia un ndgs. Lo respetamos.
				return _NonDegSet ; // revisar si es el mismo PNNParams NNpar??
			ForceNonDegSet() ;
			//_Tm = _NonDegSet->_Tm ; //	_maxTm = _NonDegSet->_maxTm ; _minTm = _NonDegSet->_minTm ;	_Tm    = (_maxTm + _minTm )/2 ;
		}
	return _NonDegSet;
}

CMultSec *CSec::ForceNonDegSet()
{
		if (_NonDegSet)			// pero existia un ndgs !!
			delete _NonDegSet ;	// lo borramos 
		_NonDegSet= new CMultSec(_NNpar) ;
		_NonDegSet->AddSec( GenerateNonDegVariant(this, 0, 0) ) ;
		_Tm = _NonDegSet->_Local._Tm ; 

		//assert ( ( (cout << "Post Deg set generation: "<< _name << "\t" << _c << "\t" 
		//			 << "Tm=" << (_minTm - 273)  << " °C"
		//			 << " ("  << (_Tm - 273)     << " °C"  << ") "
		//			          << (_maxTm - 273)  << " °C"  << "\n" ) , 1 ) ) ;

	return _NonDegSet;
}

CSec *	CSec::CopyFirstBases(long pos) 
{	
	CSec *sec = new CSec ( Len(), _NNpar) ;
	assert(sec);
    sec->_name =  _name ;
    sec->_Clas =  _Clas ;

	sec->_GCp	= _GCp ;
	sec->_GrDeg	= _GrDeg ;
	for (Base b=0	  ; b<n_dgba   ; b++)	sec->_Count[b]= _Count[b] ;
	sec->_NDB = _NDB ;
    if (pos)
    {
        sec->_c   .assign(_c  .begin(), _c  .begin()+pos+1);
	    sec->_b   .assign(_b  .begin(), _b  .begin()+pos+1); 
	    sec->_SdS .assign(_SdS.begin(), _SdS.begin()+pos); /// To calculate current value we need next base ??
	    sec->_SdH .assign(_SdH.begin(), _SdH.begin()+pos); /// Responsabilty passed to calling
    }
    //long i=1;
	//for (; i<pos; i++)
	//{	
	//	sec->_c  .push_back(_c[i]);
	//	sec->_b  .push_back(_b[i]);
	//	sec->_SdS.push_back(_SdS[i]);
	//	sec->_SdH.push_back(_SdH[i]);
	//}
	//sec->_c  .push_back(_c[i]);
	//sec->_b  .push_back(_b[i]);

	return sec ;
}

CSec *	CSec::GenerateNonDegVariant ( CSec *s, long pos, Base ndb) /// \todo: crear variante que inserte las variantes en la lista dada.????
{	Base pre ; Base b_or,  cur ;								   /// para eso anadir ultimo parametro CMultiSec &ndg=_nds
	CSec *sec ;
	if (pos==0) 
	{	sec = s->CopyFirstBases(0);   // caso esp: ni pos -1, ni muto a ndb
		pre = sec->_b[0];
		//sec->_SdS.push_back(_SdS[0]);
		//sec->_SdH.push_back(_SdH[0]);
	}
	else 
	{
		// copia la parte inicial, que ya esta bien
		sec = s->CopyFirstBases(pos-1);     // anadirle algo al nombre ??

		// cambia la base deg en la 'pos' a 'ndb' - no deg base --> MUTACION  !!
			  pre = sec->_b[pos-1];
		Base b_or = _c[pos], 
			  cur = bk2nu[ndb];	
			  
		sec->_c.push_back(	ndb );
		sec->_b.push_back(	cur );
		sec->_GCp	+= float ((is_GC[ndb]-is_GC[b_or]))/Len() ;
		sec->_NDB-- ;
		sec->_GrDeg *= (grad_deg[ndb] / ( grad_deg[b_or] ? grad_deg[b_or] : 1) ) ;	
		
/*		if ( pos > 1 ) 	
		{	*/
        sec->_SdS.push_back(  sec->_SdS.back() + _NNpar->GetSelfEntr (	pre  ,	cur) );
		sec->_SdH.push_back(  sec->_SdH.back() + _NNpar->GetSelfEnth (	pre  ,	cur) );
		//}
  //      else 
  //      {
		//    sec->_SdS.push_back(_SdS[0]);
		//    sec->_SdH.push_back(_SdH[0]);
	 //   }

		_Count[db2nu[ ndb]]++ ;
		_Count[db2nu[b_or]]-- ;

		pre = cur ;
	}
	// continua actualizando el resto
	long i, len=Len();
	for ( i=pos+1; i<=len; i++)
	{	b_or = _c[i] ;
		Base n = grad_deg[b_or];
		if (n<2)
		{	
			sec->_b.push_back( cur= _b[i]) ;
		}
		else 		// hasta que encuentra la siguiente bas deg
		{	for (Base d=0; d < n-1 ; d++)	// recorre las bas no deg de la base deg b_or, menos la ultima
			{
				_NonDegSet->AddSec( GenerateNonDegVariant ( sec, i, dg2ba [ db2nu[b_or]  ][d] )) ;
			}

			ndb			 = dg2ba [  db2nu[b_or]  ][n-1];   // aqui me quedo con la ultima variante para seguir
			sec->_GCp	+= (is_GC[ndb]-is_GC[b_or])/len;
			sec->_GrDeg	/= grad_deg[b_or] ;	
			sec->_b.push_back( cur=  bk2nu[ndb] );
			_Count[db2nu[ ndb]]++ ;
			_Count[db2nu[b_or]]-- ;
			sec->_NDB-- ;
			b_or = ndb ;      
		} 
		sec->_c.push_back( b_or ) ;
		if ( i > 1 ) 	
		{	sec->_SdS.push_back(  sec->_SdS.back() + _NNpar->GetSelfEntr (	pre  ,	cur) );
			sec->_SdH.push_back(  sec->_SdH.back() + _NNpar->GetSelfEnth (	pre  ,	cur) );
		}
		pre = cur ;
	}
	sec->_c.push_back(  basek[n_basek-1]) ; // '$' principio y fin de Kadelari.=" TGCA$"
	sec->_b.push_back(  	   n_basek-1) ; 
	//sec->_c.push_back(  0 )
	//sec->_b.push_back(  0 );

	sec->_Tm.Set(_NNpar->CalcTM( sec->_SdS.back(), sec->_SdH.back())) ;   

	sec->_name += std::to_string(_NonDegSet->_Local._NSec);

	return sec ;
}
//	assert ( ( (cout << sec->_name << "\t" << sec->_c << "\t" << (sec->_Tm - 273) << " °C" << "\n" ) , 1 ) ) ;
	//assert ( ( (cout << "Ultima mutacion: " << b_or << ndb << "\t"<< sec->_name << "\t" << sec->_c << "\t" 
	//				 << "Tm=" << (sec->_minTm - 273)  << " °C"
	//				 << " ("  << (sec->_Tm - 273)     << " °C" << ") "
	//				          << (sec->_maxTm - 273)  << " °C" << "\n" ) , 1 ) ) ;



