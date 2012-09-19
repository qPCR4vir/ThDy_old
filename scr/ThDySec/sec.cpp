//#include "stdafx.h"
#pragma unmanaged

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <math.h>
using namespace std ; 

#include "ThDySec/sec.h"
#include "ThDySec/link.h"
#include "ThDySec/common_basics.h" 

#define SEQUENCES_MAX_SIZE 100000
char *DNAStrandName[]=	{""		, "(c)", ""		, "(r)"	, "(i)", "(c)"		} ;
// enum DNAStrand		{plus	, minus, direct	, rev	, compl, rev_compl	} ;

//inline char *clone_c_str   (const char *str);
//inline char *clone_trim_str(const char *str);
//char *AddFileExt(const char *FileName, const char *Ext="")	;	// no olvide delete este pointer
char *AttachToCharStr(const char *CharStr, const char *Attach)			// no olvide delete este pointer
{	char *NewCharStr=new char[strlen(CharStr) +1+ strlen(Attach)];
	strcpy (NewCharStr,CharStr);	strcat (NewCharStr,Attach);
	return NewCharStr ;
}
char *ChangeCharStrAttaching(char *&CharStrToChange, const char *Attach) // CharStrToChange : debe ser una cadena que se creo con new, 
																	// y que sera borrada y vuelta a crear !!!
{	char *OldCharStr=CharStrToChange;
	CharStrToChange=AttachToCharStr(CharStrToChange, Attach)	;
	delete OldCharStr;
	return CharStrToChange ;
}
char *ChangeCharStrAttaching(char *&CharStrToChange, const int Attach)
{	char num[20];
	return ChangeCharStrAttaching(CharStrToChange, itoa (Attach, num, 10) ) ;
}

		CSecBasInfo::CSecBasInfo( long l):			_len( l ) , 			_NonDegSet( nullptr ), 				_GCp( 0 ),	
													_GrDeg( 1 ),			_NDB( 0 ),							_c(new Base[l+3])
{}

		CSec::CSec ( long l, CSaltCorrNN *NNpar) 		:	CSecBasInfo ( l ) ,			_NNpar(NNpar),		_b(new Base[l+3]),
															_SdS(new Entropy[l+3]),		_SdH(new Energy[l+3])

