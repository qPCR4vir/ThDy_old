/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDySec\th_dy_align.h
*
* @brief 
*/
#pragma unmanaged	
#ifndef _TH_DY_ALIGN_H
#define _TH_DY_ALIGN_H
//#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include "sec.h"
#include "common.h" 

//  OJO !!! La primera sec a alinear es la sonda - "simple cadena", mientras que la seg sec es el Target - de doble cadena
//  y por tanto normalmente (by default) se hibrida la sonda con la cadena complementaria del Target.
//  proximamente se hibridara con las dos cadenas.

class CHit;
class CHitAligned ;
//extern char sep[];
class ThDyAlign													// clase abstr?   hace lo que ThDyAlign_Tm ... simplificar? ------   ThDyAlign  ---
{public:		friend class	CHitAligned ;	 friend class	CHit ;
					 ThDyAlign	(LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar, float InitMax= 0 );
	virtual			~ThDyAlign	() ;
	virtual const char *AlignMeth(){return "ThAl";}
	Temperature		SetTa	(Temperature Ta)					{Temperature T=_Ta;		_NNpar->_Ta=_Ta=Ta;	   return T;}
	Temperature	     Ta		()const								{return _Ta;} // da Ta "seteada", la que se elige para el "experimento"  ---REVISAR esto de las Ta 
	void			SetSig	(Temperature Tm_sig, Energy G_sig)	{_Tm_sig=Tm_sig;	_G_sig=G_sig ;}	
	virtual void	Align	(CSec  *sonde, CSec *target)		{	ClearHits();
																	Use	(sonde, target);	//	InitBorder	();
																	Run();
																	SelectOptParam();	// a la Ta en que se calculo
																}
	void			force_ResizeTable();		//	void			force_ResizeTable(long LenSond, long LenTarg) ;
	void			ResizeTable(LonSecPos LenSond, LonSecPos LenTarg);
	Temperature		last_Ta()const{return _Ta_lastCalc;} // da Ta a la que se realizo el ultimo calculo, (-1) antes del calculo
	enum Step {	st_a=1, st_i=1, st_b=2, st_j=2, st_d=3, NoStep=0,		// back punter matriz a:i+1 dirc sonda, b:j+1 dir tg, d:diag
				st_0=0, st_1, st_2, st_3, st_4, st_5, st_6, st_7,
				c_01=01, c_02=2,c_04=4, c_05=5, c_06=6, c_08=8, c_09=9, c_10=10, 
			  } *_pre	;   /*,*_pre0, *_pre1, *_pre2*/ 
protected:
	void			Use			(CSec  *sonde, CSec *target);
	void			ResizeTable	()							{	ResizeTable(_sd->Len() , _tg->Len() ) ;}
	void			Run			(LonSecPos tg_j_StartPos =1);
	void			ClearHits	()							{	_Hits.Destroy();}
	virtual	bool	AddIfHit	(LonSecPos i, LonSecPos j)	{	return false;}
private:
	void			InitBorder	();
public:
	float	(ThDyAlign::*CalcParam)(Entropy S, Energy H)const ;
			Temperature	CalcParamTm(Entropy S, Energy H) const {return    _NNpar->CalcTM (S, H);}  // = 0; hacer virtual ????
			Energy		CalcParamG (Entropy S, Energy H) const {return  - _NNpar->CalcG  (S, H);} 
			Temperature	CalcParamRs(Entropy S, Energy H) const {	Temperature Tm = _NNpar->CalcTM (S +_restS, H +_restH);	//  EXPERIMENTAL
																	if ( _NNpar->CalcTM (S , H )==0 ) Tm= 0;		//if ( Tm < _minTm ) return 0;
																	Tm -= _sd->_Tm.Ave();
																	return 		Tm ;
																}  
								// "OUTPUT" usar las sig. solo despues de CalculateTable  !!!
protected:													// --------- Get_X	----------    da param en (i,j) para Tm max !!!!    ?????????
	Energy		 Get_H_max_para	(LonSecPos i, LonSecPos j)const;	
	Entropy		 Get_S_max_para	(LonSecPos i, LonSecPos j)const;
	Temperature	 Get_Tm_max_para(LonSecPos i, LonSecPos j)const;
	Energy		 Get_G_max_para	(LonSecPos i, LonSecPos j)const {return Get_G_max_para(i,j, last_Ta() );}		// a la Ta en que se calculo
	Energy		 Get_G_max_para	(LonSecPos i, LonSecPos j, Temperature Ta )const;						// eliges a que Ta
															// --------- Getmaxglo_X	---    
	//Energy		 Getmaxglo_H()				const		{return Get_H (_maxgloi,_maxgloj);}
	//Entropy		 Getmaxglo_S()				const		{return Get_S (_maxgloi,_maxgloj);}
	//Temperature	 Getmaxglo_Tm()				const		{return Get_Tm(_maxgloi,_maxgloj);}
	//Energy		 Getmaxglo_G()				const		{return Get_G (_maxgloi,_maxgloj);}			// a la Ta en que se calculo
	//Energy		 Getmaxglo_G(Temperature Ta)const		{return Get_G (_maxgloi,_maxgloj,Ta);}		// eliges a que Ta
	void		 SelectOptParam_max_para(LonSecPos i, LonSecPos j, Temperature Ta );				// eliges a que Ta

