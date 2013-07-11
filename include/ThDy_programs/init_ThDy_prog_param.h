#ifndef _INIT_ThDy_PROG_PARAM_H
#define _INIT_ThDy_PROG_PARAM_H

#include "init_prog_param.h" 
#include <memory>
#include "ThDySec/sec.h"



class CParamSondeLimits: public IBParam
{   SondeLimits sL;
 public: 
    CParamNumMinMax<Energy> G; 
    CParamNumMinMax<Temperature> T; 
    CParamNumMinMax<SecPos> L; 
	 // Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamSondeLimits (IProg *pp, const std::string& titel, SondeLimits &parRef, 
		             const std::string& etiqGmin, Energy minGmin,        Energy maxGmin,         Energy defValueGmin,
		             const std::string& etiqGmax, Energy minGmax,        Energy maxGmax,         Energy defValueGmax,
		             const std::string& etiqTmin, Temperature minTmin,   Temperature maxTmin,    Temperature defValueTmin,
		             const std::string& etiqTmax, Temperature minTmax,   Temperature maxTmax,    Temperature defValueTmax,
		             const std::string& etiqLmin, SecPos minLmin,        SecPos maxLmin,         SecPos defValueLmin,
		             const std::string& etiqLmax, SecPos minLmax,        SecPos maxLmax,         SecPos defValueLmax,
                           std::string  UnitG="kcal/mol", std::string UnitT="°C", std::string UnitL="nt" 

					) : IBParam ( titel), 
					    G(pp, titel+". Perf.Match dG", parRef._G,
						    " Min", etiqGmin,  minGmin, maxGmin, defValueGmin, 
						    " Max", etiqGmax,  minGmax, maxGmax, defValueGmax, 
						    UnitG),
					    T(pp, titel+". Perf.Match Tm", parRef._Tm,
						    " Min", etiqTmin,  minTmin, maxTmin, defValueTmin, 
						    " Max", etiqTmax,  minTmax, maxTmax, defValueTmax, 
						    UnitT),
					    L(pp, titel+". Length", parRef._L,
						    " Min", etiqLmin,  minLmin, maxLmin, defValueLmin, 
						    " Max", etiqLmax,  minLmax, maxLmax, defValueLmax, 
						    UnitL)
	          { 
	          } 
	        // Num &parRef,   usa _v y por tanto no necesita un parametro externo
    CParamSondeLimits (IProg *pp, const std::string& titel, //SondeLimits &parRef, 
		             const std::string& etiqGmin, Energy minGmin,        Energy maxGmin,         Energy defValueGmin,
		             const std::string& etiqGmax, Energy minGmax,        Energy maxGmax,         Energy defValueGmax,
		             const std::string& etiqTmin, Temperature minTmin,   Temperature maxTmin,    Temperature defValueTmin,
		             const std::string& etiqTmax, Temperature minTmax,   Temperature maxTmax,    Temperature defValueTmax,
		             const std::string& etiqLmin, SecPos minLmin,        SecPos maxLmin,         SecPos defValueLmin,
		             const std::string& etiqLmax, SecPos minLmax,        SecPos maxLmax,         SecPos defValueLmax,
                     const std::string& UnitG="kcal/mol", const std::string& UnitT="°C", const std::string& UnitL="nt" 

					) : IBParam ( titel), 
					    G(pp, titel+". Perf.Match dG", sL._G,
						    " Min", etiqGmin,  minGmin, maxGmin, defValueGmin, 
						    " Max", etiqGmax,  minGmax, maxGmax, defValueGmax, 
						    UnitG),
					    T(pp, titel+". Perf.Match Tm", sL._Tm,
						    " Min", etiqTmin,  minTmin, maxTmin, defValueTmin, 
						    " Max", etiqTmax,  minTmax, maxTmax, defValueTmax, 
						    UnitT),
					    L(pp, titel+". Length", sL._L,
						    " Min", etiqLmin,  minLmin, maxLmin, defValueLmin, 
						    " Max", etiqLmax,  minLmax, maxLmax, defValueLmax, 
						    UnitL)
	          { 
	          } 
};

