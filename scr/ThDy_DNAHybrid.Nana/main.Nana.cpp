#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging
#include <filesystem>

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/tooltip.hpp>

#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/progress.hpp>

#include "thdy_programs\init_thdy_prog_param.h"
#include "matrix.h" 
#include "common_basics.h" 



#include "ThDy_DNAHybrid.Nana\main.Nana.h"
//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"

//if you want to keep the Windows subsystem you can just hint at what your entry point is, 
//because you haven't defined ___tmainCRTStartup. You can do this by adding the following 
//to Properties -> Linker -> Command line:
//
//    /ENTRY:"mainCRTStartup"
//
//This way you get rid of the console window.


int main(int argc, char *argv[]) 
{

    //std::ifstream in("in.txt");
    //std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    //std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!

    //std::ofstream out("out.txt");
    //std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    //std::string word;
    //std::cin >> word;           //input from the file in.txt
    //std::cout << word << "  ";  //output to the file out.txt

    //f(); //call function


    //std::cin.rdbuf(cinbuf);   //reset to standard input again

  nana::color::current_schema[nana::color::schema::list_header_border]=nana::color::Red;
  nana::color::current_schema[nana::color::schema::list_header_bg]=nana::color::Yellow;    // 0xF1F2F4 
  nana::color::current_schema[nana::color::schema::list_header_highlighted_bg]=nana::color::Rose;    // 0xFFFFFF 
  nana::color::current_schema[nana::color::schema::list_header_pressed_bg]=nana::color::AliceBlue;  
  nana::color::current_schema[nana::color::schema::list_header_grabed_bg]=nana::color::Ash_Gray;    // 0x8BD6F6 
  nana::color::current_schema[nana::color::schema::list_header_floated_bg]=nana::color::Aztech_Purple;	   // 0xBABBBC 

  try	
  {
    IParBind::SetDef(PriorizeDefault::Parametr );
    ThDyNanaForm tdForm(  argc,  argv);
	tdForm.show();
	nana::exec();
	return 0;
  }
    catch (std::exception& e)
        {
            std::cerr<< std::endl<< e.what();
            throw ;
        } 
    catch (...)
        {
            std::cerr<< std::endl<< "exeption !!";
            throw ;
        }

    
    //std::cout.rdbuf(coutbuf); //reset to standard output again

} 
void  SetupPage::AsignWidgetToFields ()  
    {
      _setup<< link( _Pr._cp._OutputFile      ,       _results  )
            << link( _Pr._cp._InputTargetFile ,       _targets  )
            << link( _Pr._cp._TRecurDir         ,     _chkTargRecDir)
            << link( _Pr._cp._TDirStrOnly      ,     _chkTargOnlyStruct)
            << link( _Pr._cp._NonTargetFile     ,       _nTsec  )
            << link( _Pr._cp._nTRecurDir      ,     _chk_nTgRecDir)
            << link( _Pr._cp._nTDirStrOnly      ,     _chk_nTgOnlyStruct)
            << link( _Pr._cp._PCRfiltrPrFile  ,       _PCRfiltre)
            << link( _Pr._mPCR._InputSondeFile , _PrimersFilePCR)            
            << link( _Pr._mPCR._PrRecurDir      ,     _chkPrimRecDir)
            << link( _Pr._mPCR._PrDirStrOnly      ,     _chkPrOnlyStruct)
            << link( _Pr._uArr._InputSondeFile , _Prob_uArr)            
            << link( _Pr._uArr._PrRecurDir      ,     _chkProbRecDir)
            << link( _Pr._uArr._PrDirStrOnly      ,     _chkProbOnlyStruct)
            << link( _Pr._cp._InputNNFile       , _NNParamFile  )
            << link( _Pr._cp.ConcSd	    ,       numUpDowSdConc  )
            << link( _Pr._cp.ConcSalt	     , numUpDowSalConc  )
            << link( _Pr._cp.ConcTg	    ,       numUpDowTgConc  )
            << link( _Pr._cp.Ta	            ,       numUpDowTa  )        
            << link( _Pr._cp.SaltCorr	  ,      comBoxSalMeth  )        
            << link( _Pr._cp.TAMeth       ,       comBoxTAMeth  )        
            << link( _Pr._cp.st_savTm	  ,       ckBx_savTm    )        
            << link( _Pr._cp.st_savPos	  ,       ckBx_savPos   )        
            << link( _Pr._cp.st_savG 	  ,         ckBx_savG   )        
            << link( _Pr._cp.st_savAlign  ,     ckBx_savAlign   )        
            << link( _Pr._cp.st_savProj   ,     ckBx_savProj    )        
            << link( _Pr._cp.st_savG_Plasm , ckBx_savG_Plasm    )        
            << link( _Pr._cp.st_savTm_Plasm , ckBx_savTm_Plasm  )     
            << link( _Pr._cp.st_savLog 	  ,       ckBx_savLog   )        
            << link( _Pr._cp.st_Exp_sond  ,ckBx_savExportSond   )        
            << link( _Pr._cp.st_ExpTarg	 ,ckBx_savExportTarg    )        
            << link( _Pr._cp.saveNNPar     , ckBx_savNNParam    )        
            << link( _Pr._cp.loadNNPar    , ckBx_loadNNParam    )        
          ;
            
        _place.field("Project"  )    <<  _proj        ;
	    _place.field("Results" )     <<  _results   ;
        _place.field("_targets" )    <<  _targets ;
        _place.field("TargOpt"  )    << _chkTargRecDir  << _chkTargOnlyStruct ;
        _place.field("_nTsec"   )    <<  _nTsec   ;
        _place.field("nTargOpt" )    << _chk_nTgRecDir <<  _chk_nTgOnlyStruct          ;
        _place.field("_PCRfiltre")    <<  _PCRfiltre   ;
        _place.field("_PrimersFilePCR" )    <<  _PrimersFilePCR ;
        _place.field("_PrimersFilePCROpt"  )    << _chkPrimRecDir  << _chkPrOnlyStruct ;
        _place.field("_Prob_uArr" )    <<  _Prob_uArr ;
        _place.field("_Prob_uArrOpt"  )    << _chkProbRecDir  << _chkProbOnlyStruct ;
	    _place.field("NN_param" )    << _NNParamFile  ;
	    _place.field("buttons"  )    <<  _set_def_proj << _load_def_proj;
	    _place.field("checks"   )    << L"save result" << ckBx_savTm    << ckBx_savPos     <<ckBx_savG         << ckBx_savAlign 
                                                      << ckBx_savProj  << ckBx_savG_Plasm << ckBx_savTm_Plasm << ckBx_savLog
                                                      << ckBx_savExportSond << ckBx_savExportTarg<< ckBx_savNNParam<< ckBx_loadNNParam;

	    _place.field("ConcST"  )        << numUpDowSdConc   
                                        << numUpDowTgConc ;
	    _place.field("ConcSaltTa"  )    << numUpDowSalConc 
                                        << numUpDowTa ;
	    _place.field("SMeth"  )         << L" Salt Correct. Method:"	   <<  comBoxSalMeth;
	    _place.field("AMeth"  )         << L" ThDy Align. Method"       <<  comBoxTAMeth ;
    }
