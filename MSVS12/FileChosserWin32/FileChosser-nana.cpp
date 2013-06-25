#include "C:\Prog\ThDySec\MSVS12\FileChosserWin32\file_chooser.h"


using nana::rectangle;
using nana::point;
	nana::gui::
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

	nana::gui::date_chooser s();