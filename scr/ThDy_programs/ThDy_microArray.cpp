//#include "StdAfx.h"
#pragma unmanaged

#include "ThDy_programs/prog_comm_functions.h"
int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, CMultSec &pr, CMultSec &tg, time_t t_0,  int MAxGrDegTg=1, const std::string of_x=""	);

int microArrayProg ( CProgParam_microArray *IPrgPar_uArr)  
{
	time_t t_0 = time(NULL);

	std::shared_ptr<CSaltCorrNN>  NNpar(IPrgPar_uArr->_cp._pSaltCorrNNp );
    if (!NNpar)
	    NNpar = Create_NNpar(IPrgPar_uArr->_cp); 	
	NNpar->SetTa(				CtoK(	IPrgPar_uArr->_cp._Ta));			// Aqui por si acaso. Revisar.

	std::shared_ptr<CMultSec>  pr(		IPrgPar_uArr->_probesMS );
			if (!pr)
				pr.reset ( new CMultSec(IPrgPar_uArr->_InputSondeFile.Get() ,		NNpar));	

	std::shared_ptr<CMultSec>  tg(		IPrgPar_uArr->_cp._pSeqTargets );
			if (!tg)
				tg.reset ( new CMultSec(IPrgPar_uArr->_cp._InputTargetFile.Get(),	NNpar,
										IPrgPar_uArr->_cp._MaxTgId,
										IPrgPar_uArr->_cp._SecLim  ));	

	return microArrayProg ( IPrgPar_uArr, *pr.get()	, *tg.get(), t_0 	)  ; /*, "_self"*/
}

void	CreateColumns(CTable<TmGPos> &rtbl, CMultSec &pr, int MaxGrDeg, OutStr &os )
{
	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
	{	CSec &s = *pr.CurSec() ;
		if(! s.Selected())
			continue;		
		if(	s.Degeneracy() > MaxGrDeg ) 							// sonda "demaciado" deg. No se analisa
		{
			s.Selected(false) ;
			continue;		
		}
		const std::string path =CMultSec::Path(s._parentMS) ;
		s.CreateNonDegSet ()  ;
		if ( CMultSec *nds=s.NonDegSet() ) 
		{	
			const std::string path =CMultSec::Path(s._parentMS) ;
			for (  nds->goFirstSec()   ; nds->NotEndSec()   ;   nds->goNextSec() )// recorre todos las var no deg de la sonda
			{	CSec &s = *nds->CurSec() ;
				const std::string name (path + s.Name() );
				os.Tm	 <<sep << name		;	os.G		<<sep << name		;	os.Pos	<<sep <<name		;	
				os.Pl_Tm <<"  "<< name		;	os.Pl_G		<<"  "<< name		;
				rtbl.AddColummnTit(s.Name()	);																						//s.x;
			}
		} else 
			{	
				const std::string name (path + s.Name() );
				os.Tm	 <<sep << name		;	os.G		 <<sep << name		;	os.Pos	<<sep <<name		;	
				os.Pl_Tm <<"  "<< name		;	os.Pl_G		 <<"  "<< name	;
				rtbl.AddColummnTit(s.Name()	);
			}
	}
	for (  pr.goFirstMSec(); pr.NotEndMSec()   ;   pr.goNextMSec())  
        CreateColumns(rtbl, *pr.CurMSec(),MaxGrDeg,os );
}

void	Hybrid(CTable<TmGPos> &rtbl, CMultSec &tg, CMultSec &pr, ThDyAlign	&Al, OutStr &os, int MAxGrDegTg=1)
{
	const std::string path =CMultSec::Path(&tg) 	 ;
	for (  tg.goFirstSec()   ; tg.NotEndSec()   ;   tg.goNextSec() )  // recorre todos los targets
	{	CSec &t = *tg.CurSec() ;
		if(! t.Selected())
			continue;		
		if(	t.Degeneracy() > MAxGrDegTg ) 									// No analiza las target deg...por ahora.Facil de ampliar
		{
			t.Selected(false) ;
			continue;		
		}
		const std::string name = path + t.Name()	 ;
		os.Tm	 <<endl<< name			;		os.G		<<endl<< name			;	os.Pos	<<endl<< name		;	
		os.Pl_Tm <<endl<< name<<" \t"	;		os.Pl_G		<<endl<< name<<" \t"	 ;		
		rtbl.AddRow(t.Name());	
		HybridPr (pr, t, 	Al, os.Tm, os.G,os.Pos,os.Pl_Tm,os.Pl_G,os.Al, &rtbl);
	}
	for (  tg.goFirstMSec(); tg.NotEndMSec()   ;   tg.goNextMSec())  
		Hybrid(rtbl, *tg.CurMSec(),  pr, Al,os, MAxGrDegTg);
}