class CMultSec;
class CSaltCorrNN;
/// concreta los parametros comunes. Mantiene lista de los prog Espec que los usan
class ThDyCommProgParam : public CCommProgParam 
{public:	
    ThDyCommProgParam(const std::string& titel,   CProject *proj)
		:	CCommProgParam(titel,proj), 
			_loadNNPar(false),    loadNNPar (this, "Programm option- Load NN parametr",		"LoadNNPara", _loadNNPar,  false),
			_saveNNPar(false),    saveNNPar (this, "Programm option- save NN parametr",		"SaveNNPara", _saveNNPar,  false),
			_SaltCorr(StLucia), SaltCorr(this, "Salt correction methode",					"SaltCorrMt", _SaltCorr, StLucia, StLucia, StLucia), /*StLucia), */
			_TAMeth(TAMeth_Tm),	  TAMeth  (this, "Optimized parametr during DynProg",		"AlignMethd", _TAMeth, TAMeth_Tm, TAMeth_Fract, TAMeth_Tm),
			_MaxTgId(99.0f),	  MaxTgId (this, "Filtre Seq Target with more %id"   ,		"MaxTgIdent",  _MaxTgId,  0.0f,100.0f,  99.0f  ,"%"),
								  ConcSd  (this,    "Conc of the sondes" ,					"ConcSonden",   _ConcSd,  0.0f,50e-3f,  50e-9f ,"M"), 
								  ConcTg  (this,    "Conc of the targets",					"ConcTarget",   _ConcTg,  0.0f,50e-3f,   50e-9f  ,"M"), 
								  ConcSalt(this,    "Conc of salt",							"ConcenSalt", _ConcSalt,  0.0f,1.0f,    50e-3f  ,"M"), 
			_Ta(55.0f),			  Ta      (this, "Temp anneling expected in exp",			"TempAnnelg",       _Ta,  20.0f, 90.0f, 55.0f, "°C" ),
			_SecLim( long(1), long(2001) ) ,//_SecBeg(long(1)), _SecEnd(long(1)+MAXSEC_LENGHT) ,
								  SecLim (this, "Fragments of seq. to use",	_SecLim, 
													"Beginng",								"SecBeginng", long(1), long(2001), long(1),
													"End",									"SecuencEnd", long(0), long(2001), long(2001),
											"nt"),
			_MinSecLen(0),		  MinSecLen(this, "Min length of the seq. to be useful",	"MinSeqLeng", _MinSecLen,  0, 2001, 0, "nt" ),
			_st_savTm(true) ,     st_savTm (this, "Programm option- Save Tm Table",			"SavTmTable", _st_savTm,   true), 
			_st_savPos(true),     st_savPos(this, "Programm option- Save Posicion Table",	"SavPosTabl", _st_savPos,  true),  
			_st_savG(true),       st_savG  (this, "Programm option- Save free Enrgie Table","SaveGTable", _st_savG,    true),  
			_st_savAlign(true),   st_savAlign(this,"Programm option- Save Table of aligns", "SavAlignTb", _st_savAlign,true),  
			_st_savProj(true),    st_savProj(this,"Programm option- Save Project file",     "SavProject", _st_savProj, true), 
			_st_savG_Plasm(false),st_savG_Plasm(this, "Programm option- Gtable for Plasmid","SavG_Plasm", _st_savG_Plasm,  false), 
			_st_savTm_Plasm(false),st_savTm_Plasm(this, "Programm option- Tm table for Plasmid","SavTmPlasm", _st_savTm_Plasm,  false),  
			_st_savLog(false),    st_savLog(this, "Programm option- save a log",			"Save_Logfi", _st_savLog,  false), 
			_st_Exp_sond(false),  st_Exp_sond(this, "Programm option- re-export probes",	"Exp_probes", _st_Exp_sond,false),  
			_st_ExpTarg(false),   st_ExpTarg(this, "Programm option- re-export targets",	"Exp_target", _st_ExpTarg, false),
							_InputTargetFile(this, "Imput file for Targets",				"TargetFile", ""			) ,
							_OutputFile     (this, "Results output file",					"OutputFile", ""			) ,
							_InputNNFile    (this, "Imput file with NN parametrs",			"iNNParFile", ""			) , 
							_PCRfiltrPrFile (this, "Imput file with primers for filtering",	"PCRftrFile", ""			),
			//_pSaltCorrNNp(nullptr),
			_pSeqTree			(CreateRoot()), 
			_pSeqNoUsed			(AddSeqGroup(_pSeqTree.get(), "Dont use")), 
            _pSeqTargets		(AddSeqGroup(_pSeqTree.get(), "Target seq")),
            _pSeqNonTargets		(AddSeqGroup(_pSeqTree.get(), "Non Target seq")),
            _pPCRfiltrePrimers	(AddSeqGroup(_pSeqTree.get(), "PCR Primers to <filtre> sequences"))
	{	TAMeth.AddStrValues("TAMeth_Tm",	TAMeth_Tm);
		TAMeth.AddStrValues("TAMeth_G",		TAMeth_G);
		TAMeth.AddStrValues("TAMeth_Fract",	TAMeth_Fract);
		SaltCorr.AddStrValues("NoSelect",	NoSelect);
		SaltCorr.AddStrValues("StLucia",	StLucia);
		SaltCorr.AddStrValues("Owczarzy",	Owczarzy);
	} 
	//enum SaltCorrection {NoSelect  =-1 , StLucia  =0 , Owczarzy  =1 }; // ya se puede usar StLucia inicializando todo en el constructor. Parcialmente implementado cambio de Conc
	//enum SaltCorrecP	{NoSMSelect=-1 , SMStLucia=0 , SMOwczarzy=1 }; // Unificar !!
	//enum AlignMeth		{TAMeth_Tm =0  , TAMeth_G    , TAMeth_Fract };
    //        /*C_str			_InputTargetFile ;	*/
    //        /*C_str			_PCRfiltrPrFile ;	*/
    //        /*C_str			_OutputFile    ;	*/
    //        /*C_str			_InputNNFile    ;	*/
	CParamC_str     _InputTargetFile ;  
	CParamC_str     _PCRfiltrPrFile;    
	CParamC_str     _OutputFile;        
	CParamC_str     _InputNNFile; 

