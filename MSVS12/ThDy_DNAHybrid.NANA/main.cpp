#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
//#include "ThDy_Form.h"
//#include <nana/gui/wvl.hpp>
//#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>
using namespace nana;

class ThDy_Form : public nana::gui::form
{public:
	ThDy_Form ():	gui::form  ( rectangle(   point(100,100),  nana::size(600,700)   )/*, 
								gui::appear::decorate<gui::appear::minimize, gui::appear::maximize>()*/)
	{	rectangle r(   point(5,5),  nana::size(100,20) ), bOpP(r);
		_butOpenProject.create (*this, r.x,r.y,r.width,r.height);
		_butOpenProject.caption(STR("Open Project"));

		r = rectangle( point (r.x + r.width + 2,  r.y ) ,  nana::size( 400, r.height)   );
		_cbProject.create (*this, r.x,r.y,r.width,r.height);
		_cbProject.caption(STR("Open Project"));
		rectangle cbP(r);

		caption(STR("ThDy"));
	}

 private:
	gui::button _butOpenProject;
	gui::combox _cbProject;

	
}	;

#include "C:\Prog\ThDySec\MSVS12\FileChosserWin32\file_chooser_win32.h"


int main()
{
	using namespace nana;
	ThDy_Form tdf;
	
	tdf.show();

	typedef CFileChooser<char *> fch ;
	fch fc(fch::FileChosserType::open , "open");
	fc.Ask();

	gui::exec();

	return 0;
}

////#define NANA_UNICODE
//#include<nana/gui/wvl.hpp>
//#include <nana/gui/widgets/button.hpp>
//
////#define STR(s) s
//class MyClass
//{
//public:
//nana::gui::form&	wd;
//int					i;
// void				caption (){ nana::string capt(STR("Other caption")); wchar_t num[50];
//								capt += STR(".") ; capt += nana::string(_itow(i++,num,10)) ;
//								nana::gui::API::window_caption( wd, capt );}
// void operator	()()			{nana::gui::API::window_caption( wd, wd.caption() + STR("-") );					}
//
// MyClass		(nana::gui::form& W) :	wd(W), i(0) {};
//
//};
//
//
//int main()
// {
//     nana::gui::form form;
//MyClass cp(form) ;
//
//     form.caption(STR("Hello World II"));
//
//     nana::gui::button btn(form, 20, 20, 80, 35);
//     btn.caption(STR("Quit"));
//
//
//     nana::gui::button btnQ(form, 120, 20, 100, 35);
//	 btnQ.caption(STR("Fast Quit"));  
//
//     nana::gui::button btnC(form, 120, 80, 100, 35);
//	 btnC.caption(STR("Other caption"));    
//
//	 nana::gui::button btnL(form, 20, 80, 100, 35);
//	 btnL.caption(STR("Long caption"));    
//
//	 btn.make_event<nana::gui::events::click>			(nana::gui::API::exit);
//     btnQ.make_event<nana::gui::events::mouse_enter>	(nana::gui::API::exit);
//	 btnC.make_event<nana::gui::events::click>			(cp, &MyClass::caption);
//	 btnL.make_event<nana::gui::events::click>			(MyClass(form)    );
//     form.show();
//     nana::gui::exec();
// } 
//
////window_caption(window wd, const nana::string& title)
