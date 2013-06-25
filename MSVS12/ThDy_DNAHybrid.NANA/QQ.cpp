//#define NANA_UNICODE
#include<nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

//#define STR(s) s
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

     nana::gui::button btn(form, 20, 20, 80, 35);
     btn.caption(STR("Quit"));


     nana::gui::button btnQ(form, 120, 20, 100, 35);
	 btnQ.caption(STR("Fast Quit"));  

     nana::gui::button btnC(form, 120, 80, 100, 35);
	 btnC.caption(STR("Other caption"));    

	 nana::gui::button btnL(form, 20, 80, 100, 35);
	 btnL.caption(STR("Long caption"));    

	 btn.make_event<nana::gui::events::click>			(nana::gui::API::exit);
     btnQ.make_event<nana::gui::events::mouse_enter>	(nana::gui::API::exit);
	 btnC.make_event<nana::gui::events::click>			(cp, &MyClass::caption);
	 btnL.make_event<nana::gui::events::click>			(MyClass(form)    );
     form.show();
     nana::gui::exec();
 } 