{	//auto_ptr<Base>		c(new Base[l+3]);	auto_ptr<Base>		b(new Base[l+3]);	auto_ptr<Entropy> 	SdS(new Entropy[l+3]);	
	//auto_ptr<Energy>	SdH(new Energy[l+3]);	_c=c.release() ;_b=b.release() ;	_SdS=SdS.release() ;	_SdH=SdH.release() ;
	//							assert(_c && _b && _SdS && _SdH);	/* Hace algo ???? */ 
};  

		CSecBasInfo::CSecBasInfo (int id, const char *nam, char *clas) 
								:_ID		( id ), 	_NonDegSet( nullptr ), 				_GCp( 0 ),	
								_GrDeg( 1 ),			_NDB( 0 ),							_name( clone_trim_str	(nam )),
								_Clas(clas ? clone_c_str (clas) : nullptr )
{}
		CSec::CSec (const char *sec, int id, const char *nam, CSaltCorrNN *NNpar, long lmax, long secBeg, char *clas, float conc) 
			:	CSecBasInfo ( id, nam, clas) ,		_NNpar	( NNpar),				_Conc	( conc )			
							//_ID		( id ), 		_GCp	( 0 ), 	_GrDeg	( 1 ), 	_NDB	( 0 ),	_NonDegSet( 0 )
{		//assert (nam);		_name	= clone_trim_str	(nam );					auto_ptr<char> ap_name(_name);
		//					_Clas	= clas ? clone_c_str (clas) : nullptr ;		auto_ptr<char> ap_Clas(_Clas);
		if (secBeg<1) secBeg=1;

		assert (sec);		
		long sl	 ;		// seguro?????     "Midamos" la sec. Long de sec orig (sl) y  num de bases validas despues de secBeg (_len) 
		if (lmax)			// limita la cant de bases originales a leer despues de las primeras secBeg-1 bases 
		{	for (  sl= 0 ; sec[sl] && sl<secBeg-1 ; sl++)  ;		// salta las primeras secBeg-1 bases 
			long secEnd=sl+lmax ;
			for (_len= 0 ; sec[sl] && sl<secEnd   ; sl++) if( is_degbase	[Base (sec[sl])] ) _len++ ;	// salta no-bases 
		}else
		{	for (  sl= 0 ; sec[sl] && sl<secBeg-1 ; sl++)  ;											// salta las primeras secBeg-1 bases 
			for (_len= 0 ; sec[sl]  			  ; sl++) if( is_degbase	[Base (sec[sl])] ) _len++ ;	// salta no-bases 
		}
													      // '$' principio y fin de Kadelari.=" TGCA$", + '\0'
		_c   = new Base   [_len+3];      			auto_ptr<Base>	  ap_c  (_c  );
		_b   = new Base   [_len+3];      			auto_ptr<Base>    ap_b  (_b  );
		_SdS = new Entropy[_len+2];      			auto_ptr<Entropy> ap_SdS(_SdS);
		_SdH = new Energy [_len+2];      			auto_ptr<Energy>  ap_SdH(_SdH);	//assert(_c && _b && _SdS && _SdH); /*{err<< "Demaciadas sec!!!"; return 0;}*/
		register Base a_1, a;
		for (a=0; a<n_dgba; a++)	_Count[a]=0 ;

		long curPos	= 0 ;//descarta no bases
		_c  [0]	= basek[n_basek-1] ; // '$' principio y fin de Kadelari.=" TGCA$"
		_b  [0]	= n_basek-1 ; 	  	
		_SdS[0] = _NNpar->GetInitialEntropy(); // Solo dep de las conc, no de la sec. Ajustar primero la conc del target, y despues volverla a poner,?? 
		_SdH[0] = 0 ;							// y comprobar que se hacen los calculos necesarios


		long j;
		for (j=secBeg-1	; j<sl  ;j++)   //solo para la primera base, descarta la primeras secBeg-1 bases y no-bases anteriores
			if ( a_1=is_degbase	[ sec[j] ] ) 				//if ( ++b_ant < secBeg ) continue ; -significaria contar solo bases validas antes de beg
			{	_GCp	+= is_GC	[a_1] ;
				_GrDeg	*= grad_deg	[a_1] ;
				_Count  [  db2nu	[a_1]]++ ;
				if (grad_deg[a_1] >1) _NDB++ ;
				_c[1] =	a_1 ;
				_b[1] = a_1 =bk2nu[is_base[a_1]] ;						// que hacer si en la sec hay bases deg que Kadelari no considera?
				curPos= 1 ;
				break ;
			}	

		for (j++; curPos <= _len && j<sl ;j++)						// suficiente    curPos <= _len   ???
			if ( a=is_degbase	[ sec[j] ] ) 	
			{	curPos++ ;
				_GCp	+= is_GC		[a] ;						// 1-G or C, 0-lo demas.      Y que con las bases deg ??????????????????
				_GrDeg	*= grad_deg		[a] ;
				_Count  [  db2nu		[a] ]++ ;
				if (grad_deg[a] >1) _NDB++ ;
				_c  [curPos  ] = a ;
				_b  [curPos  ] = a =bk2nu[is_base[a]] ;				// que hacer si en la sec hay bases deg que Kadelari no considera?
				_SdS[curPos-1] = _SdS[curPos-2] + NNpar->GetSelfEntr (	a_1 , a);
				_SdH[curPos-1] = _SdH[curPos-2] + NNpar->GetSelfEnth (	a_1 , a);
				a_1 = a ;
			}	
		_c[_len+1]= basek[n_basek-1] ; // '$' principio y fin de Kadelari.=" TGCA$", + '\0'
		_b[_len+1]=	    n_basek-1  ; 	  	
		_c[_len+2]= 
		_b[_len+2]=	    0  ; 	  	
		_GCp	= _GCp*100/_len ;	
		_Tm.Set( NNpar->CalcTM( _SdS[_len-1], _SdH[_len-1]) ) ; //_maxTm = _minTm =

		//ap_name.release();ap_Clas.release();
		ap_c.release();ap_b.release();ap_SdS.release();ap_SdS.release();ap_SdH.release();


		//for (nl=0;		isalpha([nl])
		//			||	ispunct(nam[nl]) 
		//			||	isdigit(nam[nl])		;nl++ ) ;
		////{	if ( nam[nl]==0 )		break;
		////	if (isspace(nam[nl]) )	break;
		////}
		//		assert ( ( (cout << _name << "\t" << _c << "\t" << (_Tm - 273) << " °C" << "\n" ) , 1 ) ) ;
		//	NNpar->_ConcSd=c1 ; NNpar->_ConcTg=c2	;	// y comprobar que se hacen los calculos necesarios
		// ver como se usa concPrimers,concSequences
		//		if (_Conc != -1) _Conc = _NNpar->_ConcSd ;	// concSequences ;	// ajustar primero la conc del target, y despues volverla a poner, 
//		double c1=_NNpar->_ConcSd, c2=_NNpar->_ConcTg	;	// y comprobar que se hacen los calculos necesarios
//		// NNpar->Ct1 = NNpar->Ct2	= Conc	;	// por ahoa uso Ct de primers y Ct de target, ambos, nada cambia.	
//		// ver como se usa concPrimers,concSequences,  USO la conc de PRIMERS
		//if	(l==0)	l =     sl-secBeg+1   ;
		//else		l = ( l<sl-secBeg+1 ? l : sl-secBeg+1 ) ;
}

