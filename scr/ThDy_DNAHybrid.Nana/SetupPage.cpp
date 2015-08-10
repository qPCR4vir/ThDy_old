/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\SetupPage.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\SetupPage.h"
#include "ThDy_DNAHybrid.Nana\main.Nana.h"

//#include <iostream>    // temp, for debugging
//#include <fstream>     // temp, for debugging
//#include <filesystem>
//
//#include <nana/gui/wvl.hpp>
//#include <nana/gui/widgets/tabbar.hpp>
//#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/widgets/treebox.hpp>
//#include <nana/gui/widgets/listbox.hpp>
//#include <nana/gui/widgets/toolbar.hpp>
//#include <nana/gui/tooltip.hpp>
//
//#include <nana/gui/tooltip.hpp>
//#include <nana/gui/widgets/progress.hpp>
//
//#include "thdy_programs\init_thdy_prog_param.h"
//#include "matrix.h" 
//#include "common_basics.h" 

//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"

using namespace ParamGUIBind;

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
            
        _place.field("Project"  )    <<  _proj      ;
        _place.field("dir"      )    <<  _gr_dir    ;
	    _place.field("buttons"  )    <<  _set_def_proj << _load_def_proj;
	    _place.field("checks"   )    <<  _gr_checks ;
	    _place.field("salt"     )    <<  _gr_salt   ;

        _gr_dir ["Results"  ]    <<  _results   ;
        _gr_dir ["seq"      ]    <<  _gr_seq    ;
	    _gr_dir ["NN_param" ]    << _NNParamFile  ;
	    _gr_dir ["ckBx_loadNNParam"]    <<   ckBx_loadNNParam ;
        _gr_seq ["_targets"        ]    <<   _gr_targ  ;
        _gr_seq ["_nTsec"          ]    <<   _gr_ntarg  ;
        _gr_seq ["_PCRfiltre"      ]    <<   _gr_PCRfiltre  ;
        _gr_seq ["_PrimersFilePCR" ]    <<   _gr_PrimersFilePCR  ;
        _gr_seq ["_Prob_uArr"      ]    <<   _gr_uArr  ;

        _gr_targ ["dir"  ]    <<   _targets  ;
        _gr_targ ["Opt"  ]    <<   _chkTargRecDir  << _chkTargOnlyStruct ;

        _gr_ntarg ["dir"  ]    <<   _nTsec  ;
        _gr_ntarg ["Opt"  ]    <<   _chk_nTgRecDir <<  _chk_nTgOnlyStruct          ;

        _gr_PCRfiltre ["dir"  ]    <<   _PCRfiltre  ;
        //_gr_PCRfiltre .plc["Opt"  ]    << _chkTargRecDir  << _chkTargOnlyStruct ;

        _gr_PrimersFilePCR ["dir"  ]    <<   _PrimersFilePCR  ;
        _gr_PrimersFilePCR ["Opt"  ]    <<   _chkPrimRecDir  << _chkPrOnlyStruct ;

        _gr_uArr ["dir"  ]    <<   _Prob_uArr  ;
        _gr_uArr ["Opt"  ]    <<   _chkProbRecDir  << _chkProbOnlyStruct ;

	    _gr_checks["checks"  ]   << ckBx_savTm    << ckBx_savPos     <<ckBx_savG         << ckBx_savAlign 
                                     << ckBx_savProj  << ckBx_savG_Plasm << ckBx_savTm_Plasm << ckBx_savLog
                                     << ckBx_savExportSond << ckBx_savExportTarg<< ckBx_savNNParam;

	    _gr_salt["ConcST"     ]   << numUpDowSdConc       << numUpDowTgConc ;
	    _gr_salt["ConcSaltTa" ]   << numUpDowSalConc      << numUpDowTa ;
	    _gr_salt["SMeth"      ]   << L" Salt Correct. Method:"	   <<  comBoxSalMeth;
	    _gr_salt["AMeth"      ]   << L" ThDy Align. Method"       <<  comBoxTAMeth ;
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
FilePickBox::filtres SetupPage::FastaFiltre( )
    {
        return FilePickBox::filtres       { {STR("fasta")       , STR("*.fas;*.fasta"     ) },
                                            {STR("NCBI BLAST")  , STR("*-Alignment.xml"   ) },
                                            {STR("GB"        )  , STR("*.gb;*-sequence.xml")},
                                            {STR("Text"      )  , STR("*.txt"             ) },
                                            {STR("All sequences"), STR("*.fas;*.fasta;*.txt;*-Alignment.xml;*.gb;*-sequence.xml")},
                                            {STR("All files" )  , STR("*.*"               ) }
                                          } ;
    }
