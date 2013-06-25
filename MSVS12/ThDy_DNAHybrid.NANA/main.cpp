#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging
#include<filesystem>

#include "thdy_programs\init_thdy_prog_param.h"

class ThDyNanaForm ;

class SetupPage : public CompoWidget
{public: 
    SetupPage (ThDyNanaForm& tdForm);
    ThDyProject &p_;
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


class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{public: 
    OpenSaveBox                     proj_;
	nana::gui::tabbar<nana::string> tabbar_;
    FindSondenPage                  findSond_;
    SetupPage                       setup_; 

   ThDyNanaForm ():nana::gui::form (nana::rectangle( nana::point(200,100), nana::size(500,500) )),
                   EditableForm    (*this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")),
                   proj_           (*this, STR("Project:") ),
                   tabbar_         (*this),
                   findSond_       (*this),
                   setup_          (*this)
   {
        add_page( findSond_ );
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
	                 "       <weight=23>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("Project" )<< proj_   ;
	    _place.field("PagesTag")<< tabbar_  ;
	    _place.field("Pages"   ).fasten( findSond_).fasten(setup_)  ;
    }
    void add_page(widget& w)
    {        tabbar_.push_back (                    w.caption());
             tabbar_.relate    (tabbar_.length()-1, w          );
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
   SetupPage::SetupPage          (ThDyNanaForm& tdForm)
        : p_           (tdForm), 
          CompoWidget  (tdForm, STR("Setup"), STR("Setup.lay.txt")),
          set_def_proj_(*this)
    {
        set_def_proj_.caption( STR("Set as Def. project") );
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

