//#include "StdAfx.h"
#pragma unmanaged
#include "ThDy_programs/prog_comm_functions.h"
//int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, CMultSec &pr, CMultSec &tg, time_t t_0,  int MAxGrDegTg=1, const std::string of_x=""	);


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
				rtbl.AddColummnTit(/*name*/  s.Name()	);																						//s.x;
			}
		} else 
			{	
				const std::string name (path + s.Name() );
				os.Tm	 <<sep << name		;	os.G		 <<sep << name		;	os.Pos	<<sep <<name		;	
				os.Pl_Tm <<"  "<< name		;	os.Pl_G		 <<"  "<< name	;
				rtbl.AddColummnTit(/*name*/  s.Name()	);
			}
	}
	for (  pr.goFirstMSec(); pr.NotEndMSec()   ;   pr.goNextMSec())  
        CreateColumns(rtbl, *pr.CurMSec(),MaxGrDeg,os );
}

void	Hybrid(CTable<TmGPos> &rtbl, CMultSec &tg, CMultSec &pr, ThDyAlign	&Al, OutStr &os, int MAxGrDegTg=1)
{
	const std::string path =CMultSec::Path(&tg) 	 ;
	for (  tg.goFirstSec()   ; tg.NotEndSec()   ;   tg.goNextSec() )  /// recorre todos los targets
	{	
        CSec &t = *tg.CurSec() ;
		if(! t.Selected())
			continue;		
		if(	t.Degeneracy() > MAxGrDegTg ) 							/// No analiza las target deg...por ahora.Facil de ampliar
		{
			t.Selected(false) ;
			continue;		
		}
		const std::string name = path + t.Name()	 ;
		os.Tm	 <<endl<< name			;		os.G		<<endl<< name			;	os.Pos	<<endl<< name		;	
		os.Pl_Tm <<endl<< name<<" \t"	;		os.Pl_G		<<endl<< name<<" \t"	 ;		
		rtbl.AddRow(t.Name());	
		HybridPr (pr, t, 	Al, os.Tm, os.G,os.Pos,os.Pl_Tm,os.Pl_G,os.Al, &rtbl);
        tg.RestoreCur(&t);
	}
	for (  tg.goFirstMSec(); tg.NotEndMSec()   ;   tg.goNextMSec())  
    {	
        CMultSec *ct=tg.CurMSec();
        Hybrid(rtbl, *ct,  pr, Al,os, MAxGrDegTg);
        ct->RestoreMCur(ct);
    }

}

//int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, 
//                    CMultSec &pr, 
//                    CMultSec &tg, 
//                    time_t t_0,  
//                    int MAxGrDegTg, 
//                    const std::string& of_x =""	);


int microArrayProg ( CProgParam_microArray *IPrgPar_uArr, 
                    CMultSec &pr, 
                    CMultSec &tg, 
                    time_t t_0,  
                    int MAxGrDegTg, 
                    const std::string& of_x   =""  	)
{
    const int MaxGrDeg = 300;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??

    string of{ IPrgPar_uArr->_cp._OutputFile.get() + of_x }, f;

	f=of+".uArr.Tm.csv" ;	ofstream osTm;		if (IPrgPar_uArr->_cp.st_savTm .get()	)	{ osTm.open		(f.c_str()	);	if (!osTm) throw std::runtime_error(string("Error trying to open ")+f);}
	f=of+".uArr.G.csv"  ;	ofstream osG ;		if (IPrgPar_uArr->_cp.st_savG  .get()	)	{ osG.open		(f.c_str()	);	assert(osG		);}
	f=of+".uArr.Pos.csv";	ofstream osPos;		if (IPrgPar_uArr->_cp.st_savPos.get()	)	{ osPos.open	(f.c_str()	);	assert(osPos	);}
	f=of+".Plasm_Tm.csv";	ofstream osPl_Tm;	if (IPrgPar_uArr->_cp.st_savTm_Plasm.get()) { osPl_Tm.open	(f.c_str()	);	assert(osPl_Tm	);}
	f=of+".Plasm_G.csv";	ofstream osPl_G;	if (IPrgPar_uArr->_cp.st_savG_Plasm.get())	{ osPl_G.open	(f.c_str()	);	assert(osPl_G	);}
	f=of+".uArr.Al.csv";	ofstream osAl;		if (IPrgPar_uArr->_cp.st_savAlign.get())	{ osAl.open		(f.c_str()	);	assert(osAl		);}

    std::shared_ptr<CSaltCorrNN>  NNpar {  pr._NNPar };

	time_t t_sec = time(nullptr);

	std::unique_ptr<ThDyAlign> apAl; 	
	apAl= Create_ThDyAlign(		IPrgPar_uArr->_cp, pr._Global._Len.Max() , tg._Global._Len.Max(), NNpar);	ThDyAlign	&Al=*apAl ;

	string TableName = "Target / Probe (align method: " + string(Al.AlignMeth()) +  " )" ; 
	if (osTm)	osTm	<<TableName 	;		// No hace falta el if ?????   Se ignora I/O cuando no esta open??
				osG		<<TableName	;
				osPos	<<TableName  ;	
				osPl_Tm <<"Row_ID"	;	
				osPl_G  <<"Row_ID"	;		

	IPrgPar_uArr->_rtbl.reset( new	CTable<TmGPos> ( TableName/*,	tg->CountSelectedSeqRec(),
																	pr->CountSelectedNDegSeqRec(MaxGrDeg)  */)  ); ;
	CTable<TmGPos> &rtbl = *(  IPrgPar_uArr->_rtbl  );

	// Primero creamos non deg set y el primer renglon de las tablas con el nombre de las sondas

	OutStr os(osTm, osG,osPos,osPl_Tm,osPl_G,osAl); 
	CreateColumns( rtbl, pr, MaxGrDeg,os );
	rtbl.CreateMatrix(tg.CountSelectedSeqRec());	


	time_t t_al_created = time(nullptr);

	Hybrid(rtbl, tg,  pr, Al, os, MAxGrDegTg);

	rtbl.compact();	

	time_t t_tm_cal = time(nullptr);
	osTm<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
				<< endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
				<< endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
	return 1;
}
int microArrayProg ( CProgParam_microArray *IPrgPar_uArr)  
{
	time_t t_0 = time(nullptr);

    IPrgPar_uArr->Check_NNp_Targets_probes (IPrgPar_uArr->_probesMS.get());

	//assert(("IPrgPar_uArr->_probesMS - debiera existir siempre",IPrgPar_uArr->_probesMS));
	//CMultSec  &pr(		*IPrgPar_uArr->_probesMS.get() ); 
	//if(IPrgPar_uArr->_InputSondeFile.Get()[0] )
	//	pr.AddFromFile ( IPrgPar_uArr->_InputSondeFile.Get() );	

	auto res=microArrayProg (  IPrgPar_uArr, 
                            *IPrgPar_uArr->_probesMS.get()	, 
                            *IPrgPar_uArr->_cp._pSeqTargets.get() , 
                             t_0 ,
                             300)  ; 
    IPrgPar_uArr->_rtbl->TitTable(IPrgPar_uArr->_rtbl->TitTable()+ ". Virtual microArray.");
	return res ; 
}
