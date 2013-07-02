#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging
#include<filesystem>

#include "thdy_programs\init_thdy_prog_param.h"

class ThDyNanaForm ;

class FindSondenPage : public CompoWidget
{public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    ThDyProject &p_;
    OpenSaveBox nTsec_;
    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	                 //"       <weight=1>                \n\t"
	                 "       <NonTargSeq weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("NonTargSeq" )<<nTsec_;

    }
};

class TmCalcPage : public CompoWidget
{public: 
    ThDyProject        &_Pr;
    nana::gui::textbox  sec_, sec2align_, txtBx_ResultSec, txtBx_ResultSec2Align;
    nana::gui::checkbox chkBx_Tm_save_asPCR, chkBx_align, chkBx_copy_rev, chkBx_copy_compl;
    nana::gui::button   copy_f_s_2, copy_s, copy_s_a, run_;
    nana::gui::label    error_;
    nana::gui::NumberBox    Tm_min_Up, Tm_Up, Tm_max_Up ;
    nana::gui::NumberBox    Tm_min_Dw, Tm_Dw, Tm_max_Dw ;
    nana::gui::NumberBox    Tm_min_In, Tm_In, Tm_max_In ;
    nana::gui::NumberBox    G_min_Up,  G_Up,  G_max_Up ;
    nana::gui::NumberBox    G_min_Dw,  G_Dw,  G_max_Dw ;
    nana::gui::NumberBox    G_min_In,  G_In,  G_max_In ;

    TmCalcPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2  min=150           \n\t"
            "       < weight=50  <vertical gap=2 InputSec>   < weight=50 gap=1 CopyBut grid[2,2]>  >       \n\t "
            "       < weight=25 <weight=20><error min=50> <rev_compl weight=80>>    "
            "       < weight=90 <vertical weight=80 gap=2 Left>   < Table min=300  gap=1 grid[4,7]>  >       \n\t "
            "       < vertical weight=50  ResAlign>    "
	                 //"       <weight=1>                \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("rev_compl")<< chkBx_copy_rev << chkBx_copy_compl ;
	    _place.field("InputSec" )<< sec_ << sec2align_ ;
	    _place.field("error"    )<< error_ ;
	    _place.field("Left"     )<< run_  ;
	    _place.field("CopyBut"  )<< nana::gui::place::room (copy_f_s_2, 2, 1)<< copy_s << copy_s_a ;
	    _place.field("Table"    )<< ""          << "   min-" << "Tm(°C)"   << "-max" << "   min-"  << "G(kJ)"   << "-max   "  ;
	    _place.field("Table"    )<< "Up"        << Tm_min_Up << Tm_Up      << Tm_max_Up<<G_min_Up  <<  G_Up     <<  G_max_Up  ;
	    _place.field("Table"    )<< "Down"      << Tm_min_Dw << Tm_Dw      << Tm_max_Dw<<G_min_Dw  <<  G_Dw     <<  G_max_Dw  ;
	    _place.field("Table"    )<< "Interact"  << Tm_min_In << Tm_In      << Tm_max_In<<G_min_In  <<  G_In     <<  G_max_In  ;


	    _place.field("ResAlign" )<< txtBx_ResultSec << txtBx_ResultSec2Align ;
    }
    void Run()
    {
        _Pr._TmCal.      _Sec.CopyTrim (std::string(nana::charset (      sec_.caption ())).c_str() );		
        _Pr._TmCal._Sec2Align.CopyTrim (std::string(nana::charset (sec2align_.caption ())).c_str() );		
   //         	_TmCalThDyP = gcnew	TagBindGroup()
			//<<		TagBind 	( txtBx_Sec			 , _Pr._TmCal._Sec		)	
			//<<		TagBind 	( txtBx_Sec2Align	 , _Pr._TmCal._Sec2Align)	
			//<<		TagBind 	( chkBx_Tm_save_asPCR, _Pr._TmCal._save		)
			//<<		TagBind		( chkBx_align		 , _Pr._TmCal._align	)  
			//;
        _Pr._TmCal.Run ();
        txtBx_ResultSec      .caption (nana::charset (_Pr._TmCal._AlignedSec      .Get() ));
        txtBx_ResultSec2Align.caption (nana::charset (_Pr._TmCal._AlignedSec2Align.Get() ));
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
        //G_min_Up,  G_Up,  G_max_Up 
        //G_min_Dw,  G_Dw,  G_max_Dw 
        //G_min_In,  G_In,  G_max_In 


        //Tm_min_Up.caption ((nana::charset (std::to_string(  _Pr._TmCal._TmS.Min ()) )));
        //Tm_Up    .caption ((nana::charset (std::to_string( _Pr._TmCal._TmS.Ave ()) )));  
        //Tm_max_Up.caption ((nana::charset (std::to_string( _Pr._TmCal._TmS.Max ()) ))); 
        //Tm_min_Dw, Tm_Dw, Tm_max_Dw
        //Tm_min_In, Tm_In, Tm_max_In
        //G_min_Up,  G_Up,  G_max_Up 
        //G_min_Dw,  G_Dw,  G_max_Dw 
        //G_min_In,  G_In,  G_max_In 


    }

};

