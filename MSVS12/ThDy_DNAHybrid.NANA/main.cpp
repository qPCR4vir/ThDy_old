#include <../temp/EditableForm.hpp>
#include <../temp/Numer.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging
#include<filesystem>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>

#include "thdy_programs\init_thdy_prog_param.h"

#include "nanaBind.hpp"
using namespace ParamGUIBind;

class ThDyNanaForm ;

class FindSondenPage : public CompoWidget
{public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    ThDyProject &_Pr;
    FilePickBox nTsec_;
    BindGroup   _findSond;
    nana::gui::NumUnitUpDown _Gmin, _Gmax, _Tmmin, _Tmmax, _Lengthmin, _Lengthmax,
                             _MaxG, _MinTm, _MinG, _MaxTm, _MinSelfG, _MaxSelfTm, 	
                             numUpDw_MinTargCov, numUpDw_MaxTargCov ;	
    nana::gui::button        _design, _compare;
    nana::gui::checkbox      chkBx_unique, chkBx_common;
    void SetDefLayout   () override
    {
      _DefLayout=  
        "vertical      gap=2                 \n\t"
	    "    <weight=10     >       \n\t "
        "    <weight=195 gap=8 <weight=5><weight=350 vertical <weight=100 <weight=320 Sonde  grid[3,4]>>     \n\t "
        "                                           <TargCov    grid[2,2]                            >     \n\t "
        "                                           <weight=40 <   <><weight=300   gap=20 Run>       > >    \n\t "
        "                                           <weight=10>                                 \n\t "
        "                      >   <><weight=230 gap=1 vertical  options>    >   \n\t "
	    "    <weight=23   NonTargSeq >       \n\t "
        ;

       nTsec_.ResetLayout(100);

         _Gmin.ResetLayout     (60,45,55 );   _Gmax.ResetLayout     (1,40,50 );
        _Tmmin.ResetLayout     (60,45,55 );  _Tmmax.ResetLayout     (1,40,50 );
        _Lengthmin.ResetLayout (60,45,55 );  _Lengthmax.ResetLayout (1,40,50 );
 
        _MaxG.ResetLayout     (110,45,50 );   
        _MinTm.ResetLayout    (110,45,50 );   
        _MinG.ResetLayout     (110,45,50 );   

        _MaxTm.ResetLayout     (110,45,50 );   
        _MinSelfG.ResetLayout  (110,45,50);   
        _MaxSelfTm.ResetLayout (110,45,50 );  

        numUpDw_MinTargCov.ResetLayout (115,45,20 );  
        numUpDw_MaxTargCov.ResetLayout (115,45,20 );  


    }
    void AsignWidgetToFields() override
    {
	     /// Use room (wd,w,h) in combination with a <Table grid[W,H]>
        _place.field("NonTargSeq" )<<nTsec_;
	    _place.field("Sonde" )     << "Probes"     << "Min."    << "   Max."   
                                   <<  _place.room(_Gmin ,2,1) << _Gmax
                                   <<  _place.room(_Tmmin,2,1) << _Tmmax
                                   <<  _place.room(_Lengthmin,2,1) << _Lengthmax  ;
        _place.field("TargCov" )   << chkBx_unique << numUpDw_MinTargCov       
                                   << chkBx_common << numUpDw_MaxTargCov     	;
        _place.field("Run"     )   << _design	<< _compare	;
                 
	    _place.field("options" )   << "Probe-target"    <<  _MaxG     << _MinTm
                                   << "Probe-non-target"<<  _MinG     << _MaxTm
                                   << "Probe-self"      <<  _MinSelfG << _MaxSelfTm  
                                    ;    

    }

