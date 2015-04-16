/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\FindSondenPage.cpp
*
* @brief 
*/

//
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

#include "ThDy_DNAHybrid.Nana\main.Nana.h"
//#include "ThDy_DNAHybrid.Nana\FindSondenPage.h"

//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"

//class ThDyNanaForm;


         FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, STR("Find probes"), STR("FindSonden.lay.txt"))
    {
                bgcolor (static_cast<nana::color_rgb>(0xAAAAAA));  ///\todo: use codigo

        chkBx_showFindedProbes.check(true);
        InitMyLayout();
        SelectClickableWidget( *this);

        _design .events().click([&]() 
        {
            Run_Design(true );  
        });    

        _compare.events().click([&]() 
        {
            Run_Design(false);  
        });  

        //_Gmin.tooltip(STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be \"include\""));
   
   }
    void FindSondenPage::Run_Design(bool design)
    {
        _Pr._SdDes._design	 = design ;		
		 
		try{                                   
		        _Pr._SdDes._cp.Actualice_NNp();  
                _Pr.Run(_Pr._SdDes);	 //     _Pr._SdDes.Run ();	

                 if (chkBx_showFindedProbes.checked()) 
                    ( dynamic_cast<ThDyNanaForm&>(_Pr)).mExpl_.ShowFindedProbes_in_mPCR();
 		}
		catch ( std::exception& e)
		{ 
            (nana::msgbox(*this,STR("Error during Sonde Design !"), nana::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
    }   