class SetupPage : public CompoWidget
{public: 
    SetupPage (ThDyNanaForm& tdForm);
    ThDyProject &_Pr;
    nana::gui::button  set_def_proj_;

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	                 //"       <weight=1>                \n\t"
	                 "       <SetDefProj weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("SetDefProj" )<<set_def_proj_;
    }
};

class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{public: 
    OpenSaveBox                     proj_, targets_ , results_;
	nana::gui::tabbar<nana::string> tabbar_;
    FindSondenPage                  findSond_;
    TmCalcPage                      tmCalc_; 
    SetupPage                       setup_; 

   ThDyNanaForm ():nana::gui::form (nana::rectangle( nana::point(200,100), nana::size(500,500) )),
                   EditableForm    (nullptr, *this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")),
                   proj_           (*this, STR("Project:") ),
                   targets_        (*this, STR("Targets:") ),
                   results_        (*this, STR("Results:") ),
                   tabbar_         (*this),
                   findSond_       (*this),
                   tmCalc_         (*this),
                   setup_          (*this)
   {
        add_page( findSond_ );
        add_page( tmCalc_   );
        add_page( setup_    );

        tabbar_.active (0);

        InitMyLayout();
        AddMenuProgram();
        SelectClickableWidget( _menuBar);
   }


    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2               \n\t"
	                 "       <weight=25>               \n\t"
	                 "       <Project  weight=23>       \n\t "
	                 "       <PagesTag weight=23 >          \n\t "
	                 "       <Pages      min=200 >          \n\t "
	                 "       <Targets  weight=23>       \n\t "
	                 "       <weight=23>       \n\t "
	                 "       <Results  weight=23>       \n\t "

	                 "       <weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("Project" )<< proj_   ;
	    _place.field("Targets" )<< targets_   ;
	    _place.field("Results" )<< results_   ;
	    _place.field("PagesTag")<< tabbar_  ;
    }
    void add_page(widget& w)
    {        tabbar_.push_back (                    w.caption());
             tabbar_.relate    (tabbar_.length()-1, w          );
	    _place.field("Pages"   ).fasten( w)  ;
    }
};

   FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : p_           (tdForm), 
          CompoWidget  (tdForm, STR("Find Sonden"), STR("FindSonden.lay.txt")),
          nTsec_       (*this, STR("Non template seq:"),STR("FindSonden-OSB.NonTarg.lay.txt") )
    {
        nTsec_._DefLayout=("vertical   <weight=1>    "
                 "  <weight=20 <weight=3><   vertical weight=100 <><label weight=15><>     ><weight=1>     "
		         "               <proj_buttons weight=74 gap=1>     "
		         "			   <cbFL >       "
		         "			   <pick weight=30>  "
		         "			   <weight=3> 	>            <weight=2>    ");
        InitMyLayout();
        SelectClickableWidget( nTsec_);

    }
   TmCalcPage::TmCalcPage        (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Tm Calc"), STR("Tm Calc.lay.txt")),
          sec_(*this),  sec2align_(*this),  txtBx_ResultSec(*this),  txtBx_ResultSec2Align(*this), 
          chkBx_Tm_save_asPCR(*this, STR("save")),   chkBx_align     (*this, STR("align")),
          chkBx_copy_rev     (*this, STR("rev")),    chkBx_copy_compl(*this, STR("cpl")),
          copy_f_s_2         (*this, STR("copy")),   copy_s          (*this, STR("c")),
          copy_s_a           (*this, STR("c")),      error_          (*this, STR("no error")),
          run_               (*this, STR("Tm !")),
           Tm_min_Up(*this), Tm_Up(*this), Tm_max_Up(*this) ,
           Tm_min_Dw(*this), Tm_Dw(*this), Tm_max_Dw(*this) ,
           Tm_min_In(*this), Tm_In(*this), Tm_max_In(*this) ,
           G_min_Up(*this),   G_Up(*this),  G_max_Up(*this) ,
           G_min_Dw(*this),   G_Dw(*this),  G_max_Dw(*this) ,
           G_min_In(*this),   G_In(*this),  G_max_In(*this)  
    {
                         sec_.multi_lines(false);
                   sec2align_.multi_lines(false);
              txtBx_ResultSec.multi_lines(false);
        txtBx_ResultSec2Align.multi_lines(false);

                         sec_.editable(true);
                   sec2align_.editable(true);
              txtBx_ResultSec.editable(false);
        txtBx_ResultSec2Align.editable(false);


                         sec_.tip_string (STR("forward primer"));
                   sec2align_.tip_string (STR("reverse primer"));
              txtBx_ResultSec.tip_string (STR("alingned forward primer"));
        txtBx_ResultSec2Align.tip_string (STR("alingned reverse primer"));


        run_.make_event <nana::gui::events::click>([&](){Run();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
   SetupPage::SetupPage          (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Setup"), STR("Setup.lay.txt")),
          set_def_proj_(*this,STR("Set as Def. project") )
    {
        //set_def_proj_.caption( );
        InitMyLayout();
        SelectClickableWidget( set_def_proj_);

    }

int main()
{
	ThDyNanaForm tdForm;
	tdForm.show();
    try {
	        nana::gui::exec();

        } catch (std::exception e)
        {
            std::cout<< std::endl<< e.what();
            throw ;
        } catch (...)
        {
            std::cout<< std::endl<< "exeption !!";
            throw ;
        }
	return 0;
}

