/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\main.Nana.h
*
* @brief 
*/

#ifndef main_nana_H
#define main_nana_H

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/tabbar.hpp>

//#include <nana/gui/tooltip.hpp>
//#include <nana/gui/widgets/toolbar.hpp>
//#include <nana/gui/widgets/progress.hpp>
//#include <nana/gui/widgets/group.hpp>

//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>

#include "thdy_programs\init_thdy_prog_param.h"
//#include "matrix.h" 
//#include "common_basics.h" 

#include "SetupPage.h"
#include "SeqExpl.h"
#include "FindSondenPage.h"
//#include "MplexPCR.h"
#include "uArray.h"
#include "TmCalcPage.h"


class ThDyNanaForm : public nana::form, public EditableForm , public ThDyProject
{
    using tabbar = nana::tabbar<nana::string> ;
	tabbar                     tabbar_     {*this};
    SetupPage                  setup_      {*this};
    FindSondenPage             findSond_   {*this};
    MplexPCR                   mPCR_       {*this};
    uArray                     uArr_       {*this}; 
    TmCalcPage                 tmCalc_     {*this}; 
    nana::NumUnitUpDown        numUpDwMaxTgId  {*this, STR("Max. ident.:"        ), 99,  50 , 100 ,   "%"}, 
                               numUpDw_TgBeg   {*this, STR("Beg.:"               ),  0,   0 , 100000,"nt"},    /// rev !!
                               numUpDw_TgEnd   {*this, STR("End.:"               ),  0,   0 , 100000,"nt"},    /// rev !!	
                               numUpDw_SLenMin {*this, STR("Min.Len.:"           ),  0,   0 , 100000,"nt"},
                               numUpDw_SLenMax {*this, STR("Max.Len.:"           ),  0,   0 , 100000,"nt"};
    nana::label                _firma     {*this, STR("ArielVina.Rodriguez@fli.bund.de")};
    ParamGUIBind::BindGroup    _commPP     ;

  public:    
    std::vector<std::unique_ptr<nana::form>> _results;
    SeqExpl                         mExpl_      {*this};

    ThDyNanaForm (int argc, char *argv[])  ;
    //~ThDyNanaForm();

    void SetDefLayout       () override;
    void AsignWidgetToFields() override;
    void add_page           (widget& w);
    void ShowExpl           (){tabbar_.activate(1);}
};


#endif