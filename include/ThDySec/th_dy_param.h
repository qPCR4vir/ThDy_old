#ifndef _TH_DY_PARAM_H
#define _TH_DY_PARAM_H

#include <math.h>
#include <string.h>
#include <iostream>
//#include <iosfwd>

#include "cod_deg.h"
#include "common_basics.h" 
using namespace std;

	// empezar por poner nombre a cosas como forbidden_enthalpy, iloop_entropy, bloop_entropy, bloop_enthalpy
	// y de paso verificar todos estos datos. Dar posibilidad de ajustar solo algunos parametros (correcciones)

class COriNN  
{	Entropy			_oridS[6][6][6][6];  // A-C-G-T + gap + initiation (dangling end, $ sign)
 protected:
	Energy			_oridH[6][6][6][6];
	void			InitOriNNMatriz	();
    void			Copy_oridS		(void *dS) 			{memcpy( dS, _oridS,sizeof(_oridS)) ;}
	bool			ChangeConc		(float C1 = 50e-9,  float C2 = 50e-9 ) ;
	void			UpdatedSMatriz_forb_entr_elem(); // despues de esto hay que update cualquier SaltCorr
 public:
	Temperature			SetTa (Temperature Ta){Temperature T=_Ta; _Ta=Ta;return  T ;}  // in Kelvin --- for new Iteration, and G calc
	Temperature			Ta	()				  {							 return _Ta;} 
    Energy	&ndH			(Base a_1, Base a, Base b_1, Base b)	 {return _oridH[a_1][a][b_1][b]; }
    Entropy	&ndS			(Base a_1, Base a, Base b_1, Base b)	 {return _oridS[a_1][a][b_1][b]; }
    Entropy	GetOriEntr		(Base a_1, Base a, Base b_1, Base b)const{return _oridS[a_1][a][b_1][b]; }
	Entropy	GetOriSelfEntr	(Base a_1, Base a)					const{return GetOriEntr (		  a_1  ,		  a, 
                                                                                        bkn2c_nu[ a_1 ], bkn2c_nu[a] ); }
	Entropy	GetInitialEntropy()						 const{return 	-5.9f+_RlogC;	}
	Temperature CalcTM (Entropy S,Energy H	 )		 const{return (S>=0 || H>=forbidden_enthalpy/10000 || (H/S)<0 ) ?  0		  :  (H/S	  );}
	Energy	CalcG (Entropy S,Energy H,Temperature Ta)const{return (S>=0 || H>=forbidden_enthalpy/10000 ) ? -forbidden_freeEnerg : +(H - Ta*S);}//  ????
	Energy	CalcG (Entropy S,Energy H		 )		 const{return CalcG(S,H,_Ta);}// Usa la Ta almacenada aqui con el ultimo SetTa
	bool LoadNNParam(istream &isTDP)  ;
		COriNN						(float C1 = 50e-9,  
			                         float C2 = 50e-9 ) 
		:		R					( 1.987f),	
				_RlogC				( R * (float)log( (C1>C2)?C1-C2/2:C2-C1/2  )	),
				_Ta					( 0		),		// ineficiente si se deja asi
				forbidden_entropy	(_RlogC	),		// OJO !! dependencia de parte de la matriz dS de las conc ADN
				forbidden_enthalpy	( 1e18f	),		// initialize parameter table!
				forbidden_freeEnerg	( -999999999.0f	),	// ( -999999999.0f	),
				kein_Tm				( 0		),
				iloop_entropy		(-0.97f	),
				iloop_enthalpy		( 0.00f	),
				bloop_entropy		(-1.30f	),		// xy/-- and --/xy (Bulge Loops of size > 1)
				bloop_enthalpy		( 0.00f	),
				obulge_match_H		(-2.66f * 1000),// bulge opening
				obulge_match_S		(-14.22f),
				cbulge_match_H		(-2.66f * 1000),// bulge closing
				cbulge_match_S		(-14.22f),
				obulge_mism_H		( 0.00f * 1000),
				obulge_mism_S		(-6.45f	),
				cbulge_mism_H		( 0.00f	),
				cbulge_mism_S		(-6.45f	)   		         {	InitOriNNMatriz();}

	const float R ;
	const float forbidden_enthalpy,
				forbidden_freeEnerg,
				kein_Tm, 
				iloop_entropy,
				iloop_enthalpy,
				bloop_entropy,
				bloop_enthalpy,
				obulge_match_H,
				obulge_match_S,
				cbulge_match_H,
				cbulge_match_S,
				obulge_mism_H,
				obulge_mism_S,
				cbulge_mism_H,
				cbulge_mism_S; 
 protected:
		Entropy	_RlogC;				// rlogc = R * log( (C1>C2)?C1-C2/2:C2-C1/2  ) ;
 public:
		Entropy forbidden_entropy;	// forbidden entropy=-rlogc
	Temperature	_Ta;				// ineficiente? si se deja asi, ponerla lo mas parecido, ?pero menor que lo esperado?
									// se usa para calcular G -free energia
		  virtual ~COriNN(){}	// Hace falta ????
private:		  COriNN& operator=(const COriNN& ){} /*= delete*/ ;
};