															// ---- SelectOptParam ---    segun step !!!!!-------alternativa a   Get_X	
	void		 SelectOptParam(LonSecPos i, LonSecPos j) {return SelectOptParam(i,j, last_Ta() );}	// a la Ta en que se calculo
	void		 SelectOptParam(LonSecPos i, LonSecPos j, Temperature Ta );							// eliges a que Ta
protected:										// ---------alternativa a   Get_X	----------    da param en (i,j) segun step()i,j) !!!!    ?????????
	Energy		Get_H	(LonSecPos i, LonSecPos j, Step st) const;
	Entropy		Get_S	(LonSecPos i, LonSecPos j, Step st) const;
	Step		Get_pre	(LonSecPos i, LonSecPos j, Step st) const; 
	inline Step	step	(LonSecPos i, LonSecPos j		  ) const {return pre(i,j);} 
public:										// ------ Lo mismo que : Get_X , pero calculados todos de una vez.
	void		 SelectOptParam()				{return SelectOptParam(_maxgloi,_maxgloj);}			// a la Ta en que se calculo
	void		 SelectOptParam(Temperature Ta ){return SelectOptParam(_maxgloi,_maxgloj,Ta);}		
	Energy		 H ()const{return _optH ;}
	Entropy		 S ()const{return _optS ;}
	Temperature	 Tm()const{return _optTm;}
	Energy		 G ()const{return _optG ;} // la G depende de como selecionaste los optParam
	Energy		 G (Temperature Ta )const{return +(_optH-Ta*_optS);  } // la G NO depende de como selecionaste los optParam
	CHit		*GetOptHit();

	void		Export_Hits    (ofstream &osHits , char *sep);
	void		Export_DPMz_Tm (ofstream &osDP_mz, char *sep);
	void		Export_DPMz_H  (ofstream &osDP_mz, char *sep);
	void		Export_DPMz_S  (ofstream &osDP_mz, char *sep);
	void		Export_DPMz_Pre(ofstream &osDP_mz);

	virtual int	IterationNum()const{return 1;}			//  ??? hace falta aqui?

public:		
	static int const  sti[], stj[], sti1[], stj1[];
	std::shared_ptr<CSaltCorrNN>  _NNpar ;
	CSec				*_sd , *_tg ;
	long				_THits, _HitsOK ;   // Hacer interface
	LonSecPos			_maxgloi, _maxgloj			/*, _maxglot*/ ;

protected:	
	Temperature			_Tm_sig, _minTm, _Ta, _Ta_lastCalc;
	Energy				_G_sig ;		// _Tm_min, _Tm_max ; // _Tm_min, _Tm_max ; aqui se usan????????????????

