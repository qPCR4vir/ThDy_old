//#include "StdAfx.h"
#pragma unmanaged
//#include "ThDySec\th_dy_align.h"
#include "ThDy_programs/prog_comm_functions.h"

int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, 
                    CMultSec &pr, 
                    CMultSec &tg, 
                    time_t t_0,  
                    int MAxGrDegTg, 
                    const std::string& of_x =""	);

void CreateComplProbes(	CMultSec		&pr	)
{
	for (  pr.goFirstMSec(); pr.NotEndMSec()   ;   pr.goNextMSec())  
        CreateComplProbes( *pr.CurMSec() );
	CMultSec *cms=pr.AddMultiSec("compl");
	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
	{	CSec &s = *pr.CurSec() ;
		if( s.Selected())
		    cms->AddSec ( s.CreateCopy(rev_compl) ); 
	}
}


		//int t=MultiplexPCRProg ( IPrgPar_Calc, primers		)  ;

	int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr, CMultSec		&pr)  
{
	time_t t_0 = time(NULL);
    IPrgPar_uArr->_cp.Check_NNp_Targets ();
	CreateComplProbes(	pr	);

	microArrayProg   ( IPrgPar_uArr, pr	, pr, t_0, 300    , "_self"	)  ;
	delete IPrgPar_uArr->_rtbl_self ;
	       IPrgPar_uArr->_rtbl_self=	IPrgPar_uArr->_rtbl;

	//std::shared_ptr<CMultSec>  tg(		IPrgPar_uArr->_cp._pSeqTargets );
	//		if (!tg)
	//			tg.reset ( new CMultSec(IPrgPar_uArr->_cp._InputTargetFile.Get(),	pr._NNPar,
	//									IPrgPar_uArr->_cp._MaxTgId,
	//									IPrgPar_uArr->_cp._SecLim  ));	
	return microArrayProg ( IPrgPar_uArr, 
                            pr	, 
                            *IPrgPar_uArr->_cp._pSeqTargets.get(), 
                            t_0  , 300 
                            /*, "_self"*/	
                            )  ;
}


	int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr)  
{

    IPrgPar_uArr->Check_NNp_Targets_probes (IPrgPar_uArr->_probesMS.get());

	return MultiplexPCRProg ( IPrgPar_uArr, *IPrgPar_uArr->_probesMS.get())  ;

	
}