CSec  * CSec::CreateCopy(DNAStrand strnd) // strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
{	Base *s=GetCopy_charSec(strnd); 
	char *n; 
	CSec *newS=new CSec( (char*)s, 
						_ID,				// el mismo ????
						n=AttachToCharStr(_name, DNAStrandName[strnd])	,
						_NNpar	,
						0,1,
						_Clas,
						_Conc
						);
	delete []s;delete []n;
	return newS;
}

//CSec  * CSec::CreateCopy(DNAStrand strnd) // strnd=direct...crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
//{	Base *s=GetCopy_charSec(strnd); 
//	char *n; 
//	CSec *newS=new CSec( (char*)s, 
//						_ID,				// el mismo ????
//						n=AttachToCharStr(_name, DNAStrandName[strnd])	,
//						_NNpar	,
//						0,1,
//						_Clas,
//						_Conc);
//	delete []s;delete []n;
//	return newS;
//}

Base  *	CSecBasInfo::Copy_charSec(Base *charSecHier,long InicBase, long EndBase, DNAStrand strnd)//DNAStrand strnd=direct)
{	if ( EndBase< 1 || _len <EndBase ) EndBase=_len; 
	long l=EndBase-InicBase+1 ; charSecHier[l]=0 ;
	if (l>=0) 
	//assert(l>=0);
	switch (strnd)
	{	case DNAStrand::plus :
		case DNAStrand::direct:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=_c[p];				break;
		case DNAStrand::compl:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=c_degbase[_c[p]];	break;
		case DNAStrand::rev:		for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=_c[p];				break;
		case DNAStrand::minus:
		case DNAStrand::rev_compl:	for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=c_degbase[_c[p]];	break;

		default : return 0;
	}
	return charSecHier ;
}
Base  *	CSecBasInfo::GetCopy_charSec(DNAStrand strnd)
{	return GetCopy_charSec(1, _len, strnd);
}
Base  *	CSecBasInfo::GetCopy_charSec(long InicBase, long EndBase, DNAStrand strnd)       // recuerde los $...$, aqui se cuentan, 
{	if ( InicBase< 1 )					InicBase=1;
	if ( EndBase< 1 || _len <EndBase )	EndBase=_len;
	long l=EndBase-InicBase+1 ;
	assert(l>=0);
	Base *charSecHier=new Base[l+1];						 // asi como InicBase y EndBase inclusive!!
	assert(charSecHier);	

	return Copy_charSec(charSecHier, InicBase, EndBase, strnd)  ;
}

float	CSec::Tm	(long pi, long pf)const
{	assert( 0< pi/* && pi<=pf*/); 		assert( /*0< pi && */pi<=pf+1);		assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcTM(	_SdS[pf-1]					 ,		_SdH[pf-1]			 );
					return _NNpar->CalcTM(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2]);
}

	//float		G	(long pi, float Ta)const{return G(pi,_len, Ta);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
	//float		G	(float Ta )const	    {return G(1 ,_len, Ta);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
float	CSec::G	(long pi, long pf, float Ta)const				// G de la sonda con sec desde pi hasta pf, inclusive ambas!! 
{	assert( 0< pi/* && pi<=pf*/);		assert( /*0< pi && */pi<=pf+1);		assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcG(	_SdS[pf-1]					 ,		_SdH[pf-1]			 , Ta);
					return _NNpar->CalcG(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2], Ta);
} 

//float		G	(long pi)const	{return G(pi,_len);}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
//float		G	( )const		{return G(1,_len) ;}   // G de la sonda con sec desde pi hasta el final, inclusive ambos!!
float	CSec::G	(long pi, long pf) const
{	assert( 0< pi/* && pi<=pf*/);		assert( /*0< pi && */pi<=pf+1);		assert(pf<=_len);
	if (pi==1) 		return _NNpar->CalcG(	_SdS[pf-1]					 ,		_SdH[pf-1]			 );
					return _NNpar->CalcG(	_SdS[pf-1]-_SdS[pi-2]+_SdS[0],		_SdH[pf-1]-_SdH[pi-2]);
} 

		CSec::~CSec () 
{	delete [] _SdS ;
	delete [] _SdH ;
	delete [] _c ;
	delete [] _b ;
	delete [] _Clas ;
	delete [] _name ;
	if (_NonDegSet ) if (!_NonDegSet->Prev() && ! _NonDegSet->Next() ) delete _NonDegSet ;
	Remove();
	// en otro caso, donde borrar _NonDegSet ????. Lo borra la lista en la que esta insertado
}    

