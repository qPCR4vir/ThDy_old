/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\main.Nana.h
*
* @brief 
*/

#ifndef uArray_H
#define uArray_H

#include <../../nana.ext/include/EditableForm.hpp>
#include "../../nana.ext/include/nanaBind.hpp"
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
//#include <nana/gui/widgets/group.hpp>
//
//#include <../../nana.ext/include/Numer.hpp>
//
//#include "thdy_programs\init_thdy_prog_param.h"
//#include "matrix.h" 
//#include "common_basics.h" 
//
//
//using namespace ParamGUIBind;
//
class ThDyNanaForm ;
// 
//using List = nana::listbox;



class uArray : public CompoWidget
{ public: 
    ThDyNanaForm      &_Pr;
    nana::button  _do_uArray{*this, (" uArray ! ")};
    ParamGUIBind::BindGroup          _uArray;

    uArray (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	        "  < weight=23>       \n\t "
            "  <<><_do_uArray  vertical min=50 max=200><> weight=50>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("_do_uArray"         )<<_do_uArray;
    }

  private: void buttuArray_Click(); //	  Run      _IPrgPar_mPCR
};


class MplexPCR : public CompoWidget
{ public: 
    ThDyNanaForm      &_Pr;
    nana::button  _do_mPCR{*this, (" PCR ! ")};
    ParamGUIBind::BindGroup          _mPCR;

    MplexPCR (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	        "  <_PrimersFilePCR weight=23>       \n\t "
            "  <<><_do_mPCR  vertical min=50 max=200><> weight=50>       \n\t "
            //"  <wieght=300 <vertical min=50 max=200 buttons> <> <weight=80 checks>>   \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
       //_mPCR<< /*link(   _Pr._mPCR._InputSondeFile , _PrimersFilePCR)*/

       //     ;

	    _place.field("_do_mPCR"         )<<_do_mPCR;
	    //_place.field("checks"          )<<"save result";
    }

  private: void buttPCR_Click(); //	  Run      _IPrgPar_mPCR
};

#endif