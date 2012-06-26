//#include "StdAfx.h"
#pragma unmanaged
#include "ThDy_programs/init_prog_param.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

#include <iomanip>
using namespace std;






const long MAXSEC_LENGHT=2000-1 ; // un aproximado de lo que creo acepta mi PC con 4 GB

CCommProgParam::CCommProgParam()
		:	_ProgList(nullptr),
			_loadNNPar(false), _saveNNPar(false),
			_SaltCorr(SMStLucia),   //SMStLucia
			_TAMeth(TAMeth_Tm),
			_MaxTgId(99.0f),
			_ConcSd(50e-9), _ConcTg(50e-9), 
			_ConcSalt(50e-3),
			_Ta(55.0f), 
			_SecLim( long(1), long(1)+MAXSEC_LENGHT ) ,//_SecBeg(long(1)), _SecEnd(long(1)+MAXSEC_LENGHT) , 
			_MinSecLen(0),
			_st_savTm(true) , _st_savPos(true), _st_savG(true), _st_savAlign(true), _st_savProj(true), 
			_st_savG_Plasm(false), _st_savTm_Plasm(false), _st_savLog(false), _st_Exp_sond(false), _st_ExpTarg(false)

		{}
bool	CCommProgParam::save(ofstream &osPr)
{
	osPr	<< "TargetFile: " << _InputTargetFile.Get()	<< endl
			<< "OutputFile: " << _OutputFile.Get()		<< endl
			<< "SaltCorrMt: " << _SaltCorr			<< endl
			<< "ConcSonden: " << _ConcSd			<< endl
			<< "ConcTarget: " << _ConcTg			<< endl
			<< "ConcenSalt: " << _ConcSalt			<< endl
			<< "TempAnnelg: " << _Ta				<< endl
			<< "AlignMethd: " << _TAMeth			<< endl
			<< "MaxTgIdent: " << _MaxTgId			<< endl
			<< "SecBeginng: " << _SecLim.Min()		<< endl	 // << _SecBeg		
			<< "SecuencEnd: " << _SecLim.Max()		<< endl  // << _SecEnd	
			<< "MinSeqLeng: " << _MinSecLen			<< endl

			<< "LoadNNPara: " << boolalpha << _loadNNPar			<< endl
			<< "SaveNNPara: " << boolalpha << _saveNNPar			<< endl
			<< "iNNParFile: " << _InputNNFile.Get()		<< endl 	

			<< "SavTmTable: " << boolalpha << _st_savTm				<< endl
			<< "SavPosTabl: " << boolalpha << _st_savPos			<< endl
			<< "SaveGTable: " << boolalpha << _st_savG				<< endl
			<< "SavAlignTb: " << boolalpha << _st_savAlign			<< endl
			<< "SavProject: " << boolalpha << _st_savProj			<< endl
			<< "SavG_Plasm: " << boolalpha << _st_savG_Plasm		<< endl
			<< "SavTmPlasm: " << boolalpha << _st_savTm_Plasm		<< endl
			<< "Save_Logfi: " << boolalpha << _st_savLog			<< endl
			<< "Exp_probes: " << boolalpha << _st_Exp_sond			<< endl
			<< "Exp_target: " << boolalpha << _st_ExpTarg			<< endl


;	return true;
}
bool	CCommProgParam::load(string &var, ifstream &isPr)
{	string val ;			int num ;
	if			(var.find( "TargetFile"  ) != string::npos)	{getline (isPr,	val) ; 	TargetFile(val.c_str()  ) ;} 
	else if		(var.find( "OutputFile"  ) != string::npos)	{getline (isPr,	val) ;	OutputFile(val.c_str()  ) ;}
	else if		(var.find( "SaltCorrMt"  ) != string::npos) isPr>>_SaltCorr	;
	else if		(var.find( "ConcSonden"  ) != string::npos) isPr>>_ConcSd	;
	else if		(var.find( "ConcTarget"  ) != string::npos) isPr>>_ConcTg	;
	else if		(var.find( "ConcenSalt"  ) != string::npos) isPr>>_ConcSalt	;
	else if		(var.find( "TempAnnelg"  ) != string::npos) isPr>>_Ta		;
	else if		(var.find( "AlignMethd"  ) != string::npos){isPr>> num				; _TAMeth=AlignMeth(num) ;}
	else if		(var.find( "MaxTgIdent"  ) != string::npos) isPr>>_MaxTgId	;
	else if		(var.find( "SecBeginng"  ) != string::npos) isPr>>_SecLim.Min()					/*>>_SecBeg  */	;
	else if		(var.find( "SecuencEnd"  ) != string::npos) isPr>>_SecLim.Max()					/*>>  _SecEnd */;
	else if		(var.find( "MinSeqLeng"  ) != string::npos) isPr>>_MinSecLen;

	else if		(var.find( "LoadNNPara"  ) != string::npos) isPr>>boolalpha>>_loadNNPar		;
	else if		(var.find( "SaveNNPara"  ) != string::npos) isPr>>boolalpha>>_saveNNPar		;
	else if		(var.find( "iNNParFile"  ) != string::npos)	{getline (isPr,	val) ;	NNParaFile(val.c_str()  ) ;} 

	else if		(var.find( "SavTmTable"  ) != string::npos) isPr>>boolalpha>>_st_savTm		;
	else if		(var.find( "SavPosTabl"  ) != string::npos) isPr>>boolalpha>>_st_savPos		;
	else if		(var.find( "SaveGTable"  ) != string::npos) isPr>>boolalpha>>_st_savG		;
	else if		(var.find( "SavAlignTb"  ) != string::npos) isPr>>boolalpha>>_st_savAlign	;
	else if		(var.find( "SavProject"  ) != string::npos) isPr>>boolalpha>>_st_savProj	;
	else if		(var.find( "SavG_Plasm"  ) != string::npos) isPr>>boolalpha>>_st_savG_Plasm	;
	else if		(var.find( "SavTmPlasm"  ) != string::npos) isPr>>boolalpha>>_st_savTm_Plasm;
	else if		(var.find( "Save_Logfi"  ) != string::npos) isPr>>boolalpha>>_st_savLog		;
	else if		(var.find( "Exp_probes"  ) != string::npos) isPr>>boolalpha>>_st_Exp_sond	;
	else if		(var.find( "Exp_target"  ) != string::npos) isPr>>boolalpha>>_st_ExpTarg	;

	else return false ;		
	return true ;
}
bool	CCommProgParam::save_all(ofstream &osPr)
{	save(osPr);
	for (int i=0; _ProgList[i]; i++) 
		_ProgList[i]->save(osPr) ;
	return true ;
}
bool	CCommProgParam::load_all(string &var, ifstream &isPr)
{	if ( load(var, isPr)) return true ;
	for (int i=0; _ProgList[i]; i++) 
		if (_ProgList[i]->load(var, isPr)) return true ;
	return false ;			
}