void  SetupPage::MakeResponive()
    {
        _proj.add_filter(STR("ThDy project"),STR("*.ThDy.txt"));
        _proj.onOpenAndSelectFile ([this](const nana::string &file)
        { 
            this->LoadProject (  file  );
                std::wcerr << L"onOpenAndSelectFile: Loaded Project file: " << file << std::endl;
        } );
        _proj.onSaveFile          ([this](const nana::string &file)
        { 
            this->_Pr.save (nana::charset ( (  file  ))); 
                std::wcerr << L"onSaveFile: Saved Project file: " << file << std::endl;
        } );

        AddFastaFiltre(_targets );
        AddFastaFiltre(_nTsec );
        AddFastaFiltre(_PCRfiltre );
        AddFastaFiltre(_PrimersFilePCR );

        _NNParamFile.add_filter      (STR("Nearest Neibrhud Parametr"),STR("*.NN.csv"));
        _NNParamFile.onOpenAndSelect ([this]()
            { 

            //assert((  std::cerr<< "\nBefore loading NNfile, SetupPage: " , true  ));;
            //assert((  std::wcerr<< caption() << std::endl , true  ));;
            
            std::ifstream nn(_Pr._cp._InputNNFile.get());
                _Pr._cp._pSaltCorrNNp->LoadNNParam(nn ) ;

            //assert((  std::cerr << "onOpenAndSelect: Opened NNp file: " << _Pr._cp._InputNNFile.get() << std::endl, true  ));;
                return true;

            } );
        _NNParamFile.onSave          ([this]()
        {

            //assert((  std::cerr<< "\nBefore saving NNfile, SetupPage: " , true  ));;
            //assert((  std::wcerr<< caption() << std::endl , true  ));;
            
            std::ofstream{ _Pr._cp._InputNNFile.get() /* + STR(".NN.csv") */} << *_Pr._cp._pSaltCorrNNp;        

            //assert((  std::cerr << "onSave: Saved NNp file: " << _Pr._cp._InputNNFile.get() << std::endl, true  ));;
         } );

        _set_def_proj .events().click ([&](){ setAsDefProject() ;} );
        _load_def_proj.events().click ([&](){ RestDefPr      () ;} );
    }
