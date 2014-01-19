//#include "StdAfx.h"
#pragma unmanaged
#include "ThDy_programs/prog_comm_functions.h"


int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr, 	CMultSec		&primers	)  ;

int DegTmCalc ( CProgParam_TmCalc *IPrgPar_Calc)  
{
	const int MaxGrDeg=300 ;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??

    if (IPrgPar_Calc->_save)  
        IPrgPar_Calc->_cp.Check_NNp_Targets ();
    else 
        IPrgPar_Calc->_cp.Actualice_NNp ( );

	Temperature Ta=  IPrgPar_Calc->_cp._pSaltCorrNNp->Ta() ; 
	CSec			Sec		  (			IPrgPar_Calc->_Sec.Get() ,		0, "Sec",		IPrgPar_Calc->_cp._pSaltCorrNNp); 
	if (Sec.Len() < 1)  return 0 ; // Error :  no sec !!!!!!
	Sec.CreateNonDegSet();	

	if (CountDegBases(					IPrgPar_Calc->_Sec2Align.Get())		< 1)				
										IPrgPar_Calc->Update_Sec_Sec2Align(true,true);	
	CSec			Sec2Align (			IPrgPar_Calc->_Sec2Align.Get() ,	0, "Sec2Align",	IPrgPar_Calc->_cp._pSaltCorrNNp);
	Sec2Align.CreateNonDegSet();

	CMultSec	*pr,	*tg;			// Esto se puede hacer mejor
	if   (      Sec.NonDegSet()) pr=      Sec.NonDegSet() ; 	else {pr =new CMultSec(IPrgPar_Calc->_cp._pSaltCorrNNp); pr->AddSec(      &Sec);}	
	if   (Sec2Align.NonDegSet()) tg=Sec2Align.NonDegSet() ;		else {tg =new CMultSec(IPrgPar_Calc->_cp._pSaltCorrNNp); tg->AddSec(&Sec2Align);}	

		
	IPrgPar_Calc->_TmS  = KtoC(pr->_Local._Tm) ;// (    KtoC(pr->_minTm)   ,   KtoC(pr->_maxTm)   ) ; 
	IPrgPar_Calc->_Tm2A = KtoC(tg->_Local._Tm) ;

	CSec *pr_maxTmH=pr->goFirstSec()  ;	IPrgPar_Calc->_GS.Set ( pr_maxTmH->G()/1000  )  ; 
	CSec *tg_maxTmH=tg->goFirstSec()  ; IPrgPar_Calc->_G2A.Set( tg_maxTmH->G()/1000  )  ;
														
	auto_ptr<ThDyAlign> apAl; //	ThDyAlign	*pAl=nullptr;	

	//LonSecPos TgMaxLen= (tg->_TMaxLen > pr->_TMaxLen) ? tg->_TMaxLen : pr->_TMaxLen ;

	if ( IPrgPar_Calc->_align)	
	{	apAl= Create_ThDyAlign(	IPrgPar_Calc->_cp, pr->_Global._Len.Max() , tg->_Global._Len.Max(), IPrgPar_Calc->_cp._pSaltCorrNNp);
		
	apAl->Align( pr_maxTmH, tg_maxTmH);					apAl->SelectOptParam( Ta);	//  virtual !!! Si G la Ta pudo cambiar, por eso aqui explicita
								
		IPrgPar_Calc->_TmHy.Set ( KtoC( apAl->Tm() ) );		//	FrAl.GetOptHit();
		IPrgPar_Calc->_GHy.Set  ( apAl->G ()/1000    );		//		print_ThDyAlign (osAl, Al);	//Al.Export_DPMz_Pre(osAl);

	}
	else 
	{	AlignedSecPar al( pr_maxTmH->GetCopyFullSec() , tg_maxTmH->GetCopyFullSec(), IPrgPar_Calc->_cp._pSaltCorrNNp ); // la Ta en NNpar no cambio

		IPrgPar_Calc->_TmHy.Set ( KtoC( al.Tm() ) );
		IPrgPar_Calc->_GHy.Set  (	al.G ()/1000  );
	}

	for (	pr->goFirstSec() ; pr->NotEndSec()   ;   pr->goNextSec() ) // recorre todos las var no deg de la sonda
	{	CSec &s = *pr->CurSec() ; 					 Energy  g= s.G (Ta)/1000;			Temperature tm ;
		IPrgPar_Calc->_GS.Expand(g);
		
		for (	tg->goFirstSec() ; tg->NotEndSec()   ;   tg->goNextSec() ) // recorre todos las var no deg de la sonda
		{	CSec &t = *tg->CurSec() ;					     g= t.G (Ta)/1000 ;
			IPrgPar_Calc->_G2A.Expand(g) ;

			if ( IPrgPar_Calc->_align)	
			{	ThDyAlign	&Al=*apAl.get();
				Al.Align( &(s), &(t));				Al.SelectOptParam(Ta);			//	FrAl.GetOptHit();					
															 g= Al.G ()/1000 ;			tm=  KtoC( Al.Tm() ) ;
				IPrgPar_Calc->_GHy.Expand(g) ;
				if (IPrgPar_Calc->_TmHy.Max() <=  tm  ) 
				{	IPrgPar_Calc->_TmHy.Max()  =  tm; 
					CHitAligned Hit (Al);
					IPrgPar_Calc->Copy_AlignedSec      ( (char*)(Hit._sd ) /*)*/);
					IPrgPar_Calc->Copy_AlignedSec2Align( (char*)(Hit._tg ) /*)*/);
				} 
				else if  (IPrgPar_Calc->_TmHy.Min() >  tm  ) {IPrgPar_Calc->_TmHy.Min() = tm; }

			} else 
			{
				AlignedSecPar al( (s.GetCopyFullSec())  , (t.GetCopyFullSec()), IPrgPar_Calc->_cp._pSaltCorrNNp ); 		
															g= al.G ()/1000 ;	float tm=KtoC( al.Tm() ) ; 
				IPrgPar_Calc->_GHy.Expand(g) ;
				if       (IPrgPar_Calc->_TmHy.Max() <  tm  ) 
				{	IPrgPar_Calc->_TmHy.Max() = tm; 
					pr_maxTmH=&s; 
					tg_maxTmH=&t;
				} 
				else if  (IPrgPar_Calc->_TmHy.Min() >  tm  ) {IPrgPar_Calc->_TmHy.Min() = tm; }
			}

		}// recorre todos las var no deg
	}
	if ( ! IPrgPar_Calc->_align)	
	{
		IPrgPar_Calc->Set_AlignedSec      ( (char*)pr_maxTmH->GetCopyFullSec() );
		IPrgPar_Calc->Set_AlignedSec2Align( (char*)tg_maxTmH->GetCopyFullSec() );
	}

	if   (!      Sec.NonDegSet()) {pr->Free();		delete pr;}	
	if   (!Sec2Align.NonDegSet()) {tg->Free();		delete tg;}	
	//delete pAl;	
	if (IPrgPar_Calc->_save)	
	{
		CMultSec primers(IPrgPar_Calc->_cp._pSaltCorrNNp); primers.AddSec(      &Sec);
								 primers.AddSec(&Sec2Align);

		int t=MultiplexPCRProg ( IPrgPar_Calc, primers		)  ;
		primers.Free();
		return t;
	}

	return 1;

}

	//IPrgPar_uArr->_GS.Set();
	//IPrgPar_uArr->_G2A.Set();
	//IPrgPar_uArr->_GHy.Set();
	//IPrgPar_uArr->_TmHy.Set();
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

		//switch (	IPrgPar_uArr->_cp._TAMeth )
		//{	case TAMeth_Tm: default:	pAl=new ThDyAlign_Tm( pr->_TMaxLen ,  TgMaxLen, *NNpar);   break;
		//	case TAMeth_Fract:			pAl=new FracTDAlign ( pr->_TMaxLen ,  TgMaxLen, *NNpar);   break;
		//	case TAMeth_G:				pAl=new ThDyAlign_G ( pr->_TMaxLen ,  TgMaxLen, *NNpar);   break;
		//}
		//ThDyAlign	&Al=*pAl;
		//Al.SetTa		 (			CtoK(	IPrgPar_uArr->_cp._Ta	    ));		// OK

//AlignedSecPar al( reinterpret_cast <char *> (pr_maxTmH->_c)  , reinterpret_cast <char *> (tg_maxTmH->_c), *NNpar );