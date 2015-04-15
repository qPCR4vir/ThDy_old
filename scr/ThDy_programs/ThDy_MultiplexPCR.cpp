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
	CMultSec *cms{nullptr};
    for (  pr.goFirstMSec(); pr.NotEndMSec()   ;   pr.goNextMSec())    // usar solo selected   !!!
        if     ( pr.CurMSec()->_name == "compl")
        {    
            cms=pr.CurMSec();
            cms->Destroy();
        }
        else if( pr.CurMSec()->Selected())
                CreateComplProbes( *pr.CurMSec() );
	if(!cms) 
        cms=pr.AddMultiSec("compl");
	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )				// recorre todos las sondas
	{	CSec &s = *pr.CurSec() ;
		if( s.Selected())
		    cms->AddSec ( s.Clone(DNAstrand::rev_compl) ); 
	}
}


		//int t=MultiplexPCRProg ( IPrgPar_Calc, primers		)  ;

	int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr, CMultSec		&pr)  
{
	time_t t_0 = time(nullptr);
    IPrgPar_uArr->_cp.Check_NNp_Targets ();
	CreateComplProbes(	pr	);

	          microArrayProg   ( IPrgPar_uArr, 
                                 pr	, 
                                 pr, 
                                 t_0, 300    , 
                                 "_self"	
                               )  ;

   IPrgPar_uArr->_rtbl_self = IPrgPar_uArr->_rtbl;
   IPrgPar_uArr->_rtbl->TitTable( "Primers / Primers (align method: " +IPrgPar_uArr->_cp.TAMeth.ToString() +  " ). Multiplex PCR.") ; 

	auto res= microArrayProg ( IPrgPar_uArr, 
                               pr	, 
                               *IPrgPar_uArr->_cp._pSeqTargets.get(), 
                               t_0  , 300 
                              )  ;

    IPrgPar_uArr->_rtbl->TitTable(IPrgPar_uArr->_rtbl->TitTable()+ ". Multiplex PCR.");
	return res ; 
}


	int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr)  
{

    IPrgPar_uArr->Check_NNp_Targets_probes (IPrgPar_uArr->_probesMS.get());

	return MultiplexPCRProg ( IPrgPar_uArr, *IPrgPar_uArr->_probesMS.get())  ;

	
}

