#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>

#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging


class ThDy_Form : public nana::gui::form, public EditableForm 
{public:
	ThDy_Form ():	nana::gui::form  ( rectangle(   point(100,100),  nana::size(600,700)   ), 
								gui::appear::decorate<gui::appear::minimize, gui::appear::maximize>())
	{	

	}

 private:
     	ThDyProject		  _Pr;

	gui::button _butOpenProject;
	gui::combox _cbProject;

	
}	;

