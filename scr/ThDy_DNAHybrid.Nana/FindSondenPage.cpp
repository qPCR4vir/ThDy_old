/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*  https://www.fli.de/en/institutes/institut-fuer-neue-und-neuartige-tierseuchenerreger/wissenschaftlerinnen/prof-dr-m-h-groschup/
*  distributed under the GNU General Public License, see <http://www.gnu.org/licenses/>.
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2016
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\FindSondenPage.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\FindSondenPage.h"
#include "ThDy_DNAHybrid.Nana\main.Nana.h"

FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)    try
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
catch (std::exception & e)
{
	throw std::runtime_error(std::string("An error ocurred during initialization of the Find probes page window:\n") + e.what());
}
catch (...)
{
	throw std::runtime_error(std::string("An unknonw error ocurred during initialization of the Find probes page window"));
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

