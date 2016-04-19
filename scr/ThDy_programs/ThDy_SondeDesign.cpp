/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*  https://www.fli.de/en/institutes/institut-fuer-neue-und-neuartige-tierseuchenerreger/wissenschaftlerinnen/prof-dr-m-h-groschup/
*  distributed under the GNU General Public License, see <http://www.gnu.org/licenses/>.
*
* @author Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2016
*
* @file ThDySec\scr\ThDy_programs\ThDy_SondeDesign.cpp
*
* @brief
*
*/

//#include "StdAfx.h"
#pragma unmanaged

#include "ThDySec\th_dy_align.h"
#include "ThDy_programs/prog_comm_functions.h"

void FindSonden( CMultSec *tg, /*int& tgN,*/ int& compN, CMSecCand& msCand, std::ofstream &osNCand, CProgParam_SondeDesign *IPrgPar_SdDes )
{
	for (  tg->goFirstSec(); tg->NotEndSec()   ;   tg->goNextSec() )  // recorre todos los targets
	{	CSec &nt = *tg->CurSec() ;
		
		if ( nt.Degeneracy() > 1 )  continue ; 		  // No analiza las target deg...por ahora.Facil de ampliar
		if ( ! nt.Selected() )      continue ; 		  // No analiza las target non selected
        //tgN++;
		for(CSecCand &newtg =msCand.AddBeging(nt)	;msCand.NotFinisch() ; msCand.CompNext())  // anade el curr tg y lo comp con todos los anteriormente anadidos
		{	CSecCand &curtg =msCand.curTg();
            compN++;
			osNCand	<<"\n"<< msCand._TNumPosCand << sep<< msCand._TNumCand  
					<<sep<< msCand._NSecCand	<<sep<<	compN	
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
        FindSonden(tg->CurMSec(), /*tgN,*/ compN, msCand, osNCand, IPrgPar_SdDes );
	}
}
		//msCand.Add(t1);
		//osNCand	<<endl<<	t1->Name()			<< sep<<	t1->_len	
		//		<< sep<<	msCand._TNumPosCand	<< sep<<	((CSecCand *)msCand._LSecCand.Last())->_NumPosCand
		//		<< sep<<	msCand._TNumCand	<< sep<<	((CSecCand *)msCand._LSecCand.Last())->_NumCand; 


int SondeDesignProg ( CProgParam_SondeDesign *IPrgPar_SdDes)
{	
	time_t t_0 = time(nullptr);

    IPrgPar_SdDes->_cp.Check_NNp_Targets( );

	std::ofstream osNCand(IPrgPar_SdDes->_cp._OutputFile.get() + ".TgCand.csv");
	osNCand.precision(2);
	osNCand	<<"\n"<<"Num T Pos" <<sep<< "Num T Cand"	
			<<sep<<	"Targ Num"	<<sep<< "Iterat#"  	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<< "Num T Hits"<<sep<< "Num Hits OK"				
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<<	"Targ name" <<sep<<	"Num Pos"  << sep<<  "Num Cand"	
			<<sep<< "Num T Pos" <<sep<< "Num T Cand"	
			<< std::fixed			; 

	time_t t_sec = time(nullptr);

	CMSecCand	msCand	(	
				   convCtoK_ctok( IPrgPar_SdDes->_sL ) ,
							CtoK(IPrgPar_SdDes->_Tm_sig),		 IPrgPar_SdDes->_G_sig      * 1000,
							CtoK(IPrgPar_SdDes->_MaxSd_nTgTm),	 IPrgPar_SdDes->_MinSd_nTgG * 1000,	
							CtoK(IPrgPar_SdDes->_MaxSelfTm),	 IPrgPar_SdDes->_MinSelfG   * 1000	 );

    if (!IPrgPar_SdDes->_cp._pSeqTargets->_NNPar)   /// Make seq espesific
        IPrgPar_SdDes->_cp._pSeqTargets->_NNPar=IPrgPar_SdDes->_cp._pSaltCorrNNp;

    msCand.Use                 (IPrgPar_SdDes->_cp._pSeqTargets);
	msCand._TDATmC->SetTa (CtoK(IPrgPar_SdDes->_cp._Ta         ));

	time_t t_al_created = time(nullptr);

	int /*tgN(0),*/ compN=0;

    FindSonden(IPrgPar_SdDes->_cp._pSeqTargets.get(), /*tgN,*/ compN, msCand, osNCand, IPrgPar_SdDes );

    NumRang<float> ExtrCovPerc(IPrgPar_SdDes->Coverage.get());
    if (! IPrgPar_SdDes->common.get())         ExtrCovPerc.SetMax(101.0f);
    if (! IPrgPar_SdDes->unique.get())         ExtrCovPerc.SetMin( -1.0f);

    /// Will return probes with a percent of other-target coverage with is not intern to the range ExtrCovPerc.
    /// That is: probes with hybrid in one target but in not than more than in ExtrCovPerc.Min % of the others, 
    /// and addicionaly, probes with hybrid in one target and at last in ExtrCovPerc.Max % of the others.

	msCand.ExportCommonSonden(  IPrgPar_SdDes->_cp._OutputFile.get().c_str(), 
                                IPrgPar_SdDes->_design, 
                                ExtrCovPerc, 
                                fileFormat ( (int)fasta | (int)csv )
                              );


	time_t t_tm_cal = time(nullptr);
	osNCand<< "\n" << "\n" <<"Time sec= "			<< sep<< t_sec			- t_0		
				   << "\n" <<"Time Ob crea="		<< sep<< t_al_created	- t_sec		
				   << "\n" <<"Time Tm calc= "		<< sep<< t_tm_cal		-t_al_created ;
	return 1;
}




        
        
    //if (!tg || ! tg->_Global._NSec)
	//std::shared_ptr<CMultSec>  tg(IPrgPar_SdDes->_cp._pSeqTargets );
        //._pSeqTargets.reset ( new CMultSec	(	, NNpar,
								//    IPrgPar_SdDes->_cp._MaxTgId,
								//    IPrgPar_SdDes->_cp._SecLim ,
        //                            IPrgPar_SdDes->_cp._MinSecLen ));
					//CMultSec     *nms=_Pr._cp.AddSeqFromFile	
					//									(ms, CreateStdFromManString
					//											(this->openFileDialog_targets->FileName)	);

