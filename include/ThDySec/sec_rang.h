/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDySec\sec_rang.h
*
* @brief 
*/

#ifndef _SEC_RANG_H
#define _SEC_RANG_H

#pragma unmanaged	

#include <stdlib.h>
#include <fstream>
#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
namespace filesystem = std::tr2::sys; //std::experimental::filesystem


using namespace std;

#include "link.h"
#include "cod_deg.h"
#include "sec.h"

#include "th_dy_param.h"   // crear un nuevo par de fuente cpp con las cosas que nec los dos .h + sec.h
#include "common.h" 


class CRangBase : public NumRang<long> // ---------------------------------------   CRang	: AMPLIAR y mejorar !!!  ---------------------------------------
{public:	
	CRangBase (long i,long f) : NumRang<long>(i,f), _c(   f+1, i-1   )  { /* open();*/} 		// NumRang<long> _p;

public:
	CRangBase MatchRange() {return CRangBase( _c.Min(), _c.Max() );} // NO ME GUSTA ASI  !!!!!! pensar algo mas eficiente
	void open(void){  _c.Set(   Max()+1, Min()-1   )     ;}
						//    pi      pf                fi
											//----|++++++++|-----------------|--------			El rango inicial, y como va quedando
											// pfcur                        fi					El rango para calculo ("cur"), antes del comienzo	
											//---|----------|----------------|--------			Asi se queda si no hibridan entre si las sec en esta zona,
											//             picur            fi					y entonces "colapsa" el rango
											//            pfcur             fi					En este caso encontro 5 "cand" comunes	
											//--------|+++|------------------|--------			
											//       picur                  fi					
	//NumRang<long>	_p, _pcur ;
	void adjustCur(long p){ _c.Expand(p); } //if( _c.Min()>p ) 	_c.Min()=p;		if( _c.Max()<p )	_c.Max()=p;}
	bool isOpen  ()const{ return _c.Min() > _c.Max() ;}
	bool hasMatch()const{ return !isOpen   ()    ;}
	long NumMatch()const{ return _c.length() + 1;}
	void SchrinkToMatch(){Set(_c);}					// scheck if open????  
	//long length()const  { return Max() - Min() ;}
	void schift(int s) { Min()+=s;Max()+=s;_c.Min()+=s;_c.Max()+=s;}   //{ _pf+=s;_pi+=s;_pfcur+=s;_picur+=s;}
	bool addMatch(long i){ if (inRang(i)) {adjustCur(i);return true;} else return false;}

 protected:
	NumRang<long> _c;		//	long			_pi,_picur, _pf,_pfcur; //  _picur= _pf+1; _pfcur= _pi-1;}	
} ; 
class CRangBaseSchift /*: public CRangBase */
{	CRangBase &_R;
	long _sch;
public:
	CRangBaseSchift	(CRangBase& r, long sch) : _R(r), _sch(sch)
				{
					_R.schift(_sch);
				}
	void AddSchift	(int sch)
				{
					_sch+=sch;
					_R.schift(sch);
				}
	void ResetSchift()
				{
					_R.schift(-_sch);
					_sch=0;
				}
		~CRangBaseSchift()
				{
					ResetSchift();
				}
};



class CRang : public CRangBase// ---------------------------------------   CRang	: AMPLIAR y mejorar !!!  ---------------------------------------
{public:		//NumRang<long>	_p, _pcur ;

	       CRang (long i,long f) : CRangBase ( i, f),	 matchs(new int[length()+1])    { for (int i=0 ; i<= length() ; ++i) matchs[i]=0;} 
		   CRang (CRangBase  &R) : CRangBase ( R ),	     matchs(new int[length()+1])    { for (int i=0 ; i<= length() ; ++i) matchs[i]=0;} 
	int	   *matchs;
	      ~CRang  ( )  { delete []matchs; }
    void  IncrMatchs() { 	for (int pi_pos=_c.Min()    ; pi_pos <= _c.Max() ; pi_pos++ ) 	matchs[pi_pos - Min() ]++;		}

} ; 


//! -------------------------   CSecCand	-------------------------------
/// destinado a formar parte de una lista en un busq de sondas.
class CSecCand : public CLink 
{public:						
	long _NumPosCand, _NumCand  , _NumPosCandIn, _NumCandIn  ,_NumCandExact ;	
	CSec                             &_Sec ;	// ref a la sec, que ni se modifica ni se cambia de lugar
	std::vector<std::unique_ptr<CRang>> _rg;

    /// Inicializa array de posibles cand en esta sec.
	CSecCand(CSec &sec, 	SondeLimits sL		);
							//float	G_min	, float G_max ,					// en kcal ...
							//float	Tm_min	, float Tm_max ,  
							//int		L_min	, int L_max 

	long ColapseRangs(bool colapse=true);
};


/// destinado a formar parte de una lista en un alineamieto.  \todo REVISE!! 
class CSecAl : public CLink 
{public:
	CSec                 &_Sec     ; ///< ref a la sec, que ni se modifica ni se cambia de lugar
	std::vector<long> _inAlp_B ; ///< array que dice que base de la sec va en esa pos del Al (len=Al)
	std::vector<long> _inBp_Al ; ///< array que dice en que pos del Al va esa base de la sec (len=sec)

	CSecAl(CSec &sec, long LenAlign)
		: _Sec(sec), 
		  _inAlp_B(sec.Len()), 
		  _inBp_Al(LenAlign)
	   {}
	CSecAl(CSec &sec) 
		: _Sec(sec), 
		  _inAlp_B(sec.Len())
	  {}

	char *CopyAlignedSecChar(long Al_pBeg, long Al_pEnd, char *CharSec)	;///< CUIDADO !! asume suficiente espacio !!  \todo REVISE!! 
	char *GetAlignedSecChar (long Al_pBeg, long Al_pEnd) ;               ///< "regala" esta memoria, no olvide delete ! \todo REVISE!! 
};

    
		
#endif