//int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr, 	CMultSec		&pr		)  
//{
//	std::shared_ptr<CSaltCorrNN>  NNpar =  pr._NNPar;
//	time_t t_0 = time(NULL);
//	const int MaxGrDeg=300 ;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??
//
//		string of( IPrgPar_uArr->_cp._OutputFile.Get() ), f;
//
//	f=of+".uArr.Tm.csv" ;	ofstream osTm;		if (IPrgPar_uArr->_cp._st_savTm		)	osTm.open	(f.c_str()	);	
//	f=of+".uArr.G.csv"  ;	ofstream osG ;		if (IPrgPar_uArr->_cp._st_savG		)	osG.open	(f.c_str()	);	
//	f=of+".uArr.Pos.csv";	ofstream osPos;		if (IPrgPar_uArr->_cp._st_savPos	)	osPos.open	(f.c_str()	);	
//	f=of+".Plasm_Tm.csv";	ofstream osPl_Tm;	if (IPrgPar_uArr->_cp._st_savTm_Plasm)	osPl_Tm.open(f.c_str()	);	
//	f=of+".Plasm_G.csv";	ofstream osPl_G;	if (IPrgPar_uArr->_cp._st_savG_Plasm)	osPl_G.open	(f.c_str()	);	
//	f=of+".uArr.Al.csv";	ofstream osAl;		if (IPrgPar_uArr->_cp._st_savAlign	)	osAl.open	(f.c_str()	);	
//
//	std::shared_ptr<CMultSec>  tg(		IPrgPar_uArr->_cp._pSeqTargets );
//			if (!tg)
//				tg.reset ( new CMultSec(IPrgPar_uArr->_cp._InputTargetFile.Get(),	NNpar,
//										IPrgPar_uArr->_cp._MaxTgId,
//										IPrgPar_uArr->_cp._SecLim  ));		
//
//	time_t t_sec = time(NULL);
//
//	LonSecPos TgMaxLen= (tg->_Global._Len.Max() > pr._Global._Len.Max()) ? tg->_Global._Len.Max() : pr._Global._Len.Max() ; // Los primers tambien los voy a usar como target !!
//	auto_ptr<ThDyAlign> apAl; 	
//	apAl= Create_ThDyAlign(				IPrgPar_uArr->_cp, pr._Global._Len.Max() , TgMaxLen, NNpar);	ThDyAlign	&Al=*apAl.get();
//
//	osTm	<< Al.AlignMeth()<<" -- T/S" 	;	osG		<< Al.AlignMeth()<<" -- T/S" 	;	osPos <<Al.AlignMeth()<<" -- T/S"  ;	
//	osPl_Tm <<"Row_ID"						;	osPl_G  <<"Row_ID"	;	
//
//	// Primero creamos non deg set y el primer renglon de las tablas con el nombre de las sondas
//	// Tambien crear los sec complementarias a cada sonda/primer
//	// Ponerle nombre con num consec a las variantes no deg. y (c) a las sec rev-complement.
//
//	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
//	{	CSec &s = *pr.CurSec() ;
//		if (s.Degeneracy() > MaxGrDeg   ) continue;								// sonda "demaciado" deg. No se analisa
//		/*pr.AddMultiSec*/ (  s.CreateNonDegSet () ) ;						// creo non deg set. No lo inserta en ninguna parte ??!!
//		if ( CMultSec *nds=s.NonDegSet() ) 
//		{	for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda
//			{	CSec &s = *nds->CurSec() ;
//				CSec &cs= *s.CreateCopy(rev_compl);			// Hacer algo con esta copia !!!!!
//				nds->InsertSec ( &cs ); // inserta cs en el nds delante de cur, o sea delante de s.
//
//				osTm	<<sep <<cs.Name()	;	osG		<<sep <<cs.Name()	;	osPos	<<sep <<cs.Name()	;	
//				osPl_Tm <<"  "<<cs.Name()	;	osPl_G  <<"  "<<cs.Name()	;
//
//				osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
//				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
//			}// recorre todos las var no deg
//		} else 
//			{	CSec &cs= *s.CreateCopy(rev_compl);			// Hacer algo con esta copia !!!!!
//				pr.InsertSec ( &cs );
//
//				osTm	<<sep <<cs.Name()	;	osG		<<sep <<cs.Name()	;	osPos	<<sep <<cs.Name()	;	
//				osPl_Tm <<"  "<<cs.Name()	;	osPl_G  <<"  "<<cs.Name()	;
//
//				osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
//				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
//			}
//	}	time_t t_al_created = time(NULL);
//
//	// Ahora comenzamos los analisis con la hibridacion entre primmer y sondas (no los targets)
//
//	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
//	{	CSec &t = *pr.CurSec() ;										// y primero las uso como target
//		if ( t.Degeneracy() > MaxGrDeg   ) continue;								// sonda-target "demaciado" deg. No se analisa
//		if ( CMultSec *nds=t.NonDegSet() ) 
//		{	for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda-target
//			{	CSec &t = *nds->CurSec() ;
//		
//				osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//				osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;	
//
//				HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//				nds->RestoreCur(&t);
//			}
//		} else    // sonda-target no deg
//			{
//				osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//				osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;	
//
//				HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//			}
//		pr.RestoreCur(&t);
//	}// recorre todos las sondas-targets	
//
//
//	// Ahora con las target de verdad
//
//	for (  tg->goFirstSec()   ; tg->NotEndSec()   ;   tg->goNextSec() )  // recorre todos los targets
//	{	CSec &t = *tg->CurSec() ;
//		if ( t.Degeneracy() > 1 ) continue ;				// No analiza las target deg...por ahora.Facil de ampliar
//		osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//		osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;		
//		
//		HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//
//	}// recorre todos los targets	
//
//	time_t t_tm_cal = time(NULL);
//	osTm<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
//				<< endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
//				<< endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
//
////		workbook wb; wb.
//	return 1;
//}
//


	//Al.SetTmLimits (					IPrgPar_uArr->_Tm_sig,			// OK
	//									IPrgPar_uArr->_Tm_min,			// Pero esto ??
	//									IPrgPar_uArr->_Tm_max    );		// Pero esto ??
	//bool  TAMeth_is_Tm	=			 (	IPrgPar_uArr->_TAMeth == TAMeth_Tm) ;
	//bool  TAMeth_is_Fr	=			 (	IPrgPar_uArr->_TAMeth == TAMeth_Fract) ;	
							//if (	IPrgPar_uArr->_TAMeth == TAMeth_Fract)
							//{			FracTDAlign *pfAl=new FracTDAlign ( pr._TMaxLen ,  TgMaxLen, *NNpar);		pfAl->SetmaxG_der(0.001f);
							//			pAl=pfAl ;
							//} else 		
							//if (	IPrgPar_uArr->_TAMeth == TAMeth_G)
							//{			ThDyAlign_G *pfAl=new ThDyAlign_G ( pr._TMaxLen ,  TgMaxLen, *NNpar);		//pfAl->SetmaxG_der(0.1f);
							//			pAl=pfAl ;
							//} else 		pAl=              new ThDyAlign_Tm( pr._TMaxLen ,  TgMaxLen, *NNpar);
	//CSaltCorrNN		_NNpar	(			IPrgPar_uArr->_cp._ConcSd, 
	//									IPrgPar_uArr->_cp._ConcTg, 
	//									IPrgPar_uArr->_cp._ConcSalt   ); 		CSaltCorrNN *NNpar = &_NNpar;
	//_NNpar.SetTa(				CtoK(	IPrgPar_uArr->_cp._Ta));			// Aqui por si acaso. Revisar.
	//if (IPrgPar_uArr->_cp._loadNNPar){ifstream isTDP(IPrgPar_uArr->_cp._InputNNFile.Get());	assert(isTDP);	NNpar->LoadNNParam(isTDP) ;	}
	//if (IPrgPar_uArr->_cp._saveNNPar)
	//{	string OutputTDP(IPrgPar_uArr->_cp._OutputFile.Get()) ; 		OutputTDP += ".ThDyParam.csv";
	//	ofstream osTDP	(OutputTDP.c_str());				assert(osTDP);	
	//	osTDP << _NNpar ;
	//}
	//string OutputTm		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputTm		+=".uArr.Tm.csv" ;	ofstream osTm	(OutputTm.c_str()	);	assert(osTm	);
	//string OutputG		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputG		+=".uArr.G.csv"  ;	ofstream osG	(OutputG.c_str()	);	assert(osG	);
	//string OutputPos	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPos	+=".uArr.Pos.csv";	ofstream osPos	(OutputPos.c_str()	);	assert(osPos);
	//string OutputPl_Tm	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPl_Tm	+=".Plasm_Tm.csv";	ofstream osPl_Tm(OutputPl_Tm.c_str());	assert(osPl_Tm);
	//string OutputPl_G	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPl_G	+=".Plasm_G.csv" ;	ofstream osPl_G	(OutputPl_G.c_str()	);	assert(osPl_G);
	//string OutputAl		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputAl		+=".uArr.Al.csv" ;	ofstream osAl	(OutputAl.c_str()	);	assert(osAl	);	