void  SetupPage::SaveProj()
	{	 
        if(  _proj.Canceled () )  return;
        _Pr.save (nana::charset ( _proj.FileName())); 
	}
 //   void  OpenNN()
	//{	 
 //        if( ! _NNParamFile.Canceled () )   
 //           return LoadProject ( nana::charset ( _NNParamFile.FileName() )) ;  
	//}
 //   void  SaveNN()
	//{	 
 //       if(  _NNParamFile.Canceled () )  return;
 //       _Pr.save (nana::charset ( _NNParamFile.FileName())); 
	//}
void  SetupPage::setAsDefProject()
    {
		string caption = "Set current setting as Default project";
		string message = std::string("This will overwrite the current Default Project." )        + "\n\n"
						+  "Are you sure?"   + "\n\n"
						+ "\tYes:  The default project will be overwrited. " + "\n"
						+ "\tNo:  No action will be taken. " + "\n"
						;
		switch ( (nana::msgbox(  *this, nana::charset (caption) , nana::msgbox::yes_no )
                        <<  message
                    ).icon(nana::msgbox::icon_question ) .show (  ))
		{
			case  nana::msgbox::pick_yes :  
                                    _Pr.save_asDefPr() ; 					 // crea el Def Project.
				return;

			case  nana::msgbox::pick_no:    
            default:;
        }
    }
void  SetupPage::RestDefPr	 ( )		// Restore (USE) Deff  Proj File
{		 
    try{
		    _Pr.load_defPr() ;			// cuando no existe Def Project: 1ra vez que se usa el prog??
		}
	catch ( std::exception& e)
	{ 
		(nana::msgbox ( STR("Error loading Def Project" ) )<< e.what()).show() ;
 	}		 
}
void  SetupPage::AddMenuItems(nana::menu& menu)
{
//      		_menuFile.append  (STR("&Open..."   ),[this](nana::menu::item_proxy& ip){ this->_OSbx.open(nana::string(nana::charset(this->_textBox.filename())));this->OpenFile()  ;}                );
//      _menuFile.append  (STR("&Save"      ),[&](nana::menu::item_proxy& ip){  ForceSave(nana::string(nana::charset(_textBox.filename())) ) ;}   );
	//_menuFile.append  (STR("Save &As..."),[&](nana::menu::item_proxy& ip){ _OSbx.save(nana::string(nana::charset(_textBox.filename())));SaveFile() ;} );

    menu.append(STR("New"    )  , [&](nana::menu::item_proxy& ip)  {  ;  } );
    menu.append(STR("Open...")  , [&](nana::menu::item_proxy& ip)  
    { 
        _proj.open(_proj.FileName()); /*OpenProj() ;*/   
        if (!_proj.Canceled())
            LoadProject( nana::charset(_proj.FileName()));
        //this->_OSbx.open(nana::string(nana::charset(this->_textBox.filename())));this->OpenFile()  ;}                );

    } );
    menu.append(STR("Save...")  , [&](nana::menu::item_proxy& ip)  
    { 
        _proj.save(_proj.FileName()); 
        if( ! _proj.Canceled () )   
            _Pr.save (nana::charset ( _proj.FileName())); 

        //SaveProj() ;  
    } );
    menu.append_splitter();
    menu.append(STR("Set as deffault") , [&](nana::menu::item_proxy& ip)  {;  });
    menu.append(STR("Restore deffault"), [&](nana::menu::item_proxy& ip)  {;  });
    menu.append_splitter();
    menu.append(STR("Exit"    )  , [&](nana::menu::item_proxy& ip)  {  _Pr.close();  } );

}
void  SetupPage::LoadProjectAndReg(nana::string file)
    {
        LoadProject(file);
 		_proj.FileName(file  );
   }
