/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\TmCalcPage.h
*
* @brief 
*/

#ifndef TmCalcPage_H
#define TmCalcPage_H

#include "thdy_programs\init_thdy_prog_param.h"
#include "../../nana.ext/include/nanaBind.hpp"
#include <../../nana.ext/include/EditableForm.hpp>
#include <../../nana.ext/include/Numer.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/tooltip.hpp>
//#include <nana/gui/widgets/progress.hpp>





//
//#include <iostream>    // temp, for debugging
//#include <fstream>     // temp, for debugging
//#include <filesystem>
//
//#include <nana/gui/wvl.hpp>
//#include <nana/gui/widgets/tabbar.hpp>
//#include <nana/gui/widgets/treebox.hpp>
//#include <nana/gui/widgets/listbox.hpp>
//#include <nana/gui/widgets/toolbar.hpp>
//
//
//#include "matrix.h" 
//#include "common_basics.h" 
//
//
//using namespace ParamGUIBind;
//
class ThDyNanaForm ;
// 
//using List = nana::listbox;




class TmCalcPage : public CompoWidget
{
    ThDyProject             &_Pr;
    nana::group             primers             {*this, STR("<bold=true> Primers: </>" ), true}, 
                            interaction         {*this, STR("<bold=true> Interaction: </>" ), true},
                            align               {*this, STR("<bold=true> Alignment: </>" ), true}; 
    nana::textbox           sec_                {primers.inner()},  
                            sec2align_          {primers.inner()},  
                            txtBx_ResultSec     {align.inner()},  
                            txtBx_ResultSec2Align{align.inner()};
    nana::checkbox          chkBx_Tm_save_asPCR {*this, STR("save")},   
                            chkBx_align         {*this, STR("align")},
                            chkBx_copy_rev      {primers.inner(), STR("rev")},    
                            chkBx_copy_compl    {primers.inner(), STR("cpl")};
    nana::button            run_                {*this, STR("Tm !")},
                            copy_f_s_2          {primers.inner(), STR("copy")},   
                            copy_s              {primers.inner(), STR("c")},
                            copy_s_a            {primers.inner(), STR("c")};      
    nana::label             error_              {primers.inner(), STR("no error")};
    nana::NumberBox         Tm_min_Up{interaction.inner()}, Tm_Up{interaction.inner()}, Tm_max_Up{interaction.inner()} ,
                            Tm_min_Dw{interaction.inner()}, Tm_Dw{interaction.inner()}, Tm_max_Dw{interaction.inner()} ,
                            Tm_min_In{interaction.inner()}, Tm_In{interaction.inner()}, Tm_max_In{interaction.inner()} ,
                            G_min_Up {interaction.inner()},  G_Up{interaction.inner()},  G_max_Up{interaction.inner()} ,
                            G_min_Dw {interaction.inner()},  G_Dw{interaction.inner()},  G_max_Dw{interaction.inner()} ,
                            G_min_In {interaction.inner()},  G_In{interaction.inner()},  G_max_In{interaction.inner()} ;

    ParamGUIBind::BindGroup              _TmCalc;
public:     
    TmCalcPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= 	
	"vertical      gap=8  min=150    margin=5                              		\n\t"
	"		       < weight=95  primers  >                                               		\n\t"
	"		        < weight=95 horizontal gap=2  <weight=120 vertical gap=2  margin=[15,45,0,15]   Left  >              	\n\t"
	"                                                                       <weight=320   Table     >  >           		\n\t"
	"		        < weight=70 vertical  ResAlign>    		\n\t"
	"		\n\t"
            ;

         primers.plc().div("vert <weight=50 horizontal margin=[0,5,0,5] <min=100 vertical gap=2 InputSec>                            "  
                        "                                       <weight=50 gap=1 CopyBut grid=[2,2]  collapse(0,0,1,2)> > \n\t "
                        "<weight=23 horizontal  <weight=20>"
                        "                       <min=50    error     > "
                        "                       <weight=80 rev_compl >     >         \n\t  ");

         interaction.plc().div("vert <min=280    margin=[0,5,5,5] Table    grid=[7,4]    >                "  );

