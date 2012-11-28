#ifndef _PROG_COMM_FUNTIONS_H
#define _PROG_COMM_FUNTIONS_H
#pragma unmanaged
#include <memory>

#include "init_ThDy_prog_param.h"
#include "..\ThDySec\th_dy_align.h"
#include "..\ThDySec\matrix.h"


auto_ptr<CSaltCorrNN> Create_NNpar    (ThDyCommProgParam& _cp);

auto_ptr<ThDyAlign>   Create_ThDyAlign(ThDyCommProgParam& _cp, LonSecPos MaxLenSond, LonSecPos MaxLenTarg, std::shared_ptr<CSaltCorrNN>  NNpar);

class OutStr
{public:
	ofstream &Tm, &G, &Pos, &Pl_Tm, &Pl_G, &Al;
	OutStr(	ofstream &osTm,
			ofstream &osG,
			ofstream &osPos,
			ofstream &osPl_Tm,
			ofstream &osPl_G,
			ofstream &osAl
		  )
			:Tm(osTm), G(osG),Pos(osPos),Pl_Tm(osPl_Tm),Pl_G(osPl_G),Al(osAl)
			{}
};


inline void Hybrid(CSec &s, CSec &t, 	ThDyAlign &Al,	ofstream &osTm,
														ofstream &osG,
														ofstream &osPos,
														ofstream &osPl_Tm,
														ofstream &osPl_G,
														ofstream &osAl,
														CTable<TmGPos> *_rtbl = nullptr	/*,
														CTable<Temperature> *_tlTm = nullptr	,
														CTable<Energy>	*tlG = nullptr,
														CTable<SecPos> *tlPos  = nullptr*/);


void HybridPr(CMultSec &pr, CSec &t, 	ThDyAlign &Al,	ofstream &osTm,
														ofstream &osG,
														ofstream &osPos,
														ofstream &osPl_Tm,
														ofstream &osPl_G,
														ofstream &osAl,
														CTable<TmGPos> *_rtbl = nullptr	/*,
														CTable<Temperature> *_tlTm = nullptr	,
														CTable<Energy>	*tlG = nullptr,
														CTable<SecPos> *tlPos  = nullptr*/);


#endif