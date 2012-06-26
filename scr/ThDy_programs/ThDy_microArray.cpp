//#include "StdAfx.h"
#pragma unmanaged

#include "ThDy_programs/prog_comm_functions.h"


int microArrayProg ( CProgParam_microArray *IPrgPar_uArr)  
{
	time_t t_0 = time(NULL);
	const int MaxGrDeg=300 ;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??

		string of( IPrgPar_uArr->_cp._OutputFile.Get() ), f;

	f=of+".uArr.Tm.csv" ;	ofstream osTm;		if (IPrgPar_uArr->_cp._st_savTm		)	{ osTm.open		(f.c_str()	);	assert(osTm		);}
	f=of+".uArr.G.csv"  ;	ofstream osG ;		if (IPrgPar_uArr->_cp._st_savG		)	{ osG.open		(f.c_str()	);	assert(osG		);}
	f=of+".uArr.Pos.csv";	ofstream osPos;		if (IPrgPar_uArr->_cp._st_savPos	)	{ osPos.open	(f.c_str()	);	assert(osPos	);}
	f=of+".Plasm_Tm.csv";	ofstream osPl_Tm;	if (IPrgPar_uArr->_cp._st_savTm_Plasm)	{ osPl_Tm.open	(f.c_str()	);	assert(osPl_Tm	);}
	f=of+".Plasm_G.csv";	ofstream osPl_G;	if (IPrgPar_uArr->_cp._st_savG_Plasm)	{ osPl_G.open	(f.c_str()	);	assert(osPl_G	);}
	f=of+".uArr.Al.csv";	ofstream osAl;		if (IPrgPar_uArr->_cp._st_savAlign	)	{ osAl.open		(f.c_str()	);	assert(osAl		);}

	auto_ptr<CSaltCorrNN> apNNpar (Create_NNpar(IPrgPar_uArr->_cp)); 	CSaltCorrNN *NNpar = apNNpar.get() ;
	
	CMultSec		pr		(			IPrgPar_uArr->_InputSondeFile.Get() ,		NNpar),
					tg		(			IPrgPar_uArr->_cp._InputTargetFile.Get(),	NNpar,
										IPrgPar_uArr->_cp._MaxTgId	,	
										IPrgPar_uArr->_cp._SecLim		); /*,IPrgPar_uArr->_cp._SecBeg,	IPrgPar_uArr->_cp._SecEnd */

	time_t t_sec = time(NULL);

	auto_ptr<ThDyAlign> apAl; 	
	apAl= Create_ThDyAlign(				IPrgPar_uArr->_cp, pr._TMaxLen , tg._TMaxLen, *NNpar);	ThDyAlign	&Al=*apAl.get();

	string TableName = "Target / Probe (" + string(Al.AlignMeth()) +  " ). Virtual microArray." ; 
	if (osTm)	osTm	<<TableName 	;		// No hace falta el if ?????   Se ignora I/O cuando no esta open??
				osG		<<TableName	;
				osPos	<<TableName  ;	
				osPl_Tm <<"Row_ID"	;	
				osPl_G  <<"Row_ID"	;		

	delete IPrgPar_uArr->_rtbl ;
	CTable<TmGPos> &rtbl = *(  IPrgPar_uArr->_rtbl = new	CTable<TmGPos> ( TableName )   );

	//delete IPrgPar_uArr->_tlTm ;	delete IPrgPar_uArr->_tlG ;		delete IPrgPar_uArr->_tlPos ;
	//CTable<Temperature> &tlTm = *(  IPrgPar_uArr->_tlTm = new	CTable<Temperature> ("Tm: " + TableName )   );
	//CTable<Energy>		&tlG  = *(  IPrgPar_uArr->_tlG  = new	CTable<Energy>		(" G: " + TableName )   );	
	//CTable<SecPos>		&tlPos= *(  IPrgPar_uArr->_tlPos= new	CTable<SecPos>		("Pos: " + TableName)   );	

	// Primero creamos non deg set y el primer renglon de las tablas con el nombre de las sondas

	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
	{	CSec &s = *pr.CurSec() ;
		if (s.Degeneracy() > MaxGrDeg   ) continue;								// sonda "demaciado" deg. No se analisa
		/*pr.AddMultiSec*/ (  s.CreateNonDegSet () ) ;
		if ( CMultSec *nds=s.NonDegSet() ) 
		{	for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda
			{	CSec &s = *nds->CurSec() ;
				osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
				rtbl.AddColummnTit(s.Name()	);																						//s.x;
				//tlTm.AddColummnTit(s._name	);tlG.AddColummnTit(s._name	);tlPos.AddColummnTit(s._name	);
			}// recorre todos las var no deg
		} else 
			{	osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
				rtbl.AddColummnTit(s.Name()	);
				//tlTm.AddColummnTit(s._name	);tlG.AddColummnTit(s._name	);tlPos.AddColummnTit(s._name	);
			}
	}
	rtbl.CreateMatrix(tg._TNSec);	
	//tlTm.CreateMatrix(tg._TNSec);	tlG.CreateMatrix(tg._TNSec);	tlPos.CreateMatrix(tg._TNSec);

	time_t t_al_created = time(NULL);

	for (  tg.goFirstSec()   ; tg.NotEndSec()   ;   tg.goNextSec() )  // recorre todos los targets
	{	CSec &t = *tg.CurSec() ;
		if ( t.Degeneracy() > 1 ) continue ;				// No analiza las target deg...por ahora.Facil de ampliar
		osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
		osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;		
		rtbl.AddRow(t.Name());	
		//tlTm.AddRow(t.Name());		tlG.AddRow(t.Name());		tlPos.AddRow(t.Name());
		HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, &rtbl);
		//HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, &tlTm, &tlG, &tlPos);
		
	}// recorre todos los targets
	rtbl.compact();	
	//tlTm.compact();	tlG.compact();	tlPos.compact();

	time_t t_tm_cal = time(NULL);
	osTm<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
				<< endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
				<< endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
	return 1;
}



	////FracTDAlign			FrAl		( pr._MaxLen ,  tg._TMaxLen, *NNpar);
	////FrAl.SetTmLimits (					IPrgPar_uArr->_Tm_sig,			// OK
	////									IPrgPar_uArr->_Tm_min,			// Pero esto ??
	////									IPrgPar_uArr->_Tm_max    );		// Pero esto ??
	////FrAl.SetTa		 (			CtoK(	IPrgPar_uArr->_Ta	    ));		// OK
	////FrAl.SetmaxG_der(0.1f);
	//ThDyAlign_Tm			TmAl	( pr._MaxLen ,  tg._TMaxLen, *NNpar);
	////TmAl.SetTmLimits (					IPrgPar_uArr->_Tm_sig,			// OK
	////									IPrgPar_uArr->_Tm_min,			// Pero esto ??
	////									IPrgPar_uArr->_Tm_max    );		// Pero esto ??
	//TmAl.SetTa		 (			CtoK(	IPrgPar_uArr->_Ta	    ));		// OK
	//ThDyAlign	*pAl;
	//						if (	IPrgPar_uArr->_TAMeth == TAMeth_Fract)
	//						{			FracTDAlign *pfAl=new FracTDAlign ( pr._MaxLen ,  tg._TMaxLen, *NNpar);		pfAl->SetmaxG_der(0.001f);
	//									pAl=pfAl ;
	//						} else 		
	//						if (	IPrgPar_uArr->_TAMeth == TAMeth_G)
	//						{			ThDyAlign_G *pfAl=new ThDyAlign_G ( pr._TMaxLen ,  tg._TMaxLen, *NNpar);		//pfAl->SetmaxG_der(0.1f);
	//									pAl=pfAl ;
	//						} else 		pAl=              new ThDyAlign_Tm( pr._MaxLen ,  tg._TMaxLen, *NNpar);


	//ThDyAlign	&Al=*pAl;
	//Al.SetTa		 (			CtoK(	IPrgPar_uArr->_Ta	    ));		// OK
	//time_t t_al_created = time(NULL);
//	CSaltCorrNN		_NNpar	(			IPrgPar_uArr->_cp._ConcSd, 
//										IPrgPar_uArr->_cp._ConcTg, 
//										IPrgPar_uArr->_cp._ConcSalt   ); 		CSaltCorrNN *NNpar = &_NNpar;
//	_NNpar.SetTa(				CtoK(	IPrgPar_uArr->_cp._Ta));			// Aqui por si acaso. Revisar.
//	if (IPrgPar_uArr->_cp._loadNNPar){ifstream isTDP(IPrgPar_uArr->_cp._InputNNFile.Get());	assert(isTDP);	NNpar->LoadNNParam(isTDP) ;	}
//	if (IPrgPar_uArr->_cp._saveNNPar)
//	{	string OutputTDP(IPrgPar_uArr->_cp._OutputFile.Get()) ; 		OutputTDP += ".ThDyParam.csv";
//		ofstream osTDP	(OutputTDP.c_str());				assert(osTDP);	
//		osTDP << _NNpar ;
//	}
////char *OutputTDP	= AddFileExt (	IPrgPar_uArr->_OutputFile, ".ThDyParam.csv");	
//
//