bool	CProgParam_microArray::save(ofstream &osPr)
{	osPr	<< "iSonde_uAr: " << _InputSondeFile.Get()	<< endl 
;	return true;
}
bool	CProgParam_microArray::load(string &var, ifstream &isPr)
{	string val ;	
	if	(var.find( "iSonde_uAr"  ) != string::npos) {getline (isPr,	val) ;	SondeFile(val.c_str() ) ;} 
	else return false ;		
	return true ;
}
int		microArrayProg ( char *InputPrimer, char *InputTarget, char *OutputTm) // se apropia de los punteros a FileName's
{	CCommProgParam cp;
	CProgParam_microArray  IPrgPar_uArr(cp);
		IPrgPar_uArr.SetSondeFile ( InputPrimer );
		IPrgPar_uArr._cp.SetTargetFile( InputTarget  ) ;
		IPrgPar_uArr._cp.SetOutputFile( OutputTm) ;

		return	IPrgPar_uArr.Run();
}

bool	CProgParam_MultiplexPCR::save(ofstream &osPr)
{	osPr	<< "iSonde_PCR: " << _InputSondeFile.Get()	<< endl 
;	return true;
}
bool	CProgParam_MultiplexPCR::load(string &var, ifstream &isPr)
{	string val ;	
	if	(var.find( "iSonde_PCR"  ) != string::npos) {getline (isPr,	val) ;	SondeFile(val.c_str() ) ;} 
	else return false ;		
	return true ;
}

const 	NumRang<float> G_def(-5,-1), Tm_def(57,63) ;
const 	NumRang<int>   L_def(20,35) ;
CProgParam_SondeDesign::CProgParam_SondeDesign (CCommProgParam &commParam):		
									CEspProgParam (commParam),
									_design (true),
									_sL(G_def, Tm_def, L_def),
									//_G_min (-5)	,		_G_max (-1)   ,
									//_Tm_min (57),		_Tm_max (63)	,
									//_L_min (20),		_L_max (35),  // arreglar esto !!!
									_G_sig (10),		_Tm_sig (30)	, 
									_MinSd_nTgG (15) ,	_MaxSd_nTgTm (10) , 
									_MinSelfG (10),		_MaxSelfTm (10),	
									_MinTgCov (100)								{}  // revisar cuales deben ser estos valores !!!!