	SaltCorrection 	_SaltCorr ;			
    CParamEnumRange<SaltCorrection>	SaltCorr ;	 				//  SaltCorrection

	float					_ConcSd,	_ConcTg,	_ConcSalt ;
	CParamNumRange<float>	 ConcSd,	 ConcTg,	 ConcSalt;

	Temperature		_Ta ;				CParamNumRange<Temperature>	Ta ;	
	AlignMeth		_TAMeth ;			CParamEnumRange<AlignMeth>	TAMeth ;	 
	float			_MaxTgId ;			CParamNumRange<float>	MaxTgId ;	
	LonSecPosRang	_SecLim;			CParamNumMinMax<LonSecPos> SecLim;	//	long _SecBeg, _SecEnd;  // convertir en NumRang<long> _SecLim;  ?????
	SecPos			_MinSecLen;			CParamNumRange<SecPos>	MinSecLen;		
	
	bool			_loadNNPar,		_saveNNPar ;	CParamBool loadNNPar,	saveNNPar ; 
	bool			_st_savTm, _st_savPos, _st_savG, _st_savAlign, _st_savProj, _st_savG_Plasm, _st_savTm_Plasm, _st_savLog, _st_Exp_sond, _st_ExpTarg ;
	CParamBool		 st_savTm,  st_savPos,  st_savG,  st_savAlign,  st_savProj,  st_savG_Plasm,  st_savTm_Plasm,  st_savLog,  st_Exp_sond,  st_ExpTarg ;