void	CSec::CorrectSaltOwczarzy() 
{	for (long j=2; j<=_len;j++)
		_SdS[j-1] = _SdS[j-2] + _NNpar->GetCorrectSaltOwczarzySelfEntr ( _b[j-1] , _b[j] , _GCp );

	_Tm.Set( _NNpar->CalcTM( _SdS[_len-1], _SdH[_len-1]) ) ; //_maxTm = _minTm =	_Tm  = _maxTm = _minTm =  ;
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
		_Tm = _NonDegSet->_Tm ; //	_maxTm = _NonDegSet->_maxTm ; _minTm = _NonDegSet->_minTm ;	_Tm    = (_maxTm + _minTm )/2 ;
		//assert ( ( (cout << "Post Deg set generation: "<< _name << "\t" << _c << "\t" 
		//			 << "Tm=" << (_minTm - 273)  << " °C"
		//			 << " ("  << (_Tm - 273)     << " °C"  << ") "
		//			          << (_maxTm - 273)  << " °C"  << "\n" ) , 1 ) ) ;

	return _NonDegSet;
}

CSec *	CSec::CopyFirstBases(long pos) 
{	
	CSec *sec = new CSec ( _len, _NNpar) ;
	assert(sec);
	sec->_name = new char[strlen(_name)+1];  assert(sec->_name); // al "final" se cambia anadiendo num _NSec
	strcpy( sec->_name, _name );

	if (_Clas!=NULL) 
	{	sec->_Clas = new char[strlen(_Clas)+1];	     assert(sec->_Clas );
		strcpy( sec->_Clas, _Clas );
	} else sec->_Clas=NULL ;

	sec->_GCp	= _GCp ;
	sec->_GrDeg	= _GrDeg ;
	for (Base b=0	  ; b<n_dgba   ; b++)	sec->_Count[b]= _Count[b] ;
	sec->_NDB = _NDB ;

	for (long i=0; i<=pos; i++)
	{	sec->_c[i] = _c[i] ;
		sec->_b[i] = _b[i] ;
		sec->_SdS[i] = _SdS[i] ;
		sec->_SdH[i] = _SdH[i] ;
	}
	return sec ;
}