bool	CProgParam_SondeDesign::save(ofstream &osPr)
{	osPr	<< "MinSonde_G: " << _sL._G.Min()		<< endl // <<  _G_min
			<< "MaxSonde_G: " << _sL._G.Max()		<< endl // <<  _G_max
			<< "MinSondeTm: " << _sL._Tm.Min()		<< endl // <<   _Tm_min
			<< "MaxSondeTm: " << _sL._Tm.Max()		<< endl // << _sL._Tm.Max()    _Tm_max
			<< "MinSondeLn: " << _sL._L.Min()		<< endl // << _sL._L.Min()  _L_min
			<< "MaxSondeLn: " << _sL._L.Max() 		<< endl // << _sL._L.Max()   _L_max

			<< "MaxSd_Tg_G: " << _G_sig				<< endl   
			<< "MinSd_TgTm: " << _Tm_sig			<< endl   
			<< "MinSdnTg_G: " << _MinSd_nTgG		<< endl   
			<< "MaxSdnTgTm: " << _MaxSd_nTgTm		<< endl   
			<< "MinSdSlf_G: " << _MinSelfG			<< endl 
			<< "MaxSdSlfTm: " << _MaxSelfTm			<< endl 

			<< "Min_Tg_Cov: " << _MinTgCov			<< endl 
;	return true;
}
bool	CProgParam_SondeDesign::load(string &var, ifstream &isPr)
{		 if		(var.find( "MinSonde_G"  ) != string::npos) isPr>>_sL._G.Min()	; //  _sL._G.min _G_min
	else if		(var.find( "MaxSonde_G"  ) != string::npos) isPr>>_sL._G.Max()	; //  _sL._G.max _G_max
	else if		(var.find( "MinSondeTm"  ) != string::npos) isPr>>_sL._Tm.Min()	; //  _sL._Tm.min _Tm_min
	else if		(var.find( "MaxSondeTm"  ) != string::npos) isPr>>_sL._Tm.Max() 	; //  _sL._Tm.max _Tm_max
	else if		(var.find( "MinSondeLn"  ) != string::npos) isPr>>_sL._L.Min()	; //  _sL._L.min _L_min
	else if		(var.find( "MaxSondeLn"  ) != string::npos) isPr>>_sL._L.Max()	; //  _sL._L.max _L_max

	else if		(var.find( "MaxSd_Tg_G"  ) != string::npos) isPr>>_G_sig		;
	else if		(var.find( "MinSd_TgTm"  ) != string::npos) isPr>>_Tm_sig		;
	else if		(var.find( "MinSdnTg_G"  ) != string::npos) isPr>>_MinSd_nTgG	;
	else if		(var.find( "MaxSdnTgTm"  ) != string::npos) isPr>>_MaxSd_nTgTm	;
	else if		(var.find( "MinSdSlf_G"  ) != string::npos) isPr>>_MinSelfG		;
	else if		(var.find( "MaxSdSlfTm"  ) != string::npos) isPr>>_MaxSelfTm	;

	else if		(var.find( "Min_Tg_Cov"  ) != string::npos) isPr>>_MinTgCov		;
	else return false ;		
	return true ;
}

bool	CProgParam_TmCalc::save(ofstream &osPr)
{	osPr	<< "TmCalc_Sec: " << _Sec.Get()			<< endl   
			<< "TmC_Sec2Al: " << _Sec2Align.Get()	<< endl  	<< boolalpha 
			<< "TmCsaveRes: " << _save				<< endl  
			<< "TmUseAlign: " << _align				<< endl  
;	return true;
}
bool	CProgParam_TmCalc::load(string &var, ifstream &isPr)
{	string val ;	
	if		(var.find( "TmCalc_Sec"  ) != string::npos) {getline (isPr,	val) ; 	_Sec.CopyTrim(val.c_str() )		  ;}
	else if	(var.find( "TmC_Sec2Al"  ) != string::npos) {getline (isPr,	val) ;	_Sec2Align.CopyTrim(val.c_str() ) ;} 
	else if	(var.find( "TmCsaveRes"  ) != string::npos) isPr>>boolalpha>>_save	;
	else if	(var.find( "TmUseAlign"  ) != string::npos) isPr>>boolalpha>>_align	;
	else return false ;		
	return true ;
}



bool	ThDyProjet::load()
{
	ifstream isPr( _ProjetFileName.Get() ); 	
	if ( ! isPr ) return false; //{cerr << "File "<< _ProjetFileName <<" could not be opened "<<endl ; return false;} 
	isPr >> skipws ;

	do {	string var ;			    getline (isPr, var,':') ;	       if ( ! isPr.good() ) return true;
			if ( ! load_all(var, isPr))	         isPr.ignore(1000,'\n')  ;
		}
	while (isPr.good() ); 
	return true;
}
bool	ThDyProjet::save    (ofstream &osPr)			// salvar param especif unicos del proj : ProgToRun	
{	/*osPr	<< "TmCalc_Sec: " << _Sec.Get()			<< endl   
			<< "TmC_Sec2Al: " << _Sec2Align.Get()	<< endl  	<< boolalpha 
			<< "TmCsaveRes: " << _save				<< endl  
			<< "TmUseAlign: " << _align				<< endl  */
;	return true;
}
bool	ThDyProjet::load    (string &var, ifstream &isPr)	// load param especif unicos del proj : ProgToRun	
{	string val ;	
	if		(var.find( "iSondeFile"  ) != string::npos) {getline (isPr,	val) ; 	_uArr.SondeFile(val.c_str() ) ;			// compatib con versiones 
																				_mPCR.SondeFile(val.c_str() ) ;	  ;}	// viejas con param comunes
	//else if	(var.find( "TmC_Sec2Al"  ) != string::npos) {getline (isPr,	val) ;	_Sec2Align.CopyTrim(val.c_str() ) ;} 
	//else if	(var.find( "TmCsaveRes"  ) != string::npos) isPr>>boolalpha>>_save	;
	//else if	(var.find( "TmUseAlign"  ) != string::npos) isPr>>boolalpha>>_align	;
	else return false ;		
	return true ;
} 	