    void Run_Design(bool design)
    {
        _Pr._SdDes._design	 = design ;		
		 
		try{                                   
		    _Pr._SdDes._cp.Actualice_NNp();  
            _Pr.Run(_Pr._SdDes);	 //     _Pr._SdDes.Run ();	
		}
		catch ( std::exception& e)
		{ 
            (nana::gui::msgbox(*this,STR("Error during Sonde Design !"), nana::gui::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
    }   
};

class MplexPCR : public CompoWidget
{public: 
    ThDyProject        &_Pr;
    BindGroup          _mPCR;
    nana::gui::button  _do_mPCR;
    FilePickBox        _PrimersFilePCR;

    MplexPCR (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	        "  <_PrimersFilePCR weight=23>       \n\t "
            "  <<><_do_mPCR  vertical min=50 max=200><> weight=50>       \n\t "
            //"  <wieght=300 <vertical min=50 max=200 buttons> <> <weight=80 checks>>   \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("_PrimersFilePCR" )<<_PrimersFilePCR;
	    _place.field("_do_mPCR"         )<<_do_mPCR;
	    //_place.field("checks"          )<<"save result";
    }

    private: void buttPCR_Click				( ) //	  Run      _IPrgPar_mPCR
			 {	 			
		           try{                                   
		                    _Pr._mPCR._cp.Actualice_NNp();  
 		                    _Pr.Run(_Pr._mPCR);	
		           }
		           catch ( std::exception& e)
		           { cerr<< e.what()    ;
                    (nana::gui::msgbox(*this,STR("Error during multiplex PCR analis !"), 
                                                         nana::gui::msgbox::button_t::ok)   <<e.what()) (  ) ;
		            return;
		           }
		//ShowResTbl(_Pr._mPCR._rtbl );
		//_Pr._uArr._rtbl = nullptr;

		//ShowResTbl(_Pr._mPCR._rtbl_self );
		//_Pr._mPCR._rtbl_self = nullptr;

		    }


};

class SeqExpl : public CompoWidget
{
    using Tree = nana::gui::treebox;
    using Node = Tree::item_proxy;
    using List = nana::gui::listbox;

    ThDyProject        &_Pr;
    Tree                _tree{ *this };
    List                _list{ *this };
    bool				_showAllseq{true}, _showFiltered{true};
    std::vector<CSec*>      _dragSec;
    std::vector<CMultSec*>  _dragMSec;

    using pSec = CSec*;
    class ListSeqMaker : public List::resolver_interface <pSec>
    {
        nana::string decode(size_t col, const pSec &sec) const override
        {
            static const int    blen{ 50 }, slen{ 1000 };
            nana::char_t val[blen];

            switch (col)
            {
            case 0: return nana::charset(sec->Name());
            case 1: swprintf(val,blen,     STR("%*d")  , 6,           sec->Len()       );
                    return val;
            case 2: swprintf(val,blen, STR("% *.*f °C"), 6, 1,  KtoC( sec->_Tm.Ave() ) );
                    return val;
            case 3: swprintf(val,blen,     STR("%*d")  , 5,           sec->Degeneracy());
                    return val;  
            case 4: return nana::charset( sec->Description());
            case 5: return nana::charset( std::string(sec->Get_charSec(), slen));

            default:
                return nana::string{};
            }
        }
        void encode(pSec&, std::size_t col, const nana::string& txt) const override
        {
            switch (col)
            {
            case 0:  /*sec->Name(nana::charset(txt));*/                        break;
                //case 1:  nana::charset(std::to_string(sec->Len()));          break;
                //case 2:  nana::charset(std::to_string(sec->_Tm.Ave() ));       break;
                //case 3:  nana::charset(std::to_string(sec->Degeneracy()));       break;
                //case 4:  nana::charset( sec->Description() );                break;
                //case 5:  nana::charset( sec->Get_charSec()  );                break;

            default:
                break;
            }
        }
    };
    //BindGroup          _mExpl;
    //static const CMultSec* msec(Tree::item_proxy& node)
    //{
    //    return (node.value<CMultSec*>());
    //}
    //static const CSec*      sec(List::item_proxy& item)
    //{
    //    return (item.value<CSec*>());
    //}

    
    void SetDefLayout() override
    {
        _DefLayout = "    horizontal  gap=2   <Tree weight=25% > <List >         \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
        _place.field("Tree") << _tree;
        _place.field("List") << _list;
    }
    void MakeResponive()
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
    }

    Node &Refresh(Tree::item_proxy& node)
    {
            _tree.auto_draw(false);

            populate(node);
            node.expend(true);
            RefreshList(node);

            _tree.auto_draw(true);
            return node;
    }
    void RefreshList(                      ) { RefreshList(_tree.selected());         }
    void RefreshList(Tree::item_proxy& node) { RefreshList( node.value<CMultSec*>()); }
    void RefreshList(CMultSec*ms)
    {
            _list.auto_draw(false);

            _list.clear();
            populate_list_recur(ms);

            _list.auto_draw(true);
    }
    void populate_list(CMultSec*ms)
    {
        for ( ms->goFirstSec() ;  ms->NotEndSec() ; ms->goNextSec() )
		  if ( _showFiltered || !ms->CurSec()->Filtered() ) 
              AddSecToList(ms->CurSec());
    }
    void populate_list_recur(Tree::item_proxy& node)
    {
        populate_list_recur(node.value<CMultSec*>()); // msec(node)  );
    }
    void populate_list_recur(CMultSec     *ms)
		{
			populate_list(ms);
            if ( _showAllseq )
	            for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
                    populate_list_recur(ms->CurMSec());
		}

    List::item_proxy AddSecToList     (CSec* s)
    {
        return _list.at(0).append(s).value  ( s             )
                                    .check  ( s->Selected() )
                                    .fgcolor( s->Filtered()     ?   0xFF00FF 
                                                                :   0x0     );//nana::gui::color::gray_border );
    }

    Node AddRoot          (CMultSec*ms)  
    {
        nana::string name = nana::charset(ms->_name);
        return _tree.insert(name, name).value(ms).check(ms->Selected());
    }
 static Node appendNewNode(Node &node, CMultSec*ms) /// Add a new node to the child of node.
    {
        nana::string name = nana::charset(ms->_name);
        return node->append(name, name).value(ms).check(ms->Selected());
    }
    Node &populate     (Node &node)  /// crea y add to the child of node un nodo nuevo por cada seq in ms. Asume el nodo estaba vacio
    {
        while(node.size()) 
            _tree.erase(node.child());

        CMultSec *ms = node.value<CMultSec*>(); //  msec(node);
		for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
			populate( appendNewNode(node, ms->CurMSec())) ;
        return node;
    }

    Node AddNewSeqGr  (Tree::item_proxy& node) 
		{	try{    
					return appendNewNode(node, _Pr._cp.AddSeqGroup(node.value<CMultSec*>(),"New group")).expend(true);
		        }
				catch ( std::exception& e)
		        { 
				  (nana::gui::msgbox ( STR("Error adding new group" ) )<< e.what()).show() ;
                  return node;
		        }		
		}
    Node AddMSeqFiles (const std::string &file, bool  all_in_dir) 
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
    Node Replace      (Tree::item_proxy& tn, CMultSec *ms, const std::string& Path, bool all_in_dir)
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
    Node ReloadDir    (Tree::item_proxy& tn)
    {            
        CMultSec *ms = tn.value<CMultSec*>();
        if (! ms->_Path.empty()) 
           return Replace(tn, ms, ms->_Path,false);
        for (Tree::item_proxy& ntn : tn)
            ReloadDir(ntn);
        return tn;
}
    Node ReloadFile   (Tree::item_proxy& tn)
    {            
        CMultSec *ms = tn.value<CMultSec*>();
        if (ms->_Path.empty())  
           return tn;
        else
           return Replace(tn, ms, ms->_Path,false);
    }

public:
    void AddMenuItems(nana::gui::menu& menu)
    {
        menu.append_splitter();

        menu.append(STR("Add a new, empty, group for sequences")  , [&](nana::gui::menu::item_proxy& ip) {  AddNewSeqGr(_tree.selected());    } );
        menu.append(STR("Add a group of sequences from a file..."), [&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb.title(STR("Add a group of sequences from a file"));
            if (fb()) 
               AddMSeqFiles(nana::charset(fb.file()), false);
        });
        menu.append(STR("Add a tree of groups of sequences from a directory..."),[&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb.title(STR("Add a tree of groups of sequences from a directory"));
            if (fb()) 
                AddMSeqFiles(nana::charset(fb.file()), true);
        });

        menu.append_splitter();

        menu.append(STR("Reproduce only the structure of directory..."),[&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb.title(STR("Reproduce the structure of directory..."));
            if (!fb()) return;

            auto      tn    = _tree.selected();
            CMultSec* ms    = tn.value<CMultSec*>();
            CMultSec* newms = _Pr._cp.CopyStructFromDir	( ms, nana::charset(fb.file())	);
            _tree.auto_draw(false);
			populate(  appendNewNode  (tn, newms) );
            tn.expend(true);
            _tree.auto_draw(true);
        });
        menu.append(STR("Reload from the original file" )  , [&](nana::gui::menu::item_proxy& ip)   {  ReloadFile(_tree.selected());    });
        menu.append(STR("Reload from the original directory"), [&](nana::gui::menu::item_proxy& ip) {  ReloadDir(_tree.selected());     });
        menu.append(STR("Replace from a file..." )  , [&](nana::gui::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (tn.isRoot())
            {
                nana::gui::msgbox ( STR("Sorry, you can´t replace group " + tn.text()) ).show() ;
                return;
            }
            nana::gui::filebox  fb{ *this, true };
            fb.title(STR("Replace/reload a group of sequences from a file"));
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

        menu.append(   STR("Show Only local sequences")  ,   [&](nana::gui::menu::item_proxy& ip) 
        {
            _list.auto_draw(false);
            _showAllseq = ! menu.checked(ip.index());// =! _showAllseq; 
            _list.clear();
             populate_list_recur(_tree.selected());
            _list.auto_draw(true);
        });
        menu.check_style(menu.size()-1, nana::gui::menu::check_t::check_highlight );
        menu.checked (menu.size()-1, false );

        menu.append(STR("Show filtered sequences"),[&](nana::gui::menu::item_proxy& ip) 
        {
            _showFiltered = menu.checked(ip.index());// !_showFiltered;
            _list.auto_draw(false);
            _list.clear();
             populate_list_recur(_tree.selected());
            _list.auto_draw(true);
        });
        menu.check_style(menu.size()-1, nana::gui::menu::check_highlight); // check_option
        menu.checked (menu.size()-1, true );

        menu.append_splitter();
        menu.append(STR("Cut selected sequences from list"),[&](nana::gui::menu::item_proxy& ip) 
        {
            //_showFiltered = menu.checked(ip.index());// !_showFiltered;
            //_list.auto_draw(false);
            //_list.clear();
            // populate_list_recur(_tree.selected());
            //_list.auto_draw(true);
        });
        menu.append(STR("Cut selected groups of sequences from tree"),[&](nana::gui::menu::item_proxy& ip) 
        {
			auto tn= _tree.selected();
            if (tn->owner()->owner().empty())
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
        menu.append(STR("Paste the sequences"),[&](nana::gui::menu::item_proxy& ip) 
        {
			auto       tn = _tree.selected();
            CMultSec *pms = tn.value<CMultSec*>();

            for (auto ms : _dragMSec)
                pms->AddMultiSec(ms);

            _dragMSec.clear();

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

        menu.append_splitter();
        menu.append(STR("Del selected sequences from list"),[&](nana::gui::menu::item_proxy& ip) 
        {
            //_showFiltered = menu.checked(ip.index());// !_showFiltered;
            //_list.auto_draw(false);
            //_list.clear();
            // populate_list_recur(_tree.selected());
            //_list.auto_draw(true);
        });
        menu.append(STR("Del selected groups of sequences from tree"),[&](nana::gui::menu::item_proxy& ip) 
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
        menu.append(STR("Rename the selected group of sequences"),[&](nana::gui::menu::item_proxy& ip) 
        {
            //_showFiltered = menu.checked(ip.index());// !_showFiltered;
            //_list.auto_draw(false);
            //_list.clear();
            // populate_list_recur(_tree.selected());
            //_list.auto_draw(true);
        });

    }
    SeqExpl(ThDyNanaForm& tdForm);

};

class TmCalcPage : public CompoWidget
{public: 
    ThDyProject             &_Pr;
    BindGroup              _TmCalc;
    nana::gui::textbox          sec_                {*this},  
                                sec2align_          {*this},  
                                txtBx_ResultSec     {*this},  
                                txtBx_ResultSec2Align{*this};
    nana::gui::checkbox         chkBx_Tm_save_asPCR {*this, STR("save")},   
                                chkBx_align         {*this, STR("align")},
                                chkBx_copy_rev      {*this, STR("rev")},    
                                chkBx_copy_compl    {*this, STR("cpl")};
    nana::gui::button           run_                {*this, STR("Tm !")},
                                copy_f_s_2          {*this, STR("copy")},   
                                copy_s              {*this, STR("c")},
                                copy_s_a            {*this, STR("c")};      
    nana::gui::label            error_              {*this, STR("no error")};
    nana::gui::NumberBox        Tm_min_Up{*this}, Tm_Up{*this}, Tm_max_Up{*this} ,
                                Tm_min_Dw{*this}, Tm_Dw{*this}, Tm_max_Dw{*this} ,
                                Tm_min_In{*this}, Tm_In{*this}, Tm_max_In{*this} ,
                                G_min_Up{*this},   G_Up{*this},  G_max_Up{*this} ,
                                G_min_Dw{*this},   G_Dw{*this},  G_max_Dw{*this} ,
                                G_min_In{*this},   G_In{*this},  G_max_In{*this} ;

    TmCalcPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2  min=150           \n\t"
            "       < weight=50  <vertical min=100 gap=2 InputSec>   < weight=50 gap=1 CopyBut grid[2,2]>  >       \n\t "
            "       < weight=25 <weight=20><error min=50> <rev_compl weight=80>>         \n\t  "
            "       < weight=80 gap=2  <vertical weight=80 gap=2 Left>   < Table min=280 grid[7,4]>  >       \n\t "
            "       < vertical weight=50  ResAlign>    "
	                 //"       <weight=1>                \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("rev_compl")<< chkBx_copy_rev << chkBx_copy_compl ;
	    _place.field("InputSec" )<< sec_ << sec2align_ ;
	    _place.field("error"    )<< error_ ;
	    _place.field("Left"     )<< run_  << chkBx_align;
	    _place.field("CopyBut"  )<< nana::gui::vplace::room (copy_f_s_2, 1, 2)<< copy_s << copy_s_a ;
	    _place.field("Table"    )<< ""          << "   min-" << "Tm(°C)"   << "-max" << "   min-"  << "G(kJ)"   << "-max   "  ;
	    _place.field("Table"    )<< "Up"        << Tm_min_Up << Tm_Up      << Tm_max_Up<<G_min_Up  <<  G_Up     <<  G_max_Up  ;
	    _place.field("Table"    )<< "Down"      << Tm_min_Dw << Tm_Dw      << Tm_max_Dw<<G_min_Dw  <<  G_Dw     <<  G_max_Dw  ;
	    _place.field("Table"    )<< "Interact"  << Tm_min_In << Tm_In      << Tm_max_In<<G_min_In  <<  G_In     <<  G_max_In  ;


	    _place.field("ResAlign" )<< txtBx_ResultSec << txtBx_ResultSec2Align ;
    }
    void Run()
    {
		try
        {                                   
		   _Pr._TmCal._cp.Actualice_NNp();  
           _Pr._TmCal.Run ();
		}
		catch ( std::exception& e)
		{ 
            (nana::gui::msgbox(*this,STR("Error during Tm calculation !"), nana::gui::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
        txtBx_ResultSec      .caption (nana::charset (_Pr._TmCal._AlignedSec      .Get() ));
        txtBx_ResultSec2Align.caption (nana::charset (_Pr._TmCal._AlignedSec2Align.Get() ));
        Tm_min_Up.Value( _Pr._TmCal._TmS.Min ());
        Tm_Up    .Value( _Pr._TmCal._TmS.Ave ());  
        Tm_max_Up.Value( _Pr._TmCal._TmS.Max ()); 

        Tm_min_Dw.Value( _Pr._TmCal._Tm2A.Min ());
        Tm_Dw    .Value( _Pr._TmCal._Tm2A.Ave ());  
        Tm_max_Dw.Value( _Pr._TmCal._Tm2A.Max ()); 

        Tm_min_In.Value( _Pr._TmCal._TmHy.Min ());
        Tm_In    .Value( _Pr._TmCal._TmHy.Ave ());  
        Tm_max_In.Value( _Pr._TmCal._TmHy.Max ()); 

        G_min_Up.Value( _Pr._TmCal._GS .Min ());
        G_Up    .Value( _Pr._TmCal._GS.Ave ());  
        G_max_Up.Value( _Pr._TmCal._GS.Max ()); 

        G_min_Dw.Value( _Pr._TmCal._G2A .Min ());
        G_Dw    .Value( _Pr._TmCal._G2A.Ave ());  
        G_max_Dw.Value( _Pr._TmCal._G2A.Max ()); 

        G_min_In.Value( _Pr._TmCal._GHy.Min ());
        G_In    .Value( _Pr._TmCal._GHy.Ave ());  
        G_max_In.Value( _Pr._TmCal._GHy.Max ()); 
    }
    void Copy()
    {
        //_Pr._TmCal._Sec.CopyTrim (std::string(nana::charset (   sec_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec_Sec2Align	(rev, compl) ;

        //sec2align_.caption (nana::charset (_Pr._TmCal._Sec2Align.Get() ));
    }
    void Self()
    {
        //_Pr._TmCal._Sec.CopyTrim (std::string(nana::charset (  sec_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec	(rev, compl) ;

        //sec_.caption (nana::charset (_Pr._TmCal._Sec   .Get() ));
    }
    void Rev()
    {
        //_Pr._TmCal._Sec2Align.CopyTrim (std::string(nana::charset (  sec2align_.caption ())).c_str() );
         bool rev  =  chkBx_copy_rev.checked(), compl=  chkBx_copy_compl.checked() ;	

		_Pr._TmCal.Update_Sec2Align	(rev, compl) ;

        //sec2align_.caption (nana::charset (_Pr._TmCal._Sec2Align  .Get() ));
    }

};

class SetupPage : public CompoWidget
{public: 
    ThDyProject        &_Pr;
    FilePickBox        /*  targets_    { *this, STR("Targets:") }, */
                        results_    { *this, STR("Results:") }/*, 
                        PCRfiltre_  { *this, STR("PCR-filtre:")}*/;
    OpenSaveBox                     proj_       { *this, STR("Project:") };
    nana::gui::combox               comBoxSalMeth   {*this}, 
                                    comBoxTAMeth    {*this};
    nana::gui::NumUnitUpDown        numUpDowTgConc  {*this, STR("Target Conctr:"      ), 50, 0.1 , 1000,  "µM"}, 
                                    numUpDowSalConc {*this, STR("Salt Conc [Cations]:"), 50, 0.1 , 10000000,"µM"} , 
                                    numUpDowTa      {*this, STR("Temp. Anneling:"     ), 55,  40 , 75,    "°C"},  
                                    numUpDowSdConc  {*this, STR("Sonde Conctr:"       ), 50, 0.1 , 1000,  "µM"}  ;
    nana::gui::button  set_def_proj_    {*this,STR("Set as Def. project") };
    FilePickBox        _NNParamFile     {*this, STR("NN param:")};
    BindGroup          _setup;

    SetupPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= 
    "vertical      gap=2             	\n\t"
	"	       <<Project         max=800>     weight=23 >      	\n\t"
	"	        <<Results        max=800>     weight=23 >      	\n\t"
	"	        <<NN_param max=800>     weight=23 >      	\n\t"
	"	\n\t"
	"	   <weight=100 <weight=2><vertical min=50 max=200 gap=2 buttons> <> <weight=80 checks>>   	\n\t"
	"	\n\t"
	"	        < weight=46  gap=2 <><vertical ConcST   weight=200  gap=2>  <>               	\n\t"
	"	                              <vertical ConcSaltTa   weight=230  gap=2>   <>       	\n\t"
	"	                                   <vertical   weight=250 <SMeth gap=2>          	\n\t"
	"	                                             <AMeth gap=2> > <>  >      	\n\t"
	"	 	\n\t"
	"	\n\t"
            ;
        //PCRfiltre_    .ResetLayout (60 );
        numUpDowSdConc.ResetLayout (80 );  
        numUpDowTa.    ResetLayout (90 );  
        numUpDowTgConc.ResetLayout (80 );
        numUpDowSalConc.ResetLayout (110 );

    }
    void AsignWidgetToFields() override
    {
	    _place.field("Project" )        << proj_   ;
	    _place.field("buttons"  )<<set_def_proj_;
	    _place.field("NN_param" )<<_NNParamFile;
	    _place.field("checks"   )<<"save result";
	    //_place.field("PCRfiltre" )      << PCRfiltre_   ;
	    _place.field("Results" )        << results_   ;
	    _place.field("ConcST"  )        << numUpDowSdConc   
                                        << numUpDowTgConc ;
	    _place.field("ConcSaltTa"  )    << numUpDowSalConc 
                                        << numUpDowTa ;
	    _place.field("SMeth"  )         << " Salt Correct. Method:"	   <<  comBoxSalMeth;
	    _place.field("AMeth"  )         << " ThDy Align. Method"       <<  comBoxTAMeth ;
    }
    void AddMenuItems(nana::gui::menu& menu)
    {
        menu.append(STR("New"    )  , [&](nana::gui::menu::item_proxy& ip)  {  ;  } );
        menu.append(STR("Open...")  , [&](nana::gui::menu::item_proxy& ip)  { proj_.open(proj_.FileName()); OpenProj() ;  } );
        menu.append(STR("Save...")  , [&](nana::gui::menu::item_proxy& ip)  { proj_.save(proj_.FileName()); SaveProj() ;  } );
        menu.append_splitter();
        menu.append(STR("Set as deffault") , [&](nana::gui::menu::item_proxy& ip)  {;  });
        menu.append(STR("Restore deffault"), [&](nana::gui::menu::item_proxy& ip)  {;  });
        menu.append_splitter();
        menu.append(STR("Exit"    )  , [&](nana::gui::menu::item_proxy& ip)  {  ;  } );

    }

    void LoadProject(std::string file)
	{
		try
		{
			_Pr.load(   file );
			 proj_.FileName(nana::charset ( file  ));
 		}
		catch (std::exception& e)
		{
			string caption = "Error trying to load the project file:";
			string message = file         + "\n\n"
							+  e.what()   + "\n\n"
							+  "Use the Default project?"   + "\n\n"
							+ "\tYes:  The default project file will be loaded. " + "\n"
							+ "\tNo:  Select a project file to be loaded. " + "\n"
							+ "\tCancel: Use the values correctly loaded mixed with the\t\t\t previus existing. "
							;
			switch ( (nana::gui::msgbox(  *this, nana::charset (caption) , nana::gui::msgbox::yes_no_cancel )
                            <<  message
                        ).icon(nana::gui::msgbox::icon_error) .show (  ))
			{
				case  nana::gui::msgbox::pick_yes :  
					    _Pr.load_defPr();
                        proj_.FileName(nana::charset ( _Pr.ProjetFile ()  ));
					return;

				case  nana::gui::msgbox::pick_no:    
                        proj_.open (nana::charset (file));
                        if ( !  proj_.Canceled() )
                                LoadProject(nana::charset (  proj_.FileName()));
                        return;
			}
		}
	}

    void  OpenProj()
	{	 
         if(  proj_.Canceled () )  return;
         LoadProject ( nana::charset ( proj_.FileName() )) ;  

      //try {
      //          load (); 
      //}
      //  catch(std::exception& e)
      //  {
      //       (nana::gui::msgbox(*this, STR("Error during project loading: ")) /*.icon(msgbox::icon_information)*/
      //                           <<STR("\nIn windows:\n\t ") << Titel()
      //                           <<STR("\nIn project:\n\t ") << proj_.FileName()
      //                           <<STR("\nException :\n\t ") << e.what() 
      //       ).show();
      //  }
      ////tmCalc_._TmCalc.UpDateForm();
	}
    void  SaveProj()
	{	 
        if(  proj_.Canceled () )  return;
        _Pr.save (nana::charset ( proj_.FileName())); 
	}
};

class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{public: 
    nana::gui::tabbar<nana::string> tabbar_     {*this};
    //FilePickBox                   /*  targets_    { *this, STR("Targets:") }, */
    //                                results_    { *this, STR("Results:") }/*, 
    //                                PCRfiltre_  { *this, STR("PCR-filtre:")}*/;
    //nana::gui::checkbox             chkBx_RecDir{ *this, STR("RecurDir") };
    FindSondenPage                  findSond_   {*this};
    TmCalcPage                      tmCalc_     {*this}; 
    SetupPage                       setup_      {*this};
    MplexPCR                        mPCR_       {*this};
    SeqExpl                         mExpl_      {*this};
    BindGroup                       _commPP     ;
    nana::gui::NumUnitUpDown        numUpDwMaxTgId  {*this, STR("Max. ident.:"        ), 99,  50 , 100 ,   "%"}, 
                                    numUpDw_TgBeg   {*this, STR("Beg.:"               ),  0,   0 , 100000,"nt"},    /// rev !!
                                    numUpDw_TgEnd   {*this, STR("End.:"               ),  0,   0 , 100000,"nt"},    /// rev !!	
                                    numUpDw_SLenMin {*this, STR("Min.Len.:"           ),  0,   0 , 100000,"nt"},
                                    numUpDw_SLenMax {*this, STR("Max.Len.:"           ),  0,   0 , 100000,"nt"};

   ThDyNanaForm (int argc, char *argv[])
                  :nana::gui::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                   EditableForm    (nullptr, *this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")) 
   {
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //background (0xEEEEEE);
        //foreground(1);
       
        add_page( setup_    );
        add_page( mExpl_    );
        add_page( findSond_ );
        add_page( mPCR_     );
        add_page( tmCalc_   );

        tabbar_.active (0);

        setup_.proj_.FileName(nana::charset ( ProjetFile()  ));
        try{ 
			    if ( argc > 1 )
				    setup_.LoadProject( argv[1] )   ;
			    else
				    load() ;						// cuando no existe Def Project: 1ra vez que se usa el prog??
		    }
    	catch ( std::exception& e )      // Por ejemplo cuando no existe Def Project: 1ra vez que se usa el prog.
		{   
            (nana::gui::msgbox(*this,STR("Error during initial project load !\n\t"), nana::gui::msgbox::button_t::ok)
                               << e.what()    << "\n\n A new Default Project will be created. "
                          ).show (  ) ;
		    save_defPr() ; 					                
        }

		//this->comBoxTAMeth->SelectedIndex  = SMStLucia;     
		//_seqExpl = gcnew SeqExpl(this->_Pr);


        _commPP  /*<< link( _cp._InputTargetFile ,       targets_  )*/
                 /*<< link( _cp._RecurDir      ,       chkBx_RecDir)*/
                 //<< link( _cp._OutputFile      ,       results_  )
                 /*<< link( _cp._PCRfiltrPrFile  ,       PCRfiltre_)*/
                 << link( _cp.MaxTgId                 ,       numUpDwMaxTgId  )
                 << link( _cp.SecLim         , numUpDw_TgBeg,  numUpDw_TgEnd  )
                 << link( _cp.SecLenLim   ,  numUpDw_SLenMin, numUpDw_SLenMax )
            ;
 

        InitMyLayout();

        setup_.AddMenuItems (_menuBar.push_back(STR("P&roject")));
        mExpl_.AddMenuItems (_menuBar.push_back(STR("&Sequences")));
        AddMenuProgram();
        
        SelectClickableWidget( _menuBar);

   }

    //~ThDyNanaForm();
    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2                   \n\t "
	                 "       <weight=25>                   \n\t "
	                 "       <PagesTag    weight=23 >      \n\t "
	                 "       <Pages       min=255   >      \n\t "
	                 "       < <weight=30><TargetsOptions><weight=10> weight=23>      \n\t "
	                 "       <weight=1 >                   \n\t "
	                 "       < weight=23 <><Firma><> >                   \n\t "
            ;

        numUpDwMaxTgId .ResetLayout (60,40,30 );  
        numUpDw_TgBeg  .ResetLayout (35,40,30 );  
        numUpDw_TgEnd  .ResetLayout (35,40,30 );  
        numUpDw_SLenMin.ResetLayout (60,40,30 );   
        numUpDw_SLenMax.ResetLayout (60,40,30 );  
    }
    void AsignWidgetToFields() override
    {
	    _place.field("PagesTag")        << tabbar_  ;
	    _place.field("TargetsOptions" ) << numUpDwMaxTgId<<   numUpDw_TgBeg << numUpDw_TgEnd << numUpDw_SLenMin << numUpDw_SLenMax;
	    _place.field("Firma"  )         <<  "INNT - FLI :       ArielVina.Rodriguez@fli.bund.de"

                                ;
    }                                        
    void add_page(widget& w)
    {   
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //w.background (1);
        tabbar_.push_back (                    w.caption());
        tabbar_.relate    (tabbar_.length()-1, w          );
	    _place.field("Pages"   ).fasten( w)  ;
    }         
};

   FindSondenPage::FindSondenPage(ThDyNanaForm& tdForm)
        : _Pr        (tdForm), 
          CompoWidget(tdForm, STR("Find probes"), STR("FindSonden.lay.txt")),
          nTsec_  (*this, STR("Non template seq:"),STR("FindSonden-OSB.NonTarg.lay.txt") ),
          _Gmin   (*this, STR("G :"    ), -5, -10 , 10,"kcal/mol"),      _Gmax  (*this, STR(""), -1, -10, 10, "kcal/mol" ),
          _Tmmin  (*this, STR("Tm :"   ), 57,  40 , 60,"°C"      ),     _Tmmax  (*this, STR(""), 63,  45, 75, "°C"       ),
      _Lengthmin  (*this, STR("Length:"), 20,  15 , 35,"nt"      ), _Lengthmax  (*this, STR(""), 35,  15, 40, "nt"       ),
      chkBx_unique(*this, STR("Report unique probes, ")),          chkBx_common (*this, STR("Report common probes, ")),
          _MinG   (*this, STR("Min G" ), 15, -10 , 30,"kcal/mol"),    _MaxG  (*this, STR("Max G" ), 10, -10, 30, "kcal/mol" ),
          _MinTm  (*this, STR("Tm :"  ), 30,  10 , 60,"°C"      ),   _MaxTm  (*this, STR("Max Tm"), 10, -10, 75, "°C"       ),
         _MinSelfG(*this, STR("Min G" ), 10, -10 , 30,"kcal/mol"), _MaxSelfTm(*this, STR("Max Tm"), 10, -10, 75, "°C"       ),

      numUpDw_MaxTargCov(*this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%"),
      numUpDw_MinTargCov(*this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%"),
          _design (*this, STR("Design !" )),  
          _compare(*this, STR("Compare !"))
    {
                background (0xAAAAAA);

        //nTsec_._DefLayout=("vertical   <weight=1>    "
        //         "  <weight=20 <weight=3><   vertical weight=100 <><label weight=15><>     ><weight=1>     "
		      //   "			   <cbFL >       "
		      //   "			   <pick weight=30>  "
		      //   "			   <weight=3> 	>            <weight=2>    ");

        _findSond << link (   _Pr._SdDes.G_sig ,            _MaxG     )    
                  << link (   _Pr._SdDes.Tm_sig ,           _MinTm    )
                  << link (   _Pr._SdDes.MinSd_nTgG,        _MinG     )
                  << link (   _Pr._SdDes.MaxSd_nTgTm,       _MaxTm    )
                  << link (   _Pr._SdDes.MinSelfG,          _MinSelfG )
                  << link (   _Pr._SdDes.MaxSelfTm,         _MaxSelfTm)
                  << link (   _Pr._SdDes.sL.G,        _Gmin,_Gmax     )
                  << link (   _Pr._SdDes.sL.T,       _Tmmin,_Tmmax    )
                  << link (  _Pr._SdDes.sL.L,    _Lengthmin,_Lengthmax)
                  << link (  _Pr._SdDes.common,           chkBx_common)
                  << link (  _Pr._SdDes.unique,           chkBx_unique)
                  << link ( _Pr._SdDes.Coverage,  numUpDw_MinTargCov,  numUpDw_MaxTargCov)	



                  //<< link (  _Pr._cp.  ,               nTsec_)
            ;

        InitMyLayout();
        SelectClickableWidget( nTsec_);
        SelectClickableWidget( *this);

        _design .make_event<nana::gui::events::click>([&]() 
        {
            Run_Design(true );  });    
        _compare.make_event<nana::gui::events::click>([&]() 
        {
            Run_Design(false);  });  
     }
   TmCalcPage::TmCalcPage        (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Tm Calc"), STR("Tm Calc.lay.txt"))
    {
                         sec_.multi_lines(false);
                   sec2align_.multi_lines(false);
              txtBx_ResultSec.multi_lines(false);
        txtBx_ResultSec2Align.multi_lines(false);

                         sec_.editable(true);
                   sec2align_.editable(true);
              txtBx_ResultSec.editable(false);
        txtBx_ResultSec2Align.editable(false);

                         sec_.tip_string (STR("forward primer"));
                   sec2align_.tip_string (STR("reverse primer"));
              txtBx_ResultSec.tip_string (STR("alingned forward primer"));
        txtBx_ResultSec2Align.tip_string (STR("alingned reverse primer"));

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
   SetupPage::SetupPage          (ThDyNanaForm& tdForm)
        : _Pr           (tdForm), 
          CompoWidget  (tdForm, STR("Setup"), STR("Setup.lay.txt"))
          /*,
          set_def_proj_(*this,STR("Set as Def. project") ),
          _NNParamFile (*this, STR("NN param:") )*/
    {
      _setup<< link( _Pr._cp._InputNNFile       , _NNParamFile  )
            << link( _Pr._cp.ConcSd	    ,       numUpDowSdConc  )
            << link( _Pr._cp.ConcSalt	     , numUpDowSalConc  )
            << link( _Pr._cp.ConcTg	    ,       numUpDowTgConc  )
            << link( _Pr._cp.Ta	            ,       numUpDowTa  )        
            << link( _Pr._cp.SaltCorr	  ,      comBoxSalMeth  )        
            << link( _Pr._cp.TAMeth       ,       comBoxTAMeth  )        
            ;

        InitMyLayout();
        SelectClickableWidget( set_def_proj_);
        SelectClickableWidget( *this);

        proj_.add_filter(STR("ThDy project"),STR("*.ThDy.txt"));
        proj_.Open.make_event	<nana::gui::events::click> ([&](){ OpenProj() ;} );
		proj_.Save.make_event	<nana::gui::events::click> ([&](){ SaveProj() ;} );


    }
   MplexPCR::MplexPCR            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("MplexPCR"), STR("MplexPCR.lay.txt")),
          _do_mPCR        (*this, STR(" PCR ! ") ),
          _PrimersFilePCR (*this, STR("Primers seq. file:") )
    {
       _mPCR<< link(   _Pr._mPCR._InputSondeFile , _PrimersFilePCR)

            ;

        _do_mPCR      .make_event <nana::gui::events::click>([&](){buttPCR_Click ();});

        InitMyLayout();
        SelectClickableWidget( _PrimersFilePCR);
        SelectClickableWidget( *this);
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

        _list.auto_draw(false);
        _tree.auto_draw(false);

        CMultSec *ms=_Pr._cp._pSeqTree.get();
        for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
			populate( AddRoot( ms->CurMSec())) ;

        populate_list_recur(_Pr._cp._pSeqTree.get());

        AddMenuItems(_menuProgram);
        MakeResponive();

        _tree.auto_draw(true);
        _list.auto_draw(true);
    }


//using MIndex = unsigned char;
//
//const MIndex  Invalid_Menu_idx = std::numeric_limits<MIndex>::max();     // global ?? Could be another "logical" value, for example, just 100.
// 
//
////struct SmartMIndex
////{
//// MIndex  i {Invalid_Menu_idx};       
//// operator size_t(size_t idx)
////  {
////   if ( idx >= Invalid_Menu_idx )                                  // or if ( idx == npos)     ?????
////           return SmartMIndex{} ;                                    // not found
////   else
////          return SmartMIndex{  MIndex (t_idx)}  ;                          
////  }
////
////};
////SmartMIndex Invalid_SMenu_idx;        // global ??



int main(int argc, char *argv[]) try
{

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

