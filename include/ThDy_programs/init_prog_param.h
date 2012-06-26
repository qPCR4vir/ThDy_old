#ifndef _INIT_PROG_PARAM_H
#define _INIT_PROG_PARAM_H
#pragma unmanaged
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <assert.h>
#include <time.h>
using namespace std ; 


/*   Para crear y anadir un nuevo programa:
		- crear interfase de usuario para tener idea de los parametros a usar
		- crear funcion del programa en su propio .cpp en el proyecto de prog
		- crear class tomando como base CEspProgParam o uno de sus derivados : anadir los paramet espec com se vio en la interfase e inicializarlos en el constr
		- implementar funciones de actualizar parametros <=> interfase de usuario : UpdateThDyP() & UpdateThDyForm()
		- implementar funciones load/save del project. Load: con etiqueta para cada param, "	<< boolalpha " para bool, 
*/
// Definiciones y declaraciones para user interface. A usar tambien por programs. Casi Primaria, depende solo de Common basics.
#pragma warning( disable : 4996 )

#include "..\ThDySec\common_basics.h" 
//#include "..\ThDySec\matrix.h" 
//  PROBLEMA : como organizar estos parametros si usamos procesos? Hacer copia de ellos !!!!!!!!

class	CEspProgParam ;
typedef CEspProgParam *pCEspProgParam ;

class CProgParam // -------	  Clase base "interfase" para param de prog.Solo salva/load project y run prog (virtual todo)   ----------
{public:	
	virtual bool	save		(ofstream	&osPr				) = 0;
	virtual bool	load		(string		&var, ifstream &isPr) = 0; 
	virtual bool	save_all	(ofstream	&osPr				) = 0; 
	virtual bool	load_all	(string		&var, ifstream &isPr) = 0; 
	virtual	int		Run			(CProgParam &prog				){return prog.Run();}
	virtual int		Run			(		void					){for(int WorkToDo=Initialize(); WorkToDo>0 ; WorkToDo=Continue()) 
																	CallBack(WorkToDo); 
																  return Finalize();
																 } 
	virtual int		Initialize	(		void					){ return 0;} 
	virtual int		Continue	(		void					){ return false;}
	virtual int		Finalize	(		void					){ return 0;} 
	virtual void	CallBack	(		int WorkToDo			){}

};	

class CCommProgParam : public CProgParam // concreta los parametros comunes. Mantiene lista de los prog Espec que los usan
{public:	
	CCommProgParam();
	pCEspProgParam	*_ProgList ;
	C_str			_InputTargetFile ;
	C_str			_PCRfiltrPrFile ;
	C_str			_OutputFile    ;
	C_str			_InputNNFile    ;
	int				_SaltCorr ;								//  SaltCorrection
	float			_ConcSd,		_ConcTg,	_ConcSalt ;
	Temperature		_Ta ;
	AlignMeth		_TAMeth ;
	float			_MaxTgId ;
	LonSecPosRang	_SecLim;								//	long _SecBeg, _SecEnd;  // convertir en NumRang<long> _SecLim;  ?????
	SecPos			_MinSecLen;
	
	bool			_loadNNPar,		_saveNNPar ;
	bool			_st_savTm, _st_savPos, _st_savG, _st_savAlign, _st_savProj, _st_savG_Plasm, _st_savTm_Plasm, _st_savLog, _st_Exp_sond, _st_ExpTarg ;


	// convertirlas en funciones "previas a la paralelizacion", que hacen copias propias de los parametros en serie, no en paralelo
	void    TargetFile(const char *InputTargetFile)	{	_InputTargetFile.CopyTrim(InputTargetFile);	}
	void    OutputFile(const char *OutputFile     )	{	_OutputFile.CopyTrim(OutputFile)    ;	}
	void    NNParaFile(const char *InputNNFile)		{	_InputNNFile.CopyTrim(InputNNFile)	;	}
	
	// OJO !!!!!!!!!   las sig funciones se aduenan del pointer, y luego lo deletean    !!!!!!!!
	void SetTargetFile(      char *InputTargetFile)	{	TargetFile(InputTargetFile)	;	delete []InputTargetFile;	}
	void SetOutputFile(      char *Output_File    )	{	OutputFile(Output_File )	;	delete []Output_File		;	}
	void SetNNParaFile(      char *InputNNFile    )	{	NNParaFile(InputNNFile )	;	delete []InputNNFile		;	}

	virtual	~CCommProgParam()	{	delete []_ProgList;}
	virtual bool	save	(ofstream	&osPr				);
	virtual bool	load	(string		&var, ifstream &isPr); 
	virtual bool	save_all(ofstream	&osPr				);
	virtual bool	load_all(string		&var, ifstream &isPr); 
};