CSec *	CSec::GenerateNonDegVariant ( CSec *s, long pos, Base ndb) // crear variante que inserte las variantes en la lista dada.????
{	Base pre ; Base b_or,  cur ;									// para eso anadir ultimo parametro CMultiSec &ndg=_nds
	CSec *sec ;
	if (pos==0) 
	{	sec = s->CopyFirstBases(0);   // caso esp: ni pos -1, ni muto a ndb
		pre = sec->_b[0];
	}
	else 
	{
		// copia la parte inicial, que ya esta bien
		sec = s->CopyFirstBases(pos-1);     // anadirle algo al nombre ??

		// cambia la base deg en la 'pos' a 'ndb' - no deg base --> MUTACION  !!
		Base b_or = _c[pos], 
			  cur = bk2nu[ndb];	
			  pre = sec->_b[pos-1];
			  
		sec->_c[pos] =		ndb ;
		sec->_b[pos] =		cur ;
		sec->_GCp	+= float ((is_GC[ndb]-is_GC[b_or]))/_len ;
		sec->_NDB-- ;
		sec->_GrDeg *= (grad_deg[ndb] / ( grad_deg[b_or] ? grad_deg[b_or] : 1) ) ;	
		
		if ( pos > 1 ) 	
		{	sec->_SdS[pos-1] = sec->_SdS[pos-2] + _NNpar->GetSelfEntr (	pre  ,	cur);
			sec->_SdH[pos-1] = sec->_SdH[pos-2] + _NNpar->GetSelfEnth (	pre  ,	cur);
		}
		_Count[db2nu[ ndb]]++ ;
		_Count[db2nu[b_or]]-- ;

		pre = cur ;
	}
	// continua actualizando el resto
	long i;
	for ( i=pos+1; i<=_len; i++)
	{	b_or = _c[i] ;
		Base n = grad_deg[b_or];
		if (n<2)
		{	sec->_b[i] = cur= _b[i];
		}
		else 		// hasta que encuentra la siguiente bas deg
		{	for (Base d=0; d < n-1 ; d++)	// recorre las bas no deg de la base deg b_or, menos la ultima
			{
				_NonDegSet->AddSec( GenerateNonDegVariant ( sec, i, dg2ba [ db2nu[b_or]  ][d] )) ;
			}

			ndb			 = dg2ba [  db2nu[b_or]  ][n-1];   // aqui me quedo con la ultima variante para seguir
			sec->_GCp	+= (is_GC[ndb]-is_GC[b_or])/_len;
			sec->_GrDeg	/= grad_deg[b_or] ;	
			sec->_b[i] = cur=  bk2nu[ndb];
			_Count[db2nu[ ndb]]++ ;
			_Count[db2nu[b_or]]-- ;
			sec->_NDB-- ;
			b_or = ndb ;      
		} 
		sec->_c[i] = b_or ;
		if ( i > 1 ) 	
		{	sec->_SdS[i-1] = sec->_SdS[i-2] + _NNpar->GetSelfEntr (	pre  ,	cur);
			sec->_SdH[i-1] = sec->_SdH[i-2] + _NNpar->GetSelfEnth (	pre  ,	cur);
		}
		pre = cur ;
	}
	sec->_c[i]= basek[n_basek-1] ; // '$' principio y fin de Kadelari.=" TGCA$"
	sec->_b[i]=	    n_basek-1  ; 
	sec->_c[i+1]= 
	sec->_b[i+1]= 0 ;

	sec->_Tm.Set(_NNpar->CalcTM( sec->_SdS[_len-1], sec->_SdH[_len-1])) ;  // usar _len or i ?????= sec->_maxTm = sec->_minTm 

	ChangeCharStrAttaching(sec->_name, _NonDegSet->_NSec);
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

{	long fi, i0;			assert (_rg);	
	for (fi=0; fi< sL._L.Min() ; fi++)  _rg[fi]=0;		// se salta las primeras pos
														// al comienzo fi = L_min	
	for (	; fi<=sec.Len(); fi++)						// fi - final base of candidate, recorre toda la sec
	{	
		long        pi = fi - sL._L.Max() +1	; if (pi < 1 ) pi=1;
		CRangBase R(pi	,fi - sL._L.Min() +1 )  ;					   assert( fi>R.Max() );		assert( pi<=R.Max() );

		for (i0=R.Min() ; i0<=R.Max() ; ++i0 )
		{	if ( sL._Tm.inRang( sec.Tm(i0,fi) )    &&    sL._G.inRang( sec.G(i0,fi) ) ) 	
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

int		CMultSec::AddFromFile (const char *file)		// return la cantidad de sec add --------------------  AddFromFile   -------------------
{		if (  _SecLim.Max() <= _SecLim.Min() ) _SecLim.SetMax(0) ; // if ( _SecEnd<=_SecBeg) _SecEnd=0 ;
	ifstream ifile( file ); assert (ifile);	if ( ! ifile ) {cerr << "File "<< file <<" could not be opened "<<endl ; return 0;} 
	int j=0;
	char c1;
	ifile>>skipws  >> c1;
	if ( ! ifile.good() ) {cerr << "File "<< file <<" could not be opened "<<endl ; return 0;} 

	if( c1 =='>' ) 																		return AddFromFileFASTA (ifile);
					// esto es FASTA, si no  BLAST o GB o ...
	if (c1 =='<' )
	{	string xml_DOCTYPE ;
		getline (ifile, xml_DOCTYPE,'>') ;   // <?xml version="1.0"?>
		getline (ifile, xml_DOCTYPE,'>') ;   // <!DOCTYPE BlastOutput PUBLIC "-//NCBI//NCBI BlastOutput/EN" "NCBI_BlastOutput.dtd">
		if ( ! ifile.good() ) {cerr << "File "<< file <<" could not be opened "<<endl ; return 0;} 

		if		(string::npos!=xml_DOCTYPE.find("DOCTYPE BlastOutput") )				return AddFromFileBLAST (ifile);
		else if (string::npos!=xml_DOCTYPE.find("DOCTYPE Bioseq-set" ) )				return AddFromFileGB (ifile);
		return 0;
	}
	if (c1 =='L' )	{	ifile.putback(c1);												return AddFromFileGBtxt (ifile);	}
	return 0;   // cerr unknow format		
}	 

int		CMultSec::AddFromFileFASTA (ifstream &ifile)  // "modernizar" con string -------------------    AddFromFileFASTA   ------------
{	int j=0 ;	//long l= (_SecEnd ? _SecEnd-_SecBeg +1 : 0) ;

	do	{	string Fasta_NAME_DESCR  ;
			getline (ifile, Fasta_NAME_DESCR); //<?xml version="1.0"?>//char line [SEQUENCES_MAX_SIZE];	
												//line[0]='>';//ifile.getline (line, SEQUENCES_MAX_SIZE) ;
			if ( ! ifile.good() )  return 0;            // cerr FASTA sec not readed 
			//int nl;		// name len
			//for (nl=0;	nl < Fasta_NAME_DESCR.length() && (	// HORROR - habia olvidado comprobar esta long Esto seguramente se puede hacer mas elegante con string funtion
			//				isalpha(Fasta_NAME_DESCR[nl])		// 	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz			isgraph ??   
			//			||	ispunct(Fasta_NAME_DESCR[nl])		//  !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~								poner solo '._-'    ?????    
			//			||	isdigit(Fasta_NAME_DESCR[nl])  );	//  01234567890
			//	 nl++ ) ; 
			//
			//string Fasta_NAME = Fasta_NAME_DESCR. .substr(0,nl);	//char *name = new char[nl+1];assert(name);//strncpy(name,Fasta_NAME_DESCR.c_str(), nl );name[nl]=0;
			string Fasta_NAME = Fasta_NAME_DESCR.substr(0,Fasta_NAME_DESCR.find_first_not_of(
									"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890!#$()*+-./<=>@[]^_{|}~"    ) );

  			string Fasta_SEC ;					//	char Seq [SEQUENCES_MAX_SIZE];
			getline (ifile, Fasta_SEC,'>') ;	//	ifile.getline( Seq, SEQUENCES_MAX_SIZE,'>'); 

			if ( Fasta_SEC.length() >= _SecLim.Min() )		//  if ( _SecBeg <= ifile.gcount ( ) )
			{	if ( _SecLim.Max() ) Fasta_SEC=Fasta_SEC.substr(_SecLim.Min()-1, _SecLim.Max()-_SecLim.Min()+1 ) ;//if (_SecEnd) Seq[_SecEnd]=0 ;
				else				 Fasta_SEC=Fasta_SEC.substr(_SecLim.Min()-1 ) ;					// hasta el final
				CSec *sec=  new CSec(Fasta_SEC.c_str() , _NSec, Fasta_NAME.c_str() , _NNPar); assert(sec);
						
				if ( sec->Len() >= _SecLim.Min()  && NotIdem(*sec) )		{	j++;		AddSec ( sec ) ;	}
				else delete sec;
			}
			if (!ifile.good()) 				break ;		//			ifile.getline( line, SEQUENCES_MAX_SIZE);
		}
	while (ifile.good() );
	return j;	
}

int		CMultSec::AddFromFileBLAST (ifstream &ifile) // ----------------  CMultSec::            AddFromFileBLAST  -----------------------------
{	unsigned int	_BlastOutput_query_len ;		// x todos los "hits"
	int			 id=0;
	string xml_line ;

	do  {	getline (ifile, xml_line,'>') ;			if ( ! ifile.good() ) return 0; }   // BLAST format error
	while  (string::npos==xml_line.find("BlastOutput_query-len") ); ifile>>_BlastOutput_query_len;//  <BlastOutput_query-len>267</BlastOutput_query-len>
	
	do {	unsigned int	_Hit_num=0 ;			// para cada hit
			char		*	_Hit_id=nullptr ;				
			char		*	_Hit_def=nullptr ;				
			char		*	_Hit_accession=nullptr	;
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
			char *			_Hsp_midline=nullptr ;
			bool			_FormatOK=0 ;
		char		*sec=nullptr;						// para CSec

		char		*nam=nullptr;
		long		 l=0;
		char		*clas=nullptr;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  // BLAST format error
			while  (string::npos==xml_line.find("Hit_num") ) ; ifile>>_Hit_num;					//  <Hit_num>1</Hit_num>
			
			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; } 	//<Hit_id>gi|84028434|gb|DQ318020.1|</Hit_id> 
			while  (string::npos==xml_line.find("Hit_id") );
			getline (ifile, xml_line,'<') ;				_Hit_id=new char[xml_line.length()+1] ;
			xml_line.copy(_Hit_id,xml_line.length()) ;	_Hit_id	[xml_line.length()]=0;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //<Hit_def>Wets NIle virus strain ArB3573/82, complete genome</Hit_def>
			while  (string::npos==xml_line.find("Hit_def") );
			getline (ifile, xml_line,'<') ; _Hit_def=new char[xml_line.length()+1] ;
			xml_line.copy(_Hit_def,xml_line.length()) ;	_Hit_def[xml_line.length()]=0;	

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }		//<Hit_accession>DQ318020</Hit_accession>
			while  (string::npos==xml_line.find("Hit_accession") );
			getline (ifile, xml_line,'<') ; _Hit_accession=new char[xml_line.length()+1] ;
			xml_line.copy(_Hit_accession,xml_line.length()) ;	_Hit_accession[xml_line.length()]=0;	

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hit_len>11048</Hit_len>
			while  (string::npos==xml_line.find("Hit_len") );ifile>>_Hit_len;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_bit-score>482.786</Hsp_bit-score>
			while  (string::npos==xml_line.find("Hsp_bit-score") );ifile>>_Hsp_bit_score;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_score>534</Hsp_score>
			while  (string::npos==xml_line.find(  "Hsp_score"  ) );ifile>>   _Hsp_score   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_evalue>3.71782e-133</Hsp_evalue>
			while  (string::npos==xml_line.find(  "Hsp_evalue"  ) );ifile>>   _Hsp_evalue   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_query-from>1</Hsp_query-from>
			while  (string::npos==xml_line.find(  "Hsp_query-from"  ) );ifile>>   _Hsp_query_from   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_query-to>267</Hsp_query-to>
			while  (string::npos==xml_line.find(  "Hsp_query-to"  ) );ifile>>   _Hsp_query_to   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_hit-from>9043</Hsp_hit-from>
			while  (string::npos==xml_line.find(  "Hsp_hit-from"  ) );ifile>>   _Hsp_hit_from   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_hit-to>9309</Hsp_hit-to>
			while  (string::npos==xml_line.find(  "Hsp_hit-to"  ) );ifile>>   _Hsp_hit_to   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_query-frame>1</Hsp_query-frame>
			while  (string::npos==xml_line.find(  "Hsp_query-frame"  ) );ifile>>   _Hsp_query_frame   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_hit-frame>1</Hsp_hit-frame>
			while  (string::npos==xml_line.find(  "Hsp_hit-frame"  ) );ifile>>   _Hsp_hit_frame   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_identity>267</Hsp_identity>
			while  (string::npos==xml_line.find(  "Hsp_identity"  ) );ifile>>   _Hsp_identity   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_positive>267</Hsp_positive>
			while  (string::npos==xml_line.find(  "Hsp_positive"  ) );ifile>>   _Hsp_positive   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_gaps>0</Hsp_gaps>
			while  (string::npos==xml_line.find(  "Hsp_gaps"  ) );ifile>>   _Hsp_gaps   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  //  <Hsp_align-len>267</Hsp_align-len>
			while  (string::npos==xml_line.find(  "Hsp_align-len"  ) );ifile>>   _Hsp_align_len   ;			

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }	// <Hsp_hseq>TACAACATGATGGGAAAGAGAGAGAAGAAG
			while  (string::npos==xml_line.find(  "Hsp_hseq"  ) );
			getline (ifile, xml_line,'<') ;         sec=new char[xml_line.length()+1] ;
			xml_line.copy(sec,xml_line.length()) ;	sec[xml_line.length()]=0;	

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }	//       <Hsp_midline>|||||||||||||||||||||||||||||||||||||||||
			while  (string::npos==xml_line.find(  "Hsp_midline"  ) );
			getline (ifile, xml_line,'<') ;         _Hsp_midline=new char[xml_line.length()+1] ;
			xml_line.copy(_Hsp_midline,xml_line.length()) ;	_Hsp_midline[xml_line.length()]=0;	

			// long SecBeg = _SecBeg - _Hsp_query_from +1 ;			// if (_SecBeg >= _Hsp_query_from)
			//if ( (_SecBeg		<= _Hsp_query_to) && ( (!_SecEnd)		 || _SecEnd		  >=_Hsp_query_from) ) // _SecEnd=0 significa no recortar la sec.
			if ( (_SecLim.Min() <= _Hsp_query_to) && ( (!_SecLim.Max())  || _SecLim.Max() >=_Hsp_query_from) ) // _SecLim.Max()=0 significa no recortar la sec.
			{	CSecBLASTHit *secH=  new CSecBLASTHit(      _BlastOutput_query_len ,
														// para cada hit
														_Hit_num ,
														_Hit_id ,				
														_Hit_def ,				
														_Hit_accession	,
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
														_Hsp_midline ,
														_FormatOK ,
														sec		,_SecLim, // _SecBeg, _SecEnd,
														id,			//Hit_num   ???		//	char		*	nam,		Hit_def
														_NNPar /*,  	//	long l=0,	(Hit_len ---> NO ) !!!  -->_Hsp_align_len -OK clas,	conc*/
														);
				if(secH->Len() > 2  && NotIdem(*secH))     		{	id++;	AddSec ( secH ) ;	}
				else delete secH;
			}
			delete []sec ;
		}
	while (ifile.good() ); 
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

			if(secGBtxt->Len() > 2  && NotIdem(*secGBtxt))     //   == Seq_inst_length ??
			{	id++;		AddSec ( secGBtxt ) ;	}
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
			if(secGB->Len() > 2 && NotIdem(*secGB)  )     	{	id++;		AddSec ( secGB ) ;		}
			else delete secGB;
		}
	while (ifile.good() ); 
	return id; 
}
	
