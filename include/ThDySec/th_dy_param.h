/**
* Copyright (C) 2009-2015, Ariel Vina Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDySec\th_dy_param.h
*
* @brief A representation of the Nearest Neighbor Model Parameters, intented to be a simplification of that
*        developed and reported by Santa Lucia: http://www.annualreviews.org/doi/abs/10.1146/annurev.biophys.32.110601.141800 
*        This representation is based on the ideas and code of Kaderali (http://bioinformatics.oxfordjournals.org/content/21/10/2375.abstract) 
*        but with many modifications, so that the original authors have no responsability on the many erros, 
*        simplifications or inconsistencies I have introduce.
*        The original source file had the folowing header:
* //=============================================================================
* // Module:        nnparams.h
* // Project:       Diploma Thesis - Probe Selection for DNA Microarrays
* // Type:          header file - Nearest Neighbor Parameters / Model.
* // Language:      c++
* // Compiler:      microsoft visual c++ 6.0, unix/linux gcc
* // System/OS:     Windows 32, Sun solaris, Linux, other unix systems (untested)
* // Database:      none
* // Description:   class CNNParams - Nearest Neighbor Model Parameters
* // Author:        kaderali
* // Date:          9-12/2000
* // Copyright:     (c) L. Kaderali, 9/2000 - 12/2000
* //
* // Revision History
* // $              00sep07 LK : created
* //                00dec29 LK : changed to include dangling end data
* //                01jan09 LK : included CalcSelfTM function
* //                01feb07 LK : optimized
* // #$
* //=============================================================================
* Which is accesible under GNU GPL at: http://dna.engr.uconn.edu/?page_id=85
*/

#ifndef _TH_DY_PARAM_H
#define _TH_DY_PARAM_H

#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include "cod_deg.h"
#include "common.h" 
using namespace DegCod ;
	///  \todo name thing like: forbidden_enthalpy, iloop_entropy, bloop_entropy, bloop_enthalpy
	///  \todo review all this data! see http://public.lanl.gov/jgans/tntblast/tntblast_doc.html

class COriNN  
{	Entropy			_oridS[6][6][6][6];  ///< A-C-G-T + gap + initiation (dangling end, $ sign)
 protected:
	Energy			_oridH[6][6][6][6];
	void			InitOriNNMatriz	();
    void			Copy_oridS		(void *dS) 			{memcpy( dS, _oridS,sizeof(_oridS)) ;}
	bool			ChangeConc		(float C1 = 50e-9,  float C2 = 50e-9 ) ;
	void			UpdatedSMatriz_forb_entr_elem(); ///< despues de esto hay que update cualquier SaltCorr
 public:
	Temperature			SetTa (Temperature Ta){Temperature T=_Ta; _Ta=Ta;return  T ;}  ///< in Kelvin --- for new Iteration, and G calc
	Temperature			Ta	()				  {							 return _Ta;} 
    Energy	&ndH			(Base a_1, Base a, Base b_1, Base b)	 {return _oridH[a_1][a][b_1][b]; }
    Entropy	&ndS			(Base a_1, Base a, Base b_1, Base b)	 {return _oridS[a_1][a][b_1][b]; }
    Entropy	GetOriEntr		(Base a_1, Base a, Base b_1, Base b)const{return _oridS[a_1][a][b_1][b]; }
	Entropy	GetOriSelfEntr	(Base a_1, Base a)					const{return GetOriEntr (		  a_1  ,		  a, 
                                                                                        bkn2c_nu[ a_1 ], bkn2c_nu[a] ); }
	Entropy	GetInitialEntropy()						 const{return 	-5.9f+_RlogC;	}
	Temperature CalcTM (Entropy S,Energy H	 )		 const{return (S>=0 || H>=forbidden_enthalpy/10000 || (H/S)<0 ) ?  0		  :  (H/S	  );}
	Energy	CalcG (Entropy S,Energy H,Temperature Ta)const{return (S>=0 || H>=forbidden_enthalpy/10000 ) ? -forbidden_freeEnerg : +(H - Ta*S);}//  ????
	Energy	CalcG (Entropy S,Energy H		 )		 const{return CalcG(S,H,_Ta);}///< Usa la Ta almacenada aqui con el ultimo SetTa
	bool LoadNNParam(std::istream &isTDP)  ;
		COriNN						(float C1 = 50e-9,  
			                         float C2 = 50e-9, const std::string &NNfileName="" ) 
		:		_RlogC				( R * (float)log( (C1>C2)?C1-C2/2:C2-C1/2  )	),
				forbidden_entropy	(_RlogC	)		///< OJO !! dependencia de parte de la matriz dS de las conc ADN
	{	
        InitOriNNMatriz(); 
        if (NNfileName.empty())
        {
            std::ifstream nnf{NNfileName};
            LoadNNParam(nnf);
        }
    }