void   SetupPage::SetDefLayout   () 
    {
        _DefLayout =
	"vertical      gap=3    margin=10    				\n\t"
	"	  < min=465  horizontal    gap=5   				\n\t"
	"	                   <weight=5>   		                		\n\t"
	"	                  <min=450   vertical   gap=5 	    	\n\t"
	"			                      <weight=26  Project       >       	\n\t"
	"				                  <weight=400 dir>       	\n\t"
	"				                  <weight=30 horizontal  	            	\n\t"
	"				                            <weight=20>             	    	\n\t"
	"				                            <min=280 max=700  gap=5 buttons>   	\n\t"
	"				                            <>       	                    	\n\t"
	"			                             >	   	\n\t"
	"		                         >                                      	\n\t"
	"	                  <weight=5>   		                		\n\t"
	"		               <weight=120 vertical   <weight=235  checks > <>  >    		\n\t"
	"		              >			                                	\n\t"
	"	  < weight=70  salt    >      		\n\t"
	"     <>				\n\t"
	"		\n\t" ;

        _results      .ResetLayout (70);
        _targets      .ResetLayout (70);
        _nTsec        .ResetLayout (70);
        _PCRfiltre    .ResetLayout (70 );
        _PrimersFilePCR.ResetLayout(70 );
        _Prob_uArr    .ResetLayout (70 );
        _NNParamFile  .ResetLayout (70 );

        numUpDowSdConc.ResetLayout (80 );  
        numUpDowTa.    ResetLayout (90 );  
        numUpDowTgConc.ResetLayout (80 );
        numUpDowSalConc.ResetLayout (110 );

        _gr_dir .div(" vert "
                         "  <weight=26  Results  >       \n\t"
				         "  <min=280 margin=5 seq>    	    \n\t"
	 		             "  <weight=26  <NN_param><weight=80 ckBx_loadNNParam> >       \n\t" );

        _gr_seq .div(" vert "
                         "  <weight=62  margin=[0,5,0,5] _targets       >    		         \n\t"
				         "  <weight=62  margin=[0,5,0,5] _nTsec         >    		         \n\t"
				         "  <weight=50  margin=[0,5,0,5] _PCRfiltre     >    		         \n\t"
				         "  <weight=62  margin=[0,5,0,5] _PrimersFilePCR>    		         \n\t"
				         "  <weight=67  margin=[0,5,5,5] _Prob_uArr     >    		         \n\t");

        _gr_targ .div(" vert "
                          "<weight=22 dir>                                           \n\t"
                          "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                          "<>                                                        \n\t");

        _gr_ntarg .div( " vert "
                            "<weight=22 dir>                                           \n\t"
                            "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                            "<>                                                        \n\t");

        _gr_PCRfiltre .div(" vert "
                               "<weight=22 dir>                                           \n\t"
                               "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                               "<>                                                        \n\t");

        _gr_PrimersFilePCR .div(" vert "
                                    "<weight=22 dir>                                           \n\t"
                                    "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                                    "<>                                                        \n\t");

        _gr_uArr .div(" vert "
                          "<weight=22 dir>                                           \n\t"
                          "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                          "<>                                                        \n\t");

        _gr_checks .div(	" <vertical weight=210 checks>     				\n\t");

        _gr_salt .div(  
            "     <horizontal <> 		                                    \n\t" 
	        "                 <weight=200 vertical ConcST        gap=2>     \n\t"
	        "                 <> 		                                    \n\t" 
	        "		          <weight=230 vertical ConcSaltTa    gap=2>     \n\t"
	        "                 <> 		                                    \n\t" 
            "		          <weight=250 vertical gap=5  <weight=23 SMeth >        	\n\t"
            "				                              <weight=23 AMeth >  >       \n\t"
	        "                 <>  		 >                                  \n\t");
    }


