/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\uArray.cpp
*
* @brief 
*/


//#include <iostream>    // temp, for debugging
//#include <fstream>     // temp, for debugging
//#include <filesystem>
//
//#include <nana/gui/wvl.hpp>
//#include <nana/gui/widgets/tabbar.hpp>
//#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/widgets/treebox.hpp>
//#include <nana/gui/widgets/listbox.hpp>
//#include <nana/gui/widgets/toolbar.hpp>
//#include <nana/gui/tooltip.hpp>
//
//#include <nana/gui/tooltip.hpp>
//#include <nana/gui/widgets/progress.hpp>
//
//#include "thdy_programs\init_thdy_prog_param.h"
//#include "matrix.h" 
//#include "common_basics.h" 
//
//
//
#include "ThDy_DNAHybrid.Nana\main.Nana.h"
#include "ThDy_DNAHybrid.Nana\TableResults.h"

//#include "ThDy_DNAHybrid.Nana\uArray.h"

//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"


          MplexPCR::MplexPCR            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("MplexPCR"), STR("MplexPCR.lay.txt"))
    {

        _do_mPCR      .events().click([&](){buttPCR_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
     void MplexPCR::buttPCR_Click()  //	  Run      _IPrgPar_mPCR
	{	 			
	 try{                                   
		  _Pr._mPCR._cp.Actualice_NNp();  
 		  _Pr.Run(_Pr._mPCR);	

          _Pr.mExpl_.RefreshProbes_mPCR(/*false*/); 

          _Pr._results.emplace_back(new TableRes(_Pr._mPCR._rtbl));
          _Pr._results.back()->show();
          _Pr._results.emplace_back(new TableRes(_Pr._mPCR._rtbl_self));
          _Pr._results.back()->show();
		}
	catch ( std::exception& e)
		{ 
          cerr<< e.what()    ;
          (nana::msgbox(*this,STR("Error during multiplex PCR analis !"), 
                                                nana::msgbox::button_t::ok)   <<e.what()) (  ) ;
		  return;
		}
        //ShowResTbl(_Pr._mPCR._rtbl );
        //_Pr._uArr._rtbl = nullptr;

        //ShowResTbl(_Pr._mPCR._rtbl_self );
        //_Pr._mPCR._rtbl_self = nullptr;

	}
            uArray::uArray            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("uArray"), STR("uArray.lay.txt"))
    {

        _do_uArray      .events().click([&](){buttuArray_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
       void uArray::buttuArray_Click()  
	{	 			
	 try{                                   
		  _Pr._uArr ._cp.Actualice_NNp();  
 		  _Pr.Run(_Pr._uArr);	

          _Pr._results.emplace_back(new TableRes(_Pr._uArr._rtbl));
          _Pr._results.back()->show();
		}
	catch ( std::exception& e)
		{ 
          cerr<< e.what()    ;
          (nana::msgbox(*this,STR("Error during uArr analis !"), 
                                                nana::msgbox::button_t::ok)   <<e.what()) (  ) ;
		  return;
		}
	}