	std::shared_ptr<CSaltCorrNN>  _pSaltCorrNNp;
	std::shared_ptr<CMultSec>     _pSeqTree;
	std::shared_ptr<CMultSec>     _pSeqNoUsed;
	std::shared_ptr<CMultSec>     _pSeqTargets;
	std::shared_ptr<CMultSec>     _pSeqNonTargets;
	std::shared_ptr<CMultSec>     _pPCRfiltrePrimers;
	CMultSec *AddSeqFromFile(CMultSec *parentGr, const std::string& FileName);
	CMultSec *AddTargetFromFile(const std::string& FileName)
	{
		return AddSeqFromFile(_pSeqTargets.get(),FileName);
	}
	CMultSec *AddNoTargetFromFile(const std::string& FileName)
	{
		return AddSeqFromFile(_pSeqNonTargets.get(),FileName);
	}
	CMultSec *AddPCRfiltreFromFile(const std::string& FileName)
	{
		return AddSeqFromFile(_pPCRfiltrePrimers.get(),FileName);
	}

	CMultSec *AddSeqGroup	(CMultSec *parentGr, const std::string&     Name);
	CMultSec* CreateRoot	();

	// convertirlas en funciones "previas a la paralelizacion", que hacen copias propias de los parametros en serie, no en paralelo
	void    TargetFile(const char *InputTargetFile)	{	_InputTargetFile.CopyTrim(InputTargetFile);	}
	void    OutputFile(const char *OutputFile     )	{	_OutputFile.CopyTrim(OutputFile)    ;	}
	void    NNParaFile(const char *InputNNFile)		{	_InputNNFile.CopyTrim(InputNNFile)	;	}
	
	// OJO !!!!!!!!!   las sig funciones se aduenan del pointer, y luego lo deletean    !!!!!!!!
	void SetTargetFile(      char *InputTargetFile)	{	TargetFile(InputTargetFile)	;	delete []InputTargetFile;	}
	void SetOutputFile(      char *Output_File    )	{	OutputFile(Output_File )	;	delete []Output_File		;	}
	void SetNNParaFile(      char *InputNNFile    )	{	NNParaFile(InputNNFile )	;	delete []InputNNFile		;	}

	virtual	~ThDyCommProgParam(void) override;//	{	/*delete []_ProgList;*/}
};

class CProgParam_microArray ;
int microArrayProg	( char *InputPrimer, char *InputTarget, char *OutputTm)  ;
int microArrayProg   ( CProgParam_microArray   *IPrgPar_uArr )  ;
template <typename Num> class CTable ;
//typedef uArr_RT CTable<TmGPos> ;
class CEspThDyProgParam : public CEspProg
{public:
	ThDyCommProgParam& _cp;
	CEspThDyProgParam(const std::string& titel, ThDyCommProgParam &commThDyParam)
		: CEspProg(titel, commThDyParam),
		  _cp(commThDyParam)
	{}
	~CEspThDyProgParam()override{}
};

class CProgParam_microArray : public CEspThDyProgParam
{public:	
	std::shared_ptr<CMultSec>   _probesMS;
		CMultSec *AdduArrFromFile(const std::string& FileName)
	{
		return _cp.AddSeqFromFile(_probesMS.get(),FileName);
	}
	CParamC_str		_InputSondeFile ;   /*C_str			_InputSondeFile ; */
	//bool			    _I, _G;			// Outpu table of I, G. 
	//CParamBool		 I,  G;			// Outpu table of I, G. 

    explicit 	CProgParam_microArray(const std::string& titel, ThDyCommProgParam &commThDyParam) 
		    :	CEspThDyProgParam(titel, commThDyParam), 
				_InputSondeFile	(this, "Imput file for Sondes",				"iSonde_uAr", ""			) ,
				_rtbl(nullptr),
                _probesMS(_cp.AddSeqGroup(_cp._pSeqTree.get(), "Probes of Virtual uArr"))
	        {		
			} 

