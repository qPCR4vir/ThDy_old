/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*  https://www.fli.de/en/institutes/institut-fuer-neue-und-neuartige-tierseuchenerreger/wissenschaftlerinnen/prof-dr-m-h-groschup/
*  distributed under the GNU General Public License, see <http://www.gnu.org/licenses/>.
*
* @author Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2016
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\SeqExpl.h
*
* @brief GUI to explore sequences
*
*/

#ifndef SeqExpl_H
#define SeqExpl_H

#include "thdy_programs\init_thdy_prog_param.h"
#include "../../nana.ext/include/nanaBind.hpp"
#include <../../nana.ext/include/EditableForm.hpp>

#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/tooltip.hpp>


class ThDyNanaForm ;
 
using List = nana::listbox;

class SeqExpl : public CompoWidget
{
    using Tree = nana::treebox;
    using Node = Tree::item_proxy;

    ThDyNanaForm       &_Pr;
    Tree                _tree{ *this };
    List                _list{ *this };
    bool				_showAllseq{true}, _showFiltered{true};
    std::vector<CSec*>      _dragSec;
    std::vector<CMultSec*>  _dragMSec;

    nana::button    _loadFile     {*this,("Load"   )},       //nana::toolbar  _tbar { *this };
                    _re_loadFile  {*this,("reLoad" )},   
                    _loadDir      {*this,("Load"   )},       
                    _re_loadDir   {*this,("reLoad" )},
                    _scanDir      {*this,("Scan"   )},
                    _cut          {*this,("Cut"    )},
                    _paste        {*this,("Paste"  )},
                    _del          {*this,("Del"    )},
                    _cutSec       {*this,("Cut"    )},
                    _delSec       {*this,("Del"    )},
                    _show_locals_s{*this,("local"  )},
                    _show_filt_s  {*this,("filter" )}
                    ; 
    nana::tooltip    _loadFileTT {_loadFile,("File load: Add a group of sequences from a file")},
                          _re_loadFileTT ;  
	nana::label     _statusbar    { *this };

    using pSec = CSec*;
    void SetDefLayout() override
    {
        _DefLayout = 
	                "vertical                                                 \n\t"
	                "		  <weight=23 <toolbar weight=680 margin=2 ><>>    \n\t"
	                "		  <      <Tree  > |75% <List >   >      		  \n\t"
	                "		  <weight=21 <statusbar margin=2             >    \n\t"
	                "				                                          \n\t"
	                "		                                                  \n\t"
	                "	                                                      \n\t"
            ;
    }
    void AsignWidgetToFields() override
    {
 	    _place["toolbar"] <<  "   Files:"  << _loadFile << _re_loadFile  << _paste           
                          <<  "      Dir:" << _loadDir  << _re_loadDir   << _scanDir << _cut      << _del      
                          <<  "      Seq:" << _show_locals_s  << _show_filt_s   << _cutSec   << _delSec
                                ;
        _place[ "Tree"  ]    << _tree;
        _place[ "List"  ]    << _list;
        _place[ "statusbar"] << _statusbar;
    }
    void MakeResponive();

