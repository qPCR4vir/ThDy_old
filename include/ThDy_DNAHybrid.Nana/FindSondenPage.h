/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\FindSondenPage.h
*
* @brief 
*/

#ifndef FindSondenPage_H
#define FindSondenPage_H


#include <nana/gui/tooltip.hpp>
#include <../../nana.ext/include/EditableForm.hpp>
#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/widgets/group.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include <nana/gui/tooltip.hpp>

//#include "../../nana.ext/include/nanaBind.hpp"

//#include "thdy_programs\init_thdy_prog_param.h"



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
//#include <nana/gui/widgets/progress.hpp>
//
//
//#include "matrix.h" 
//#include "common_basics.h" 
//
//

//
//class ThDyNanaForm ;
// 
//using List = nana::listbox;




class FindSondenPage : public CompoWidget
{    
    ThDyProject &_Pr;
    ParamGUIBind::BindGroup   _findSond;
    nana::group         _gr_probes  {*this, STR("<bold=true> Probes: </>"          ), true},
                        _gr_prob_tg {*this, STR("<bold=true> Probe-target: </>"    ), true},
                        _gr_prob_ntg{*this, STR("<bold=true> Probe-non-target: </>"), true},
                        _gr_find_prb{*this, STR(           " Find Probes "         ), true},
                        _gr_probself{*this, STR("<bold=true> Probe-self: </>"      ), true};
    nana::NumUnitUpDown _Gmin     {_gr_probes.inner(), STR("G : "    ), -5, -10 , 10,"kcal/mol"},   _Gmax   {_gr_probes.inner(), STR(""), -1, -10, 10, "kcal/mol"}, 
                        _Tmmin    {_gr_probes.inner(), STR("Tm : "   ), 57,  40 , 60,"°C"      },  _Tmmax   {_gr_probes.inner(), STR(""), 63,  45, 75, "°C"      }, 
                        _Lengthmin{_gr_probes.inner(), STR("Length: "), 20,  15 , 35,"nt"      }, _Lengthmax{_gr_probes.inner(), STR(""), 35,  15, 40, "nt"      },
                        _MaxG     {_gr_prob_tg.inner(), STR("Max G: " ), 10, -10, 30, "kcal/mol" },  _MinTm   {_gr_prob_tg.inner(), STR("Tm: "  ), 30,  10 , 60,"°C"}, 
                        _MinG     {_gr_prob_ntg.inner(), STR("Min G: " ), 15, -10 , 30,"kcal/mol" }, _MaxTm   {_gr_prob_ntg.inner(), STR("Max Tm: "), 10, -10, 75, "°C"}, 
                        _MinSelfG {_gr_probself.inner(), STR("Min G: " ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{_gr_probself.inner(), STR("Max Tm: "), 10, -10, 75, "°C"}, 	
                        numUpDw_MinTargCov{ _gr_find_prb.inner(), STR("max."),   0.0, 0.0 , 100.0,"%" }, 
                        numUpDw_MaxTargCov{ _gr_find_prb.inner(), STR("min."), 100.0, 0.0 , 100.0,"%" } ;
    nana::tooltip _Gmintt     {_Gmin, STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be included"    ) }/*,   _Gmax   {*this, STR(""), -1, -10, 10, "kcal/mol"}, 
                             _Tmmin    {*this, STR("Tm :"   ), 57,  40 , 60,"°C"      },  _Tmmax   {*this, STR(""), 63,  45, 75, "°C"      }, 
                             _Lengthmin{*this, STR("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, STR(""), 35,  15, 40, "nt"      },
                             _MaxG     {*this, STR("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, STR("Tm :"  ), 30,  10 , 60,"°C"}, 
                             _MinG     {*this, STR("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, STR("Max Tm"), 10, -10, 75, "°C"}, 
                             _MinSelfG {*this, STR("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, STR("Max Tm"), 10, -10, 75, "°C"}, 	
                             numUpDw_MinTargCov{ *this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" }, 
                             numUpDw_MaxTargCov{ *this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" }*/ ;

    nana::button        _design {*this, STR("Design !" )}, 
                        _compare{*this, STR("Compare !")};

    nana::checkbox      chkBx_unique{_gr_find_prb.inner(), STR("Unique, with target coverage ")}, 
                        chkBx_common{_gr_find_prb.inner(), STR("Common, with target coverage ")}, 
                        chkBx_showFindedProbes{*this, STR("Show Finded Probes")};
	nana::tooltip       chkBx_uniqueTT{chkBx_unique, STR("For each target seq, probes with hybrid on it, AND maximum on a given percent of the OTHER targets will be reported")};
	nana::tooltip       chkBx_commonTT{chkBx_common, STR("All probes with hybrid on at laest the given percent of targets will be reported")};
public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    void SetDefLayout   () override
    {
      _DefLayout=  
	"vertical   gap=2    margin=5                    			\n\t"
	"		<weight=10     >       			\n\t"
	"	   <weight=260 gap=8 <weight=5> <weight=388 vertical   		                      	\n\t"
	"	                                              <weight=115 <weight=388 Sonde  > >		             	\n\t"
	"		                                           <weight=10>			\n\t"
	"			                                       <weight=72 TargCov        >    			\n\t"
	"		                                           <weight=10> 			\n\t"
	"			                                       <weight=40 <   <> <weight=300   gap=20 Run>  <>     > >    			\n\t"
	"			                                       <weight=10>                                 			\n\t"
	"			                         >   <> <weight=230 gap=1 vertical  options> <weight=5>                          			\n\t"
	"	   >   			\n\t"
	"		<weight=23   <weight=140> <Output>   <> >       		          	\n\t"
	"			 			\n\t"
	"		\n\t"
        ;


    _gr_probes .plc().div("vert < Sonde  margin=2 gap= 2 grid=[2,4]  	    \n\t"
	"					                                    		\n\t"
	"						                                  >	\n\t");

    _gr_prob_tg .plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_prob_ntg.plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_probself.plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_find_prb.plc().div("<  margin=5 gap= 2 TargCov grid=[2,2]>");

         _Gmin.ResetLayout     (45,40,55 );   _Gmax.ResetLayout     (1,40,75 );
        _Tmmin.ResetLayout     (45,40,55 );  _Tmmax.ResetLayout     (1,40,75 );
        _Lengthmin.ResetLayout (45,40,55 );  _Lengthmax.ResetLayout (1,40,75 );
 
        _MaxG.ResetLayout     (110,45,50 );   
        _MinTm.ResetLayout    (110,45,50 );  

        _MinG.ResetLayout     (110,45,50 );   
        _MaxTm.ResetLayout    (110,45,50 );   

        _MinSelfG.ResetLayout  (110,45,50);   
        _MaxSelfTm.ResetLayout (110,45,50 );  

        numUpDw_MinTargCov.ResetLayout (30,40,40 );  
        numUpDw_MaxTargCov.ResetLayout (30,40,40 );  
    }
    void AsignWidgetToFields() override
    {
        using ParamGUIBind::link;

        _findSond << link (   _Pr._SdDes.G_sig ,            _MaxG     )    
                  << link (   _Pr._SdDes.Tm_sig ,           _MinTm    )
                  << link (   _Pr._SdDes.MinSd_nTgG,        _MinG     )
                  << link (   _Pr._SdDes.MaxSd_nTgTm,       _MaxTm    )
                  << link (   _Pr._SdDes.MinSelfG,          _MinSelfG )
                  << link (   _Pr._SdDes.MaxSelfTm,         _MaxSelfTm)
                  << link (   _Pr._SdDes.sL.G,        _Gmin,_Gmax     )
                  << link (   _Pr._SdDes.sL.T,       _Tmmin,_Tmmax    )
                  << link (  _Pr._SdDes.sL.L,    _Lengthmin,_Lengthmax)
                  << link (  _Pr._SdDes.common,           chkBx_common)
                  << link (  _Pr._SdDes.unique,           chkBx_unique)
                  << link ( _Pr._SdDes.Coverage,  numUpDw_MinTargCov,  numUpDw_MaxTargCov)	

            ;
        
        /// Use room (wd,w,h) in combination with a <Table grid=[W,H]>
	            _place["Sonde"]    << _gr_probes ;
                _place["TargCov"]  << _gr_find_prb ;     
        _place.field("Run"     )   << _design	<< _compare	;
	    _place.field ("options" )  << _gr_prob_tg <<  _gr_prob_ntg     << _gr_probself;
        _place.field("Output"  )   << chkBx_showFindedProbes;

          _gr_probes.plc()["Sonde"]  << STR("                               Min." )<< STR("           Max."  ) 
                                   <<   _Gmin     <<   _Gmax  
                                   <<   _Tmmin    <<   _Tmmax  
                                   << _Lengthmin  <<   _Lengthmax  ; 

        _gr_find_prb.plc()["TargCov"]<< chkBx_unique << numUpDw_MinTargCov       
                                   << chkBx_common << numUpDw_MaxTargCov     	;
                 
	    _gr_prob_tg .plc()["options"] <<   _MaxG     << _MinTm;
        _gr_prob_ntg.plc()["options"] <<   _MinG     << _MaxTm;
        _gr_probself.plc()["options"] <<   _MinSelfG << _MaxSelfTm    ;


    }

    void Run_Design(bool design);
};

#endif