	LonSecPos			_LenSond, _LenTarg,  _LenSondPlus1 ;
	long				_TableSize;

	Energy				*_dH0, *_dH1, *_dH2;          // Dynamic Programming Table for Entropy
	Entropy				*_dS0, *_dS1, *_dS2;          // Dynamic Programming Table for Enthalpy

	float		_InitMax, _maxglo, _max ;   // max del parametro rector :  como G o Tm

	Entropy		_optS;
	Energy		_optH, _optG;
	Temperature	_optTm ;
	CList		_Hits ;


 protected:
	inline Energy  &dH0(LonSecPos i, LonSecPos j)const{return _dH0[i +j*_LenSondPlus1]; }  //  {return _dH0[i +j*_LenSond]; }   {return *(_dH0 + i +j*_LenSond); }
	inline Energy  &dH1(LonSecPos i, LonSecPos j)const{return _dH1[i +j*_LenSondPlus1]; }
	inline Energy  &dH2(LonSecPos i, LonSecPos j)const{return _dH2[i +j*_LenSondPlus1]; }
	inline Entropy &dS0(LonSecPos i, LonSecPos j)const{return _dS0[i +j*_LenSondPlus1]; }
	inline Entropy &dS1(LonSecPos i, LonSecPos j)const{return _dS1[i +j*_LenSondPlus1]; }
	inline Entropy &dS2(LonSecPos i, LonSecPos j)const{return _dS2[i +j*_LenSondPlus1]; }
	inline Step    &pre(LonSecPos i, LonSecPos j)const{return _pre[i +j*_LenSondPlus1]; }

	Entropy		_restS;		
	Energy		_restH ;
	inline void			RestHS(LonSecPos i)	{	_restS = _sd->_SdS[_sd->Len() -1 ] - (i==1?0:_sd->_SdS[i-2]);
												_restH = _sd->_SdH[_sd->Len() -1 ] - (i==1?0:_sd->_SdH[i-2]);
											;}
};

class CHit : public CLink // Hit dentro de la Matriz de prog dinamica
{	
public: 	
	LonSecPos		_i,_j, _i0, _j0, _l;
	DNAStrand		_strnd;
	ThDyAlign::Step _Step ;
	Energy			_H,  _G ;
	Entropy			_S ;
	Temperature		_Tm ;
	float			_max ;
	CHit (LonSecPos i, LonSecPos j, Temperature Tm): _i(i), _j(j), _Tm(Tm){}
	CHit (LonSecPos i, LonSecPos j, Energy H, Entropy S,float max, ThDyAlign::Step st): _i(i), _j(j), _H(H), _S(S), _max(max),_Step(st) {};
	CHit (LonSecPos i, LonSecPos j, LonSecPos i0, LonSecPos j0, LonSecPos l,Energy H, Entropy S,float max, ThDyAlign::Step st)
		: _i(i), _j(j),  _i0(i0), _j0(j0), _l(l)      ,_H(H), _S(S), _max(max),_Step(st) {};
	CHit (ThDyAlign &Al);  // Hit optimo: recalcula optP de AL a la temp Ta actual NNpar
	explicit CHit (){}
};

class CHitAligned  : public CHit
{
public:
	ISec::sequence          _sd, _tg ;
	vector<ThDyAlign::Step> _st ;
	long                    _mt, _mm, _sgap, _tgap ;    // count sonde and target - matchs , mistmatch, and gaps
	float		            _Hr, _Sr, _Gr, _Tmr ;

	explicit CHitAligned(ThDyAlign &Al) : CHit(Al) {ExtractAligment(Al);}
	explicit CHitAligned(ISec::sequence s, ISec::sequence t, std::shared_ptr<CSaltCorrNN>  NNpar )  
									: _sd(std::move(s)), 
									  _tg(std::move(t))
	                              {ReCalcule( NNpar );}; 