         align.plc().div("vert < weight=50 vertical margin=[0,5,0,5]  ResAlign  >                            " );


    }
    void AsignWidgetToFields() override
    {
	    _place.field("primers"  )<< primers    ;
	    _place.field("Left"     )<< run_        << chkBx_align;
	    _place.field("Table"    )<< interaction;
	    _place.field("ResAlign" )<< align      ;

	    primers.plc()["InputSec" ]<< sec_          << sec2align_ ;
	    primers.plc()["CopyBut"  ]<<  copy_f_s_2   << copy_s      << copy_s_a ;
	    primers.plc()["error"    ]<< error_        ;
	    primers.plc()["rev_compl"]<< chkBx_copy_rev << chkBx_copy_compl ;

	    interaction.plc()["Table" ]<< L""          << L"   min-" << L"Tm(°C)"   << L"-max" << L"   min-"  << L"G(kJ)"   << L"-max   "   
	                             << L"Up"        << Tm_min_Up << Tm_Up        << Tm_max_Up<<G_min_Up   <<  G_Up      <<  G_max_Up   
	                             << L"Down"      << Tm_min_Dw << Tm_Dw        << Tm_max_Dw<<G_min_Dw   <<  G_Dw      <<  G_max_Dw   
	                             << L"Interact"  << Tm_min_In << Tm_In        << Tm_max_In<<G_min_In   <<  G_In      <<  G_max_In  ;


        align.plc()["ResAlign" ]  << txtBx_ResultSec << txtBx_ResultSec2Align ;
    }
    void Run()
    {
		try
        {                                   
		   _Pr._TmCal._cp.Actualice_NNp();  
           _Pr._TmCal.Run ();
		}
		catch ( std::exception& e)
		{ 
            (nana::msgbox(*this,STR("Error during Tm calculation !"), nana::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
        txtBx_ResultSec      .caption (nana::string(nana::charset(_Pr._TmCal._AlignedSec )       ));
        txtBx_ResultSec2Align.caption (nana::string(nana::charset(_Pr._TmCal._AlignedSec2Align  )));
        Tm_min_Up.Value( _Pr._TmCal._TmS.Min ());
        Tm_Up    .Value( _Pr._TmCal._TmS.Ave ());  
        Tm_max_Up.Value( _Pr._TmCal._TmS.Max ()); 

        Tm_min_Dw.Value( _Pr._TmCal._Tm2A.Min ());
        Tm_Dw    .Value( _Pr._TmCal._Tm2A.Ave ());  
        Tm_max_Dw.Value( _Pr._TmCal._Tm2A.Max ()); 

        Tm_min_In.Value( _Pr._TmCal._TmHy.Min ());
        Tm_In    .Value( _Pr._TmCal._TmHy.Ave ());  
        Tm_max_In.Value( _Pr._TmCal._TmHy.Max ()); 

        G_min_Up.Value( _Pr._TmCal._GS .Min ());
        G_Up    .Value( _Pr._TmCal._GS.Ave ());  
        G_max_Up.Value( _Pr._TmCal._GS.Max ()); 

        G_min_Dw.Value( _Pr._TmCal._G2A .Min ());
        G_Dw    .Value( _Pr._TmCal._G2A.Ave ());  
        G_max_Dw.Value( _Pr._TmCal._G2A.Max ()); 

        G_min_In.Value( _Pr._TmCal._GHy.Min ());
        G_In    .Value( _Pr._TmCal._GHy.Ave ());  
        G_max_In.Value( _Pr._TmCal._GHy.Max ()); 
    }
    void Copy()
    {
        //_Pr._TmCal._Sec.CopyTrim (std::string(nana::charset (   sec_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec_Sec2Align	(rev, compl) ;

        //sec2align_.caption (nana::charset (_Pr._TmCal._Sec2Align.Get() ));
    }
    void Self()
    {
        //_Pr._TmCal._Sec.CopyTrim (std::string(nana::charset (  sec_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec	(rev, compl) ;

        //sec_.caption (nana::charset (_Pr._TmCal._Sec   .Get() ));
    }
    void Rev()
    {
        //_Pr._TmCal._Sec2Align.CopyTrim (std::string(nana::charset (  sec2align_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec2Align	(rev, compl) ;

        //sec2align_.caption (nana::charset (_Pr._TmCal._Sec2Align  .Get() ));
    }

};

#endif