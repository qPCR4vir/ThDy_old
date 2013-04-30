#include <fstream> 
#include <iostream> 

#include <nana/charset.hpp>
#include <nana/gui/wvl.hpp>
#include <nana/gui/filebox.hpp>        // include in project: src/nana/gui/filebox.cpp
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/menubar.hpp>

const char* readLayot(const nana::string& FileName, std::string& layot)
{
		std::ifstream loy(FileName);
		std::string temp;

		while (std::getline(loy,temp)) layot+=temp;
		std::cout<< layot;
		return layot.c_str();
}	

class EditLayot_Form : public nana::gui::form
{public:
	EditLayot_Form (	nana::gui::place     &pl, 
						const std::string    &lay="", 
						const nana::string   &DefLayotFileName=STR(""),
						nana::gui::form      *fm=nullptr  )
		: nana::gui::form  ( nana::rectangle(   nana::point(100,100),  nana::size(600,400) )),
					_OriPlace		(pl),
					_DefLayFile		(DefLayotFileName),
				    _butOpenProject	(*this),      
					_butSaveProject	(*this),
					_pickProjectName(*this),
					_ReCollcate		(*this),
					_textBox		(*this),
					_ReLayot		(*this),
					_cbProject		(*this),
					_lProject		(*this),
				    _place			(*this),
					_save			(true),
					_menu			(*this),
					_menuFile		(_menu.push_back(STR("&File"))),
					_menuProgram	(_menu.push_back(STR("&Programm"))),
					_Titel          (STR("Editing layot of: "))
	{	
		if (fm) _Titel +=fm->caption(); 
		InitCaptions();
		InitMyLayot();

		_cbProject.editable(true);
		_textBox.editable(true);

		MakeResponsive();
		if(DefLayotFileName!=STR(""))
		{	_cbProject.push_back	(DefLayotFileName);
		    _cbProject.option(0);
		}
		if (lay!="") 
			_textBox.append(nana::string(nana::charset(lay)),false );

	}
 private:
	nana::string		_Titel, _DefLayFile;
	nana::gui::place	_place, &_OriPlace;
	nana::gui::button	_butOpenProject, _butSaveProject, _pickProjectName, _ReCollcate, _ReLayot;
	nana::gui::combox	_cbProject;
	nana::gui::label	_lProject;
	nana::gui::textbox	_textBox;
	nana::gui::menubar	_menu;
	nana::gui::menu	    &_menuFile,&_menuProgram/*,&_menuLayot*/;
	std::unique_ptr <EditLayot_Form> _myEdLayForm;
	bool				_save;

	void InitCaptions()
	{
		 caption				(_Titel);
		_lProject.caption		(STR("Project:"	));
		_butOpenProject.caption	(STR("Open"		));
		_butSaveProject.caption	(STR("Save"		));
		_pickProjectName.caption(STR("..."		));
		_ReCollcate.caption		(STR("@"		));
		_ReLayot.caption		(STR("lay"		));
		_textBox.tip_string		(STR("type or load a layot to be applied to the calling window..."		));
	}
    void MakeResponsive()
	{
		_menuFile.append  (STR("&Open..."),[&](nana::gui::menu::item_proxy& ip){OpenFile();});
		_menuFile.append  (STR("&Save..."),[&](nana::gui::menu::item_proxy& ip){SaveFile();});
		_menuProgram.append (STR("&Apply Layot to calling windows"),[&](nana::gui::menu::item_proxy& ip){ReLayot();});
		_menuProgram.append (STR("&Edit this windows Layot"),[&](nana::gui::menu::item_proxy& ip){EditMyLayot();});

		_butOpenProject.make_event	<nana::gui::events::click> (*this , &EditLayot_Form::OpenFile		);
		_butSaveProject.make_event	<nana::gui::events::click> (*this , &EditLayot_Form::SaveFile		);
		_ReCollcate.make_event		<nana::gui::events::click> (_place, &nana::gui::place::collocate	);
		_ReLayot.make_event			<nana::gui::events::click> (*this , &EditLayot_Form::ReLayot		);
		_cbProject.ext_event().selected = [&](nana::gui::combox&cb)
		{
			SaveFile();  // salvar si editado	;
			if(_cbProject.the_number_of_options()>0)
				_textBox.load(_cbProject.text(_cbProject.option()).c_str() );
		};

	}
	void InitMyLayot()
	{
		std::string layot;
        _place.div( readLayot(STR("layot.txt"), layot ="") );
		_place.field("proj_buttons")<< _butOpenProject<< _butSaveProject;
		_place.field("label"       )<< _lProject ;
		_place.field("proj_txt"    )<< _cbProject;
		_place.field("pick"		   )<< _pickProjectName;
		_place.field("textBox"	   )<< _textBox;
		_place.field("re"		   )<<_ReLayot<< _ReCollcate ;
		_place.collocate ();
	}
	void ReLayot()
	{   SaveFile();  // salvar si editado	;
		std::string layot;
		nana::string FileName (  _cbProject.text(_cbProject.option())  );
        _OriPlace.div( readLayot(FileName, layot ="") );  // try std::runtime_error msgbox
		_OriPlace.collocate ();
	}
	void OpenFile()
	{	  
		 nana::gui::filebox fb(0, true);
		 fb.add_filter(STR("Text File"), STR("*.txt;*.doc"));
		 fb.add_filter(STR("All Files"), STR("*.*"));
		 nana::string file;
		 if(fb())  
		 { 
			 file = fb.file();
			std::wcout<<std::endl<<STR("caption: ")<<_cbProject.caption()<<std::endl;
			std::wcout<<std::endl<<STR("Option: ")<<_cbProject.option()<<std::endl;
			//std::wcout<<std::endl<<STR("Option text: ")<<_cbProject.text(_cbProject.option())<<std::endl;
			std::wcout<<std::endl<<STR("Option Last: ")<<_cbProject.the_number_of_options()<<std::endl;
			//std::wcout<<std::endl<<STR("text Last-1: ")<<_cbProject.text(_cbProject.the_number_of_options()-1)<<std::endl;
			 _save=false;
			_cbProject.push_back(file).option(_cbProject.the_number_of_options());
			caption	(_Titel+STR(" <")+ file+STR(">"));
			_textBox.load(file.c_str() );
			 _save=true;

			std::wcout<<std::endl<<STR("OpenFIle: ")<<file<<std::endl;
			std::wcout<<std::endl<<STR("text 0 pos: ")<<_cbProject.text(0)<<std::endl;
			std::wcout<<std::endl<<STR("Option: ")<<_cbProject.option()<<std::endl;
			std::wcout<<std::endl<<STR("Option text: ")<<_cbProject.text(_cbProject.option())<<std::endl;
			std::wcout<<std::endl<<STR("Option Last: ")<<_cbProject.the_number_of_options()<<std::endl;
			std::wcout<<std::endl<<STR("text Last-1: ")<<_cbProject.text(_cbProject.the_number_of_options()-1)<<std::endl;
		 }
	}
	void SaveFile()
	{	 if(!_save) return;
	   
		 nana::gui::filebox fb(0, false);
		 fb.add_filter(STR("Text File"), STR("*.txt;*.doc"));
		 fb.add_filter(STR("All Files"), STR("*.*"));
		 nana::string file;
		 if(fb())  
		 { 
			 file = fb.file();
			 _save=false;
			_cbProject.push_back(file).option(_cbProject.the_number_of_options());
			caption	(_Titel+STR(" <")+ file+STR(">"));
			_textBox.store(file.c_str() );
			 _save=true;

			std::wcout<<std::endl<<STR("OpenFIle: ")<<file<<std::endl;
		 }
	}
	void EditMyLayot()
		{
			if (!_myEdLayForm) 
				_myEdLayForm.reset (new EditLayot_Form (_place,"", STR("layot.txt") , this ));
			_myEdLayForm->show ();
		}
}	;