	CTable<TmGPos> *_rtbl;		                                    //uArr_RT *_rtbl;
    void RenameSondesMS(const std::string& name);
	virtual int Run (){	return microArrayProg ( this )  ;}

	void    SondeFile (const char *InputSondeFile )	{	_InputSondeFile.CopyTrim(InputSondeFile) ;	}
	// OJO !!!!!!!!!   la sig funcion se aduena del pointer, y luego lo deletea   !!!!!!!!
	void SetSondeFile (      char *InputSondeFile )	{	SondeFile( InputSondeFile) ;	delete []InputSondeFile   ;	}

	// cuando se corre un proceso paralelo ver donde es mejor hacer estos delete.
	virtual ~CProgParam_microArray()	override	;
};
 /*,_tlG(nullptr),_tlPos(nullptr), UpDate(nullptr)*/ 
				/*_I(false),   I				(this, "Programm option- Save Tm Table",			"SavTmTable", _I,   false), 
				  _G(true),    G				(this, "Programm option- Save Tm Table",			"SavTmTable", _G,   true), */


class	CProgParam_uArrExp;
int		microArrayProgTest ( CProgParam_uArrExp *IPrgPar_uArr)  ;
class CProgParam_uArrExp  : public CProgParam_microArray
{public:	
	C_str		_Input_uArrExpFile ; 
	bool		_exclSd;					// No analizar (excluir) determinadas sondas (otros org - micobact, contaminadas, Post contr, Neg contr, etc)
	C_str		_exclSdMark;				// solo las sondas con esta marca son excl. Si ="" todas las marcadas. Si _exclSd -  se ignora, y nada se excl.
	bool		_IxI,		_IxI_d;			// output these table (metodo de comparacion - solo similitudes o similitudes menos diferencias?)
	bool		_Normalize;					// las tablas anteriores : puede suponer perdida de informacion para calcular signif estadistica??
	Energy		_Isat, _Isen, _Gsat, _Gsen;

    explicit 	CProgParam_uArrExp (const std::string& titel, ThDyCommProgParam &commThDyParam);

	int		Run		(	){	return microArrayProg ( this )  ;}
	void    ExpFile (const char *Input_uArrExpFile )	{	_Input_uArrExpFile.CopyTrim(Input_uArrExpFile) ;	}

	// OJO !!!!!!!!!   la sig funcion se aduena del pointer, y luego lo deletea   !!!!!!!!
	void SetExpFile (      char *Input_uArrExpFile )	{	ExpFile( Input_uArrExpFile) ;	delete []Input_uArrExpFile   ;	}

	// cuando se corre un proceso paralelo ver donde es mejor hacer estos delete.
	virtual ~CProgParam_uArrExp()		{}
};

class CProgParam_MultiplexPCR ;
int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr )  ;
class CProgParam_MultiplexPCR : public CProgParam_microArray
{public:
	CProgParam_MultiplexPCR(const std::string& titel, ThDyCommProgParam &commThDyParam) ;
	int		Run		(){	return MultiplexPCRProg ( this )  ;}
	CTable<TmGPos> *_rtbl_self;		                                    //uArr_RT *_rtbl;
};

class CProgParam_SondeDesign ;
int SondeDesignProg  ( CProgParam_SondeDesign  *IPrgPar_SdDes)  ;
class CProgParam_SondeDesign : public CEspThDyProgParam			//  .------------------------	CProgParam_SondeDesign	----------------
{public:



