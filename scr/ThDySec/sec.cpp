/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDySec\sec.cpp
*
* @brief 
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

#include "ThDySec/multsec.h"
#include "ThDySec/common.h" 
using namespace DegCod;

#define SEQUENCES_MAX_SIZE 100000
char *DNAStrandName[]=	{""		, "(c)", ""		, "(r)"	, "(i)", "(c)"		} ;
// enum DNAStrand		{plus	, minus, direct	, rev	, compl, rev_compl	} ;


CSecBasInfo::~CSecBasInfo()
{
	if (_NonDegSet) 
	     if (!_NonDegSet->Prev() && !_NonDegSet->Next()) 
			 delete _NonDegSet;
	// en otro caso, donde borrar _NonDegSet ????. Lo borra la lista en la que esta insertado
}
	 
std::string& CSecBasInfo::Copy_Seq  	(std::string &SecHier,  long InicBase, long EndBase, DNAStrand strnd) const
{	
	if ( EndBase< 1 || Len() <EndBase ) EndBase= Len(); 
	long l=EndBase-InicBase+1 ;  
	if (l>=0) 	//assert(l>=0);
	{
		SecHier.clear();
		SecHier.reserve(l);
		switch (strnd)
		{	case DNAStrand::plus :
			case DNAStrand::direct:		for(long p=InicBase;  p<=EndBase;    p++) 	SecHier.push_back ( _c[p] );  				    break;
			case DNAStrand::compl:		for(long p=InicBase;  p<=EndBase;    p++) 	SecHier.push_back ( c_degbase[_c[p]]);      	break;
			case DNAStrand::rev:		for(long p=EndBase ;  p>=InicBase;   p--)	SecHier.push_back ( _c[p] );  				    break;
			case DNAStrand::minus:
			case DNAStrand::rev_compl:	for(long p=EndBase ;  p>=InicBase;   p--)	SecHier.push_back ( c_degbase[_c[p]]);      	break;
 		}
	}
	return  SecHier ;
}

/// Best just return sequence
//Base  *	CSecBasInfo::Copy_charSec(Base *charSecHier,long InicBase, long EndBase, DNAStrand strnd)//DNAStrand strnd=direct)
//{	if ( EndBase< 1 || Len() <EndBase ) EndBase= Len(); 
//	long l=EndBase-InicBase+1 ; charSecHier[l]=0 ;
//	if (l>=0) 
//	//assert(l>=0);
//	switch (strnd)
//	{	case DNAStrand::plus :
//		case DNAStrand::direct:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=_c[p];				break;
//		case DNAStrand::compl:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=c_degbase[_c[p]];	break;
//		case DNAStrand::rev:		for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=_c[p];				break;
//		case DNAStrand::minus:
//		case DNAStrand::rev_compl:	for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=c_degbase[_c[p]];	break;
//
//		default : return 0;
//	}
//	return charSecHier ;
//}
//Base  *	CSecBasInfo::GetCopy_charSec(DNAStrand strnd)
//{	return GetCopy_charSec(1, Len(), strnd);
//}
//Base  *	CSecBasInfo::GetCopy_charSec(long InicBase, long EndBase, DNAStrand strnd)       // recuerde los $...$, aqui se cuentan, 
//{	if ( InicBase< 1 )					InicBase=1;
//	if ( EndBase< 1 || Len() <EndBase )	EndBase=Len();
//	long l=EndBase-InicBase+1 ;
//	assert(l>=0);
//	Base *charSecHier=new Base[l+1];						 // asi como InicBase y EndBase inclusive!!
//	assert(charSecHier);	
//
//	return Copy_charSec(charSecHier, InicBase, EndBase, strnd)  ;
//}


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

CSec::CSec (    const std::string&  sec, 
                int                 id, 
                const std::string&  nam, 
                std::shared_ptr<CSaltCorrNN> NNpar, 
                long                lmax,   /// lamx - limita la cant de bases originales a leer despues de las primeras secBeg-1 bases 
                long                secBeg,  
                const std::string&  clas, 
                float               conc        ) 