    Node &Refresh(Tree::item_proxy& node)
    {
            _tree.auto_draw(false);

            populate(node);
            node.expand(true);
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
			RefreshStatusInfo(ms);
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
    void populate_list(CMultSec*ms)
    {
        for ( ms->goFirstSec() ;  ms->NotEndSec() ; ms->goNextSec() )
		  if ( _showFiltered || !ms->CurSec()->Filtered() ) 
              AddSecToList(ms->CurSec());
    }

    List::item_proxy AddSecToList     (CSec* s)
    {
        return _list.at(0).append(s).value  ( s             )
                                    .check  ( s->Selected() )
                                    .fgcolor( static_cast<nana::color_rgb>(
                                              (s->Filtered()     ?   0xFF00FF   ///\todo: use coding
                                                                :   0x0   )  ));//nana::color::gray_border );
    }

    Node AddRoot          (CMultSec*ms)  
    {
        std::string name = ms->_name;
        return _tree.insert(name, name).value(ms).check(ms->Selected());
    }
    bool isRoot(Node &node)
    {
        return node.level() == 1;
    }
 static Node appendNewNode(Node &node, CMultSec*ms) /// Add a new node to the child of node.
    {
        std::string name = ms->_name;
        return node->append(name, name, ms).check(ms->Selected());
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

    Node AddNewSeqGr  (Tree::item_proxy& node) ;
    Node AddMSeqFiles (const std::string &file, bool  all_in_dir) ;
    Node Replace      (Tree::item_proxy& tn, CMultSec *ms, const std::string& Path, bool all_in_dir);
    Node ReloadDir    (Tree::item_proxy& tn)
    {            
        CMultSec *ms = tn.value<CMultSec*>();
        if (ms->_Path.empty()) 
        {
            for (Tree::item_proxy& ntn : tn)
                ReloadDir(ntn);
            return tn;
        }
        else return Refresh(Replace(tn, ms, ms->_Path,true)).expand(true).select(true);//true
    }
    Node ReloadFile   (Tree::item_proxy& tn)
    {            
        CMultSec *ms = tn.value<CMultSec*>();
        if (ms->_Path.empty())  
           return tn;
        else
           return Refresh(Replace(tn, ms, ms->_Path,false));
    }
    void ShowLocals(bool showLocals)
    {        
        if(showLocals != _showAllseq) return ;
        else _showAllseq = ! showLocals;

        _list.auto_draw(false);
        _list.clear();
            populate_list_recur(_tree.selected());
        _list.auto_draw(true);
    }
    void ShowFiltered(bool showFiltered)
    {        
        if(showFiltered == _showFiltered) return ;
        else _showFiltered = showFiltered;

        _list.auto_draw(false);
        _list.clear();
            populate_list_recur(_tree.selected());
        _list.auto_draw(true);
    }
	void RefreshStatusInfo(CMultSec *ms)
	{
		_statusbar.caption(     "  <bold>Local</> (sq: "   + std::to_string( ms->_Local._NSec     )   
			                   +    ", gr: "     + std::to_string( ms->_Local._NMSec    )  +"),"
	  + (ms->_Local._NSec ? "    Length ["  + std::to_string( ms->_Local._Len.Min())
		                      + "-"             + std::to_string( ms->_Local._Len.Max()) + "]"
				           + ",    Tm [ "  + temperature_to_string( ms->_Local._Tm.Min() ) + "-"
		                                         + temperature_to_string( ms->_Local._Tm.Max() ) + "]  "
				          :      "" )
		                  + ".        <bold>Global</> (sq: "  + std::to_string( ms->_Global._NSec     )             
			              +         ", gr: "  + std::to_string( ms->_Global._NMSec    )             +"),"                             
	  + (ms->_Global._NSec ? "    Length ["     + std::to_string( ms->_Global._Len.Min())
		                       + "-"               + std::to_string( ms->_Global._Len.Max()) + "]"
				               + ", Tm [ "  + temperature_to_string(ms->_Global._Tm.Min() ) + "-"
		                                          + temperature_to_string(ms->_Global._Tm.Max() ) + " ]  "
				          :      "" )
		                 );
	}
	std::string temperature_to_string(Temperature t)
	{
		int w = 10;  // -250,0 °C
		std::string s(w, 0);

		std::snprintf( &s[0],  w+1, (u8"% *.*f °C"), w-4, 1, KtoC(t) );
		return s;
	}

public:
    SeqExpl(ThDyNanaForm& tdForm);
    void ShowFindedProbes_in_mPCR(bool show_=true);
    void RefreshProbes_mPCR(bool show_=true);
    void AddMenuItems(nana::menu& menu);
    void InitTree();
};

List::oresolver& operator<<(List::oresolver & ores, CSec * const sec );

class RenameFrom : public nana::form, public EditableForm
{
    std::string     _name;
    bool            _renamed    {false};
    nana::textbox   edit        {*this};
    nana:: button   OK          {*this, ("rename")}, 
                    Cancel      {*this, ("abort" )};
    ParamGUIBind::BindGroup       _bind     ;


  public:
    RenameFrom(nana::window owner, std::string name) : 
            _name(name),  
             nana::form  (nana::rectangle( nana::point(150,500), nana::size(300,50) )),
             EditableForm(owner, *this, ("Rename") )     
        {
			edit.multi_lines(false);
			edit.caption(_name);
            InitMyLayout();
            OK.events().click([this]()
            {
                std::string  name=edit.caption(); 
                _renamed = (_name!=name);
                if (_renamed)
                    _name=name;
                this->close(); 
            });
            Cancel.events().click([this](){_renamed = false; this->close(); });

        }
    std::string Name(){return _name;}
    void SetDefLayout   () override
    {
        _DefLayout= "vertical gap=2 <Edit weight=24> <weight=24 <free_left><Buttons gap=10>>                   \n\t ";
    }
    void AsignWidgetToFields() override
    {
        //_commPP  << link( _cp.MaxTgId                 ,       numUpDwMaxTgId  )
        //         << link( _cp.SecLim         , numUpDw_TgBeg,  numUpDw_TgEnd  )
        //         << link( _cp.SecLenLim   ,  numUpDw_SLenMin, numUpDw_SLenMax )
        //    ;
 
	    _place.field("Edit")     << edit  ;
	    _place.field("Buttons" ) << OK <<   Cancel  ;
                               
    }                                        

};

#endif