class	CEspProgParam  : public CProgParam // clase base para los parametros "Especificos" de programas "Especificos"
{public:										// Permite no duplicar los parametros comunes en los parametros especificos
	explicit CEspProgParam(CCommProgParam &commParam) : _cp(commParam){}
	CCommProgParam &_cp;
	virtual bool	save_all(ofstream &osPr)				{  _cp.save(osPr) ;			return save(osPr) ;    }
	virtual bool	load_all(string &var, ifstream &isPr)	{ 	if ( _cp.load(var, isPr)) return true ;
																return load(var, isPr);					 }
} ;

class CProgParam_microArray ;
int microArrayProg	( char *InputPrimer, char *InputTarget, char *OutputTm)  ;
int microArrayProg   ( CProgParam_microArray   *IPrgPar_uArr )  ;
template <typename Num> class CTable ;
//typedef uArr_RT CTable<TmGPos> ;

class CProgParam_microArray : public CEspProgParam
{public:	
	C_str			_InputSondeFile ; 
	bool			_I, _G;			// Outpu table of I, G. 
    explicit 	CProgParam_microArray(CCommProgParam &commParam) :	_I(false), _G(true), CEspProgParam(commParam), 
																	_rtbl(nullptr)/*,_tlG(nullptr),_tlPos(nullptr), UpDate(nullptr)*/ {} 
	//uArr_RT *_rtbl;
	CTable<TmGPos> *_rtbl;	

	virtual int Run (){	return microArrayProg ( this )  ;}
	virtual bool	save(ofstream &osPr);
	virtual bool	load(string &var, ifstream &isPr); 
	//void	(*UpDate)();
	//void	UpDateUI(){if (UpDate) UpDate(); }

	void    SondeFile (const char *InputSondeFile )	{	_InputSondeFile.CopyTrim(InputSondeFile) ;	}
	// OJO !!!!!!!!!   la sig funcion se aduena del pointer, y luego lo deletea   !!!!!!!!
	void SetSondeFile (      char *InputSondeFile )	{	SondeFile( InputSondeFile) ;	delete []InputSondeFile   ;	}

	// cuando se corre un proceso paralelo ver donde es mejor hacer estos delete.
	virtual ~CProgParam_microArray()		{ /*delete _tlTm;*/}
};

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

    explicit 	CProgParam_uArrExp (CCommProgParam &commParam):	_exclSd(false),		_IxI(true),		_IxI_d(true),			_Normalize(true), 
																_Isat(Energy(0.87f)),_Isen(Energy(0.01f)),	_Gsat(Energy(-2.0f)),	_Gsen(Energy(2.0f)),
																CProgParam_microArray (commParam) {} 

	int		Run		(	){	return microArrayProg ( this )  ;}
	bool	save	(ofstream &osPr				);
	bool	load	(string &var, ifstream &isPr); 

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
	CProgParam_MultiplexPCR(CCommProgParam &commParam) : CProgParam_microArray(commParam){}
	int		Run		(){	return MultiplexPCRProg ( this )  ;}
	bool	save	(ofstream &osPr				);
	bool	load	(string &var, ifstream &isPr); 
};

class CProgParam_SondeDesign ;
int SondeDesignProg  ( CProgParam_SondeDesign  *IPrgPar_SdDes)  ;
class CProgParam_SondeDesign : public CEspProgParam			//  ------------------------	CProgParam_SondeDesign	----------------
{public:

	CProgParam_SondeDesign(CCommProgParam &commParam) ;
	bool		_design ;  // realizar solo diseno de sondas o solo comparacion de sec????
	SondeLimits _sL ;					//float		_G_min	, _G_max ;				// en kcal ...
										//float		_Tm_min	, _Tm_max ;  
										//int		_L_min	, _L_max ;
	Temperature	_Tm_sig ;			Energy		_G_sig ;				// sonde  - target
	Temperature	_MaxSd_nTgTm ;		Energy		_MinSd_nTgG ;			// sonde  - non target
	Temperature	_MaxSelfTm ;		Energy		_MinSelfG  ;			// sonde 

	float		_MinTgCov ;

	int		Run		(){	return  SondeDesignProg( this )  ;}
	bool	save	(ofstream &osPr				);
	bool	load	(string &var, ifstream &isPr); 
};



class CProgParam_TmCalc ;
int DegTmCalc ( CProgParam_TmCalc *IPrgPar)  ; //int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr )  ;
//typedef /*unsigned*/ char Base;
//extern Base *Generate_DegSec( const char *sec, bool rev, bool compl, long l=0) ;// , long l=0) ;
class CProgParam_TmCalc : public CProgParam_MultiplexPCR
{public:
	bool			_save, _align ; //  save results as mPCR, using the TargetSecFile and ResultFile comunes. Use ThDyAlign or just calc Tm?
	TemperatureRang _TmS, _Tm2A , _TmHy ;	// Para recoger los reslutados y display it back. 
	EnergyRang		_GS , _G2A  , _GHy  ; 	
	CProgParam_TmCalc (CCommProgParam &commParam) :	_save (false), _align(true),
													CProgParam_MultiplexPCR (commParam){}

