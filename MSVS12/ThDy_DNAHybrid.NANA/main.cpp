#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include "ThDy_programs\init_prog_param.h" 
//#include "ThDy_Form.h"
//#include <nana/gui/wvl.hpp>
//#include <nana/gui/widgets/button.hpp>
#include<filesystem>
#include <nana/gui/widgets/combox.hpp>
using namespace nana;

class ThDy_Form : public nana::gui::form
{public:
	ThDy_Form ():	gui::form  ( rectangle(   point(100,100),  nana::size(600,700)   )/*, 
								gui::appear::decorate<gui::appear::minimize, gui::appear::maximize>()*/)
	{	rectangle r(   point(5,5),  nana::size(100,20) ), bOpP(r);
		_butOpenProject.create (*this, r);
		_butOpenProject.caption(STR("Open Project"));

		//r = rectangle( point (r.x + r.width + 2,  r.y ) ,  nana::size( 400, r.height)   );
		_cbProject.create (*this, r=r.left_at(20).w(200));
		_cbProject.caption(STR("Open Project"));
		rectangle cbP(r);
		
		caption(STR("ThDy"));
	}

 private:
	gui::button _butOpenProject;
	gui::combox _cbProject;
	ThDyProject		_Pr;
	
}	;

#include "C:\Prog\ThDySec\MSVS12\FileChosserWin32\file_chooser.h"


int main()
{

	typedef CFileFiltre<std::string> ff ;
	ff txt_filtres= ff( "Plain txt") << "*.txt" << "*.ascii",   all_filtre=ff ("All files")<<"*.*";
	// CFileFiltre<std::string> txt_filtres ("Plain txt",{"*.txt","*.ascii"}), all_filtre=ff ("All files",{"*.*"}); 

	typedef CFileChooser<std::string> fch ;  // no vector initialize list in VC++ yet
	fch Txt_open (fch::FileChosserType::open, "Chosse your txt file:", "../dir/default.txt");
	    Txt_open.addFiltre(txt_filtres).addFiltre(all_filtre);

	//CFileChooser<std::string> Txt_open (CFileChooser<std::string>::open, "Chosse your txt file:", "../dir/default.txt",
	//											{    "Plain txt",{"*.txt","*.ascii"},   "All files",{*.*}     }); 

	std::ofstream ofs_txt ;
	try 	
	//{ ofs_txt.open( fch (fch::FileChosserType::open, "Chosse your txt file:", "../dir/default.txt",
	//							{    "Plain txt",{"*.txt","*.ascii"},   "All files",{*.*}     }     ).AskOne());}	
	{ ofs_txt.open( Txt_open.AskOne());}
	catch   (...)
	{}



	using namespace nana;
	ThDy_Form tdf;
	
	tdf.show();

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