	const float R                   { 1.987f } ;
	const float forbidden_enthalpy  { 1e18f  },
				forbidden_freeEnerg {-999999999.0f} ,
				kein_Tm             { 0     }, 
				iloop_entropy       {-0.97f },
				iloop_enthalpy      { 0.00f },	///< xy/-- and --/xy (Bulge Loops of size > 1)
				bloop_entropy       {-1.30f	},
				bloop_enthalpy      { 0.00f	},
				obulge_match_H      {-2.66f * 1000},///< bulge opening
				obulge_match_S      {-14.22f},
				cbulge_match_H      {-2.66f * 1000},///< bulge closing
				cbulge_match_S      {-14.22f},
				obulge_mism_H       { 0.00f * 1000},
				obulge_mism_S       {-6.45f	},
				cbulge_mism_H       { 0.00f	},
				cbulge_mism_S       {-6.45f	}; 
 protected:
		Entropy	_RlogC;				///< rlogc = R * log( (C1>C2)?C1-C2/2:C2-C1/2  ) ;
 public:
		Entropy forbidden_entropy;	///< forbidden entropy=-rlogc
	Temperature	_Ta{ 0     };		///< ineficiente? si se deja asi, ponerla lo mas parecido, ?pero menor que lo esperado?
									///< se usa para calcular G -free energia
		  virtual ~COriNN(){}	    ///< Hace falta ????
private:		  COriNN& operator=(const COriNN& ){} /*= delete*/ ;
};

//enum SaltCorrection {NoSelect=-1,StLucia=0, Owczarzy=1};
////  \todo ya se puede usar StLucia inicializando todo en el constructor. Parcialmente implementado cambio de Conc

class CSaltCorrNN : public COriNN
{	SaltCorrection	_SaltCorr;   
	float			_ConcSd, _ConcTg ; ///< \todo pasar a ori?????
	float			_ConcSalt;
	float			_GCp ;		// cambiar para que acepte CSec ???, o solo que acepte el GCp ya calculado 
								///  \todo para calcular Owczarzy SaltCorrection  // todo

	void	InitSaltNNMatriz() ; ///< inicial ?
	void	InitStLuciaSaltNNMatriz ();///< llamarla al cambiar conc de sal o DNA(este posible parcial)
	void	InitOwczarzySaltNNMatriz();///< llamarla solo despues de cambio de GC

	float			_dS[6][6][6][6];	///<	float	_dH[6][6][6][6];  // A-C-G-T + gap + initiation (dangling end, $ sign)
	
public:
	CSaltCorrNN	(float  ConcSd	 =50e-9     ,  float		  ConcTg= 50e-9, 
				 float CationConc=50e-3, SaltCorrection sc = StLucia, const std::string &NNfileName="" ) 
			: 	COriNN			( ConcSd, ConcTg, NNfileName) ,
				_ConcSd	( ConcSd ),
				_ConcTg	( ConcTg ),
				_ConcSalt		( CationConc),
				_SaltCorr		( sc )						{	InitSaltNNMatriz()  ;}
	bool    LoadNNParam(std::istream &isTDP)  ;
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
	bool	SetConc(float C1, float C2, float CationConc=50e-3) ;///<	bool	SetConc(double C1, double C2 )
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
    inline float GetCorrectSaltOwczarzySelfEntr ( Base a_1, Base a , float GCp ) const   ///<  \todo: como considerar GCp ?
	{	float LogSC = log(_ConcSalt);
		return GetOriSelfEntr(a_1, a) + GetSelfEnth(a_1, a)  *((4.29f * 100*_GCp -3.95f )*(1e-5f)*LogSC+ (9.4e-6f)*LogSC*LogSC);	
	}

	friend std::ostream &operator<<(std::ostream &osTDP, const CSaltCorrNN &sp)  ;
		  virtual ~CSaltCorrNN(){}	///< Hace falta ????
};







#endif 