int		CMultSec::AddFromFileODT (ifstream &ifileODT){return 0;}
int		CMultSec::AddFromFileODS (ifstream &ifileODS){return 0;}

bool	CMultSec::NotIdem ( CSec &sec )   // ----------------  CMultSec::            NotIdem  --- busqueda trivial de sec identicas -------------
{	if ( 100 == _MaxTgId ) return true ;
	long Lcs=sec.Len() ;											// len of candidate sec (to be in the list, with MaxId)
	long MaxErCS= ceil(float(Lcs*(100.0f-_MaxTgId) ) / 100.0f);					// min of not Id base to be in the list
	for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec
	{	CSec &s = *CurSec() ; 
		long l, MaxEr ;
		if (s.Len() < Lcs) 		{	l=s.Len() ;			MaxEr= ceil(l*(100-_MaxTgId)  / 100);		} 
		else 					{	l=Lcs ;				MaxEr= MaxErCS;						}
		long i=0 , Er=0 ;
		while ( ++i <=l ) 	if ( MaxEr   <   ( Er += ( s[i] != sec[i] )) )  break;
		if (i>l) return false ;		
	}
	return true ;
}
void	CMultSec::AddSec ( CSec *sec )
{	if (!sec) return ;
	_LSec.Add(sec);
	UpdateTotals ( sec );
}
void	CMultSec::InsertSec ( CSec *sec ) // insert 'link' sec ,  antes de 'Cur'
{	if (!sec) return ;
	_LSec.Insert(sec);
	UpdateTotals ( sec );
}
void	CMultSec::UpdateTotals ( CSec *sec ) // insert 'link' sec ,  antes de 'Cur'
{	_NSec++; _TNSec ++ ;

	if ( _NSec == 1 )									//   si esta es la primera sec inicializar los max
	{	_MaxLen = sec->Len() ;
		_Tm     = sec->_Tm ; //_maxTm  = sec->_maxTm ; 	_minTm  = sec->_minTm ;
		if ( _TNSec == 1 )	_TMaxLen = _MaxLen ;
	} else
	{	if ( _MaxLen  < sec->Len()  ) 		 _MaxLen  = sec->Len() ;
		if ( _TMaxLen < _MaxLen    ) 		 _TMaxLen = _MaxLen ;
		_Tm.Expand(sec->_Tm  ) ;//	if ( _maxTm < sec->_maxTm  )  _maxTm = sec->_maxTm ; if ( _minTm > sec->_minTm  )  _minTm = sec->_minTm ;
	}
}
void	CMultSec::AddMultiSec ( CMultSec *ms )  //--------------------------------------    AddMultiSec    --------------------
{	if (!ms) return ;	
	_LMSec.Add(ms);

	if ( _TNSec == 0 )				_TMaxLen = ms->_TMaxLen ;  // estas son las primeras sec en esta lista
	if ( _TMaxLen < ms->_TMaxLen) 	_TMaxLen = ms->_TMaxLen ;
	_NMSec++; 
	_TNMSec += ms->_TNMSec + 1 ;
	_TNSec  += ms->_TNSec ;
}

		CMultSec::~CMultSec ()				// funciona bien solo si la lista es "lineal"
{	_LSec.Destroy  ()	;
	_LMSec.Destroy ()	;
	Remove();
	// CSec		*_Consenso ;
}    



