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

#include "nanaBind.hpp"
using namespace ParamGUIBind;

class ThDyNanaForm ;

class FindSondenPage : public CompoWidget
{public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    ThDyProject &_Pr;
    FilePickBox nTsec_;
    BindGroup   _findSond;
    nana::gui::NumUnitUpDown _Gmin, _Gmax, _Tmmin, _Tmmax, _Lengthmin, _Lengthmax, 
                             _MaxG, _MinTm, _MinG, _MaxTm, _MinSelfG, _MaxSelfTm, 	
                             numUpDw_MinTargCov ;	
    nana::gui::button        _design, _compare;
    void SetDefLayout   () override
    {
      _DefLayout=  
        "vertical      gap=2             \n\t"
	    "    <weight=10     >       \n\t "
        "    <weight=195 gap=8 <weight=320 vertical <weight=100 <weight=320 Sonde  grid[3,4]>>     \n\t "
        "                                           <TargCov                                 >     \n\t "
        "                                           <weight=50 < <><weight=100 vertical gap=2 Run><>        >  >   \n\t "
        "                      >   <><weight=230 gap=1 vertical  options>    >   \n\t "
	    "    <weight=23   NonTargSeq >       \n\t "
        ;

       nTsec_.ResetLayout(100);
 
         _Gmin.ResetLayout     (60,45,55 );   _Gmax.ResetLayout     (1,40,50 );
        _Tmmin.ResetLayout     (60,45,55 );  _Tmmax.ResetLayout     (1,40,50 );
        _Lengthmin.ResetLayout (60,45,55 );  _Lengthmax.ResetLayout (1,40,50 );
 
        _MaxG.ResetLayout     (110,45,50 );   
        _MinTm.ResetLayout    (110,45,50 );   
        _MinG.ResetLayout     (110,45,50 );   

        _MaxTm.ResetLayout     (110,45,50 );   
        _MinSelfG.ResetLayout  (110,45,50);   
        _MaxSelfTm.ResetLayout (110,45,50 );  

        numUpDw_MinTargCov.ResetLayout (120,45,20 );  


    }
    void AsignWidgetToFields() override
    {
	     /// Use room (wd,w,h) in combination with a <Table grid[W,H]>
        _place.field("NonTargSeq" )<<nTsec_;
	    _place.field("Sonde" )     << "Sonde"     << "Min."    << "   Max."   
                                   <<  _place.room(_Gmin ,2,1) << _Gmax
                                   <<  _place.room(_Tmmin,2,1) << _Tmmax
                                   <<  _place.room(_Lengthmin,2,1) << _Lengthmax  ;
        _place.field("TargCov" )   << numUpDw_MinTargCov       	;
        _place.field("Run"     )   << _design	<< _compare	;
                 
	    _place.field("options" )   << "Sonde-target"    <<  _MaxG     << _MinTm
                                   << "Sonde-non-target"<<  _MinG     << _MaxTm
                                   << "Sonde-self"      <<  _MinSelfG << _MaxSelfTm  
                                    ;    

    }

    }
};

class TmCalcPage : public CompoWidget
{public: 
    ThDyProject        &_Pr;
    nana::gui::textbox  sec_, sec2align_, txtBx_ResultSec, txtBx_ResultSec2Align;
    nana::gui::checkbox chkBx_Tm_save_asPCR, chkBx_align, chkBx_copy_rev, chkBx_copy_compl;
    nana::gui::button   copy_f_s_2, copy_s, copy_s_a, run_;
    nana::gui::label    error_;
    BindGroup              _TmCalc;
    nana::gui::NumberBox    Tm_min_Up,  Tm_Up, Tm_max_Up ,
                            Tm_min_Dw,  Tm_Dw, Tm_max_Dw,
                            Tm_min_In,  Tm_In, Tm_max_In,
                            G_min_Up,   G_Up,  G_max_Up ,
                            G_min_Dw,   G_Dw,  G_max_Dw ,
                            G_min_In,   G_In,  G_max_In ;

    TmCalcPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2  min=150           \n\t"
            "       < weight=50  <vertical min=100 gap=2 InputSec>   < weight=50 gap=1 CopyBut grid[2,2]>  >       \n\t "
            "       < weight=25 <weight=20><error min=50> <rev_compl weight=80>>         \n\t  "
            "       < weight=80 gap=2  <vertical weight=80 gap=2 Left>   < Table min=280 grid[7,4]>  >       \n\t "
            "       < vertical weight=50  ResAlign>    "
	                 //"       <weight=1>                \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("rev_compl")<< chkBx_copy_rev << chkBx_copy_compl ;
	    _place.field("InputSec" )<< sec_ << sec2align_ ;
	    _place.field("error"    )<< error_ ;
	    _place.field("Left"     )<< run_  << chkBx_align;
	    _place.field("CopyBut"  )<< nana::gui::vplace::room (copy_f_s_2, 1, 2)<< copy_s << copy_s_a ;
	    _place.field("Table"    )<< ""          << "   min-" << "Tm(°C)"   << "-max" << "   min-"  << "G(kJ)"   << "-max   "  ;
	    _place.field("Table"    )<< "Up"        << Tm_min_Up << Tm_Up      << Tm_max_Up<<G_min_Up  <<  G_Up     <<  G_max_Up  ;
	    _place.field("Table"    )<< "Down"      << Tm_min_Dw << Tm_Dw      << Tm_max_Dw<<G_min_Dw  <<  G_Dw     <<  G_max_Dw  ;
	    _place.field("Table"    )<< "Interact"  << Tm_min_In << Tm_In      << Tm_max_In<<G_min_In  <<  G_In     <<  G_max_In  ;


	    _place.field("ResAlign" )<< txtBx_ResultSec << txtBx_ResultSec2Align ;
    }
    void Run()
    {
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

class SetupPage : public CompoWidget
{public: 
    ThDyProject        &_Pr;
    BindGroup          _setup;
    nana::gui::button  set_def_proj_;
    FilePickBox        _NNParamFile;

    SetupPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
            "  <wieght=300 <vertical min=50 max=200 buttons> <> <weight=80 checks>>   \n\t"
	        "  <NN_param weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("buttons"  )<<set_def_proj_;
	    _place.field("NN_param" )<<_NNParamFile;
	    _place.field("checks"   )<<"save result";
    }
};

class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{public: 
    OpenSaveBox                     proj_;
    FilePickBox                     targets_ , results_, PCRfiltre_;
	nana::gui::tabbar<nana::string> tabbar_;
    FindSondenPage                  findSond_;
    TmCalcPage                      tmCalc_; 
    SetupPage                       setup_;
    BindGroup                       _commPP;
    nana::gui::combox               comBoxSalMeth, comBoxTAMeth;
    nana::gui::NumUnitUpDown        numUpDwMaxTgId, numUpDowTgConc, numUpDowSalConc , numUpDw_TgBeg, numUpDw_TgEnd,	numUpDw_MinLen,
                                    numUpDowTa,  numUpDowSdConc  ;

