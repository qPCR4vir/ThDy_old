//#define NANA_UNICODE
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

//using namespace nana;

class MyClass
{
public:
nana::gui::form&	wd;
int					i;
 void				caption (){ nana::string capt(STR("Other caption")); wchar_t num[50];
								capt += STR(".") ; capt += nana::string(_itow(i++,num,10)) ;
								nana::gui::API::window_caption( wd, capt );}
 void operator	()()			{nana::gui::API::window_caption( wd, wd.caption() + STR("-") );					}

 MyClass		(nana::gui::form& W) :	wd(W), i(0) {};

};


int main()
 {
     nana::gui::form form;
MyClass cp(form) ;

     form.caption(STR("Hello World II"));
	 nana::rectangle r(20, 20, 80, 35), rleft;

     nana::gui::button btn(form, r);
     btn.caption(STR("Quit"));


	 nana::gui::button btnQ(form, rleft=r.left_at(80).w(100));
	 btnQ.caption(STR("Fast Quit"));  

	 nana::gui::button btnC(form, rleft.down_at(55) );
	 btnC.caption(STR("Other caption"));    

	 nana::gui::button btnL(form, r.down_at(25).w(100) );
	 btnL.caption(STR("Long caption"));    

	 btn.make_event<nana::gui::events::click>			(nana::gui::API::exit);
     btnQ.make_event<nana::gui::events::mouse_enter>	(nana::gui::API::exit);
	 btnC.make_event<nana::gui::events::click>			(cp, &MyClass::caption);
	 btnL.make_event<nana::gui::events::click>			(MyClass(form)    );
     form.show();
     nana::gui::exec();
 } 
