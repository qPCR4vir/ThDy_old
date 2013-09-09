//#include "StdAfx.h"
#pragma unmanaged

#include "ThDySec\th_dy_align.h"
#include "ThDy_programs/prog_comm_functions.h"

void FindSonden( CMultSec *tg, int& tgN, int& compN, CMSecCand& msCand, ofstream &osNCand, CProgParam_SondeDesign *IPrgPar_SdDes )
{
	for (  tg->goFirstSec(); tg->NotEndSec()   ;   tg->goNextSec() )  // recorre todos los targets
	{	CSec &nt = *tg->CurSec() ;
		
		if ( nt.Degeneracy() > 1 )  continue ; 		  // No analiza las target deg...por ahora.Facil de ampliar
		if ( ! nt.Selected() )      continue ; 		  // No analiza las target non selected
        tgN++;
		for(CSecCand &newtg =msCand.AddBeging(nt)	;msCand.NotFinisch() ; msCand.CompNext())  // anade el curr tg y lo comp con todos los anteriormente anadidos
		{	CSecCand &curtg =msCand.curTg();
            compN++;
			osNCand	<<endl<< msCand._TNumPosCand << sep<< msCand._TNumCand  
					<<sep<< tgN				<<sep<<	compN	
					<<sep<<	newtg._Sec.Name()	<<sep<<	newtg._NumPosCand/*In*/  << sep<< newtg._NumCand/*In*/	
					<<sep<<	curtg._Sec.Name()	<<sep<<	curtg._NumPosCand/*In */ << sep<< curtg._NumCand/*In*/;	
					//<<sep<< msCand._TDATmC->_THits<< sep<< msCand._TDATmC->_HitsOK 
			
			msCand.FindCommon	( newtg, curtg, IPrgPar_SdDes->_design )	;

			osNCand	<<sep<< msCand._TDATmC->_THits<< sep<< msCand._TDATmC->_HitsOK
					//<<endl<<	tgN				<<sep<<	compN	
					<<sep<<	newtg._Sec.Name()	<<sep<<	newtg._NumPosCand  << sep<< newtg._NumCand	
					<<sep<<	curtg._Sec.Name()	<<sep<<	curtg._NumPosCand  << sep<< curtg._NumCand	
					<<sep<< msCand._TNumPosCand << sep<< msCand._TNumCand 
					 ; 
		}
	}
	for (  tg->goFirstMSec(); tg->NotEndMSec()   ;   tg->goNextMSec())  // recorre todos los targets
	{	
        FindSonden(tg->CurMSec(), tgN, compN, msCand, osNCand, IPrgPar_SdDes );
	}
}
		//msCand.Add(t1);
		//osNCand	<<endl<<	t1->Name()			<< sep<<	t1->_len	
		//		<< sep<<	msCand._TNumPosCand	<< sep<<	((CSecCand *)msCand._LSecCand.Last())->_NumPosCand
		//		<< sep<<	msCand._TNumCand	<< sep<<	((CSecCand *)msCand._LSecCand.Last())->_NumCand; 


int SondeDesignProg ( CProgParam_SondeDesign *IPrgPar_SdDes)
{	
	time_t t_0 = time(NULL);
	string OutputCand = IPrgPar_SdDes->_cp._OutputFile.Get() ; OutputCand += ".TgCand.csv";	ofstream osNCand(OutputCand.c_str());
	osNCand.precision(2);
	osNCand	<<endl<<"Num T Pos" <<sep<< "Num T Cand"	
			<<sep<<	"Targ Num"	<<sep<< "Iterat#"  	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<< "Num T Hits"<<sep<< "Num Hits OK"				
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<< "Num T Pos" <<sep<< "Num T Cand"	
			<< fixed			; 

	std::shared_ptr<CSaltCorrNN>  NNpar(IPrgPar_SdDes->_cp._pSaltCorrNNp );
    if (!NNpar)
	    NNpar = Create_NNpar(IPrgPar_SdDes->_cp); 	
	NNpar->SetTa(				CtoK(	IPrgPar_SdDes->_cp._Ta));			// Aqui por si acaso. Revisar.

	std::shared_ptr<CMultSec>  tg(IPrgPar_SdDes->_cp._pSeqTargets );
    if (!tg || ! tg->_Global._NSec)
		tg.reset ( new CMultSec	(	IPrgPar_SdDes->_cp._InputTargetFile.Get(), NNpar,
								    IPrgPar_SdDes->_cp._MaxTgId,
								    IPrgPar_SdDes->_cp._SecLim  ));		
	//return 1;
	time_t t_sec = time(NULL);

	CMSecCand	msCand	(	
				   convCtoK_ctok( IPrgPar_SdDes->_sL ) ,
							CtoK(IPrgPar_SdDes->_Tm_sig),		 IPrgPar_SdDes->_G_sig      * 1000,
							CtoK(IPrgPar_SdDes->_MaxSd_nTgTm),	 IPrgPar_SdDes->_MinSd_nTgG * 1000,	
							CtoK(IPrgPar_SdDes->_MaxSelfTm),	 IPrgPar_SdDes->_MinSelfG   * 1000	 );
	msCand.Use(tg);
	msCand._TDATmC->SetTa	(CtoK(IPrgPar_SdDes->_cp._Ta));

	time_t t_al_created = time(NULL);

	int tgN(0), compN=0;
    FindSonden(tg.get(), tgN, compN, msCand, osNCand, IPrgPar_SdDes );


	if (IPrgPar_SdDes->_MinTgCov) 
		msCand.ExportCommonSonden(IPrgPar_SdDes->_cp._OutputFile.Get(), IPrgPar_SdDes->_design, IPrgPar_SdDes->_MinTgCov, fasta | csv);



	time_t t_tm_cal = time(NULL);
	osNCand<< endl << endl <<"Time sec= "			<< sep<< t_sec			- t_0		
				   << endl <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
				   << endl <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
	return 1;
}