//char	*CSec::CreateTEMPORALcomplementary(long InicBase, long EndBase)       // recuerde los $...$, aqui se cuentan, 
//					{	if ( EndBase< 1 || _len <EndBase ) EndBase=_len;
//						long l=EndBase-InicBase ;
//						assert(l>=0);
//						char *temp=new char[l+1];						 // asi como InicBase y EndBase inclusive!!
//						assert(temp);						
//						for(long p=InicBase; p<=EndBase;p++) temp[p]=c_degbase	[_c[p]];
//						temp[l]=0; return temp;
//					}
										// al comienzo fi = L_min
	//for (	; fi<= sL._L.Max() ; fi++)  // fi - final base of candidate, recorre toda la sec
	//{	
	//	long pi		=1		, pf		=fi-L_min+1 ;   // al comienzo pf = 1
	//	long picur	=pf+1	, pfcur		=pi	;			// al comienzo picur = 2 , pfcur = 1

	//	for (i0=pi ; i0<=pf; i0++ )						// al comienzo i0 = 1 , pf = 1 , fi = L_min
	//	{	float Tm=sec.Tm(i0,fi), G=sec.G(i0,fi);
	//		if ( Tm_min <= Tm && Tm <= Tm_max    &&    G_min <= G && G <= G_max ) 
	//		{	if(picur>i0)
	//				picur=i0;
	//			if(pfcur<i0)
	//				pfcur=i0;
	//			 _NumCandExact++; //Seria lo correcto, pero da problemas cuando existe una "burbuja" de Tm, cosa que un "rango" no puede considerar, 
	//			//             tendria que hacerce con un conjunto. 
	//			//             Osea estoy dejando dentro del rango, interiormente la posibilidad de aceptar sondas con Tm fuera de rango
	//		}
	//	}
	//	if (picur<=pfcur)
	//	{		_NumPosCand++ ; 
	//			_rg[fi]=new CRang (picur,pfcur+1, pfcur,picur);
	//			_NumCand+= (_rg[fi]->_pf - _rg[fi]->_pi + 1);
	//	} else	_rg[fi]=0 ;
	//}
		//CSecCand::CSecCand(CSec &sec, 	SondeLimits sL
		//					//float	G_min	, float G_max ,					// en kcal ...
		//					//float	Tm_min	, float Tm_max ,  
		//					//int		L_min	, int L_max 
		//					)