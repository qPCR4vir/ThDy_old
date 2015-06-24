/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\SeqExpl.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\SeqExpl.h"
#include "ThDy_DNAHybrid.Nana\main.Nana.h"

#include <algorithm>

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
//



//#include <../../nana.ext/include/EditableForm.hpp>
//#include <../../nana.ext/include/Numer.hpp>
//#include "../../nana.ext/include/nanaBind.hpp"


         SeqExpl::SeqExpl              (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("Seq Explorer"), STR("SeqExpl.lay.txt"))
    {
        InitMyLayout();
        SelectClickableWidget( _tree);
        SelectClickableWidget( *this);
        _tree.checkable(true);
        _list.checkable(true);
        _list.append_header(STR("Name")  , 120);     // col 0: name  
        _list.append_header(STR("Lenght"), 50);      // col 1: len
        _list.append_header(STR("Tm °C") , 60);      //case 2: Tm 
        _list.append_header(STR("Deg")   , 50);      // case 3: deg   
        _list.append_header(STR("Description")   , 220);   // case 4: descr  
        _list.append_header(STR("Beg"), 50);         // case 5: beg in aln 
        _list.append_header(STR("End"), 50);         // case 6: end in aln    
        _list.append_header(STR("Seq")   , 420);
        //_list.resolver(ListSeqMaker());


        AddMenuItems(_menuProgram);
        MakeResponive();

        _tree.auto_draw(true);
        _list.auto_draw(true);
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
            
            RenameFrom rnm(_tree, nana::charset(_tree.selected().text()));
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

    List::oresolver& operator<<(List::oresolver & ores, CSec * const sec )
{
    static const long    blen{ 50 }, slen{ 1000 };
    nana::char_t val[blen];

    swprintf(val,blen,     STR("%*d")  , 6,           sec->Len()       );

    ores <<  sec->Name()                    // col 0: name  
         <<  val  ;                         // col 1: len

	Temperature t=KtoC( sec->NonDegSet() ? sec->NonDegSet()->_Local._Tm.Ave() : sec->_Tm.Ave());
	swprintf(val,blen, STR("% *.*f °C"), 6, 1,   t );
    Temperature min=57.0, max=63.0;
    double fade_rate=  t<min? 0.0 : t>max? 1.0 : (t-min)/(max-min);
    nana::color tc{static_cast<nana::color_rgb>(0xFFFFFFFF)} , 
                bc = nana::color(nana::colors::red).blend( nana::colors::blue, fade_rate); 

    ores <<  List::cell {val, bc , tc};    //case 2: Tm 

    swprintf(val,blen,     STR("%*d")  , 5,           sec->Degeneracy());
    ores <<  val                           // case 3: deg    
         << sec->Description()     ;       // case 4: descr  

    
    if( sec->_aln_fragment && sec->_aln_fragment->aln.lenght())
    {
        swprintf(val,blen,     STR("%*d")  , 6,           sec->_aln_fragment ->aln.Min()       );
        ores <<  val                       ;    // case 5: beg in aln   
        swprintf(val,blen,     STR("%*d")  , 6,           sec->_aln_fragment ->aln.Max()       );
        ores <<  val                       ;    // case 6: end in aln    
    }
    else
        ores  << " - "      << " - "        ;    // no pos in aln 

    ores <<  (char *)(  sec->Sequence().substr (1, std::min( sec->Len(), slen)).c_str()    );      // sec                                                     

    return ores;
}