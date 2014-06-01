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

#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>

#include "thdy_programs\init_thdy_prog_param.h"
#include "ThDySec\matrix.h" 
#include "ThDySec\common_basics.h" 


#include "nanaBind.hpp"

#include "main.Nana.h"


int main(int argc, char *argv[]) 
{
  try	
  {
    IParBind::SetDef(PriorizeDefault::Parametr );
    ThDyNanaForm tdForm(  argc,  argv);
	tdForm.show();
	nana::gui::exec();
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
        _list.resolver(ListSeqMaker());


        AddMenuItems(_menuProgram);
        MakeResponive();

        _tree.auto_draw(true);
        _list.auto_draw(true);
    }
   FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, STR("Find probes"), STR("FindSonden.lay.txt"))
    {
                background (0xAAAAAA);

        chkBx_showFindedProbes.check(true);
        InitMyLayout();
        SelectClickableWidget( *this);

        _design .make_event<nana::gui::events::click>([&]() 
        {
            Run_Design(true );  
        });    

        _compare.make_event<nana::gui::events::click>([&]() 
        {
            Run_Design(false);  
        });  

        //_Gmin.tooltip(STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be \"include\""));
   
   }
   MplexPCR::MplexPCR            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("MplexPCR"), STR("MplexPCR.lay.txt"))
    {

        _do_mPCR      .make_event <nana::gui::events::click>([&](){buttPCR_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
    }
   uArray::uArray            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("uArray"), STR("uArray.lay.txt"))
    {

        _do_uArray      .make_event <nana::gui::events::click>([&](){buttuArray_Click ();});

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

        run_      .make_event <nana::gui::events::click>([&](){Run ();});
        copy_f_s_2.make_event <nana::gui::events::click>([&](){Copy();});      ;   //(*this, STR("copy")),   
        copy_s    .make_event <nana::gui::events::click>([&](){Self();});      ;  //  (*this, STR("c")),
        copy_s_a  .make_event <nana::gui::events::click>([&](){Rev ();});      ;  

        InitMyLayout();
        SelectClickableWidget( *this);
        SelectClickableWidget( error_);
    }
   SeqExpl::Node SeqExpl::AddNewSeqGr  (Tree::item_proxy& node) 
		{	try{    
					return appendNewNode(node, _Pr._cp.AddSeqGroup(node.value<CMultSec*>(),"New group")).expend(true);
		        }
				catch ( std::exception& e)
		        { 
				  (nana::gui::msgbox ( STR("Error adding new group" ) )<< e.what()).show() ;
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
			(nana::gui::msgbox ( STR("Error adding sequences" ) )<< e.what()).show() ;
            return _tree.selected();
 		}		 
	}
   SeqExpl::Node SeqExpl::Replace      (Tree::item_proxy& tn, CMultSec *ms, const std::string& Path, bool all_in_dir)
    {        
    try{ 
         auto      own = tn->owner();
         CMultSec *pms = ms->_parentMS;  

         _Pr._cp._pSeqNoUsed->AddMultiSec(ms); 
         _tree.erase(tn);

		 CMultSec* newms = _Pr._cp.AddSeqFromFile	( pms, nana::charset(Path), all_in_dir	);
         return appendNewNode(own, newms).expend(true).select(true) ;
		}
		catch ( std::exception& e)
		{ 
			(nana::gui::msgbox ( STR("Error replacing sequences" ) )<< e.what()).show() ;
                  return tn;
 		}		 
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
        Refresh(probNode).expend(true).select(true);
        if (show_) 
            _Pr.ShowExpl();
    }
    void SeqExpl::AddMenuItems(nana::gui::menu& menu)
    {
        menu.append_splitter();

        menu.append(STR("Add a new, empty, group for sequences")  , [&](nana::gui::menu::item_proxy& ip) {  AddNewSeqGr(_tree.selected());    });
        menu.append(STR("Add a group of sequences from a file..."), [&](nana::gui::menu::item_proxy& ip) {  Click(_loadFile);                 });
        menu.append(STR("Add a tree of groups of sequences from a directory..."),[&](nana::gui::menu::item_proxy& ip) {  Click(_loadDir);     });

        menu.append_splitter();

        menu.append(STR("Reproduce only the structure of directory..."),[&](nana::gui::menu::item_proxy& ip)  {  Click(_scanDir);     });
        menu.append(STR("Reload from the original file" )  , [&](nana::gui::menu::item_proxy& ip)   {  ReloadFile(_tree.selected());    });
        menu.append(STR("Reload from the original directory"), [&](nana::gui::menu::item_proxy& ip) {  ReloadDir(_tree.selected());     });
        menu.append(STR("Replace from a file..." )  , [&](nana::gui::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (isRoot(tn))
            {
                nana::gui::msgbox ( STR("Sorry, you can´t replace group " + tn.text()) ).show() ;
                return;
            }
            nana::gui::filebox  fb{ *this, true };
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
        menu.append(STR("Replace from directory..."), [&](nana::gui::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (tn->owner()->owner().empty())
            {
                nana::gui::msgbox ( STR("Sorry, you can´t replace group " + tn->text()) ) ;
                return;
            }
            nana::gui::filebox  fb{ *this, true };
            fb.title(STR("Replace/reload a group of sequences from a directory"));
            if (!fb()) return;

            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS; // tn->owner.value<CMultSec*>();
            ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// TODO: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

			CMultSec* newms = _Pr._cp.AddSeqFromFile	( pms, nana::charset(fb.file()), true	);
            _tree.erase(tn);
            populate(appendNewNode  (own, newms) );
            own.expend(true);

            _list.clear();
            populate_list_recur(pms);

            _tree.auto_draw(true);
            _list.auto_draw(true);
        });

        menu.append_splitter();

        menu.append     ( STR("Show Only local sequences"),[&](nana::gui::menu::item_proxy& ip) { ShowLocals( menu.checked(ip.index())); })
            .check_style( nana::gui::menu::checks::option)
            .checked    ( false );

        menu.append     ( STR("Show filtered sequences"  ),[&](nana::gui::menu::item_proxy& ip) { ShowFiltered( menu.checked(ip.index())); })
            .check_style( nana::gui::menu::checks::highlight )
            .checked    ( true );
 
        menu.append_splitter();
        menu.append(STR("Cut selected sequences from list"          ),[&](nana::gui::menu::item_proxy& ip)  {  Click(_cutSec);  });
        menu.append(STR("Cut selected groups of sequences from tree"),[&](nana::gui::menu::item_proxy& ip)  {  Click(_cut   );  });
        menu.append(STR("Paste the sequences"                       ),[&](nana::gui::menu::item_proxy& ip)  {  Click(_paste );  });

        menu.append_splitter();
        menu.append(STR("Del selected sequences from list"),[&](nana::gui::menu::item_proxy& ip)            {  Click(_delSec);  });
        menu.append(STR("Del selected groups of sequences from tree"),[&](nana::gui::menu::item_proxy& ip)  {  Click(_del   );  });
        menu.append(STR("Rename the selected group of sequences"),[&](nana::gui::menu::item_proxy& ip) 
        {
            //_showFiltered = menu.checked(ip.index());// !_showFiltered;
            //_list.auto_draw(false);
            //_list.clear();
            // populate_list_recur(_tree.selected());
            //_list.auto_draw(true);
        })
            .enabled(false);

    }
    void SeqExpl::MakeResponive()
    {
		_tree.ext_event().selected = [&](nana::gui::window w, Tree::item_proxy node, bool selected) { if (selected) RefreshList(node); };
        _tree.ext_event().checked  = [&](nana::gui::window w, Tree::item_proxy node, bool checked)
        {                                              
            node.value<CMultSec*>()->Selected(checked);
            if (node== _tree.selected())  
                RefreshList(node);                //  ??????? Only RefreschList
        };

        _list.ext_event().checked  = [&](  List::item_proxy item, bool checked)
        {                                               
            item.value<CSec*>()->Selected(checked);
            if ( ! _showAllseq && !checked) 
                _list.erase(item) ;
        };
 
        _loadFile   .make_event<nana::gui::events::click>([this]()
                        {
                            nana::gui::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title      ( STR("File load: Add a group of sequences from a file") );

                            if (fb()) 
                               AddMSeqFiles(nana::charset(fb.file()), false);
                        });
        //_loadFileTT.set(_loadFile,STR("File load: Add a group of sequences from a file"));

        _re_loadFile.make_event<nana::gui::events::click>([this]()  {  ReloadFile(_tree.selected());    });
        _re_loadFileTT.set(_re_loadFile,STR("File reload: Reload a group of sequences from a file, \nposible using new filtres."));

        _loadDir    .tooltip(STR("Directory load: Add a tree of groups of sequences from a directory."))
                    .make_event<nana::gui::events::click>([this]()
                        {
                            nana::gui::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title(STR("Directory load: Add a tree of groups of sequences from a directory"));
                            if (fb()) 
                                AddMSeqFiles(nana::charset(fb.file()), true);
                        });
        _re_loadDir .tooltip(STR("Directory reload: Reload a tree of groups of sequences from a directory,\nposible using new filtres."))
                    . make_event<nana::gui::events::click>([this]()  {  ReloadDir (_tree.selected());    });
        _scanDir    .tooltip(STR("Directory scan: Reproduce the structure of directory..."))
                    .make_event<nana::gui::events::click>([this]()
                        {
                            nana::gui::filebox  fb{ *this, true };
                            fb .add_filter ( SetupPage::FastaFiltre( )                   )
                               .title(STR("Directory scan: Reproduce the structure of directory..."));
                            if (!fb()) return;

                            auto      tn    = _tree.selected();
                            CMultSec* ms    = tn.value<CMultSec*>();
                            CMultSec* newms = _Pr._cp.CopyStructFromDir	( ms, nana::charset(fb.file())	);
                            _tree.auto_draw(false);
			                populate(  appendNewNode  (tn, newms) );
                            tn.expend(true);
                            _tree.auto_draw(true);
                        });
        _paste      .tooltip(STR("Paste sequences"))
                    .make_event<nana::gui::events::click>([this]()
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
            tn.select(true).expend(true);

            _tree.auto_draw(false);
            _list.auto_draw(false);
        });
        _cut        .tooltip(STR("Cut a group of sequences"))
                    .make_event<nana::gui::events::click>([this]()
        {
			auto tn= _tree.selected(); 
            if (tn->owner()->owner().empty())    //   ???  if( tn->level() < 2 );
            {
                (nana::gui::msgbox ( _tree , STR("Cut a group of sequences " + tn->text()) )
                          << STR("Sorry, you can´t cut the group: ") + tn->text() )
                          .icon(nana::gui::msgbox::icon_error )
                          .show() ;
                return;
            }
            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS;  
            _Pr._cp._pSeqNoUsed->AddMultiSec(ms);
            _dragMSec.push_back(ms);
            //ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// TODO: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

            _tree.erase(tn);
            populate(appendNewNode (_tree.find(STR("Dont use") ), ms ));
            own.select(true).expend(true);
        });
        _del        .tooltip(STR("Delete a group of sequences "))
                    .make_event<nana::gui::events::click>([this]()
        {
			auto tn= _tree.selected();
            if (tn->owner()->owner().empty())
            {
                (nana::gui::msgbox ( _tree , STR("Deleting a group of sequences " + tn->text()) )
                          << STR("Sorry, you can´t delete the group: ") + tn->text() )
                          .icon(nana::gui::msgbox::icon_error )
                          .show() ;
                return;
            }
            CMultSec *ms = tn.value<CMultSec*>();
            CMultSec *pms = ms->_parentMS;           
            _Pr._cp._pSeqNoUsed->AddMultiSec(ms); //ms->MoveBefore(_Pr._cp._pSeqNoUsed->goFirstMSec() );  /// TODO: higth level MoveMSec !! (actualize globals)
            auto own = tn->owner();

            _tree.auto_draw(false);
            _list.auto_draw(false);

            _tree.erase(tn);
            populate(appendNewNode (_tree.find(STR("Dont use") ), ms ));

            own.select(true).expend(true);

        });
        _cutSec     .tooltip(STR("Cut selected sequences from list"))
                    .make_event<nana::gui::events::click>([this]()
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
                    .make_event<nana::gui::events::click>([this]()
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
                      .make_event<nana::gui::events::click>([this]() { ShowLocals( _show_locals_s.pushed());  });

        _show_filt_s.enable_pushed(true)  
                    .pushed(true) 
                    .tooltip(STR("Show filtered sequences too"))   
                    .make_event<nana::gui::events::click>([this]() { ShowFiltered( _show_filt_s.pushed());  });
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
            (nana::gui::msgbox(*this,STR("Error during Sonde Design !"), nana::gui::msgbox::button_t::ok)<<e.what()) (  ) ;
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
          (nana::gui::msgbox(*this,STR("Error during multiplex PCR analis !"), 
                                                nana::gui::msgbox::button_t::ok)   <<e.what()) (  ) ;
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
          (nana::gui::msgbox(*this,STR("Error during uArr analis !"), 
                                                nana::gui::msgbox::button_t::ok)   <<e.what()) (  ) ;
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