	CProgParam_SondeDesign(const std::string& titel,ThDyCommProgParam &commThDyParam):		
		CEspThDyProgParam (titel, commThDyParam),
		_design (true), design(this, "Make only design or full comp",	"DesigVsCmp", _design,  true),
        sL(this, "Sondes to design limits", _sL,							 
		        "MinSonde_G",  -100.0f,   100.0f,  -5.0f,
		        "MaxSonde_G",  -100.0f,   100.0f,  -1.0f,
		        "MinSondeTm",  -270.0f,    90.0f,	57.0f,
		        "MaxSondeTm",  -270.0f,    99.0f,	63.0f,
		        "MinSondeLn",  3,         200,		20,
		        "MaxSondeLn",  4,         200,		35,
                /*UnitG=*/"kcal/mol",  /*UnitT=*/"°C",  /*UnitL=*/"nt" 
		),
		_G_sig (10),		  G_sig  (this, "Significative G probe-target",		"MaxSd_Tg_G",    _G_sig,  -15.0f, 30.0f,	10.0f, "kcal/mol" ),	
		_Tm_sig (30)	, 	  Tm_sig (this, "Significative Tm probe-target",	"MinSd_TgTm",    _Tm_sig, -0.0f,  80.0f,	30.0f, "°C" ),	

		_MinSd_nTgG (15) , MinSd_nTgG(this, "Significative G probe-nontarget",	"MinSdnTg_G", _MinSd_nTgG,  0.0f, 30.0f,	15.0f, "kcal/mol" ),		
		_MaxSd_nTgTm (10),MaxSd_nTgTm(this, "Significative Tm probe-nontarget",	"MaxSdnTgTm",_MaxSd_nTgTm, -0.0f,  70.0f,	10.0f, "°C" ),	 

		_MinSelfG (10),		 MinSelfG(this, "Significative selfprobe G",		"MinSdSlf_G", _MinSelfG,  0.0f, 30.0f,		10.0f, "kcal/mol" ),		
		_MaxSelfTm (10),    MaxSelfTm(this, "Significative selfprobe Tm",		"MaxSdSlfTm",_MaxSelfTm, -0.0f,  70.0f,		10.0f, "°C" ),	
		_MinTgCov (100),	MinTgCov (this, "Find sondes with more % coverage",	"Min_Tg_Cov",  _MinTgCov,  0.0f,100.0f,		99.0f  ,"%")							
        {
	    }  // revisar cuales deben ser estos valores !!!!	

    bool		_design ;  // realizar solo diseno de sondas o solo comparacion de sec????
	CParamBool	design ;  // realizar solo diseno de sondas o solo comparacion de sec????

	SondeLimits _sL ;	
	CParamSondeLimits sL;                                    

	Temperature	               _Tm_sig ;	    Energy		            _G_sig ;				// sonde  - target
	Temperature	               _MaxSd_nTgTm ;   Energy		            _MinSd_nTgG ;			// sonde  - non target
	Temperature	               _MaxSelfTm ;	    Energy		            _MinSelfG  ;			// sonde 
	CParamNumRange<Temperature>	Tm_sig ;	    CParamNumRange<Energy>   G_sig ;				// sonde  - target
	CParamNumRange<Temperature>	MaxSd_nTgTm ;	CParamNumRange<Energy>   MinSd_nTgG ;			// sonde  - non target
	CParamNumRange<Temperature>	MaxSelfTm ;		CParamNumRange<Energy>   MinSelfG  ;			// sonde 

	float		           _MinTgCov ;
	CParamNumRange<float>	MinTgCov ;

	int		Run		(){	return  SondeDesignProg( this )  ;}
};