int main()
{
	nana::gui::form  form;
	form.caption (STR("Configurable Window"));
	nana::gui::menubar	menub(form);
	nana::gui::button	but1(form), but2(form), but3(form), but4(form);
	but1.caption (STR("But1"));
	but2.caption (STR("But2"));
	but3.caption (STR("But3"));
	but4.caption (STR("But4"));
	nana::gui::place	place(form);
	std::string lay("vertical<weight=25><b1 <b2><b3>><b4> ");
	place.div(lay.c_str ());     // try std::runtime_error msgbox
	place.field("b1")<<but1;
	place.field("b2")<<but2;
	place.field("b3")<<but3;
	place.field("b4")<<but4;
	place.collocate ();


	std::unique_ptr <EditLayot_Form> LayForm;
	menub.push_back(STR("&Programm")).append(STR("&Edit this windows Layot"),[&](nana::gui::menu::item_proxy& ip)
	{
		if (!LayForm) 
			LayForm.reset (new EditLayot_Form (place,lay, STR("") , &form ));
		LayForm->show ();
	});
	form.show();
	nana::gui::exec();
	return 0;
}


//	ThDyProjet		  _Pr;
//	#include<filesystem>
//#include "thdy_programs\init_thdy_prog_param.h"
//#include "C:\Prog\ThDySec\MSVS12\FileChosserWin32\file_chooser.h"

//
//using nana::rectangle;
//using nana::point;
	//nana::gui::
	//typedef CFileFiltre<std::string> ff ;
	//ff txt_filtres= ff( "Plain txt") << "*.txt" << "*.ascii",   all_filtre=ff ("All files")<<"*.*";
	//// CFileFiltre<std::string> txt_filtres ("Plain txt",{"*.txt","*.ascii"}), all_filtre=ff ("All files",{"*.*"}); 

	//typedef CFileChooser<std::string> fch ;  // no vector initialize list in VC++ yet
	//fch Txt_open (fch::FileChosserType::open, "Chosse your txt file:", "../dir/default.txt");
	//    Txt_open.addFiltre(txt_filtres).addFiltre(all_filtre);

	////CFileChooser<std::string> Txt_open (CFileChooser<std::string>::open, "Chosse your txt file:", "../dir/default.txt",
	////											{    "Plain txt",{"*.txt","*.ascii"},   "All files",{*.*}     }); 

	//std::ofstream ofs_txt ;
	//try 	
	////{ ofs_txt.open( fch (fch::FileChosserType::open, "Chosse your txt file:", "../dir/default.txt",
	////							{    "Plain txt",{"*.txt","*.ascii"},   "All files",{*.*}     }     ).AskOne());}	
	//{ ofs_txt.open( Txt_open.AskOne());}
	//catch   (...)
	//{}

	//nana::gui::date_chooser s();

	//using namespace nana;

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