void  SetupPage::LoadProject(nana::string file)
	{
		try
		{
			_Pr.load(  nana::charset (  file ));
 		}
		catch (std::exception& e)
		{
			string caption = "Error trying to load the project file:";
			string message = string (nana::charset (  file ))         + "\n\n"
							+  e.what()   + "\n\n"
							+  "Use the Default project?"   + "\n\n"
							+ "\tYes:  The default project file will be loaded. " + "\n"
							+ "\tNo:  Select a project file to be loaded. " + "\n"
							+ "\tCancel: Use the values correctly loaded mixed with the\t\t\t previus existing. "
							;
			switch ( (nana::msgbox(  *this, nana::charset (caption) , nana::msgbox::yes_no_cancel )
                            <<  message
                        ).icon(nana::msgbox::icon_error) .show (  ))
			{
				case  nana::msgbox::pick_yes :  
					    _Pr.load_defPr();
                        _proj.FileName(nana::charset ( _Pr.ProjetFile ()  ));
					return;

				case  nana::msgbox::pick_no:    
                        _proj.open (nana::charset (file));
                        if ( !  _proj.Canceled() )
                                LoadProject(nana::charset (  _proj.FileName()));
                        return;
			}
		}
	}

   SetupPage::SetupPage          (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Setup"), STR("Setup.lay.txt"))
    {
        InitMyLayout();
        SelectClickableWidget( _set_def_proj);
        SelectClickableWidget( *this);

        MakeResponive();
    }
   SeqExpl::SeqExpl              (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("Seq Explorer"), STR("SeqExpl.lay.txt"))
    {
        InitMyLayout();
        SelectClickableWidget( _tree);
        SelectClickableWidget( *this);
        _tree.checkable(true);
        _list.checkable(true);
        _list.append_header(STR("Name")  , 120);
        _list.append_header(STR("Lenght"), 50);
        _list.append_header(STR("Tm °C") , 60);
        _list.append_header(STR("Deg")   , 50);
        _list.append_header(STR("Description")   , 220);
        _list.append_header(STR("Seq")   , 420);
        //_list.resolver(ListSeqMaker());


        AddMenuItems(_menuProgram);
        MakeResponive();

        _tree.auto_draw(true);
        _list.auto_draw(true);
    }
   FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, STR("Find probes"), STR("FindSonden.lay.txt"))
    {
                background (0xAAAAAA);  ///\todo: use codigo

        chkBx_showFindedProbes.check(true);
        InitMyLayout();
        SelectClickableWidget( *this);

        _design .events().click([&]() 
        {
            Run_Design(true );  
        });    

        _compare.events().click([&]() 
        {
            Run_Design(false);  
        });  

        //_Gmin.tooltip(STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be \"include\""));
   
   }
   MplexPCR::MplexPCR            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("MplexPCR"), STR("MplexPCR.lay.txt"))
    {

        _do_mPCR      .events().click([&](){buttPCR_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
   uArray::uArray            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("uArray"), STR("uArray.lay.txt"))
    {

        _do_uArray      .events().click([&](){buttuArray_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
   TmCalcPage::TmCalcPage        (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Tm Calc"), STR("Tm Calc.lay.txt"))
    {
                         sec_.multi_lines(false).editable(true ).tip_string (STR("forward primer"));
                   sec2align_.multi_lines(false).editable(true ).tip_string (STR("reverse primer"));
              txtBx_ResultSec.multi_lines(false).editable(false).tip_string (STR("alingned forward primer"));
        txtBx_ResultSec2Align.multi_lines(false).editable(false).tip_string (STR("alingned reverse primer"));

        _TmCalc << link (   _Pr._TmCal.align      ,    chkBx_align    )    
                << link (   _Pr._TmCal._Sec       ,    sec_           )
                << link (   _Pr._TmCal._Sec2Align ,    sec2align_     )
                ;

        run_      .events().click([&](){Run ();});
        copy_f_s_2.events().click([&](){Copy();});      ;   //(*this, STR("copy")),   
        copy_s    .events().click([&](){Self();});      ;  //  (*this, STR("c")),
        copy_s_a  .events().click([&](){Rev ();});      ;  

        InitMyLayout();
        SelectClickableWidget( *this);
        SelectClickableWidget( error_);
    }
   SeqExpl::Node SeqExpl::AddNewSeqGr  (Tree::item_proxy& node) 
		{	try{    
					return appendNewNode(node, _Pr._cp.AddSeqGroup(node.value<CMultSec*>(),"New group")).expand(true);
		        }
				catch ( std::exception& e)
		        { 
				  (nana::msgbox ( STR("Error adding new group" ) )<< e.what()).show() ;
                  return node;
		        }		
		}
   SeqExpl::Node SeqExpl::AddMSeqFiles (const std::string &file, bool  all_in_dir) 
	{	 
    try{ 
			auto      tn    = _tree.selected();
            CMultSec* ms    = tn.value<CMultSec*>();
            CMultSec* newms = _Pr._cp.AddSeqFromFile	(ms , file, all_in_dir	);
			return Refresh(   tn);
		}
		catch ( std::exception& e)
		{ 
			(nana::msgbox ( STR("Error adding sequences" ) )<< e.what()).show() ;
            return _tree.selected();
 		}		 
	}
   SeqExpl::Node SeqExpl::Replace      (Tree::item_proxy& tn, CMultSec *ms, const std::string& Path, bool all_in_dir)
    {        
    try{ 
         Tree::item_proxy   own = tn->owner();
         CMultSec          *pms = ms->_parentMS;  

		 CMultSec* newms = _Pr._cp.AddSeqFromFile	( pms, nana::charset(Path), all_in_dir	);

         _Pr._cp._pSeqNoUsed->AddMultiSec(ms); 
         _tree.erase(tn);
         populate(_tree.find( nana::charset(_Pr._cp._pSeqNoUsed->_name)));
         return appendNewNode(own, newms).expand(true).select(true) ;
		}
		catch ( std::exception& e)
		{ 
			(nana::msgbox ( STR("Error replacing sequences: " ) ).icon(nana::msgbox::icon_error)
               << "into "<< nana::charset(tn.key())                                 
               << "from " << (all_in_dir?"directory ":"file ") << Path     <<"\n"<< e.what()
            ).show() ;
 		}		
		catch(...)
		{
             (nana::msgbox(STR("An uncaptured exception during replacing sequences: "))
                    .icon(nana::msgbox::icon_error) 
               << "into "<< nana::charset(tn.key())                                 
               << "from " << (all_in_dir?"directory ":"file ") << Path    
             ).show();
	    }
        return tn;
    }
    void SeqExpl::ShowFindedProbes_in_mPCR(bool show_/*=true*/)
    {
        auto idp = _Pr._cp.MaxTgId.get();
        _Pr._cp.MaxTgId.set(100);
        CMultSec *ms= _Pr._cp.AddSeqFromFile	(_Pr._mPCR._probesMS.get() , _Pr._cp._OutputFile.get() + ".sonden.fasta", false	);
        _Pr._cp.MaxTgId.set(idp);

        RefreshProbes_mPCR( show_ );
    }
    void SeqExpl::RefreshProbes_mPCR(bool show_/*=true*/)
    {
        auto probNode = _tree.find(nana::charset(_Pr._mPCR._probesMS->_name));
        Refresh(probNode).expand(true).select(true);
        if (show_) 
            _Pr.ShowExpl();
    }
    void SeqExpl::AddMenuItems(nana::menu& menu)
    {
        menu.append_splitter();

        menu.append(STR("Add a new, empty, group for sequences")  , [&](nana::menu::item_proxy& ip) {  AddNewSeqGr(_tree.selected());    });
        menu.append(STR("Add a group of sequences from a file..."), [&](nana::menu::item_proxy& ip) {  Click(_loadFile);                 });
        menu.append(STR("Add a tree of groups of sequences from a directory..."),[&](nana::menu::item_proxy& ip) {  Click(_loadDir);     });

        menu.append_splitter();

        menu.append(STR("Reproduce only the structure of directory..."),[&](nana::menu::item_proxy& ip)  {  Click(_scanDir);     });
        menu.append(STR("Reload from the original file" )  , [&](nana::menu::item_proxy& ip)   {  ReloadFile(_tree.selected());    });
        menu.append(STR("Reload from the original directory"), [&](nana::menu::item_proxy& ip) {  ReloadDir(_tree.selected());     });
        menu.append(STR("Replace from a file . . ." )  , [&](nana::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (isRoot(tn))
            {
                nana::msgbox ( STR("Sorry, you can´t replace group " + tn.text()) ).show() ;
                return;
            }
            nana::filebox  fb{ *this, true };
            fb .add_filter ( SetupPage::FastaFiltre( )                   )
               .title(STR("Replace/reload a group of sequences from a file"));
            if (!fb()) return;

            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS; // tn->owner.value<CMultSec*>();
            _Pr._cp._pSeqNoUsed->AddMultiSec(ms);
			_Pr._cp.AddSeqFromFile	( pms, nana::charset(fb.file()), false	);
            Refresh(tn->owner());
            //_tree.auto_draw(false);
            //_tree.erase(tn);
            //Refresh(appendNewNode  (own, newms) );
            //_tree.auto_draw(true);
        });
        menu.append(STR("Replace from directory . . ."), [&](nana::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (tn->owner()->owner().empty())
            {
                nana::msgbox ( STR("Sorry, you can´t replace group " + tn->text()) ) ;
                return;
            }
            nana::filebox  fb{ *this, true };
            fb.title(STR("Replace/reload a group of sequences from a directory"));
            if (!fb()) return;

            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS; // tn->owner.value<CMultSec*>();
            ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// \todo: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

			CMultSec* newms = _Pr._cp.AddSeqFromFile	( pms, nana::charset(fb.file()), true	);
            _tree.erase(tn);
            populate(appendNewNode  (own, newms) );
            own.expand(true);

            _list.clear();
            populate_list_recur(pms);

            _tree.auto_draw(true);
            _list.auto_draw(true);
        });

        menu.append_splitter();

        menu.append     ( STR("Show Only local sequences"),[&](nana::menu::item_proxy& ip) { ShowLocals( menu.checked(ip.index())); })
            .check_style( nana::menu::checks::option)
            .checked    ( false );

        menu.append     ( STR("Show filtered sequences"  ),[&](nana::menu::item_proxy& ip) { ShowFiltered( menu.checked(ip.index())); })
            .check_style( nana::menu::checks::highlight )
            .checked    ( true );
 
        menu.append_splitter();
        menu.append(STR("Cut selected sequences from list"          ),[&](nana::menu::item_proxy& ip)  {  Click(_cutSec);  });
        menu.append(STR("Cut selected groups of sequences from tree"),[&](nana::menu::item_proxy& ip)  {  Click(_cut   );  });
        menu.append(STR("Paste the sequences"                       ),[&](nana::menu::item_proxy& ip)  {  Click(_paste );  });

        menu.append_splitter();
        menu.append(STR("Del selected sequences from list"),[&](nana::menu::item_proxy& ip)            {  Click(_delSec);  });
        menu.append(STR("Del selected groups of sequences from tree"),[&](nana::menu::item_proxy& ip)  {  Click(_del   );  });
        menu.append(STR("Rename the selected group of sequences"),[&](nana::menu::item_proxy& ip) 
        {
            
            RenameFrom rnm(&_tree, nana::charset(_tree.selected().text()));
            nana::API::modal_window( rnm );
            _tree.selected().text(nana::charset(rnm.Name()));
            _tree.selected().value<CMultSec*>()->_name = rnm.Name() ;

        }).enabled(true);

        menu.append_splitter();
        auto  indxFASTA = menu.append(STR("Export FASTA . . ."          ),[&](nana::menu::item_proxy& ip)            {  /*_tree.selected().value<CMultSec*>()->ExportFASTA();*/  }).index();
        auto& menuFASTA = *menu.create_sub_menu(indxFASTA);
        menuFASTA.append(STR("Only current sequences"     ),[&](nana::menu::item_proxy& ip)            {  ;  });
        menuFASTA.append(STR("Selected sequences in group"),[&](nana::menu::item_proxy& ip)            { _Pr.ExportFASTA(_tree.selected().value<CMultSec*>(), true );  });
        menuFASTA.append(STR("All sequences in group"     ),[&](nana::menu::item_proxy& ip)            { _Pr.ExportFASTA(_tree.selected().value<CMultSec*>(), false);  });
        menuFASTA.append(STR("All selected sequences"     ),[&](nana::menu::item_proxy& ip)            { _Pr._cp._pSeqTargets->Export_as("export.fasta", true )  ;  });
        menuFASTA.append(STR("All sequences"              ),[&](nana::menu::item_proxy& ip)            { _Pr._cp._pSeqTargets->Export_as("export.fasta", false)  ;  });

    }
    void SeqExpl::MakeResponive()
    {
		_tree.events().selected ( [&]( const nana::arg_treebox &tbox_arg_info ) { if (tbox_arg_info.operated) RefreshList(tbox_arg_info.item); });
        _tree.events().checked  ( [&]( const nana::arg_treebox &tbox_arg_info )
        {                                              
            tbox_arg_info.item.value<CMultSec*>()->Selected(tbox_arg_info.operated);
            if (tbox_arg_info.item== _tree.selected())  
                RefreshList(tbox_arg_info.item);                //  ??????? Only RefreschList
        });

        _list.events().checked  ( [&](  const nana::arg_listbox &lbox_arg_info )
        {                                               
            lbox_arg_info.item.value<CSec*>()->Selected(lbox_arg_info.selected);
            if ( ! _showAllseq && !lbox_arg_info.selected) 
                _list.erase(lbox_arg_info.item) ;
        });
 
        _loadFile   .events().click([this]()
                        {
                            nana::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title      ( STR("File load: Add a group of sequences from a file") );

                            if (fb()) 
                               AddMSeqFiles(nana::charset(fb.file()), false);
                        });
        //_loadFileTT.set(_loadFile,STR("File load: Add a group of sequences from a file"));

        _re_loadFile.events().click([this]()  {  ReloadFile(_tree.selected());    });
        _re_loadFileTT.set(_re_loadFile,STR("File reload: Reload a group of sequences from a file, \nposible using new filtres."));

        _loadDir    .tooltip(STR("Directory load: Add a tree of groups of sequences from a directory."))
                    .events().click([this]()
                        {
                            nana::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title(STR("Directory load: Add a tree of groups of sequences from a directory"));
                            if (fb()) 
                                AddMSeqFiles(nana::charset(fb.file()), true);
                        });
        _re_loadDir .tooltip(STR("Directory reload: Reload a tree of groups of sequences from a directory,\nposible using new filtres."))
                    . events().click([this]()  {  ReloadDir (_tree.selected());    });
        _scanDir    .tooltip(STR("Directory scan: Reproduce the structure of directory..."))
                    .events().click([this]()
                        {
                            nana::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title(STR("Directory scan: Reproduce the structure of directory..."));
                            if (!fb()) return;

                            auto      tn    = _tree.selected();
                            CMultSec* ms    = tn.value<CMultSec*>();
                            CMultSec* newms = _Pr._cp.CopyStructFromDir	( ms, nana::charset(fb.file())	);
                            _tree.auto_draw(false);
			                populate(  appendNewNode  (tn, newms) );
                            tn.expand(true);
                            _tree.auto_draw(true);
                        });
        _paste      .tooltip(STR("Paste sequences"))
                    .events().click([this]()
        {
			auto       tn = _tree.selected();
            CMultSec *pms = tn.value<CMultSec*>();

            for (auto ms : _dragMSec)
                pms->AddMultiSec(ms);
            for (auto s : _dragSec)
                pms->AddSec(s);

            _dragMSec.clear();
            _dragSec .clear();

            _tree.auto_draw(false);
            _list.auto_draw(false);

            populate(tn);
            populate(_tree.find(STR("Dont use") ));
            _list.clear();
            populate_list_recur(tn);
            tn.select(true).expand(true);

            _tree.auto_draw(false);
            _list.auto_draw(false);
        });
        _cut        .tooltip(STR("Cut a group of sequences"))
                    .events().click([this]()
        {
			auto tn= _tree.selected(); 
            if (tn->owner()->owner().empty())    //   ???  if( tn->level() < 2 );
            {
                (nana::msgbox ( _tree , STR("Cut a group of sequences " + tn->text()) )
                          << STR("Sorry, you can´t cut the group: ") + tn->text() )
                          .icon(nana::msgbox::icon_error )
                          .show() ;
                return;
            }
            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS;  
            _Pr._cp._pSeqNoUsed->AddMultiSec(ms);
            _dragMSec.push_back(ms);
            //ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// \todo: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

            _tree.erase(tn);
            populate(appendNewNode (_tree.find(STR("Dont use") ), ms ));
            own.select(true).expand(true);
        });
        _del        .tooltip(STR("Delete a group of sequences "))
                    .events().click([this]()
        {
			auto tn= _tree.selected();
            if (tn->owner()->owner().empty())
            {
                (nana::msgbox ( _tree , STR("Deleting a group of sequences " + tn->text()) )
                          << STR("Sorry, you can´t delete the group: ") + tn->text() )
                          .icon(nana::msgbox::icon_error )
                          .show() ;
                return;
            }
            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS;           
            _Pr._cp._pSeqNoUsed->AddMultiSec(ms); //ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// \todo: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

            _tree.erase(tn);
            populate(appendNewNode (_tree.find(STR("Dont use") ), ms ));

            own.select(true).expand(true);

        });
        _cutSec     .tooltip(STR("Cut selected sequences from list"))
                    .events().click([this]()
        {
			auto sel =	_list.selected() ; 
			for (auto i : sel)
			{
				auto s=_list.at(i ).value<CSec*>();
                _Pr._cp._pSeqNoUsed->AddSec( s );
                _dragSec.push_back(s);
			}
			RefreshList();
        });
        _delSec     .tooltip(STR("Delete selected sequences from list"))
                    .events().click([this]()
        {
			auto sel =	_list.selected() ; 
			for (auto i : sel)
			{
				auto s=_list.at(i ).value<CSec*>();
                _Pr._cp._pSeqNoUsed->AddSec( s );
			}
			RefreshList();
        });

        _show_locals_s.enable_pushed(true)
                      .pushed(false)
                      .tooltip(STR("Show only local sequences, and not the sequences in internal trees"))   
                      .events().click([this]() { ShowLocals( _show_locals_s.pushed());  });

        _show_filt_s.enable_pushed(true)  
                    .pushed(true) 
                    .tooltip(STR("Show filtered sequences too"))   
                    .events().click([this]() { ShowFiltered( _show_filt_s.pushed());  });
    }

    void SeqExpl::InitTree()
    {
        _list.auto_draw(false);
        _tree.auto_draw(false);

        CMultSec *ms=_Pr._cp._pSeqTree.get();
        for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
			populate( AddRoot( ms->CurMSec())) ;

        _tree.find(STR("Target seq")).select(true);
        populate_list_recur(_Pr._cp._pSeqTree.get());

    }

    void FindSondenPage::Run_Design(bool design)
    {
        _Pr._SdDes._design	 = design ;		
		 
		try{                                   
		        _Pr._SdDes._cp.Actualice_NNp();  
                _Pr.Run(_Pr._SdDes);	 //     _Pr._SdDes.Run ();	

                 if (chkBx_showFindedProbes.checked()) 
                    ( dynamic_cast<ThDyNanaForm&>(_Pr)).mExpl_.ShowFindedProbes_in_mPCR();
 		}
		catch ( std::exception& e)
		{ 
            (nana::msgbox(*this,STR("Error during Sonde Design !"), nana::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
    }   
    void MplexPCR::buttPCR_Click()  //	  Run      _IPrgPar_mPCR
	{	 			
	 try{                                   
		  _Pr._mPCR._cp.Actualice_NNp();  
 		  _Pr.Run(_Pr._mPCR);	

          _Pr.mExpl_.RefreshProbes_mPCR(/*false*/); 

          _Pr._results.emplace_back(new TableRes(_Pr._mPCR._rtbl));
          _Pr._results.back()->show();
          _Pr._results.emplace_back(new TableRes(_Pr._mPCR._rtbl_self));
          _Pr._results.back()->show();
		}
	catch ( std::exception& e)
		{ 
          cerr<< e.what()    ;
          (nana::msgbox(*this,STR("Error during multiplex PCR analis !"), 
                                                nana::msgbox::button_t::ok)   <<e.what()) (  ) ;
		  return;
		}
        //ShowResTbl(_Pr._mPCR._rtbl );
        //_Pr._uArr._rtbl = nullptr;

        //ShowResTbl(_Pr._mPCR._rtbl_self );
        //_Pr._mPCR._rtbl_self = nullptr;

	}
    void uArray::buttuArray_Click()  
	{	 			
	 try{                                   
		  _Pr._uArr ._cp.Actualice_NNp();  
 		  _Pr.Run(_Pr._uArr);	

          _Pr._results.emplace_back(new TableRes(_Pr._uArr._rtbl));
          _Pr._results.back()->show();
		}
	catch ( std::exception& e)
		{ 
          cerr<< e.what()    ;
          (nana::msgbox(*this,STR("Error during uArr analis !"), 
                                                nana::msgbox::button_t::ok)   <<e.what()) (  ) ;
		  return;
		}
	}



    //MIndex  idx{Invalid_Menu_idx};
    //{ auto t_idx=std::string::npos;
    //  if (t_idx == std::string::npos )
    //           t_idx = Invalid_Menu_idx ;
    //  idx = t_idx;                    // use a cast to avoid false warning??
    //}                                       // this could be a "graped" for some standard function
    //if (idx == Invalid_Menu_idx)
    //        ;      // not found
    //else
    //       ;      // found
    //  auto t_idx = std::string::npos;
    //  if ( t_idx >= Invalid_Menu_idx )
    //          idx = Invalid_Menu_idx ;    // not found
    //  else
    //          idx = t_idx;                           // found, use a cast to avoid false warning??
   ////if (SmartMIndex idx= std::find(some_menu_item....)!= Invalid_SMenu_idx )
   ////  { ;}                                                 // found, use idx.i
   ////else
   ////  { ;}                                                 // not found, still use idx.i if you want to return a non_value.

//List::oresolver operator<<(List::oresolver& ores, const TableRes::Index& i)
//{
//    ores<<   (*i.table.val) .table->TitRow(i.row); 
//}