	void ExtractAligment(ThDyAlign &Al);

	virtual ~CHitAligned()  { }
	void ReCalcule( std::shared_ptr<CSaltCorrNN>  NNpar );
};

class AlignedSecPar : public CHitAligned
{public: 
explicit AlignedSecPar( ISec::sequence s,  ISec::sequence t, std::shared_ptr<CSaltCorrNN>  NNpar ):CHitAligned(s, t, NNpar){};
Temperature Tm(){return _Tmr;}
float  G(){return _Gr ;}
	virtual ~AlignedSecPar()  {_sd=_tg=nullptr;}
};

//
//ofstream &operator<<(ofstream &stream, CHit &TmACHitl) 
//{	stream	<< endl<< "T melting Align";     //<< (ThDyAlign)TmAl;
//	print_ThDyAlign (stream, TmAl);
//	return stream;
//}


class ThDyAlign_Tm			: public ThDyAlign  // -----------------------------Tm--------------------ThDyAlign_Tm------
{public:	
	ThDyAlign_Tm(long MaxLenSond, long MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar)
		:ThDyAlign(MaxLenSond, MaxLenTarg, NNpar, NNpar->kein_Tm)
																		{CalcParam = &ThDyAlign::CalcParamTm;} 
	virtual const char *AlignMeth(){return "Tm";}
	Temperature	GetMax_Tm()const{return _maxglo;}
};

class ThDyAlign_TmHits			: public ThDyAlign_Tm  // -----------------------------Tm---------------ThDyAlign_TmHits----- no en uso????------
{public:	
	ThDyAlign_TmHits(long MaxLenSec, std::shared_ptr<CSaltCorrNN>  NNpar, float Tm_min=CtoK(57), float Tm_max=CtoK(63) )
		:ThDyAlign_Tm(MaxLenSec, MaxLenSec, NNpar),	 _Tm_min(Tm_min), _Tm_max(Tm_max){} 
	virtual const char *AlignMeth(){return "TmHits";}
	virtual	bool	AddIfHit(long i, long j);
	Temperature  _Tm_min, _Tm_max ; //aqui se usan????????????????
};

class ThDyAlign_TmCand			: public ThDyAlign_Tm  // -----------------------------Tm-----------------ThDyAlign_TmCand---------
{public:	
	ThDyAlign_TmCand(long MaxLenSec, std::shared_ptr<CSaltCorrNN>  NNpar)       /*, float Tm_min=CtoK(57), float Tm_max=CtoK(63) */
		:ThDyAlign_Tm(MaxLenSec, MaxLenSec, NNpar){}                            /*,	 _Tm_min(Tm_min), _Tm_max(Tm_max)*/
	virtual const char *AlignMeth(){return "TmCand";}
	virtual bool	AddIfHit	(long i, long j);
	void	Use			(CSecCand  *cand1, CSecCand *cand2)	{	_cs=cand1; _ct=cand2; ThDyAlign_Tm::Use	( &_cs->_Sec, &_ct->_Sec);}
	void	FindCommon	(CSecCand  *cand1, CSecCand *cand2, bool colapse=true) {	Use	(cand1, cand2);
																					Align( &_cs->_Sec, &_ct->_Sec); 
																					_cs->ColapseRangs(colapse);
																					_ct->ColapseRangs(colapse);		}
	CSecCand	*_cs, *_ct;
	//float  _Tm_min, _Tm_max ; //aqui se usan????????????????
};
//