   ThDyNanaForm ():nana::gui::form (nana::rectangle( nana::point(200,100), nana::size(700,550) )),
                   EditableForm    (nullptr, *this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")),
                   proj_           (*this, STR("Project:") ),
                   targets_        (*this, STR("Targets:") ),
                   results_        (*this, STR("Results:") ),
                   numUpDwMaxTgId  (*this, STR("Max. ident.:"), 99, 50 , 100,"%"),
                   numUpDowSdConc  (*this, STR("Sonde Conctr:"), 50, 0.1 , 1000,"µM"),
                   numUpDowSalConc (*this, STR("Salt Conc [Cations]:"), 50, 0.1 , 10000000,"µM"),
                   numUpDowTgConc  (*this, STR("Target Conctr:"), 50, 0.1 , 1000,"µM"),
                   numUpDowTa      (*this, STR("Temp. Anneling:"), 55, 40 , 75,"°C"),
                   tabbar_         (*this),
                   findSond_       (*this),
                   tmCalc_         (*this),
                   setup_          (*this)
   {
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //background (0xEEEEEE);
        //foreground(1);

       
        add_page( findSond_ );
        add_page( tmCalc_   );
        add_page( setup_    );

        tabbar_.active (0);

        proj_.FileName(nana::charset ( _ProjetFileName.Get () ));

                //targets_.make_event <nana::gui::events::focus>([&](const nana::gui::eventinfo& ei)
                //{  
                //    std::cerr<< "\nBefore " << (ei.focus.getting ? "geting ":"lossing ") << "Focus: (set in ThDyNanaForm Constr), FilePickBox: ";
                //    std::wcerr<< targets_._Titel << std::endl;
                //    //if (!ei.focus.getting) 
                //    //    validate_edit( );
                //}); 
                //targets_._fileName.make_event <nana::gui::events::focus>([&](const nana::gui::eventinfo& ei)
                //{  
                //    std::cerr<< "\nBefore " << (ei.focus.getting ? "geting ":"lossing ") << "Focus: (set in ThDyNanaForm Constr), FilePickBox:_fileName ";
                //    std::wcerr<< targets_._Titel << std::endl;
                //    //if (!ei.focus.getting) 
                //    //    validate_edit( );
                //}); 

        _commPP  << link( _cp._InputTargetFile , targets_  )
                 << link( _cp._OutputFile      , results_  )
                 << link( _cp._PCRfiltrPrFile  ,       PCRfiltre_)
                 << link( _cp.MaxTgId    , numUpDwMaxTgId  )
                 << link( _cp.SecLim , numUpDw_TgBeg,numUpDw_TgEnd  )
                 << link( _cp.MinSecLen  ,       numUpDw_MinLen  )
                 << link( _cp.ConcSd	 ,       numUpDowSdConc  )
                 << link( _cp.ConcSalt	      , numUpDowSalConc  )
                 << link( _cp.ConcTg	 , numUpDowTgConc  )
                 << link( _cp.Ta	         ,       numUpDowTa  )        
                 << link( _cp.SaltCorr	  ,       comBoxSalMeth  )        
                 << link( _cp.TAMeth       ,       comBoxTAMeth  )        
            ;


        InitMyLayout();

        AddMenuProgram();
        SelectClickableWidget( _menuBar);

        proj_.add_filter(STR("ThDy project"),STR("*.ThDy.txt"));
        proj_.Open.make_event	<nana::gui::events::click> ([&](){ OpenProj() ;} );
		proj_.Save.make_event	<nana::gui::events::click> ([&](){ SaveProj() ;} );


   }
    //~ThDyNanaForm();

    void  OpenProj()
	{	 
      if(  proj_.Canceled () )  return;
      ProjetFile (std::string(nana::charset ( proj_.FileName())).c_str());  /// TODO: revise ortografia
      load (); 
      //tmCalc_._TmCalc.UpDateForm();
	}
    void  SaveProj()
	{	 
      if(  proj_.Canceled () )  return;
      ProjetFile (std::string(nana::charset ( proj_.FileName())).c_str());  /// TODO: revise ortografia
      save (); 
	}
    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2               \n\t"
	                 "       <weight=25>               \n\t"
	                 "       <Project  weight=23>       \n\t "
	                 "       <PagesTag weight=23 >          \n\t "
	                 "       <Pages      min=220 >          \n\t "
	                 "       <Targets  weight=23>       \n\t "
	                 "       <weight=23>       \n\t "
	                 "       <Results  weight=23>       \n\t "
                     "       <ComPar grid[2,4]  min=50 gap=2>       \n\t "

	                 "       <weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("Project" )<< proj_   ;
	    _place.field("PagesTag")<< tabbar_  ;
	    _place.field("Targets" )<< targets_   ;
	    _place.field("Targets" )<< numUpDwMaxTgId   ;
	    _place.field("Results" )<< results_   ;
	    _place.field("ComPar"  )<< numUpDowSdConc  << numUpDowSalConc 
                                << numUpDowTgConc  << "Salt Correct. Method:"	
                                << numUpDowTa	   << "ThDy Align. Method"
                                << ""              << "  INNT - FLI \n ArielVina.Rodriguez@fli.bund.de"

                                ;

    }                                        
    void add_page(widget& w)
    {        tabbar_.push_back (                    w.caption());
             tabbar_.relate    (tabbar_.length()-1, w          );
	         _place.field("Pages"   ).fasten( w)  ;
    }         
};

   FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, STR("Find Sonden"), STR("FindSonden.lay.txt")),
          nTsec_     (*this, STR("Non template seq:"),STR("FindSonden-OSB.NonTarg.lay.txt") ),
          _Gmin   (*this, STR("G :" ), -5, -10 , 10,"kcal/mol"),     _Gmax  (*this, STR(""), -1, -10, 10, "kcal/mol" ),
          _Tmmin  (*this, STR("Tm :"), 57, 40 , 60,"°C"),           _Tmmax  (*this, STR(""), 63, 45, 75, "°C"  ),
      _Lengthmin  (*this, STR("Length:"), 20, 15 , 35,"nt"),    _Lengthmax  (*this, STR(""), 35, 15, 40, "nt"  ),
          _MinG   (*this, STR("Min G" ), 15, -10 , 30,"kcal/mol"),   _MaxG  (*this, STR("Max G"), 10, -10, 30, "kcal/mol" ),
          _MinTm  (*this, STR("Tm :"), 30, 10 , 60,"°C"),          _MaxTm  (*this, STR("Max Tm"), 10, -10, 75, "°C"  ),
         _MinSelfG(*this, STR("Min G" ), 10, -10 , 30,"kcal/mol"), _MaxSelfTm(*this, STR("Max Tm"), 10, -10, 75, "°C"  )
    {
        nTsec_._DefLayout=("vertical   <weight=1>    "
                 "  <weight=20 <weight=3><   vertical weight=100 <><label weight=15><>     ><weight=1>     "
		         "			   <cbFL >       "
		         "			   <pick weight=30>  "
		         "			   <weight=3> 	>            <weight=2>    ");



                 _MaxG.make_event <nana::gui::events::focus>([&](const nana::gui::eventinfo& ei)
                {  
                    std::cerr<< "\nBefore " << (ei.focus.getting ? "geting ":"lossing ") << "Focus: (set in FindSond Constr), NumerUnitUpDown: ";
                    std::wcerr<< _Titel << std::endl;
                    //if (!ei.focus.getting) 
                    //    validate_edit( );
                }); 


        _findSond << link (   _Pr._SdDes.G_sig ,        _MaxG     )    
                  << link (   _Pr._SdDes.Tm_sig ,       _MinTm    )
                  << link (   _Pr._SdDes.MinSd_nTgG,    _MinG     )
                  << link (   _Pr._SdDes.MaxSd_nTgTm,   _MaxTm    )
                  << link (   _Pr._SdDes.MinSelfG,      _MinSelfG )
                  << link (   _Pr._SdDes.MaxSelfTm,     _MaxSelfTm)
                  << link (   _Pr._SdDes.sL.G,        _Gmin,_Gmax )
                  << link (   _Pr._SdDes.sL.T,      _Tmmin,_Tmmax )
                  << link (  _Pr._SdDes.sL.L,_Lengthmin,_Lengthmax)


                  //<< link (  _Pr._cp.  ,               nTsec_)
            ;

        InitMyLayout();
        SelectClickableWidget( nTsec_);
        SelectClickableWidget( *this);

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

        _TmCalc << link (   _Pr._TmCal.align      ,    chkBx_align    )    
                << link (   _Pr._TmCal._Sec       ,    sec_           )
                << link (   _Pr._TmCal._Sec2Align ,    sec2align_     )
                ;

        run_      .make_event <nana::gui::events::click>([&](){Run ();});
        copy_f_s_2.make_event <nana::gui::events::click>([&](){Copy();});      ;   //(*this, STR("copy")),   
        copy_s    .make_event <nana::gui::events::click>([&](){Self();});      ;  //  (*this, STR("c")),
        copy_s_a  .make_event <nana::gui::events::click>([&](){Rev ();});      ;  


        InitMyLayout();
        SelectClickableWidget( *this);
        SelectClickableWidget( error_);
    }
   SetupPage::SetupPage          (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Setup"), STR("Setup.lay.txt")),
          set_def_proj_(*this,STR("Set as Def. project") ),
          _NNParamFile (*this, STR("NN param:") )
    {
        _setup<< link(   _Pr._cp._InputNNFile , _NNParamFile)
            ;

        InitMyLayout();
        SelectClickableWidget( set_def_proj_);
        SelectClickableWidget( *this);
    }

int main()
{
	IParBind::SetDef(PriorizeDefault::Parametr );

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

