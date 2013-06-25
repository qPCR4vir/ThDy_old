#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging


	//ThDyProject		  _Pr;
#include<filesystem>
#include "thdy_programs\init_thdy_prog_param.h"
class ThDyNanaForm ;
class FindSonden : public CompoWidget
{public: 
    FindSonden(ThDyNanaForm& tdForm): p_(tdForm), CompoWidget (tdForm, STR("Find Sonden"),STR("FindSonden.lay.txt")),
    proj       (*this, STR("Project:") )
    {}
    ThDyProject &p_;
    OpenSaveBox proj;
};


class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{public: 
   ThDyNanaForm ():nana::gui::form (nana::rectangle( nana::point(200,100), nana::size(500,800) )),
                   EditableForm    (*this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")){}
	nana::gui::tabbar<nana::string> tabbar_;

        void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	                 "       <weight=25>                \n\t"
	                 "       <Project weight=23>       \n\t "
	                 "       <gap=2 min=30 <b1> <b2 min=200> <b3 weight=200> >   \n\t "
	                 "       <<b4 min=200 >min=30 <weight=6>>           \n\t "
	                 "       <  weight=22 <label weight=60 ><Num> <vertical weight=50 <UpDown> >>     \n\t   "
	                 "       <  weight=23 <Ta  weight= 200><> <num  weight=280 ><>  <Unit weight=50 gap=2 >>         \n\t  "
	                 "       <Project2 weight=23>       "
                  ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("Project" )<<osb;
	    _place.field("Project2")<<osb2;
	    _place.field("b1")<<but1;
	    _place.field("b2")<<but2;
	    _place.field("b3")<<but3;
	    _place.field("b4")<<but4;
	    _place.field("num")<<num1 << num2;
	    _place.field("Num"    ) << _num  ;
	    _place.field("Unit"   ) << UPicker/*._cb*/ ;
	    _place.field("UpDown" ) << _up << _down  ;
	    _place.field("label"  ) << _label;
	    _place.field("Ta"  ) << Ta;
    }

};
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


//window_caption(window wd, const nana::string& title)
