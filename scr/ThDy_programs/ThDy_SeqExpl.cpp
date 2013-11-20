#pragma unmanaged



//
//int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr)  
//{
//	auto_ptr<CSaltCorrNN> apNNpar (Create_NNpar(IPrgPar_uArr->_cp)); 	CSaltCorrNN *NNpar = apNNpar.get() ;
//
//	CMultSec		pr		(			IPrgPar_uArr->_InputSondeFile.Get() ,		NNpar);
//	return MultiplexPCRProg ( IPrgPar_uArr, pr		)  ;
//}
//
//int MultiplexPCRProg ( CProgParam_MultiplexPCR *IPrgPar_uArr, 	CMultSec		&pr		)  
//{
//	CSaltCorrNN *NNpar =  pr._NNPar;
//	time_t t_0 = time(nullptr);
//	const int MaxGrDeg=300 ;			// crear NonDegSet para las sondas con menos de este gr de deg. Poner como ProgParam??
//	
//	string OutputTm		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputTm		+=".uArr.Tm.csv" ;	ofstream osTm	(OutputTm.c_str()	);	assert(osTm	);
//	string OutputG		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputG		+=".uArr.G.csv"  ;	ofstream osG	(OutputG.c_str()	);	assert(osG	);
//	string OutputPos	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPos	+=".uArr.Pos.csv";	ofstream osPos	(OutputPos.c_str()	);	assert(osPos);
//	string OutputPl_Tm	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPl_Tm	+=".Plasm_Tm.csv";	ofstream osPl_Tm(OutputPl_Tm.c_str());	assert(osPl_Tm);
//	string OutputPl_G	= IPrgPar_uArr->_cp._OutputFile.Get(); OutputPl_G	+=".Plasm_G.csv" ;	ofstream osPl_G	(OutputPl_G.c_str()	);	assert(osPl_G);
//	string OutputAl		= IPrgPar_uArr->_cp._OutputFile.Get(); OutputAl		+=".uArr.Al.csv" ;	ofstream osAl	(OutputAl.c_str()	);	assert(osAl	);	
//
//
//	CMultSec		tg		(			IPrgPar_uArr->_cp._InputTargetFile.Get(),		NNpar,
//										IPrgPar_uArr->_cp._MaxTgId,
//										IPrgPar_uArr->_cp._SecLim 	);/*,IPrgPar_uArr->_cp._SecBeg,	IPrgPar_uArr->_cp._SecEnd */	
//	time_t t_sec = time(nullptr);