class CMSecCand : public CLink		//--------------------------------Tm------ CMSecCand --------------------------------
{public:
	CMSecCand(	SondeLimits sL ,
				float	Tm_sig		, float G_sig ,			// sonde  - target		: hace falta aqui?
				float	MaxSd_nTgTm , float MinSd_nTgG ,	// sonde  - non target
				float	MaxSelfTm   , float MinSelfG  	)	// sonde 
				:	_sL(sL),
					_Tm_sig(Tm_sig) ,			_G_sig(G_sig),
					_MaxSd_nTgTm(MaxSd_nTgTm) , _MinSd_nTgG(MinSd_nTgG), 
					_MaxSelfTm(MaxSelfTm),		_MinSelfG(MinSelfG),

					_TNumCand(0),	_NumPosCand(0),		_NumCand(0),	_NSecCand (0),
					_TNumPosCand(-1)  /*,		// valor imposible, inicial	
					_osPaarComp(0)	  */	
		{} 

	void		Use(shared_ptr<CMultSec> MSec);//	void		Set_PaarComparExport(ofstream &osPaarComp){_osPaarComp=osPaarComp;};
	CSecCand	*Add(CSec &sec);
	CSecCand	&AddBeging	(CSec &sec) ;
	CSecCand	&curTg		()		{return *((CSecCand *)_LSecCand.Cur());	}
	bool		NotFinisch	();
	void		FindCommon	(CSecCand  &cand1, CSecCand &cand2, bool design=true)	;
	CSecCand	*CompNext	();

	void		ExportCommonSonden(const std::string &fileName, bool colpased, NumRang<float> ExtrCovPerc, int format);
	virtual ~CMSecCand(){	 
							_LSecCand.Destroy() ; 
							_LMSecCand.Destroy() ; }
	SondeLimits _sL ;

	float	_Tm_sig, _G_sig ;				// sonde  - target
	float	_MaxSd_nTgTm , _MinSd_nTgG ;	// sonde  - non target
	float	_MaxSelfTm , _MinSelfG  ;		// sonde 

	long	_NumPosCand,	_NumCand;  // Solo los "locales"
	long	_TNumPosCand,	_TNumCand;  // Tambien cuenta los de la multiseq
    long    _NSecCand;

	ofstream _osPaarComp;

	shared_ptr<CMultSec>            _MSec ;
	shared_ptr<ThDyAlign_TmCand>	_TDATmC ;  // donde se crea y se borra???   _______________ PROBLEMA !!!!!!!!!!!!!!!!


	CList	_LSecCand, _LMSecCand;
};

class ThDyAlign_G			: public ThDyAlign	// ------------------------------G-------------------------
{public:
	ThDyAlign_G(LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar, Temperature Tm)
		:	ThDyAlign(MaxLenSond, MaxLenTarg, NNpar, NNpar->forbidden_freeEnerg)
																	{CalcParam = &ThDyAlign::CalcParamG;
																	 SetTa (Tm);
																	} 
	ThDyAlign_G(LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar)// usar ultima Ta de calculo en NNpar, o "Set" despues
		:	ThDyAlign(MaxLenSond, MaxLenTarg, NNpar, NNpar->forbidden_freeEnerg)
																	{CalcParam = &ThDyAlign::CalcParamG;} 	
	virtual const char *AlignMeth(){return "G";}
	float		 GetMax_G()const{return -_maxglo;} // mas o menos lo mismo, pero "comprobado", usa ultima Ta de calculo
};

class FracTDAlign	: public ThDyAlign			// --------------------------Frac-----------------------------
{	bool	_finisch ;
	int		_iterations, _maxNumIt, _fixedNumIter; //, _totalIterations 
	Energy	_maxG_der ;  // epsilum 
 public:
	virtual const char *AlignMeth(){return "FractG";}
	 FracTDAlign(LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar)  
		 :	ThDyAlign	(MaxLenSond, MaxLenTarg, NNpar),
																			_iterations(0),	// prohibido comenzar sin BeginAlign
																			_maxG_der(1.0f),// super excesivo
																			_maxNumIt(10),	// super excesivo
																			_finisch(true),	// prohibido comenzar sin BeginAlign
																			_fixedNumIter(0)// NOT fixed !!	
																		{}
	 void			SetmaxG_der	(Energy maxG_der) { _maxG_der = maxG_der;}	// max desviacion permisible de la G sobre 0 o de G(Tm)
	 void			SetEpsilum  (Energy maxG_der) {  SetmaxG_der( maxG_der);} // exactamente lo mismo
	 void			SetMaxNumIt	(int maxNumIt)	 { _maxNumIt = maxNumIt;}
	 int			SetFixedNumIt(int fixedNumIter){int fixedNum = _fixedNumIter;_fixedNumIter=fixedNumIter;return fixedNum;}