int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, CMultSec &pr, CMultSec &tg, time_t t_0,  int MAxGrDegTg, const std::string of_x 	)
{
	const int MaxGrDeg=300 ;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??

		string of=string( IPrgPar_uArr->_cp._OutputFile.Get() ) + of_x    , f;

	f=of+".uArr.Tm.csv" ;	ofstream osTm;		if (IPrgPar_uArr->_cp._st_savTm		)	{ osTm.open		(f.c_str()	);	assert(osTm		);}
	f=of+".uArr.G.csv"  ;	ofstream osG ;		if (IPrgPar_uArr->_cp._st_savG		)	{ osG.open		(f.c_str()	);	assert(osG		);}
	f=of+".uArr.Pos.csv";	ofstream osPos;		if (IPrgPar_uArr->_cp._st_savPos	)	{ osPos.open	(f.c_str()	);	assert(osPos	);}
	f=of+".Plasm_Tm.csv";	ofstream osPl_Tm;	if (IPrgPar_uArr->_cp._st_savTm_Plasm)	{ osPl_Tm.open	(f.c_str()	);	assert(osPl_Tm	);}
	f=of+".Plasm_G.csv";	ofstream osPl_G;	if (IPrgPar_uArr->_cp._st_savG_Plasm)	{ osPl_G.open	(f.c_str()	);	assert(osPl_G	);}
	f=of+".uArr.Al.csv";	ofstream osAl;		if (IPrgPar_uArr->_cp._st_savAlign	)	{ osAl.open		(f.c_str()	);	assert(osAl		);}

	std::shared_ptr<CSaltCorrNN>  NNpar =  pr._NNPar;

	time_t t_sec = time(NULL);

	auto_ptr<ThDyAlign> apAl; 	
	apAl= Create_ThDyAlign(		IPrgPar_uArr->_cp, pr._Global._Len.Max() , tg._Global._Len.Max(), NNpar);	ThDyAlign	&Al=*apAl.get();

	string TableName = "Target / Probe (" + string(Al.AlignMeth()) +  " ). Virtual microArray." ; 
	if (osTm)	osTm	<<TableName 	;		// No hace falta el if ?????   Se ignora I/O cuando no esta open??
				osG		<<TableName	;
				osPos	<<TableName  ;	
				osPl_Tm <<"Row_ID"	;	
				osPl_G  <<"Row_ID"	;		

	delete IPrgPar_uArr->_rtbl ;

	CTable<TmGPos> &rtbl = *(  IPrgPar_uArr->_rtbl = new	CTable<TmGPos> ( TableName/*,	tg->CountSelectedSeqRec(),
																						pr->CountSelectedNDegSeqRec(MaxGrDeg)  */)  );

	// Primero creamos non deg set y el primer renglon de las tablas con el nombre de las sondas

	OutStr os(osTm, osG,osPos,osPl_Tm,osPl_G,osAl); 
	CreateColumns( rtbl, pr, MaxGrDeg,os );
	rtbl.CreateMatrix(tg.CountSelectedSeqRec());	


	time_t t_al_created = time(NULL);

	Hybrid(rtbl, tg,  pr, Al, os, MAxGrDegTg);

	rtbl.compact();	

	time_t t_tm_cal = time(NULL);
	osTm<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
				<< endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
				<< endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
	return 1;
}
		//CTable(string TitTable,index capRow, index capCol): CMatrix_RA<Num>(capRow, capCol),		_titTable(TitTable), 

	
	//CMultSec		pr		(			IPrgPar_uArr->_InputSondeFile.Get() ,		NNpar),
	//				tg		(			IPrgPar_uArr->_cp._InputTargetFile.Get(),	NNpar,
	//									IPrgPar_uArr->_cp._MaxTgId	,	
	//									IPrgPar_uArr->_cp._SecLim		); /*,IPrgPar_uArr->_cp._SecBeg,	IPrgPar_uArr->_cp._SecEnd */
	//delete IPrgPar_uArr->_tlTm ;	delete IPrgPar_uArr->_tlG ;		delete IPrgPar_uArr->_tlPos ;
	//CTable<Temperature> &tlTm = *(  IPrgPar_uArr->_tlTm = new	CTable<Temperature> ("Tm: " + TableName )   );
	//CTable<Energy>		&tlG  = *(  IPrgPar_uArr->_tlG  = new	CTable<Energy>		(" G: " + TableName )   );	
	//CTable<SecPos>		&tlPos= *(  IPrgPar_uArr->_tlPos= new	CTable<SecPos>		("Pos: " + TableName)   );	
	//tlTm.CreateMatrix(tg._TNSec);	tlG.CreateMatrix(tg._TNSec);	tlPos.CreateMatrix(tg._TNSec);
				//tlTm.AddColummnTit(s._name	);tlG.AddColummnTit(s._name	);tlPos.AddColummnTit(s._name	);
				//tlTm.AddColummnTit(s._name	);tlG.AddColummnTit(s._name	);tlPos.AddColummnTit(s._name	);
		//tlTm.AddRow(t.Name());		tlG.AddRow(t.Name());		tlPos.AddRow(t.Name());
		//HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, &tlTm, &tlG, &tlPos);
		
	//for (  tg.goFirstSec()   ; tg.NotEndSec()   ;   tg.goNextSec() )  // recorre todos los targets
	//{	CSec &t = *tg.CurSec() ;
	//	if ( t.Degeneracy() > 1 ) continue ;				// No analiza las target deg...por ahora.Facil de ampliar
	//	osTm	<<endl<< t.Name()		;		osG		<<endl<< t.Name()		;	osPos	<<endl<< t.Name()		;	
	//	osPl_Tm <<endl<< t.Name()<<" \t"	;		osPl_G  <<endl<< t.Name()<<" \t"	 ;		
	//	rtbl.AddRow(t.Name());	
	//	//tlTm.AddRow(t.Name());		tlG.AddRow(t.Name());		tlPos.AddRow(t.Name());
	//	HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, &rtbl);
	//	//HybridPr (pr, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, &tlTm, &tlG, &tlPos);
	//	
	//}// recorre todos los targets
	//tlTm.compact();	tlG.compact();	tlPos.compact();