:	CSecBasInfo ( id, nam, clas) ,		
	_NNpar	    ( NNpar),
	_Conc	    ( conc )
{		
		if (secBeg<1) secBeg=1;

        LonSecPos sb,       /* se,*/  sp,      /// s - string seq. Original string text of the seq.  (index in sec[])
                  ob=secBeg, oe,  op,      /// o - original "abstract" seq for with s intent to be the representation
                 /* fb,*/        /*fe,*/  fp;      /// f - filtred seq, or what will be the resulting seq   (index in _c[], _b, etc.)
               //  beg ,      end, position  

        const LonSecPos sLen=sec.length();
			
        for (sp= 0, op= 0 ; sp<sLen   ; sp++) 	// salta no-bases y las primeras secBeg-1 bases 
            if( is_degbase	[Base (sec[sp])] ) 
            {   
                if ( op == ob-1 )
                    break;
                ++op ;
            }  
        if ( sp >= sLen-1 )   /// return if only 0 or 1 base to analize
            return;
        sb=sp; 

 		if (lmax)			
		{	
            oe=ob+lmax-1 ;
			for (         ; sp<sLen   ; sp++) 
                if( is_degbase	[Base (sec[sp])] ) 
                {   
                    ++op ;
                    if ( op == oe )
                        break;
                }  
		}else
		{	 
			for (         ; sp<sLen            ; sp++) 
                if( is_degbase	[Base (sec[sp])] ) 
                    ++op ;	// salta no-bases 
		}
        oe=op ;
        LonSecPos len = oe-ob+1;   /// _len is the numer of "bases" to be readed (posible including gaps and deg-bases)
			              /// as in:" TGCA$" . Dont count the 2 '$' - principio y fin de Kadelari and the final '\0'
        if ( sLen < 2 )   /// return if only 0 or 1 base to analize
            return;
		_c  .reserve(len+2)   ;
		_b  .reserve(len+2)   ;
		_SdS.reserve(len+1)   ;
		_SdH.reserve(len+1)   ;	

		register Base a_1, a;
		for (a=0; a<n_dgba; a++)	_Count[a]=0 ;

		//_c  .push_back( basek[n_basek-1] ); // '$' principio y fin de Kadelari.=" TGCA$"   in fp=0
		//_b  .push_back( n_basek-1) ; 	  	
		//_SdS.push_back( _NNpar->GetInitialEntropy()); // Solo dep de las conc, no de la sec. Ajustar primero la conc del target, y despues volverla a poner,?? 
		//_SdH.push_back(  0 );						  // y comprobar que se hacen los calculos necesarios

	    a_1=is_degbase	[ sec[sb] ] ; 		// in fp=1 when sp=sb
		_GCp	+= is_GC	[a_1] ;
		_GrDeg	*= grad_deg	[a_1] ;
		_Count  [  db2nu	[a_1]]++ ;
		if (grad_deg[a_1] >1) _NDB++ ;
		_c.push_back( a_1  );
		_b.push_back( a_1 =bk2nu[is_base[a_1]]) ;						// que hacer si en la sec hay bases deg que Kadelari no considera?

		for (sp=sb+1, fp= 2; fp <= len /*&& sp<se*/ ; ++sp)						// suficiente    curPos <= _len   ???
			if ( a=is_degbase	[ sec[sp] ] ) 	
			{	
				_GCp	+= is_GC		[a] ;						// 1-G or C, 0-lo demas.      Y que con las bases deg ??????????????????
				_GrDeg	*= grad_deg		[a] ;
				_Count  [  db2nu		[a] ]++ ;
				if (grad_deg[a] >1) _NDB++ ;
				_c  .push_back( a                     );
				_b  .push_back( a =bk2nu[is_base[a]]  );				// que hacer si en la sec hay bases deg que Kadelari no considera?
				_SdS.push_back( _SdS.back() + NNpar->GetSelfEntr (	a_1 , a)  );
				_SdH.push_back( _SdH.back() + NNpar->GetSelfEnth (	a_1 , a)  );
				a_1 = a ;
                fp++ ;
			}	
		_c.push_back( basek[n_basek-1] ); // '$' principio y fin de Kadelari.=" TGCA$", + '\0'
		_b.push_back(       n_basek-1  ); 	  	
		_GCp	= _GCp*100/len ;	
		_Tm.Set( NNpar->CalcTM( _SdS.back(), _SdH.back()) ) ;      //_maxTm = _minTm =

}

//CSec  * CSec::CreateCopy(DNAStrand strnd) // strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
//{	Base *s=GetCopy_charSec(strnd); 
//	//char *n; 
//	CSec *newS=new CSec( (char*)s, 
//						NewS_ID(),				
//						_name + DNAStrandName[strnd],
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
    