	bool	Set_Sec				 (char *Sec){_Sec.Take(Sec)		;		 return true ;}
	bool	Set_Sec2Align		 (char *Sec){_Sec2Align.Take(Sec)		;return true ;}
	bool	Set_AlignedSec		 (char *Sec){_AlignedSec.Take(Sec)		;return true ;}
	bool	Set_AlignedSec2Align (char *Sec){_AlignedSec2Align.Take(Sec);return true ;}
	bool	Copy_Sec			 (char *Sec){_Sec.Copy(Sec)		;		 return true ;}
	bool	Copy_Sec2Align		 (char *Sec){_Sec2Align.Copy(Sec)		;return true ;}
	bool	Copy_AlignedSec		 (char *Sec){_AlignedSec.Copy(Sec)		;return true ;}
	bool	Copy_AlignedSec2Align(char *Sec){_AlignedSec2Align.Copy(Sec);return true ;}


	bool	Update_Sec			(bool rev, bool compl)	{return Set_Sec      ( Generate_DegSec_char( _Sec.Get(),		rev, compl)  ); }
	bool	Update_Sec_Sec2Align(bool rev, bool compl)	{return Set_Sec2Align( Generate_DegSec_char( _Sec.Get(),		rev, compl)  ); }
	bool	Update_Sec2Align	(bool rev, bool compl)	{return Set_Sec2Align( Generate_DegSec_char( _Sec2Align.Get(),	rev, compl)  ); }
	bool	Update_Sec2Align_Sec(bool rev, bool compl)	{return Set_Sec		 ( Generate_DegSec_char( _Sec2Align.Get(),	rev, compl)  ); }

	~CProgParam_TmCalc(){}
	int		Run		(){	return DegTmCalc ( this )  ;}
	bool	save	(ofstream &osPr				);
	bool	load	(string &var, ifstream &isPr); 
//private:
	C_str	 _Sec ,			_Sec2Align ;
	C_str	 _AlignedSec,	_AlignedSec2Align ;
};

enum Prog2Run {µArr_Prog=0, mPCR_Prog, SdDes_Prog, TmCal_Prog, Num_of_Prog };

class ThDyProjet : public CProgParam // Permite manejar todo el projecto: con un miembro para los parametros comunes y otro para los de cada programa
{public:
		CCommProgParam			_cp;
		CProgParam_microArray   _uArr  ;
		CProgParam_MultiplexPCR _mPCR  ;
		CProgParam_SondeDesign	_SdDes ;
		CProgParam_TmCalc		_TmCal ;

 explicit	ThDyProjet():	_uArr(_cp), 
							_mPCR(_cp), 
							_SdDes(_cp), 
							_TmCal(_cp), 
							_ProjetFileName("Def.ThDy.txt")   ,
							_defPr("Def.ThDy.txt")   
					{	_cp._ProgList=new pCEspProgParam[Num_of_Prog+1];		// cambiar por un vector de la STL??
						_cp._ProgList[µArr_Prog ]	=&_uArr;
						_cp._ProgList[mPCR_Prog ]	=&_mPCR;	
						_cp._ProgList[SdDes_Prog]	=&_SdDes;	
						_cp._ProgList[TmCal_Prog]	=&_TmCal;	
						_cp._ProgList[Num_of_Prog]	= 0 ;
					}
 
		C_str						_ProjetFileName ;
		C_str						_defPr ;

		    ~ThDyProjet() { }

	void	ProjetFile	(const char *ProjetFileName){	_ProjetFileName.CopyTrim(	ProjetFileName);	}
	bool	save		()							{	return save_using(_ProjetFileName.Get())						 ;}
	bool	save_using	(char	*ProjetFileName)	{	ofstream osPr(ProjetFileName);				return save_all(osPr);}
		
	bool	load		(); 
	bool	save_defPr	(){ProjetFile(_defPr.Get()); return save();	}
									// OJO !!!!!!!!!   las sig funciones se aduenan del pointer, y luego lo deletean    !!!!!!!!
	void	SetProjetFile(char  *ProjetFileName){   ProjetFile(ProjetFileName) ; 		delete []ProjetFileName ;				}
	bool	saveTMP		 (char	*ProjetFileName){	bool r=save_using(ProjetFileName);	delete []ProjetFileName	; return r ;	}
	bool	save		 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return save();			}
	bool	load		 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return load();			}

	virtual bool	save    (ofstream &osPr)			 ;//{  	return true ;    }
	virtual bool	load    (string &var, ifstream &isPr);//{ 	if (true) return false ; return true ;}
	virtual bool	save_all(ofstream &osPr)				{  _cp.save_all(osPr) ;			return save(osPr) ;    }
	virtual bool	load_all(string &var, ifstream &isPr)	{ 	if ( _cp.load_all(var, isPr)) return true ;
																return load(var, isPr);					 }
	virtual	int		Run (CProgParam &prog)	{	saveTMP( AddFileExt(_cp._OutputFile.Get(),".ThDy.txt") ); return prog.Run();}
};  


