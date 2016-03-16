/**
* Copyright (C) 2009-2015, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\FindSondenPage.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\FindSondenPage.h"
#include "ThDy_DNAHybrid.Nana\main.Nana.h"
//


         FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, "Find probes", "FindSonden.lay.txt")
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

        //_Gmin.tooltip(("Only probes with stronger interaction with target (smaller G by selected Ta) will be \"include\""));
   
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
            (nana::msgbox(*this,"Error during Sonde Design !", nana::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
    }   

