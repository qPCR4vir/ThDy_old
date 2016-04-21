/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*  https://www.fli.de/en/institutes/institut-fuer-neue-und-neuartige-tierseuchenerreger/wissenschaftlerinnen/prof-dr-m-h-groschup/
*  distributed under the GNU General Public License, see <http://www.gnu.org/licenses/>.
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2016
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\FindSondenPage.h
*
* @brief 
*/

#ifndef FindSondenPage_H
#define FindSondenPage_H

#include "thdy_programs\init_thdy_prog_param.h"
#include "../../nana.ext/include/nanaBind.hpp"
#include <../../nana.ext/include/EditableForm.hpp>

#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
 
class ThDyNanaForm ;

class FindSondenPage : public CompoWidget
{    
	ThDyNanaForm &_Pr;
    ParamGUIBind::BindGroup   _findSond;

    nana::group         _gr_probes  {*this, ("<bold=true> Probes: </>"          ), true},
                        _gr_prob_tg {*this, ("<bold=true> Probe-target: </>"    ), true},
                        _gr_prob_ntg{*this, ("<bold=true> Probe-non-target: </>"), true},
                        _gr_find_prb{*this, (           " Find Probes "         ), true},
                        _gr_probself{*this, ("<bold=true> Probe-self: </>"      ), true};

    nana::NumUnitUpDown _Gmin     {_gr_probes, ("G : "    ), -5, -10 , 10, "kcal/mol"},   _Gmax   {_gr_probes,  (""), -1, -10, 10,   "kcal/mol"}, 
                        _Tmmin    {_gr_probes, ("Tm : "   ), 57,  40 , 60,  u8"°C"   },  _Tmmax   {_gr_probes,  (""), 63,  45, 75,   u8"°C"    },
                        _Lengthmin{_gr_probes, ("Length: "), 20,  15 , 35,  "nt"     }, _Lengthmax{_gr_probes,  (""), 35,  15, 40,   "nt"      },
                        _MaxG     {_gr_prob_tg, ("Max G: " ), 10, -10, 30, "kcal/mol"},  _MinTm   {_gr_prob_tg, ("Tm: "  ), 30,  10 , 60,  u8"°C"},
                        _MinG     {_gr_prob_ntg,("Min G: " ), 15, -10 , 30,"kcal/mol"},  _MaxTm   {_gr_prob_ntg,("Max Tm: "), 10, -10, 75, u8"°C"},
                        _MinSelfG {_gr_probself,("Min G: " ), 10, -10 , 30,"kcal/mol"}, _MaxSelfTm{_gr_probself,("Max Tm: "), 10, -10, 75, u8"°C"},
                        numUpDw_MinTargCov{ _gr_find_prb, ("max."),   0.0, 0.0 , 100.0,"%" }, 
                        numUpDw_MaxTargCov{ _gr_find_prb, ("min."), 100.0, 0.0 , 100.0,"%" } ;

    nana::tooltip _Gmintt     {_Gmin, ("Only probes with stronger interaction with target (smaller G by selected Ta) will be included"    ) }
		               /*,   _Gmax   {*this, (""), -1, -10, 10, "kcal/mol"}, 
                             _Tmmin    {*this, ("Tm :"   ), 57,  40 , 60,u8"°C"      },  _Tmmax   {*this, (""), 63,  45, 75, u8"°C"      }, 
                             _Lengthmin{*this, ("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, (""), 35,  15, 40, "nt"      },
                             _MaxG     {*this, ("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, ("Tm :"  ), 30,  10 , 60,u8"°C"}, 
                             _MinG     {*this, ("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, ("Max Tm"), 10, -10, 75, u8"°C"}, 
                             _MinSelfG {*this, ("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, ("Max Tm"), 10, -10, 75, u8"°C"}, 	
                             numUpDw_MinTargCov{ *this, ("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" }, 
                             numUpDw_MaxTargCov{ *this, ("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" }*/ ;

    nana::button        _design {*this, ("Design !" )}, 
                        _compare{*this, ("Compare !")};

    nana::checkbox      chkBx_unique{_gr_find_prb, ("Unique, with target coverage ")}, 
                        chkBx_common{_gr_find_prb, ("Common, with target coverage ")}, 
                        chkBx_showFindedProbes{*this, ("Show found probes")};

	nana::tooltip       chkBx_uniqueTT{chkBx_unique, ("For each target seq, probes with hybrid on it, AND maximum on a given percent of the OTHER targets will be reported")};
	nana::tooltip       chkBx_commonTT{chkBx_common, ("All probes with hybrid on at least the given percent of targets will be reported")};
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


    _gr_probes .div("vert < Sonde  margin=2 gap= 2 grid=[2,4]  	    \n\t"
	"					                                    		\n\t"
	"						                                  >	\n\t");

    _gr_prob_tg .div("<  margin=2 gap= 2 vertical   options>");
    _gr_prob_ntg.div("<  margin=2 gap= 2 vertical   options>");
    _gr_probself.div("<  margin=2 gap= 2 vertical   options>");
    _gr_find_prb.div("<  margin=5 gap= 2 TargCov grid=[2,2]>");

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
    void AsignWidgetToFields() override;

    void Run_Design(bool design);
};

#endif