CSec* CSec::Clone   	(DNAStrand strnd 	 ) const  /// unique_ptr<ISec>   strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
{	
	
	string s; 
	unique_ptr<CSec> newS {new CSec( Copy_Seq(s,strnd), 
						             NewS_ID(),				
									_name + DNAStrandName[strnd],
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



//!  Inicializa array de posibles cand en esta sec. ;---------------------  CSec------------>  CSecCand  ---------------------
		CSecCand::CSecCand(CSec &sec, 	SondeLimits sL)		  	
					:	_Sec(sec), 	
						_rg(new pCRang [sec.Len()+2]), 			//	_b[_len+2]=	    0  ; 
						_NumPosCand(0), 
						_NumCand(0),
						_NumCandExact(0)							

{	
	long fi, i0;										//	assert (_rg);	trow exeption !!
	for (fi=0; fi< sL._L.Min() ; fi++)  _rg[fi]=0;		// se salta las primeras pos
														// al comienzo fi = L_min	
	for (	; fi<=sec.Len(); fi++)						// fi - final base of candidate, recorre toda la sec
	{	
		long        pi = fi - sL._L.Max() +1	; if (pi < 1 ) pi=1;
		CRangBase R(pi	,fi - sL._L.Min() +1 )  ;					   assert( fi>R.Max() );		assert( pi<=R.Max() );

		for (i0=R.Min() ; i0<=R.Max() ; ++i0 )
		{	if ( sL._Tm.inRang( sec.Tm(i0,fi) )    &&    sL._G.inRang( sec.G(i0,fi) ) ) 	// usa calculos de Tm basados en NNpar
			{	 R.adjustCur(i0);
				 _NumCandExact++;	//	Seria lo correcto, pero da problemas cuando existe una "burbuja" de Tm, 
									//  cosa que un "rango" no puede considerar, tendria que hacerce con un conjunto. 
									//  Osea estoy dejando dentro del rango, interiormente la posibilidad de aceptar sondas 
									//	con Tm fuera de rango
			}
		}
		if (R.hasMatch())
		{		_NumPosCand++ ; 
				_NumCand+= R.NumMatch();				
				_rg[fi]=new CRang (R.MatchRange());    				assert( fi>R.Max() );	assert( fi>_rg[fi]->Max() );	
		} else	_rg[fi]=0 ;
	}
	_NumPosCandIn=_NumPosCand ; _NumCandIn=_NumCand ;
}

//NumRang<long> p(fi-sL._L.Max() +1 , fi-sL._L.Min() +1 ) , pcur;
		//long pi		=fi-sL._L.Max() +1		, pf		=fi-sL._L.Max() +1 ;   // al comienzo pf = L_max -L_min +1
		//long picur	=pf+1					, pfcur		=pi	;
//	assert(((clog<< "\nCreating: "<<sec._name<<" \t,#pos: \t"<< _NumPosCand<<", #cand: \t"<< _NumCand),1));
							//if(picur>i0)					picur=i0;
							//if(pfcur<i0)					pfcur=i0;
//(R._pfcur - R._picur + 1);




/// Despues de comparar dos seq "candidatos" analiza los rangos de una y los colapsa 
long	CSecCand::ColapseRangs(bool colapse) // hacer otra variante para "no colapse" rang by self hybri - no sec str!
{								// _NumPosCand = 0 ;
								// anadir 2 parametros: pos de com y fin de zona "efectiva" o cubierta por sec pareada
								// fuera de esa zona no colapsar
	long NumPosCand(0), NumCand(0); // 2011-05-16. Para resolver el problema del conteo de pos y cand.

											//    pi      pf                fi
											//----|++++++++|-----------------|--------			El rango inicial, y como va quedando
											// pfcur                        fi					El rango para calculo ("cur"), antes del comienzo	
											//---|----------|----------------|--------			Asi se queda si no hibridan entre si las sec en esta zona,
											//             picur            fi					y entonces "colapsa" el rango
											//            pfcur             fi					En este caso encontro 5 "cand" comunes	
											//--------|+++|------------------|--------			
											//       picur                  fi					
	
	for (long fi=0; fi<=_Sec.Len(); fi++)
	{	if (!_rg[fi]) continue ;		 
											
		CRang &R (*_rg[fi]);				assert(fi>R.Max());

		if ( R.hasMatch()  )	
		{	
			NumPosCand++ ; NumCand += R.NumMatch() ;						
			if (colapse) 
			{	
				R.SchrinkToMatch();
			}
			R.IncrMatchs();
			R.open();
		}else															
		{
			if (colapse) {	delete _rg[fi];	_rg[fi]=0 ;} 	
		}
	}

	// if (colapse)											//   REVISAR    !!!!!!!!!!!
	{	/*_NumPosCandIn =*/ _NumPosCand = NumPosCand ; 			
		/*_NumCandIn    =*/ _NumCand    = NumCand ;   
	}

	return _NumCand ;
}


// _NumPosCand-- ;	//	assert(((clog<<"\n("<<fi<<"->"<<_rg[fi]->_pi<<"-"<<_rg[fi]->_pf<<")
//	---- colapsing:"<<_rg[fi]->_picur<<"-"<<_rg[fi]->_pfcur),1));
// amplitud : pfcur - picur
//	En realidad solo puede ser =0 y no <0, o >0 claro
			//for (int pi_pos=R._pi    ; pi_pos <= R._picur ; pi_pos++ ) 
			//R.match[pi_pos - R._pi]++;
			//for (int pf_pos=R._pfcur ; pf_pos <= R._pf    ; pf_pos++ ) 
			//R.match[pf_pos - R._pi]++;
// en la ultima comparacion no se confir cand en esta fi	//_NumPosCand--;
			//	assert(((clog<< "\nColapsing "<<_Sec._name<<" \t,#pos\t"<< _NumPosCand<<"\t, #cand: "<< _NumCand),1)); ;


char *	CSecAl::CopyAlignedSecChar(long Al_pBeg, long Al_pEnd, char *CharSec)	// CUIDADO !! asume suficiente espacio !!
{	//assert (Al_pBeg<=Al_pEnd);																			// "EXPERIMENTAL" ---------------------
	assert (Al_pBeg>0); //assert (Al_pEnd>0); 
	assert (CharSec);
	long j, i;
	for ( i=Al_pBeg, j=0; i<=Al_pEnd; i++,j++)
		if (_inAlp_B[i] == -1)	CharSec[j] = nu2dgba[0] ;
		else					CharSec[j] =_Sec[_inAlp_B[i]] ; // Para las incerciones uso -1 en el Al
							
	CharSec[j]=0;
	return CharSec;
}
char *	CSecAl::GetAlignedSecChar(long Al_pBeg, long Al_pEnd)  // "regala" esta memoria, no olvide delete !
{	long l=Al_pEnd-Al_pBeg+1;																				// "EXPERIMENTAL" ---------------------
	assert (l>0);
	char *Al_sec= new char[l+1];
	assert (Al_sec);
	return CopyAlignedSecChar(Al_pBeg,  Al_pEnd, Al_sec);
}

CMultSec::CMultSec (	const std::string &file	, 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					bool           all_dir  /*= false*/,
					float		   MaxTgId	/*= 100*/, 
					LonSecPosRang  SecLim	/*= LonSecPosRang {1,0}*/,	 
                    SecPosRang     SecLenLim/*= SecPosRang    {0,0}*/,
					bool           loadSec  /*= true*/
				 )  
    :	/*_name(trim_string(file)),	*/
	    _SecLim     (SecLim),
	    _MaxTgId    (MaxTgId), 
        _SecLenLim  (SecLenLim),
	    _NNPar      (NNpar)/*,
        _Path       (file)*/
{
	filesystem::path  itf(file);

	if (all_dir)
    {
        if (filesystem::is_regular_file(itf)) itf.remove_filename();

	    _name = itf.filename ();     /// The new MSec take the name of the dir.
        _Path = itf;                 /// and the _Path point to it.

        filesystem::directory_iterator rdi{ itf }, end;

        for (; rdi != end; ++ rdi)
            AddMultiSec(  new CMultSec(  rdi->path().string().c_str() , 
                                         NNpar,
                                         filesystem::is_directory(rdi->status()), 
                                         MaxTgId, 
                                         SecLim,   
                                         SecLenLim,
                                         loadSec) );
          //      else 
          //          itf.remove_filename();
          //      _name = itf.   .basename();
		        //AddFromDir(file);
	}
	else
	    if (itf.has_filename())
	    {
		    _name = itf.filename ();     /// The new MSec take the name of the file.
            _Path = itf;                 /// and the _Path point directly to the file.
            if (loadSec)
		       AddFromFile(file);  /// will throw if not a file
	    }
}


//int		CMultSec::AddFromDir(const std::string& dir, bool  recurs  /*= false*/)		// return la cantidad de sec add --------------------  AddFromDir   -------------------
//{	
//	filesys::path  itf(dir);
//
//	if (recurs)
//    {
//        filesys::directory_iterator rdi{filesys::is_directory(itf) ? itf : itf.remove_filename() }, end;
//	    _name = itf.filename ();
//
//        for (; rdi != end; ++ rdi)
//            AddMultiSec(  new CMultSec(  rdi->path().string().c_str() , 
//                                         _NNpar,
//                                         filesys::is_directory(rdi->status()), 
//                                         _MaxTgId, 
//                                         _SecLim,   
//                                         _SecLenLim  ) );
//  //      else 
//  //          itf.remove_filename();
//  //      _name = itf.   .basename();
//		//AddFromDir(file);
//	}
//	else
//	    if (itf.has_filename())
//	    {
//		    _name = itf.filename ();
//		    AddFromFile(file);
//	    }
//}
int		CMultSec::AddFromFile (const std::string& file)		// return la cantidad de sec add --------------------  AddFromFile   -------------------
{	
    ifstream ifile( file ); 
	if ( ! ifile ) 
	{
	    throw std::ios_base::failure(string("Could not open the sequence file: ")+ file );
	}

	return AddFromFile(ifile); /// \todo: retrow anadiendo el nombre del file
}

int		CMultSec::AddFromFile (ifstream& ifile)		// return la cantidad de sec add --------------------  AddFromFile   -------------------
{		
	if (  _SecLim.Max() <= _SecLim.Min() ) 
		_SecLim.SetMax(0) ; // if ( _SecEnd<=_SecBeg) _SecEnd=0 ;

	int j=0;
	char c1;
	ifile>>skipws  >> c1;
	if ( ! ifile.good() ) 	
	{
	    throw std::ios_base::failure(string("Could not read the sequence file: ")/*+ file */);
	}

	if( c1 =='>' ) 																		
		return AddFromFileFASTA (ifile);
					// esto es FASTA, si no  BLAST o GB o ...
	if (c1 =='<' )
	{	string xml_DOCTYPE ;
		getline (ifile, xml_DOCTYPE,'>') ;   // <?xml version="1.0"?>
		getline (ifile, xml_DOCTYPE,'>') ;   // <!DOCTYPE BlastOutput PUBLIC "-//NCBI//NCBI BlastOutput/EN" "NCBI_BlastOutput.dtd">
		if ( ! ifile.good() ) 	
		{	throw std::ios_base::failure(string("Could not open the sequence file: ")/*+ file*/ );		}

		if		(string::npos!=xml_DOCTYPE.find("DOCTYPE BlastOutput") )				
			return AddFromFileBLAST (ifile);
		else if (string::npos!=xml_DOCTYPE.find("DOCTYPE Bioseq-set" ) )				
			return AddFromFileGB (ifile);
		return 0;
	}
	if (c1 =='L' )	
	{	ifile.putback(c1);												
		return AddFromFileGBtxt (ifile);	
	}
	return 0;   // cerr unknow format		
}	 

int		CMultSec::AddFromFileFASTA (ifstream &ifile)  // -------------------    AddFromFileFASTA   ------------
{	int j=0 ;													//long l= (_SecEnd ? _SecEnd-_SecBeg +1 : 0) ;
	string Descriptor  ;
	while (getline (ifile, Descriptor) )
    {
		size_t b_d=Descriptor.find_first_not_of(
								"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890!#$()*+-./<=>@[]^_{|}~"    );
		string Fasta_NAME = Descriptor.substr(0,b_d );
		Descriptor=Descriptor.substr(Fasta_NAME.length());

  		string Fasta_SEC ;					
		if (!getline (ifile, Fasta_SEC,'>')) 		break ;		

		if ( Fasta_SEC.length() < _SecLim.Min() ||  Fasta_SEC.length() < _SecLenLim.Min() )	continue;

		unique_ptr<CSec> sec (  new CSec(   Fasta_SEC , 
                                            _Local._NSec, 
                                            Fasta_NAME , 
                                            _NNPar,
                                            _SecLim.Max() ? _SecLim.Max()-_SecLim.Min()+1 : 0 ,
                                            _SecLim.Min()                                       /// \todo: cambiar constr de CSec ????
                                         )) ;                  assert(sec);
						
		if ( sec->Len() >= _SecLenLim.Min()   )		
		{	
			CSec *idem=Idem(*sec);
			if (idem) 
			{
				sec->Selected(false);
				sec->Filtered(true);
			}
			else
				j++;		
			sec->Description(trim_string(Descriptor));
			InsertSecAfter ( sec.release() , idem) ;	
		}
	}
	return j;	
}

int		CMultSec::AddFromFileBLAST (ifstream &fi) // ----------------  CMultSec::            AddFromFileBLAST  -----------------------------
{	unsigned int	_BlastOutput_query_len ;		// x todos los "hits"
	int			 id=0;
	string li ;  //  xml_line

	do  {	getline (fi, li,'>') ;			if ( ! fi.good() ) return 0; }   // BLAST format error
	while  (string::npos==li.find("BlastOutput_query-len") ); fi>>_BlastOutput_query_len;//  <BlastOutput_query-len>267</BlastOutput_query-len>
	
	do {	unsigned int	_Hit_num=0 ;			// para cada hit
			std::string	    _Hit_id  ;				
			std::string	    _Hit_def ;				// descriptor ??
			std::string	    _Hit_accession 	;
			long			_Hit_len=0 ;				
			float			_Hsp_bit_score=0 ;
			unsigned int	_Hsp_score=0 ;
			double			_Hsp_evalue=0 ;
			LonSecPos		_Hsp_query_from=0 ;    // dejar signed or unsigned !!!!????
			LonSecPos		_Hsp_query_to=0 ;
			LonSecPos 		_Hsp_hit_from=0 ;
			LonSecPos 		_Hsp_hit_to=0 ;
			int				_Hsp_query_frame=0 ;
			int				_Hsp_hit_frame=0 ;
			LonSecPos 		_Hsp_identity=0 ; // revisar type --- no sera %  : float??
			LonSecPos 		_Hsp_positive=0 ;
			LonSecPos 		_Hsp_gaps=0 ;
			LonSecPos 		_Hsp_align_len=0 ;
			std::string	    _Hsp_midline ;
			bool			_FormatOK=0 ;
		std::string	   sec;						// para CSec
		std::string	   nam;
		long		   l=0;
		std::string	   clas;

	while(getline(fi,li,'>')&& string::npos==li.find("Hit_num"      ) ) ;  fi>>_Hit_num;				//  <Hit_num>1</Hit_num>
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_id"       ) ) ;                           	//<Hit_id>gi|84028434|gb|DQ318020.1|</Hit_id> 
                                                            if ( ! getline (fi, _Hit_id, '<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_def"      ) ) ;                               //<Hit_def>Wets NIle virus strain ArB3573/82, complete genome</Hit_def>
                                                            if ( ! getline (fi, _Hit_def,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_accession") ) ;                              	//<Hit_def>Wets NIle virus strain ArB3573/82, complete genome</Hit_def>
                                                            if ( ! getline (fi, _Hit_accession,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_len"      ) ) ;  fi>>_Hit_len;				//  <Hit_len>11048</Hit_len> 
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_bit-score") ) ;  fi>>_Hsp_bit_score;			//  <Hsp_bit-score>482.786</Hsp_bit-score>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_score"    ) ) ;  fi>>_Hsp_score;              //  <Hsp_score>534</Hsp_score>		
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_evalue"   ) ) ;  fi>>_Hsp_evalue;		    	//  <Hsp_evalue>3.71782e-133</Hsp_evalue>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-from")) ;  fi>>_Hsp_query_from;		    //  <Hsp_query-from>1</Hsp_query-from>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-to" ) ) ;  fi>>_Hsp_query_to;			//  <Hsp_query-to>267</Hsp_query-to>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hit-from" ) ) ;  fi>>_Hsp_hit_from;			//  <Hsp_hit-from>9043</Hsp_hit-from>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hit-to"   ) ) ;  fi>>_Hsp_hit_to;			    //  <Hsp_hit-to>9309</Hsp_hit-to>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-frame"));  fi>>_Hsp_query_frame;		//  <Hsp_query-frame>1</Hsp_query-frame>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_identity" ) ) ;  fi>>_Hsp_identity;			//  <Hsp_identity>267</Hsp_identity>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_positive" ) ) ;  fi>>_Hsp_positive;			//  <Hsp_positive>267</Hsp_positive>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_gaps"     ) ) ;  fi>>_Hsp_gaps;	 		    //  <Hsp_gaps>0</Hsp_gaps>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_align-len") ) ;  fi>>_Hsp_align_len;		    //  <Hsp_align-len>267</Hsp_align-len>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hseq") ) ; 	                            	//  <Hsp_hseq>TACAACATGATGGGAAAGAGAGAGAAGAAG 
                                                            if ( ! getline (fi, sec         ,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_midline") ) ;                                 //  <Hsp_midline>|||||||||||||||||||||||||||||||||||||||||
                                                            if ( ! getline (fi, _Hsp_midline,'<') ) return id;

			// long SecBeg = _SecBeg - _Hsp_query_from +1 ;			// if (_SecBeg >= _Hsp_query_from)
			//if ( (_SecBeg		<= _Hsp_query_to) && ( (!_SecEnd)		 || _SecEnd		  >=_Hsp_query_from) ) // _SecEnd=0 significa no recortar la sec.

			if ( (_SecLim.Min() <= _Hsp_query_to) && ( (!_SecLim.Max())  || _SecLim.Max() >=_Hsp_query_from) ) // _SecLim.Max()=0 significa no recortar la sec.
			{	CSecBLASTHit *secH=  new CSecBLASTHit(      _BlastOutput_query_len ,
														// para cada hit
														_Hit_num ,
														std::move(_Hit_id) ,				
														std::move(_Hit_def) ,				
														std::move(_Hit_accession)	,
														_Hit_len ,				
														_Hsp_bit_score ,
														_Hsp_score ,
														_Hsp_evalue ,
														_Hsp_query_from ,
														_Hsp_query_to ,
														_Hsp_hit_from ,
														_Hsp_hit_to ,
														_Hsp_query_frame ,
														_Hsp_hit_frame ,
														_Hsp_identity ,
														_Hsp_positive ,
														_Hsp_gaps ,
														_Hsp_align_len ,
														std::move(_Hsp_midline) ,
														_FormatOK ,
														std::move(sec)		,_SecLim,                         // _SecBeg, _SecEnd,
														id,			                    //Hit_num   ???		//	char	*	nam,	Hit_def
														_NNPar               /*,  //long l=0,(Hit_len ---> NO ) !!!  -->_Hsp_align_len -OK clas,	conc*/
														);
				if ( secH->Len() >= _SecLenLim.Min()  )		
				{	
					CSec *idem=Idem(*secH);
					InsertSecAfter (secH  , idem) ;	
					if (idem) 
					{
						secH->Selected(false);
						secH->Filtered(true);
					}
					id++;		
				}
				else delete secH;
			}
		}
	while (fi.good() ); 
	return id; 
}

int		CMultSec::AddFromFileGBtxt (ifstream &ifile) // ----------------  CMultSec::            AddFromFileGBtxt  -----------------------------
{	const int gb_descr_w=12 ;	char gb_descr[gb_descr_w+1]; gb_descr[gb_descr_w]=0;
	size_t strl;
	int	id	=0     ;
	string txt_line ;

	do {	char	*	LOCUS			=nullptr      ;
			long		Seq_inst_length	=0		;	
			char	*	DEFINITION		=nullptr     ;
			char	*	ACCESSION		=nullptr     ;
			char	*	ORGANISM		=nullptr     ;
			// para CSec
			char	*	sec		=nullptr     ;		//	char		*	nam,	DEFINITION	,	//	long			l=0,		Seq_inst_length

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// LOCUS       AY702040               10675 bp    RNA     linear   VRL 24-MAR-2005
			while  (strcmp(gb_descr,	"LOCUS"	) );
			ifile>>setw (gb_descr_w)>>gb_descr ;				LOCUS=new char[1+(strl=strlen(gb_descr))] ;
			strncpy(LOCUS,gb_descr,strl) ;						LOCUS	[strl]=0;
			ifile>>Seq_inst_length ;	

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// DEFINITION  Dengue virus type 2 strain I348600, complete genome.
			while  (strcmp(gb_descr,	"DEFINITION"	) );
			getline (ifile, txt_line) ;								DEFINITION	=new char[txt_line.length()+1] ;
			txt_line.copy(  DEFINITION  ,txt_line.length()) ;		DEFINITION	[txt_line.length()]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// ACCESSION   AY702040
			while  (strcmp(gb_descr,	"ACCESSION"	) );
			ifile>>setw (gb_descr_w)>>gb_descr ;			ACCESSION=new char[1+(strl=strlen(gb_descr))] ;
			strncpy(ACCESSION,gb_descr,strl) ;						ACCESSION	[strl]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	//   ORGANISM  Dengue virus 2
			while  (strcmp(gb_descr,	"ORGANISM"	) );
			getline (ifile, txt_line) ;							ORGANISM	=new char[txt_line.length()+1] ;
			txt_line.copy(  ORGANISM  ,txt_line.length()) ;		ORGANISM	[txt_line.length()]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// ORIGIN      
			while  (strcmp(gb_descr,	"ORIGIN"	) );
			getline (ifile, txt_line,'/') ;							sec	=new char[txt_line.length()+1] ;
			txt_line.copy(  sec  ,txt_line.length()) ;				sec	[txt_line.length()]=0;

			CSecGBtxt *secGBtxt= new CSecGBtxt(	LOCUS       ,
												Seq_inst_length,	
												DEFINITION     ,
												ACCESSION      ,
												ORGANISM       ,
												sec	,	
												id,								//	char		*	nam,	DEFINITION	,	
												_NNPar);
				if ( secGBtxt->Len() >= _SecLenLim.Min()   )		
				{	
					CSec *idem=Idem(*secGBtxt);
					InsertSecAfter (secGBtxt  , idem) ;	
					if (idem) 
					{
						secGBtxt->Selected(false);
						secGBtxt->Filtered(true);
					}
					else
						id++;		
				}
				else delete secGBtxt;
			delete []sec ;
		}
	while (ifile.good() ); 
	return id; 
}

int		CMultSec::AddFromFileGB (ifstream &ifile)  // ----------------  CMultSec::            AddFromFileGB  -----------------------------
{	int		id=0;
	string xml_line ;

	do {	char		*	_Textseq_id_accession	=0 ;	
			char		*	_Org_ref_taxname		=0 ;
			char		*	_Seqdesc_title			=0 ;
			long			_Seq_inst_length		=0 ;	
			// para CSec
			char		*sec=0;			//char		*nam=0;		//long		 l=0;		//char		*clas=0;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; } 	// <Textseq-id_accession>DQ318020</Textseq-id_accession>
			while  (string::npos==xml_line.find(	"Textseq-id_accession"	) );
			getline (ifile, xml_line,'<') ;				_Textseq_id_accession=new char[xml_line.length()+1] ;
			xml_line.copy(_Textseq_id_accession,xml_line.length()) ;	_Textseq_id_accession	[xml_line.length()]=0;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; } // <Org-ref_taxname>West Nile virus</Org-ref_taxname>
			while  (string::npos==xml_line.find(	"Org-ref_taxname"	) );
			getline (ifile, xml_line,'<') ;								_Org_ref_taxname	=new char[xml_line.length()+1] ;
			xml_line.copy(  _Org_ref_taxname  ,xml_line.length()) ;		_Org_ref_taxname	[xml_line.length()]=0;
		// <Dbtag_db>taxon</Dbtag_db>
		// <Object-id_id>11082</Object-id_id>
		// <OrgName_name_virus>West Nile virus</OrgName_name_virus>
		// <OrgMod_subtype value="strain">2</OrgMod_subtype>
		// <OrgMod_subname>ArB3573/82</OrgMod_subname>
		// <OrgMod_subtype value="gb-acronym">32</OrgMod_subtype>
		// <OrgMod_subname>WNV</OrgMod_subname>
		// <OrgName_lineage>Viruses; ssRNA positive-strand viruses, no DNA stage; Flaviviridae; Flavivirus; Japanese encephalitis virus group</OrgName_lineage>
		// <SubSource_subtype value="country">23</SubSource_subtype>
		// <SubSource_name>Central African Republic</SubSource_name>
		// <SubSource_subtype value="other">255</SubSource_subtype>
		// <SubSource_name>lineage 2; SMB pass 4, C6/36 pass 1</SubSource_name>
			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }		// <Seqdesc_title>Wets NIle virus strain ArB3573/82, complete genome</Seqdesc_title>
			while  (string::npos==xml_line.find(	"Seqdesc_title"	) );
			getline (ifile, xml_line,'<') ;								_Seqdesc_title	=new char[xml_line.length()+1] ;
			xml_line.copy(  _Seqdesc_title  ,xml_line.length()) ;		_Seqdesc_title	[xml_line.length()]=0;
		// <MolInfo_biomol value="mRNA">3</MolInfo_biomol>
		// <MolInfo_completeness value="complete">1</MolInfo_completeness>
		// <Date-std_year>2006</Date-std_year>
		// <Date-std_month>1</Date-std_month>
		// <Date-std_day>1</Date-std_day>
		// <Seq-inst_mol value="rna"/>

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  // GB format error
			while  (string::npos==xml_line.find("Seq-inst_length") ) ; ifile>>_Seq_inst_length;		// <Seq-inst_length>11048</Seq-inst_length>
			
		// <Seq-inst_strand value="ss"/>

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }	// <IUPACna>AGTAGTTCGCCTGTGTGAGCTGACA.... GGTGCTAGAACACAGGATCT</IUPACna>
			while  (string::npos==xml_line.find(  "IUPACna"  ) );
			getline (ifile, xml_line,'<') ;         sec=new char[xml_line.length()+1] ;
			xml_line.copy(sec,xml_line.length()) ;	sec[xml_line.length()]=0;	


			CSecGB *secGB=  new CSecGB(      _Textseq_id_accession,
											_Org_ref_taxname	,
											_Seqdesc_title,
											_Seq_inst_length	 ,
											sec	,	
											id,			//	char		*	nam,		Hit_def
											_NNPar/*,  	//	long			l=0,		(Hit_len ---> NO ) !!!  -->_Hsp_align_len -OK clas, conc*/
										);
			delete []sec ;

				if ( secGB->Len() >= _SecLenLim.Min()  )		
				{	
					CSec *idem=Idem(*secGB);
					InsertSecAfter (secGB  , idem) ;	
					if (idem) 
					{
						secGB->Selected(false);
						secGB->Filtered(true);
					}
					else
						id++;		
				}
				else delete secGB;
		}
	while (ifile.good() ); 
	return id; 
}
	
int		CMultSec::AddFromFileODT (ifstream &ifileODT){return 0;}
int		CMultSec::AddFromFileODS (ifstream &ifileODS){return 0;}

CSec	*CMultSec::Idem ( CSec &sec )   // ----------------  CMultSec::            NotIdem  --- busqueda trivial de sec identicas -------------
{	if ( _MaxTgId >= 100  ) return nullptr ;
	long Lcs=sec.Len() ;											// len of candidate sec (to be in the list, with MaxId)
	long MaxErCS= long(ceil(float(Lcs*(100.0f-_MaxTgId) ) / 100.0f));					// min of not Id base to be in the list
	for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec de esta misma ms
	{	CSec &s = *CurSec() ; 
		if (s.Filtered()) continue;
		long l, MaxEr ;
		if (s.Len() < Lcs) 		{	l=s.Len() ;			MaxEr= long(ceil(l*(100-_MaxTgId)  / 100));		} 
		else 					{	l=Lcs ;				MaxEr= MaxErCS;						}
		long i=0 , Er=0 ;
		while ( ++i <=l ) 	if ( MaxEr   <   ( Er += ( s[i] != sec[i] )) )  break;
		if (i>l) return &s ;		
	}
	return nullptr ;
}
 CSec *	CMultSec::AddSec ( CSec *sec )
{	if (!sec) return nullptr ;
	_LSec.Add(sec);
	UpdateTotalsAdding ( sec );
	return sec;
}
 CSec *	CMultSec::InsertSec ( CSec *sec ) // insert 'link' sec ,  antes de 'Cur'
{	if (!sec) return nullptr ;
	_LSec.Insert(sec);
	UpdateTotalsAdding ( sec );
	return sec;
}
 CSec *	CMultSec::InsertSecAfter	( CSec *sec , CSec *preSec )			// insert 'link' sec ,  after preSec
{	
	if (!preSec) return AddSec (sec);
	if (!sec) return nullptr ;
	sec->InsertAfter( preSec);
	UpdateTotalsAdding ( sec );
	return sec;
}
void	CMultSec::UpdateTotalsAdding ( CSec *sec ) 
{	
	if (!sec || sec->_parentMS == this)										// no hay sec o ya estaba aqui
		return;
	CMultSec *parMS   =sec->_parentMS;					// /*._Get()*/
	CMultSec *My_parMS=     _parentMS;					// /*._Get()*/
	bool checkExtr(true) ; 
	CMultSec   *cp;

	if (parMS)
	{
	    cp=findComParent( parMS);
		parMS->_Local._NSec--	;								// la elimino de la ms orig. 
		//parMS->_Global._NSec -- ;								// 
		if (parMS->_Local._NSec)
			if (parMS->isLocExtreme(sec))
				RecalExtremes();
			else
				checkExtr=false;
		else
			checkExtr=false;

		for ( /*parMS=parMS->*/_parentMS;  parMS != cp &&  parMS ;  parMS=parMS->_parentMS)		// desde localizacion orig subiendo hasta parent comun
			{
				parMS->_Global._NSec -- ;								// elimino  de este total.
				if (checkExtr && parMS->_Global._NSec)
					if (parMS->isGlobExtreme(sec))
						RecalExtremes();
					else
						checkExtr=false;
				else
					checkExtr=false;
			}
	}else
		cp=nullptr;
	Add2LocalExtreme(*sec);
	for (My_parMS ; My_parMS!=cp && My_parMS; My_parMS=My_parMS->_parentMS)  // desde mi hacia arriba hasta el com parent anadiendo
	{
		if (checkExtr)
			My_parMS->Add2GlobalExtreme(*sec);
		else
		{
			parMS->_Global._NSec ++;								// sumo sus s a este total.
		}
	}	
	sec->_parentMS = (this) ;							//* std::weak_ptr<CMultSec> */
}


CMultSec   *CMultSec::findComParent( CMultSec *ms)
{
	if(!ms || ms==this) 
		return ms;
	std::stack<CMultSec*> myTree,		oTree;
	CMultSec			*myPms=this, *oPms=ms;
		myTree.push(myPms);
		oTree.push(oPms);

	do 
	{	myPms=myPms->_parentMS;
		myTree.push(myPms);
	}while (myPms);

	do 
	{	oPms=oPms->_parentMS;
		oTree.push(oPms);
	}while (oPms);

	do 
	{	if(  (oPms = myTree.top())   !=  oTree.top()  )
			return myPms;
		myPms= oPms;
		myTree.pop();
		oTree.pop();
	} while (!myTree.empty() && !oTree.empty() );
	return myPms;
}


CMultSec *	CMultSec::AddMultiSec ( CMultSec *ms )  //--------------------------------------    AddMultiSec    --------------------
{	if (!ms) return nullptr;	
	_LMSec.Add(ms);
	UpdateTotalsAdding ( ms );   // al llamar ya esta la ms movida fisicamente. Falta solo actualizar extremes
	return ms;
}
void	    CMultSec::UpdateTotalsAdding ( CMultSec *msec ) 
{	
	if (!msec || msec->_parentMS==this)					// no hay msec o ya estaba antes en una de mis subtrees inmediatas. 
		return;

	CMultSec *parMS   =msec->_parentMS;					// /*._Get()*/
	CMultSec *My_parMS=     _parentMS;					// /*._Get()*/
	bool checkExtr(true) ; 
	CMultSec   *cp;
	if (parMS)										// no es imprescindible. Anadido solo por claridad de intencion
	{	cp=findComParent( msec);
		for ( parMS;  parMS != cp   ;  parMS=parMS->_parentMS)			// desde localizacion orig subiendo hasta parent comun
			{
				parMS->_Global._NSec -= msec->_Global._NSec ;			// elimino sus s de este total.
				parMS->_Global._NMSec-= msec->_Global._NMSec + 1;		// elimino sus ms de este total.
				parMS->_Local._NMSec--	;								// la elimino de esta ms. 
				if (checkExtr && parMS->_Global._NSec)
					if (parMS->isGlobExtreme(msec))
						RecalExtremes();
					else
						checkExtr=false;
			}
	}else
		cp=nullptr;
	Add2LocalExtreme(*msec);
	for (My_parMS ; My_parMS!=cp && My_parMS; My_parMS=My_parMS->_parentMS)  // desde mi hacia arriba hasta el com parent anadiendo
	{
		if (checkExtr)
			My_parMS->Add2GlobalExtreme(*msec);
		else
		{
			My_parMS->_Global._NSec += msec->_Global._NSec ;			// sumo sus s a este total.
			My_parMS->_Global._NMSec+= msec->_Global._NMSec + 1;		// sumo sus ms a este total.
		}
	}
	msec->_parentMS = (this) ;							// std::weak_ptr<CMultSec> 

}

		CMultSec::~CMultSec ()				// funciona bien solo si la lista es "lineal"
{	_LSec.Destroy  ()	;
	_LMSec.Destroy ()	;
	Remove();
	// CSec		*_Consenso ;
}    


//void	CMultSec::RefreshExtremes( CMultSec *ms)
//{
//	if(!ms)
//		return;
//	if(ms->_NSec)
//
//}
	//	
	//while (parMS)									// subo por el tree hasta llegar al root
	//	if (parMS==this)						
	//		return;									// la sec estaba antes en una de mis subtrees. 
	//	else
	//auto TLen= msec->_TLen ;
	//auto TTm = msec->_TTm ;
	////auto TNMS
	//CMultSec *My_parMS=this ;    //_parentMS/*._Get()*/;		// 
	//while (My_parMS)								// subo por el tree hasta llegar al root
	//{	if ( ! My_parMS->_TNSec  )	
	//	{											
	//		My_parMS->_TLen= TLen ;				//   si ademas es la primera del todo inicializar los max totales
	//		My_parMS->_TTm = TTm ;
	//	}else
	//		{	
	//			My_parMS->_TLen.Expand( TLen) ;
	//			My_parMS->_TTm .Expand( TTm ) ;
	//		}	
	//	My_parMS->_TNSec +=msec->_TNSec;			// la elimino de este total. Que hacer con las max??
	//	My_parMS->_TNMSec+=msec->_TNMSec;
	//	TLen=My_parMS->_TLen ;
	//	TTm =My_parMS->_TTm ;
	//}