	 void			BeginAlign	(CSec  *sonde, CSec *target)	;
	 bool			NotFinisch	();
	 void			iterate		(Temperature ta);
	 inline void	iterate		() {iterate ( Tm() );}

	 virtual void	Align (CSec  *sonde, CSec *target)	{for ( BeginAlign(sonde, target) ; NotFinisch(); iterate() );}


//	 float			G			()const{return _optG  ;}// ------ usa la Ta usada para los optParam
	 float			GetMax_G	()const{return  IterationNum()>1 ? -_maxglo : _maxglo;}// mas o menos lo mismo, pero usa Ta de calculo.	CUIDADO : Tm o G . G solo si interations>1  !!!!!
	 virtual int	IterationNum()const{return _iterations;}
};

class ThDyAlign_restTm			: public ThDyAlign  // ---------------------------HACER----------------------------
{public:	
	ThDyAlign_restTm(long MaxLenSond, long MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar, Temperature minTm)
			:		ThDyAlign(MaxLenSond, MaxLenTarg, NNpar, -274)
			{		_minTm = minTm ; /*CalcParam = &ThDyAlign::CalcParamRs;*/} 
	virtual const char *AlignMeth(){return "restTm";}
	virtual inline float CalcParam (float S, float H) const		
	{	Temperature Tm = _NNpar->CalcTM (S +_restS, H +_restH);
		if ( _NNpar->CalcTM (S , H )==0 ) Tm= 0;
		//if ( Tm < _minTm ) return 0;
		Tm -= _sd->_Tm.Ave();
		return 		Tm ;
	}  
	float	GetMax_Tm()const{return _maxglo;}
};





void		print_ThDyAlign (ofstream &osTm,ThDyAlign &Al);
//void		print_Tm		(ofstream &osTm, CMultSec	&pr, int MaxGrDeg=-1, char sep[]=";" );
void print_Tm (ofstream &osTm, CMultSec	&pr, int MaxGrDeg, char sep[]);
ofstream	&operator<<(ofstream &stream,	ThDyAlign_Tm	&TmAl) ;
ofstream	&operator<<(ofstream &osTm,		ThDyAlign		&Al) ;
ofstream	&operator<<(ofstream &stream,	ThDyAlign_G		&G_Al) ;
ofstream	&operator<<(ofstream &stream,	FracTDAlign		&FrAl) ;
#endif


//	inline float CalcParamG (float S, float H) const {return NNpar->CalcG  (S, H);} 
	//virtual inline float CalcParam  (float S, float H) const {return NNpar->CalcG  (S, H);} 
//	inline float CalcParamTm(float S, float H) const {return NNpar->CalcTM (S, H);}  // = 0; hacer virtual ????
//virtual	inline float CalcParam  (float S, float H) const {return NNpar->CalcTM (S, H);}  // duplicado--- dejar????
	/*virtual inline float CalcParam  (float S, float H) const {return NNpar->CalcTM (S, H);}  // = CalcParamTm; esto  o lo sig*/
				//float Tm_sig,
				//float Tm_min, 		float Tm_max, 
				//int L_min,			int L_max, 
				//float MaxSelfTm)
			//	:	
					//_G_min(G_min)	, _G_max(G_max) ,
					//_Tm_min(Tm_min) , _Tm_max(Tm_max),
					//_L_min (L_min ) , _L_max (L_max ),