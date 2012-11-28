#pragma once
#include "ThDySec\sec.h"
#pragma managed
namespace ThDy_DNAHybridWF {

	//using namespace System;
	//using namespace System::ComponentModel;
	//using namespace System::Collections;
	//using namespace System::Windows::Forms;
	//using namespace System::Data;
	//using namespace System::Drawing;

	public ref class CMSref
	{public:
		CMultSec     *ms;
		CMSref(CMultSec     *msec):ms(msec){}
	};

	public ref class CSref
	{public:
		CSec     *s;
		CSref(CSec     *sec):s(sec){}
	};


	//ref class	String;
	//char *CreateCharFromManString(String ^Text);

}