//enum SaltCorrection {NoSelect=-1,StLucia=0, Owczarzy=1};
//// ya se puede usar StLucia inicializando todo en el constructor. Parcialmente implementado cambio de Conc

class CSaltCorrNN : public COriNN
{	SaltCorrection	_SaltCorr;   
	float			_ConcSd, _ConcTg ; // pasar a ori?????
	float			_ConcSalt;
	float			_GCp ;		// cambiar para que acepte CSec ???, o solo que acepte el GCp ya calculado 
								// para calcular Owczarzy SaltCorrection  // TODO

	void	InitSaltNNMatriz() ; // inicial ?
	void	InitStLuciaSaltNNMatriz ();// llamarla al cambiar conc de sal o DNA(este posible parcial)
	void	InitOwczarzySaltNNMatriz();// llamarla solo despues de cambio de GC

	float			_dS[6][6][6][6];	//	float	_dH[6][6][6][6];  // A-C-G-T + gap + initiation (dangling end, $ sign)
	
public:
	CSaltCorrNN	(float  ConcSd	 =50e-9     ,  float		  ConcTg= 50e-9, 
				 float CationConc=50e-3, SaltCorrection sc = StLucia, const std::string &NNfileName="" ) 
			: 	COriNN			( ConcSd, ConcTg, NNfileName) ,
				_ConcSd	( ConcSd ),
				_ConcTg	( ConcTg ),
				_ConcSalt		( CationConc),
				_SaltCorr		( sc )						{	InitSaltNNMatriz()  ;}
	bool    LoadNNParam(istream &isTDP)  ;
    bool    NeedActualization (float  ConcSd	 =50e-9     ,  float		  ConcTg= 50e-9, 
				               float CationConc=50e-3, SaltCorrection sc = StLucia) const
    {
        if (sc != _SaltCorr)
             return true;
        if ( ! IsEq (CationConc,_ConcSalt)  )
		     return true ;

        float RlogC	= R * log( (ConcSd>ConcTg)?ConcSd-ConcTg/2:ConcTg-ConcSd/2  ) ;

	    if ( IsEq (_RlogC, RlogC)  )
		    return false ;

        return true;
    }

    SaltCorrection SCMet() const{ return _SaltCorr ; }
    bool	UseOwczarzy () const{ return _SaltCorr == Owczarzy ;}

    void	UpdateSaltCorrrection () {	InitSaltNNMatriz();}
	bool	SetConc(float C1, float C2, float CationConc=50e-3) ;//	bool	SetConc(double C1, double C2 )
	bool	UpdateGC(float GC);
	float	UpdateGC(float GC1, long l1,float GC2, long l2);
	float	CalcGC  (float GC1, long l1,float GC2, long l2);
	void	SetSaltCorrection  (SaltCorrection sc) ;
	void	SetStLuciaSaltCorr (		   float C1 =50e-9, float C2 =50e-9, float CationConc=50e-3) ;
	void	SetOwczarzySaltCorr( float GC, float C1 =50e-9, float C2 =50e-9, float CationConc=50e-3) ;
    void	SetOwczarzySaltCorr() {SetSaltCorrection (Owczarzy);}

    inline float GetEntr(Base a_1, Base a, Base b_1, Base b) const	{	return _dS   [a_1][a][b_1][b]; }
    inline float GetEnth(Base a_1, Base a, Base b_1, Base b) const	{	return _oridH[a_1][a][b_1][b]; }
    inline float GetNS_S(Base a_1, Base a, Base b_1, Base b) const	{	return _dS   [a_1][a][bkn2c_nu[b_1]][bkn2c_nu[b]]; }
    inline float GetNS_H(Base a_1, Base a, Base b_1, Base b) const	{	return _oridH[a_1][a][bkn2c_nu[b_1]][bkn2c_nu[b]]; }
	inline float GetSelfEntr(Base a_1, Base a) const{return GetEntr (		  a_1  ,		  a, 
                                                                    bkn2c_nu[ a_1 ], bkn2c_nu[a] ); }
	inline float GetSelfEnth(Base a_1, Base a) const{return GetEnth (		  a_1  ,		  a, 
                                                                    bkn2c_nu[ a_1 ], bkn2c_nu[a] ); }
    inline float GetCorrectSaltOwczarzySelfEntr ( Base a_1, Base a , float GCp ) const   // TODO: como considerar GCp ?
	{	float LogSC = log(_ConcSalt);
		return GetOriSelfEntr(a_1, a) + GetSelfEnth(a_1, a)  *((4.29f * 100*_GCp -3.95f )*(1e-5f)*LogSC+ (9.4e-6f)*LogSC*LogSC);	
	}

	friend ostream &operator<<(ostream &osTDP, const CSaltCorrNN &sp)  ;
		  virtual ~CSaltCorrNN(){}	// Hace falta ????
};







#endif 

