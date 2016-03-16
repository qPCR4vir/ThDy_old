/**
* Copyright (C) 2009-2015, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\TmCalcPage.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\TmCalcPage.h"
#include "ThDy_DNAHybrid.Nana\main.Nana.h"

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

//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"


        TmCalcPage::TmCalcPage        (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, ("Tm Calc"), ("Tm Calc.lay.txt"))
    {
                         sec_.multi_lines(false).editable(true ).tip_string (("forward primer"));
                   sec2align_.multi_lines(false).editable(true ).tip_string (("reverse primer"));
              txtBx_ResultSec.multi_lines(false).editable(false).tip_string (("alingned forward primer"));
        txtBx_ResultSec2Align.multi_lines(false).editable(false).tip_string (("alingned reverse primer"));
        
        using ParamGUIBind::link;

        _TmCalc << link (   _Pr._TmCal.align      ,    chkBx_align    )    
                << link (   _Pr._TmCal._Sec       ,    sec_           )
                << link (   _Pr._TmCal._Sec2Align ,    sec2align_     )
                ;

        run_      .events().click([&](){Run ();});
        copy_f_s_2.events().click([&](){Copy();});      ;   //(*this, ("copy")),   
        copy_s    .events().click([&](){Self();});      ;  //  (*this, ("c")),
        copy_s_a  .events().click([&](){Rev ();});      ;  

        InitMyLayout();
        SelectClickableWidget( *this);
        SelectClickableWidget( error_);
    }