#endif


	//class	CInitProgParam_SondeDesign;
	//class	CInitProgParam_microArray;
	//class	CInitProgParam_MultiplexPCR;

//enum SaltCorrecP {NoSMSelect=-1,SMStLucia=0, SMOwczarzy=1};
//enum AlignMeth {TAMeth_Tm=0, TAMeth_G, TAMeth_Fract};
////  PROBLEMA : como organizar estos parametros si usamos procesos? Hacer copia de ellos !!!!!!!!
//
//inline char *clone_c_str   (const char *str);
//inline char *clone_trim_str(const char *str);
//char *AddFileExt(const char *FileName, const char *Ext="")	;	// no olvide delete este pointer
////  Crear clase para lidiar con las string como file name y sec que deben ser copiadas y liberadas todo el tiempo. Copy() vs Take() !!!!	
//
//class C_str
//{public:
//	explicit C_str()													:_s(clone_c_str		("")){}
//	explicit C_str(const char *s)										:_s(clone_c_str		(s )){}
//
//	char *Copy		(const char *s)	{if(s!=_s) delete []_s; _s=clone_c_str		(s );				return _s;}
//	char *CopyTrim	(const char *s)	{if(s!=_s) delete []_s; _s=clone_trim_str	(s );				return _s;}
//	char *Trim		(			)	{char *t=_s;  			_s=clone_trim_str	(_s); delete []t;	return _s;}
//	char *Take		(	   char *s) {if(s!=_s) delete []_s; return	 _s=				(s );}
//	char *TakeTrim	(	   char *s) { Take(s); return Trim();}
//	char *Get		()				{						return	 _s						;}
//	~C_str			()				{delete []_s;									 }
//private:
//	char *_s;
//};
//
//template <class Num>
//class NumRang
//{public: Num min,max; 
//	NumRang(					):min(0 )	,max(0 ){}; 
//	NumRang(		Num av		):min(av)   ,max(av){};
//	NumRang(Num mi,       Num ma):min(mi)   ,max(ma){};
////	NumRang(Num mi, Num av, Num ma):min(mi),ave(av)        ,max(ma){};
//
////	void Set(Num mi, Num av, Num ma){min=mi; ave=av       ; max=ma;}
//	void Set(Num mi,           Num ma){min=mi;  max=ma;}
//	void Set(         Num av         ){min=av;  max=av;}
//	void SetMin(Num mi){min=mi;  }
//	void SetMax(Num ma){max=ma;  }
////	void Set(                            ){        ave=(min+max)/2      ;}
//	Num Ave() const {return (min+max)/2 ;}
//	Num Min() const {return min			;}
//	Num Max() const {return max			;}
//	bool inRang(const Num X)const {return (min<= X && X<=max);}
//};
//template <class Num>
//inline	bool inRang(const Num X, const NumRang<Num> R)  {return  R.inRang(X) ;}
//
//extern const 	NumRang<float> G_def, Tm_def ;  //  G_def(-5,-1), Tm_def(57,63) ;
//extern const 	NumRang<int>   L_def ;			//  L_def(20,35) ;
//class SondeLimits
//{public: 
//	NumRang<float> _G, _Tm ;
//	NumRang<int>   _L ;
//	SondeLimits():_G(G_def),_Tm(Tm_def),_L(L_def){}
//	SondeLimits(NumRang<float> G, NumRang<float> Tm, NumRang<int> L):_G(G),_Tm(Tm), _L(L){}
//};
//
//class DegRes			//------------------------------------------------------------	DegRes	-------------------------------------
//{public: float min,ave,max; 
//	DegRes(							   ):min(0 ),ave(0 )		,max(0 ){}; 
//	DegRes(			 float av		   ):min(av),ave(av)        ,max(av){};
//	DegRes(float mi,           float ma):min(mi),ave((mi+ma)/2) ,max(ma){};
//	DegRes(float mi, float av, float ma):min(mi),ave(av)        ,max(ma){};
//
//	void Set(float mi, float av, float ma){min=mi; ave=av       ; max=ma;}
//	void Set(float mi,           float ma){min=mi; ave=(mi+ma)/2; max=ma;}
//	void Set(          float av          ){min=av; ave=av       ; max=av;}
//	void Set(                            ){        ave=(min+max)/2      ;}
//};