class CProgParam_TmCalc ;
int DegTmCalc ( CProgParam_TmCalc *IPrgPar)  ; //int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr )  ;
//typedef /*unsigned*/ char Base;
//extern Base *Generate_DegSec( const char *sec, bool rev, bool compl, long l=0) ;// , long l=0) ;
class CProgParam_TmCalc : public CProgParam_MultiplexPCR
{public:
	bool			_save, _align ; //  save results as mPCR, using the TargetSecFile and ResultFile comunes. Use ThDyAlign or just calc Tm?
	CParamBool		 save,  align ; //  save results as mPCR, using the TargetSecFile and ResultFile comunes. Use ThDyAlign or just calc Tm?
	TemperatureRang _TmS, _Tm2A , _TmHy ;	// Para recoger los reslutados y display it back. 
	EnergyRang		_GS , _G2A  , _GHy  ; 	
	CProgParam_TmCalc (const std::string& titel,ThDyCommProgParam &commThDyParam) :	
		                _save (false), _align(true),
						CProgParam_MultiplexPCR (titel,commThDyParam),
						save		(this, "Save result to file",			"TmCsaveRes", _save,   false),
						align		(this, "Align primers before Tm calc",	"TmUseAlign", _align,  true),
						_Sec		(this, "Primer",						"TmCalc_Sec", ""			) ,
						_Sec2Align	(this, "Primer to align",				"TmC_Sec2Al", ""			) 
					{
						RenameSondesMS("Tm calulator sondes??");
	                }

	bool	Set_Sec				 (char *Sec){_Sec.take(Sec)		;		 return true ;}
	bool	Set_Sec2Align		 (char *Sec){_Sec2Align.take(Sec)		;return true ;}
	bool	Set_AlignedSec		 (char *Sec){_AlignedSec.Take(Sec)		;return true ;}
	bool	Set_AlignedSec2Align (char *Sec){_AlignedSec2Align.Take(Sec);return true ;}
	bool	Copy_Sec			 (char *Sec){_Sec.set(Sec)		;		 return true ;}
	bool	Copy_Sec2Align		 (char *Sec){_Sec2Align.set(Sec)		;return true ;}
	bool	Copy_AlignedSec		 (char *Sec){_AlignedSec.Copy(Sec)		;return true ;}
	bool	Copy_AlignedSec2Align(char *Sec){_AlignedSec2Align.Copy(Sec);return true ;}


	bool	Update_Sec			(bool rev, bool compl)	{return Set_Sec      ( Generate_DegSec_char( _Sec.Get(),		rev, compl)  ); }
	bool	Update_Sec_Sec2Align(bool rev, bool compl)	{return Set_Sec2Align( Generate_DegSec_char( _Sec.Get(),		rev, compl)  ); }
	bool	Update_Sec2Align	(bool rev, bool compl)	{return Set_Sec2Align( Generate_DegSec_char( _Sec2Align.Get(),	rev, compl)  ); }
	bool	Update_Sec2Align_Sec(bool rev, bool compl)	{return Set_Sec		 ( Generate_DegSec_char( _Sec2Align.Get(),	rev, compl)  ); }

	~CProgParam_TmCalc(){}
	int		Run		(){	return DegTmCalc ( this )  ;}
//private:
	CParamC_str	 _Sec ,			_Sec2Align ;
	C_str	 _AlignedSec,	_AlignedSec2Align ;
};


class ThDyProject : public CProject // Permite manejar todo el projecto: con un miembro para los parametros comunes y otro para los de cada programa
{public:
		ThDyCommProgParam		_cp;
		CProgParam_microArray   _uArr  ;
		CProgParam_MultiplexPCR _mPCR  ;
		CProgParam_SondeDesign	_SdDes ;
		CProgParam_TmCalc		_TmCal ;

 explicit	ThDyProject():	CProject("ThDy DNA Hybrid Project.","Def.ThDy.txt","Def.ThDy.txt"),
                            _cp("Common parametrs for all functions",this),
                            _uArr("Virtual microarray experiment"   ,_cp), 
							_mPCR("Check multiplex PCR"             ,_cp), 
							_SdDes("Find sondes"                    ,_cp), 
							_TmCal("Tm calculator"                  ,_cp)
					{}
 	virtual std::ofstream &saveTMP() const     override       // Se me habia olvidado redefinir esta funcion para usar _cp.OutF
	{	return saveToFile((std::auto_ptr<char>(AddFileExt(_cp._OutputFile.Get(),".ThDy.txt"))).get());	}
};  




#endif


