//#include "StdAfx.h"
#pragma unmanaged
#include "ThDy_programs/prog_comm_functions.h"



auto_ptr<CSaltCorrNN> Create_NNpar(ThDyCommProgParam& _cp)
{
	auto_ptr<CSaltCorrNN> apNNpar ( new   
				CSaltCorrNN			(	_cp._ConcSd, 
										_cp._ConcTg, 
										_cp._ConcSalt   )); 		CSaltCorrNN& NNpar = *apNNpar.get() ;
	NNpar.SetTa(				CtoK(	_cp._Ta));			// Aqui por si acaso. Revisar.

	if (_cp._loadNNPar){ifstream isTDP(_cp._InputNNFile.Get());	assert(isTDP);	NNpar.LoadNNParam(isTDP) ;	}
	if (_cp._saveNNPar)
	{	string OutputTDP(_cp._OutputFile.Get()) ; 		OutputTDP += ".ThDyParam.csv";
		ofstream osTDP	(OutputTDP.c_str());				assert(osTDP);	
		osTDP << NNpar ;
	}
	return apNNpar ;
}

auto_ptr<ThDyAlign> Create_ThDyAlign(ThDyCommProgParam& _cp, LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar)
{
	auto_ptr<ThDyAlign>	apAl;
	switch (	_cp._TAMeth )
	{	case TAMeth_Tm: default:	apAl.reset(new ThDyAlign_Tm( MaxLenSond ,  MaxLenTarg, NNpar) );   break;
		case TAMeth_Fract:			apAl.reset(new FracTDAlign ( MaxLenSond ,  MaxLenTarg, NNpar) );   break;
		case TAMeth_G:				apAl.reset(new ThDyAlign_G ( MaxLenSond ,  MaxLenTarg, NNpar) );   break;
	}

	apAl->SetTa		 (			CtoK(	_cp._Ta	    ));		// OK
	return apAl;
}

inline void Hybrid(CSec &s, CSec &t, 	ThDyAlign &Al,	ofstream &osTm,
														ofstream &osG,
														ofstream &osPos,
														ofstream &osPl_Tm,
														ofstream &osPl_G,
														ofstream &osAl,
														CTable<TmGPos> *rtbl 	/*,
														CTable<Temperature> *tlTm,
														CTable<Energy>	*tlG,
														CTable<SecPos> *tlPos*/)
{	Al.Align( &(s), &(t));					//  virtual !!!
	Al.SelectOptParam(Al.Ta());				//	FrAl.GetOptHit();

	if (osTm	) osTm		<<sep	<<	KtoC(Al.Tm())			;	if (rtbl) *rtbl << TmGPos ( KtoC( Al.Tm() ), Al.G()	/ 1000	,Al._maxgloj);
	if (osG		) osG		<<sep	<<		 Al.G()	/ 1000		;	/*if (tlG)  *tlG  <<  Al.G()	/ 1000		;*/
	if (osPos	) osPos		<<sep	<<		 Al._maxgloj		;	//if (tlPos) *tlPos<<  Al._maxgloj			;	// pos del 5'
	if (osPl_Tm	) osPl_Tm	<<"\t"	<<  KtoC(Al.Tm())			;
	if (osPl_G	) osPl_G	<<"\t"	<<  	 Al.G()	/ 1000		;

	print_ThDyAlign (osAl, Al);
	//Al.Export_DPMz_Pre(osAl);


}

void HybridPr(CMultSec &pr, CSec &t, 	ThDyAlign &Al,	ofstream &osTm,
														ofstream &osG,
														ofstream &osPos,
														ofstream &osPl_Tm,
														ofstream &osPl_G,
														ofstream &osAl,
														CTable<TmGPos> *rtbl 	/*,
														CTable<Temperature> *tlTm,
														CTable<Energy>	*tlG,
														CTable<SecPos> *tlPos*/)
{	for (  pr.goFirstSec()   ; pr.NotEndSec()   ;   pr.goNextSec() )  // recorre todos los primers de nuevo
	{	CSec &s = *pr.CurSec() ; 
		if(!s.Selected()) continue;
		if ( ! s.NonDegSet()  ) 
			{	
				Hybrid (s, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, rtbl);
			} else 
				{	CMultSec *nds=s.NonDegSet() ;
					for (  nds->goFirstSec()  ; nds->NotEndSec() ; nds->goNextSec() )// recorre todos las var no deg
					{	CSec &s = *nds->CurSec() ;
						Hybrid (s, t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, rtbl);
					}// recorre todos las var no deg
				} 
	}
	for (  pr.goFirstMSec(); pr.NotEndMSec()   ;   pr.goNextMSec())  
		HybridPr (*pr.CurMSec(), t, 	Al, osTm, osG,osPos,osPl_Tm,osPl_G,osAl, rtbl);
}			