//
//	LonSecPos TgMaxLen= (tg->_Global._Len.Max() > pr._Global._Len.Max()) ? tg->_Global._Len.Max() : pr._Global._Len.Max() ; // Los primers tambien los voy a usar como target !!
//	auto_ptr<ThDyAlign> apAl; 	
//	apAl= Create_ThDyAlign(				IPrgPar_uArr->_cp, pr._Global._Len.Max() , TgMaxLen, NNpar);	ThDyAlign	&Al=*apAl.get();
//
//	osTm	<< Al.AlignMeth()<<" -- T/S" 	;	osG		<< Al.AlignMeth()<<" -- T/S" 	;	osPos <<Al.AlignMeth()<<" -- T/S"  ;	
//	osPl_Tm <<"Row_ID"						;	osPl_G  <<"Row_ID"	;	
//
//	// Primero creamos non deg set y el primer renglon de las tablas con el nombre de las sondas
//	// Tambien crear los sec complementarias a cada sonda/primer
//	// Ponerle nombre con num consec a las variantes no deg. y (c) a las sec rev-complement.
//
//	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
//	{	CSec &s = *pr.CurSec() ;
//		if (s.Degeneracy() > MaxGrDeg   ) continue;								// sonda "demaciado" deg. No se analisa
//		/*pr.AddMultiSec*/ (  s.CreateNonDegSet () ) ;						// creo non deg set. No lo inserta en ninguna parte ??!!
//		if ( CMultSec *nds=s.NonDegSet() ) 
//		{	for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda
//			{	CSec &s = *nds->CurSec() ;
//				CSec &cs= *s.CreateCopy(rev_compl);			// Hacer algo con esta copia !!!!!
//				nds->InsertSec ( &cs ); // inserta cs en el nds delante de cur, o sea delante de s.
//
//				osTm	<<sep <<cs.Name()	;	osG		<<sep <<cs.Name()	;	osPos	<<sep <<cs.Name()	;	
//				osPl_Tm <<"  "<<cs.Name()	;	osPl_G  <<"  "<<cs.Name()	;
//
//				osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
//				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
//			}// recorre todos las var no deg
//		} else 
//			{	CSec &cs= *s.CreateCopy(rev_compl);			// Hacer algo con esta copia !!!!!
//				pr.InsertSec ( &cs );
//
//				osTm	<<sep <<cs.Name()	;	osG		<<sep <<cs.Name()	;	osPos	<<sep <<cs.Name()	;	
//				osPl_Tm <<"  "<<cs.Name()	;	osPl_G  <<"  "<<cs.Name()	;
//
//				osTm	<<sep <<s.Name()		;	osG		<<sep <<s.Name()		;	osPos	<<sep <<s.Name()		;	
//				osPl_Tm <<"  "<< s.Name()	;	osPl_G  <<"  "<< s.Name()	;
//			}
//	}	time_t t_al_created = time(NULL);
//
//	// Ahora comenzamos los analisis con la hibridacion entre primmer y sondas (no los targets)
//
//	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
//	{	CSec &t = *pr.CurSec() ;										// y primero las uso como target
//		if ( t.Degeneracy() > MaxGrDeg   ) continue;								// sonda-target "demaciado" deg. No se analisa
//		if ( CMultSec *nds=t.NonDegSet() ) 
//		{	for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda-target
//			{	CSec &t = *nds->CurSec() ;
//		
//				osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//				osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;	
//
//				HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//				nds->RestoreCur(&t);
//			}
//		} else    // sonda-target no deg
//			{
//				osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//				osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;	
//
//				HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//			}
//		pr.RestoreCur(&t);
//	}// recorre todos las sondas-targets	
//
//
//	// Ahora con las target de verdad
//
//	for (  tg->goFirstSec()   ; tg->NotEndSec()   ;   tg->goNextSec() )  // recorre todos los targets
//	{	CSec &t = *tg->CurSec() ;
//		if ( t.Degeneracy() > 1 ) continue ;				// No analiza las target deg...por ahora.Facil de ampliar
//		osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
//		osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;		
//		
//		HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl);
//
//	}// recorre todos los targets	
//
//	time_t t_tm_cal = time(NULL);
//	osTm<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
//				<< endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
//				<< endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
//
////		workbook wb; wb.
//	return 1;
//}
