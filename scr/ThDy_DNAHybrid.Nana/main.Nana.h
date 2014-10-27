#ifndef main_nana_H
#define main_nana_H


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
using namespace ParamGUIBind;

class ThDyNanaForm ;
 
class TableRes  : public nana::form, public EditableForm
{   
    using List  = nana::listbox;
    using Table = CTable<TmGPos> ;
    using index = Table::index;
    struct value
    {
        Table  *table;
        value (Table &t) :table {&t}{};
        virtual float _value(index row,  index col)const =0 ;
        float operator()(index row,  index col){return _value(row,col);}
    }   ;
    struct Tm : value
    {
        float _value(index row,  index col) const override
        {
            return table->at(row,col )._Tm;
        }
        Tm(Table &t) :value {t}{};
    };
    struct G : value
    {
        float _value(index row,  index col) const override
        {
            return table->at(row,col )._G;
        }
        G(Table &t) :value {t}{};
    };
    struct Pos : value
    {
        float _value(index row,  index col)const override
        {
            return table->at(row,col )._Pos;
        }
         Pos(Table &t) :value {t}{};
   };

    std::shared_ptr<Table> _table;
    List                   _list { *this };

    nana::button      _bTm {*this,STR("Tm")},       //nana::toolbar     _tbar { *this };
                           _bG  {*this,STR("G" )},   
                           _bPos {*this,STR("Pos")}; 

    int                    n_dec{ 1 },   n_len{ 6 };
    
    Tm                     _Tm;
    G                      _G;
    Pos                    _Pos;
    value                  *val { &_Tm} ;
    std::size_t            mTm, mG, mP;
 
    void SetValType(value &val_)
    { 
        _list.auto_draw(false);
        bool freeze{true};
        freeze=_list.freeze_sort(freeze);

        val = &val_;
        Repopulate();
        
        _list.auto_draw(true);
        //_list.unsort();
        _list.freeze_sort(freeze);
    }
    void Repopulate()
    {
        for (auto &i : _list.at(0))
            i.resolve_from(i.value<index>());
    }
    class ListTableMaker : public List::resolver_interface <index>
    {
        int     &n_dec,   &n_len;
        value   **val  ;

       nana::string decode(size_t col, const index &row) const override
        {
           if (col)        return print ( (**val)  (row,index(col-1) ));

           return nana::charset(  (**val) .table->TitRow(row)  );
        }
        void encode(index&, std::size_t col, const nana::string& txt) const override
        {
           //if (col)
           //    (*table)(row,col-1)._Tm= CtoK(wstr_f(txt   ));
           //table->TitRow(row)=nana::charset(txt );
        }
  
        nana::string print(float n) const
        {
            static const int    blen{ 50 } ;
            static nana::char_t val_[blen]  ;

            swprintf(val_, blen, STR("% *.*f"), n_len, n_dec, n );
            return val_;
        }
     public:
        ListTableMaker( value *&val_, int &dec , int &len) : val{&val_},  n_len{len}, n_dec{dec}{}
        //void SetValType(value *&val_){ val = &val_;};
        //void SetFormat(int dec=1 , int len=6){ n_len=len; n_dec=dec;}
    };

    bool comp(index col, nana::any* row1_, nana::any*row2_, bool reverse)
    {
                float  v1{ (*val)(*row1_->get<index> (),col-1) }, 
                       v2{ (*val)(*row2_->get<index> (),col-1) };
                return reverse?  v2<v1 : v1<v2 ;
    }
    void SetDefLayout   () override
    {
        _DefLayout= 
	"vertical                  	\n\t"
	"	  <weight=25 <toolbar weight=200 ><>>       	\n\t"
	"	   <_list  >       	\n\t"
	"	 	\n\t"
 
            ;
    }
    void AsignWidgetToFields() override
    {
 	    _place.field("toolbar"       ) <<_bTm << _bG << _bPos ;
 	    _place.field("_list"         ) <<_list;
     }
 public:
     TableRes    (std::shared_ptr<CTable<TmGPos>> table)  : _table(table), _Tm{*table.get()}, _G{*table.get()}, _Pos{*table.get()},  
                nana::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                EditableForm    (nullptr, *this, nana::charset( table->TitTable() ), STR("TableTm.lay.txt")) 
   {
        //nana::API::zoom_window(*this, true);
        caption( nana::string(STR("Table Tm: ") +  _Titel));
        //_tbar.append(STR("Tm"));
        //_tbar.append(STR("G"));
        //_tbar.append(STR("Pos"));

        InitMyLayout();
        SelectClickableWidget( _list);
        SelectClickableWidget( *this);

        _list.auto_draw(false);
                
        _list.resolver(ListTableMaker (val,n_dec,n_len));

        _list.append_header(STR("Seq")  , 120);
        for (index col = 1; col <= table->totalCol(); ++col)
        {    
            _list.append_header(nana::charset(  table->TitColumn(col-1) ) , 100);
            _list.set_sort_compare(col,[col,this](const nana::string&, nana::any* row1_, const nana::string&, nana::any*row2_, bool reverse)
            {
                 return comp(col,row1_,row2_,reverse);
            });
        }

        for (index row = 0; row < table->totalRow(); ++row)
            _list.at(0).append(row).value  ( row );

        _list.auto_draw(true);

        //MakeResponive();
        _bTm .events().click([this]()
                        {
                            SetFormat(1);
                            SetValType(_Tm);
                            caption( nana::string(STR("Table Tm: ") +  _Titel));
                            _bTm .pushed(true);
                            _bG  .pushed(false);
                            _bPos.pushed(false);
                            _menuProgram.checked(mTm, true);
                        });
        _bG  .events().click([this]()
                        {
                            SetFormat(1);
                            SetValType(_G);
                            caption( nana::string(STR("Table G: ") +  _Titel));
                            _bTm .pushed(false);
                            _bG  .pushed(true);
                            _bPos.pushed(false);
                            _menuProgram.checked(mG, true);
                        });
        _bPos.events().click([this]()
                        {
                            SetFormat(0);
                            SetValType(_Pos);
                            caption( nana::string(STR("Table Pos: ") +  _Titel));
                            _bTm .pushed(false);
                            _bG  .pushed(false);
                            _bPos.pushed(true );
                            _menuProgram.checked(mP, true);
                        });

        _bTm .enable_pushed(true).pushed(true);
        _bG  .enable_pushed(true).pushed(false);
        _bPos.enable_pushed(true).pushed(false);


        _menuProgram.append_splitter();
        
        mTm=_menuProgram.append     ( STR("Show Tm")    , [&](nana::menu::item_proxy& ip)  { Click( _bTm); })
                        .check_style( nana::menu::checks::option)
                        .index();

        mG=_menuProgram.append      ( STR("Show delta G"), [&](nana::menu::item_proxy& ip) { Click( _bG);  })
                        .check_style( nana::menu::checks::option)
                        .index();

        mP=_menuProgram.append      ( STR("Show Pos")    , [&](nana::menu::item_proxy& ip) { Click( _bPos);})
                        .check_style( nana::menu::checks::option)
                        .index();
    }
        void SetFormat(int dec=1 , int len=6){  n_len=len; n_dec=dec; }
};

class SetupPage : public CompoWidget
{
    ThDyNanaForm       &_Pr;
    FilePickBox         _results            { *this, STR("Results:") } ;

    FilePickBox         _targets            { *this, STR("Targets:") }  ;
    nana::checkbox      _chkTargRecDir      { *this, STR("Targets - Recur Dir") },
                        _chkTargOnlyStruct  { *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _nTsec              { *this, STR("Non template seq:"),STR("FindSonden-OSB.NonTarg.lay.txt")};
    nana::checkbox      _chk_nTgRecDir      { *this, STR("Non Targets - Recur Dir") },
                        _chk_nTgOnlyStruct  { *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _PCRfiltre          { *this, STR("PCR-filtre:")};

    FilePickBox         _PrimersFilePCR     { *this, STR("Primers seq. file:") };
    nana::checkbox      _chkPrimRecDir      { *this, STR("Primers - Recur Dir") },
                        _chkPrOnlyStruct    { *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _Prob_uArr          { *this, STR("Probes seq. file:") };
    nana::checkbox      _chkProbRecDir      { *this, STR("Probes - Recur Dir") },
                        _chkProbOnlyStruct  { *this, STR("Only reproduce Dir Structure") };

    OpenSaveBox         _NNParamFile        { *this, STR("NN param:")};

    nana::combox        comBoxSalMeth       { *this}, 
                        comBoxTAMeth        { *this};
    nana::NumUnitUpDown numUpDowTgConc      { *this, STR("Target Conctr:"      ), 50, 0.1    , 1000000,  "nM"}, 
                        numUpDowSalConc     { *this, STR("Salt Conc [Cations]:"), 50, 0.0001 , 10000,    "mM"} , 
                        numUpDowTa          { *this, STR("Temp. Anneling:"     ), 55, 40     , 75,       "°C"},  
                        numUpDowSdConc      { *this, STR("Sonde Conctr:"       ), 0.8, 0.001 , 1000,     "µM"}  ;
    nana::button        _set_def_proj       { *this,STR("Set as Def. project") },
                       _load_def_proj       { *this,STR("ReLoad Def. project") };

    nana::checkbox      ckBx_savTm          { *this, STR("Tm"    ) },
                        ckBx_savPos         { *this, STR("Pos"   ) },
                        ckBx_savG           { *this, STR("G"     ) },
                        ckBx_savAlign       { *this, STR("Align" ) },
                        ckBx_savProj        { *this, STR("Proj"  ) },
                        ckBx_savG_Plasm     { *this, STR("G->Plasmid") },
                        ckBx_savTm_Plasm    { *this, STR("Tm->Plasmid") },
                        ckBx_savLog         { *this, STR("log"     ) },
                        ckBx_savExportSond  { *this, STR("Exp. probes" ) },
                        ckBx_savExportTarg  { *this, STR("Exp. targets") },
                        ckBx_savNNParam     { *this, STR("load NNparam") },
                        ckBx_loadNNParam    { *this, STR("save NNparam") }/*,*/
                        ;

    BindGroup          _setup;

    void  SetDefLayout   () override
    {
        _DefLayout =
	"vertical      gap=3        			\n\t"
	"		   < weight=400     gap=5     <weight=2><  vertical min=50    max=800 gap=2 			\n\t"
	"		               		             <  <Project>      weight=23 >      				\n\t"
	"				                         <  <Results>      weight=23 >      				\n\t"
	"			                             <    <     weight=22 _targets>               weight=50       vertical <gap=10  weight=23  <weight=10%><TargOpt   ><weight=10%>    >   <>>       			\n\t"
	"			                             <    <     weight=22 _nTsec  >              weight=50       vertical <gap=10  weight=23  <weight=10%>< nTargOpt ><weight=10%>    >   <>>       	\n\t"
	"			                             <    <     weight=22 _PCRfiltre  >         weight=50       vertical <gap=10  weight=23  <weight=10%><_PCRfiltreOpt ><weight=10%>    >   <>>       	\n\t"
	"			                             <    <     weight=22 _PrimersFilePCR>weight=50       vertical <gap=10  weight=23  <weight=10%><_PrimersFilePCROpt ><weight=10%>    >   <>>       	\n\t"
	"			                             <    <     weight=22 _Prob_uArr   >    weight=50         vertical <gap=10  weight=23  <weight=10%><_Prob_uArrOpt ><weight=10%>    >   <>>       	\n\t"
	" 		                             <  <NN_param >     weight=23 >      				\n\t"
	"				                         <min=50 <weight=2>  <vertical min=50 max=200 gap=2 buttons>  <>  >			\n\t"
	"		                            >                                                                             	\n\t"
	"	                              <  vertical weight=120 <vertical weight=210 checks> <>  >   	                          	\n\t"
	"	       >			\n\t"
	"							\n\t"
	"		   < weight=46  gap=2  <>  <vertical ConcST   weight=200  gap=2>  			\n\t"
	"		                                         <>  <vertical ConcSaltTa   weight=230  gap=2>   			\n\t"
	"		                                         <>  <vertical   weight=250 <SMeth gap=2>          				\n\t"
	"				                                                                                <AMeth gap=2>   > 			\n\t"
	"		                                         <>  >      				\n\t"
	"					\n\t"
	"				\n\t"
	"			\n\t"
	"		\n\t"
	"	\n\t"

    
            ;
        _nTsec        .ResetLayout(105);
        _PCRfiltre    .ResetLayout (60 );
        _PrimersFilePCR.ResetLayout (90 );
        _Prob_uArr    .ResetLayout (90 );
        _NNParamFile  .ResetLayout (90 );

        numUpDowSdConc.ResetLayout (80 );  
        numUpDowTa.    ResetLayout (90 );  
        numUpDowTgConc.ResetLayout (80 );
        numUpDowSalConc.ResetLayout (110 );

    }
    virtual void  AsignWidgetToFields () final  override;
    void  MakeResponive();
    void  SaveProj();
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
    void  setAsDefProject();
    void  RestDefPr	 ( )	;	// Restore (USE) Deff  Proj File

public:     
    OpenSaveBox         _proj       { *this, STR("Project:") };

    SetupPage (ThDyNanaForm& tdForm);

    static FilePickBox& AddFastaFiltre(FilePickBox &fpb)
    {
        return fpb.add_filter(FastaFiltre( ));
        //return fpb.add_filter({ {STR("fasta")       , STR("*.fas;*.fasta"     ) },
        //                        {STR("NCBI BLAST")  , STR("*-Alignment.xml"   ) },
        //                        {STR("GB"        )  , STR("*.gb;*-sequence.xml")},
        //                        {STR("Text"      )  , STR("*.txt"             ) },
        //                        {STR("All sequences"), STR("*.fas;*.fasta;*.txt;*-Alignment.xml;*.gb;*-sequence.xml")},
        //                        {STR("All files" )  , STR("*.*"               ) }
        //                     });
    }
    static FilePickBox::filtres FastaFiltre( )
    {
        return FilePickBox::filtres       { {STR("fasta")       , STR("*.fas;*.fasta"     ) },
                                            {STR("NCBI BLAST")  , STR("*-Alignment.xml"   ) },
                                            {STR("GB"        )  , STR("*.gb;*-sequence.xml")},
                                            {STR("Text"      )  , STR("*.txt"             ) },
                                            {STR("All sequences"), STR("*.fas;*.fasta;*.txt;*-Alignment.xml;*.gb;*-sequence.xml")},
                                            {STR("All files" )  , STR("*.*"               ) }
                                          } ;
    }


    void AddMenuItems(nana::menu& menu);
    void LoadProjectAndReg(nana::string file);
    void LoadProject(nana::string file);
};

class SeqExpl : public CompoWidget
{
    using Tree = nana::treebox;
    using Node = Tree::item_proxy;
    using List = nana::listbox;

    ThDyNanaForm       &_Pr;
    Tree                _tree{ *this };
    List                _list{ *this };
    bool				_showAllseq{true}, _showFiltered{true};
    std::vector<CSec*>      _dragSec;
    std::vector<CMultSec*>  _dragMSec;

    nana::button    _loadFile     {*this,STR("Load"   )},       //nana::toolbar  _tbar { *this };
                    _re_loadFile  {*this,STR("reLoad" )},   
                    _loadDir      {*this,STR("Load"   )},       
                    _re_loadDir   {*this,STR("reLoad" )},
                    _scanDir      {*this,STR("Scan"   )},
                    _cut          {*this,STR("Cut"    )},
                    _paste        {*this,STR("Paste"  )},
                    _del          {*this,STR("Del"    )},
                    _cutSec       {*this,STR("Cut"    )},
                    _delSec       {*this,STR("Del"    )},
                    _show_locals_s{*this,STR("local"  )},
                    _show_filt_s  {*this,STR("filtr"  )}
                    ; 
    nana::tooltip    _loadFileTT {_loadFile,STR("File load: Add a group of sequences from a file")},
                          _re_loadFileTT ;  
;  

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
			case 2: {Temperature t= sec->NonDegSet() ? sec->NonDegSet()->_Local._Tm.Ave() : sec->_Tm.Ave();
				     swprintf(val,blen, STR("% *.*f °C"), 6, 1,  KtoC( t ) );}
                    return val;
            case 3: swprintf(val,blen,     STR("%*d")  , 5,           sec->Degeneracy());
                    return val;  
            case 4: return nana::charset( sec->Description());
			case 5: return nana::charset( sec->Len()<slen ? sec->charSequence() 
				                                          : (char *)(sec->Sequence().substr (0,slen)).c_str()  );

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
        _DefLayout = 
	                "vertical                                                 \n\t"
	                "		  <weight=23 <toolbar weight=680 margin=2 ><>>    \n\t"
	                "		  <      <Tree  > |75% <List >   >      		  \n\t"
	                "				                                          \n\t"
	                "		                                                  \n\t"
	                "	                                                      \n\t"

            ;
    }
    void AsignWidgetToFields() override
    {
 	    _place.field("toolbar") << "   Files:"  << _loadFile << _re_loadFile   
                                << 10           << _paste           
                                << "      Dir:" << _loadDir  << _re_loadDir  << _scanDir  
                                << 10           << _cut      << _del      
                                << "      Seq:" << _show_locals_s  << _show_filt_s 
								<< 10           << _cutSec   << _delSec
                                ;
        _place.field("Tree"   ) << _tree;
        _place.field("List"   ) << _list;
    }
    void MakeResponive();

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
                                    .fgcolor( s->Filtered()     ?   0xFF00FF 
                                                                :   0x0     );//nana::color::gray_border );
    }

    Node AddRoot          (CMultSec*ms)  
    {
        nana::string name = nana::charset(ms->_name);
        return _tree.insert(name, name).value(ms).check(ms->Selected());
    }
    bool isRoot(Node &node)
    {
        return node.level() == 1;
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

    Node AddNewSeqGr  (Tree::item_proxy& node) ;
    Node AddMSeqFiles (const std::string &file, bool  all_in_dir) ;
    Node Replace      (Tree::item_proxy& tn, CMultSec *ms, const std::string& Path, bool all_in_dir);
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
public:
    SeqExpl(ThDyNanaForm& tdForm);
    void ShowFindedProbes_in_mPCR(bool show_=true);
    void RefreshProbes_mPCR(bool show_=true);
    void AddMenuItems(nana::menu& menu);
    void InitTree();
};

class FindSondenPage : public CompoWidget
{    
    ThDyProject &_Pr;
    BindGroup   _findSond;
    nana::NumUnitUpDown _Gmin     {*this, STR("G :"    ), -5, -10 , 10,"kcal/mol"},   _Gmax   {*this, STR(""), -1, -10, 10, "kcal/mol"}, 
                        _Tmmin    {*this, STR("Tm :"   ), 57,  40 , 60,"°C"      },  _Tmmax   {*this, STR(""), 63,  45, 75, "°C"      }, 
                        _Lengthmin{*this, STR("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, STR(""), 35,  15, 40, "nt"      },
                        _MaxG     {*this, STR("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, STR("Tm :"  ), 30,  10 , 60,"°C"}, 
                        _MinG     {*this, STR("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, STR("Max Tm"), 10, -10, 75, "°C"}, 
                        _MinSelfG {*this, STR("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, STR("Max Tm"), 10, -10, 75, "°C"}, 	
                        numUpDw_MinTargCov{ *this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" }, 
                        numUpDw_MaxTargCov{ *this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" } ;
    nana::tooltip _Gmintt     {_Gmin, STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be included"    ) }/*,   _Gmax   {*this, STR(""), -1, -10, 10, "kcal/mol"}, 
                             _Tmmin    {*this, STR("Tm :"   ), 57,  40 , 60,"°C"      },  _Tmmax   {*this, STR(""), 63,  45, 75, "°C"      }, 
                             _Lengthmin{*this, STR("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, STR(""), 35,  15, 40, "nt"      },
                             _MaxG     {*this, STR("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, STR("Tm :"  ), 30,  10 , 60,"°C"}, 
                             _MinG     {*this, STR("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, STR("Max Tm"), 10, -10, 75, "°C"}, 
                             _MinSelfG {*this, STR("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, STR("Max Tm"), 10, -10, 75, "°C"}, 	
                             numUpDw_MinTargCov{ *this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" }, 
                             numUpDw_MaxTargCov{ *this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" }*/ ;

    nana::button        _design{*this, STR("Design !" )}, 
                            _compare{*this, STR("Compare !")};

    nana::checkbox      chkBx_unique{*this, STR("Report unique probes, ")}, 
                             chkBx_common{*this, STR("Report common probes, ")}, 
                             chkBx_showFindedProbes{*this, STR("Show Finded Probes")};
	nana::tooltip       chkBx_uniqueTT{chkBx_unique, STR("For each target seq, probes with hybrid on it, AND maximum on a given percent of the OTHER targets will be reported")};
	nana::tooltip       chkBx_commonTT{chkBx_common, STR("All probes with hybrid on at laest the given percent of targets will be reported")};
public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    void SetDefLayout   () override
    {
      _DefLayout=  
	"vertical      gap=2                 	\n\t"
	"	    <weight=10     >       	\n\t"
	"	     <weight=235 gap=8 <weight=5><weight=350 vertical <weight=100 <weight=320 Sonde  grid[3,4]>> 	\n\t"
	"                                                                                                         <weight=10>	\n\t"
	"	                                                                                                     <TargCov    grid[2,2]     weight=45                     >    	\n\t"
	"                                                                                                        <weight=10> 	\n\t"
	"	                                                                                                     <weight=40 <   <><weight=300   gap=20 Run>       > >    	\n\t"
	"	                                                                                                     <weight=10>                                 	\n\t"
	"	                                                              >   <><weight=230 gap=1 vertical  options>    >   	\n\t"
	"	     <weight=23   <weight=140><Output>   <> >       	\n\t"
	"	 	\n\t"
        ;


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

            ;
        
        /// Use room (wd,w,h) in combination with a <Table grid[W,H]>
	    _place.field("Sonde" )     << "Probes" << "Min."         << "   Max."   
                                   <<    _place.room(_Gmin ,2,1) <<   _Gmax
                                   <<    _place.room(_Tmmin,2,1) <<   _Tmmax
                                   << _place.room(_Lengthmin,2,1)<<   _Lengthmax  ;
        _place.field("TargCov" )   << chkBx_unique << numUpDw_MinTargCov       
                                   << chkBx_common << numUpDw_MaxTargCov     	;
        _place.field("Run"     )   << _design	<< _compare	;
                 
	    _place.field("options" )   << "Probe-target"    <<  _MaxG     << _MinTm
                                   << "Probe-non-target"<<  _MinG     << _MaxTm
                                   << "Probe-self"      <<  _MinSelfG << _MaxSelfTm  
                                    ;    
        _place.field("Output"  )   << chkBx_showFindedProbes;

    }

    void Run_Design(bool design);
};

class uArray : public CompoWidget
{ public: 
    ThDyNanaForm      &_Pr;
    nana::button  _do_uArray{*this, STR(" uArray ! ")};
    BindGroup          _uArray;

    uArray (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= "vertical      gap=2             \n\t"
	        "  < weight=23>       \n\t "
            "  <<><_do_uArray  vertical min=50 max=200><> weight=50>       \n\t "

            ;
    }
    void AsignWidgetToFields() override
    {
	    _place.field("_do_uArray"         )<<_do_uArray;
    }

  private: void buttuArray_Click(); //	  Run      _IPrgPar_mPCR
};

class MplexPCR : public CompoWidget
{ public: 
    ThDyNanaForm      &_Pr;
    nana::button  _do_mPCR{*this, STR(" PCR ! ")};
    BindGroup          _mPCR;

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
       //_mPCR<< /*link(   _Pr._mPCR._InputSondeFile , _PrimersFilePCR)*/

       //     ;

	    _place.field("_do_mPCR"         )<<_do_mPCR;
	    //_place.field("checks"          )<<"save result";
    }

  private: void buttPCR_Click(); //	  Run      _IPrgPar_mPCR
};

class TmCalcPage : public CompoWidget
{
    ThDyProject             &_Pr;
    nana::textbox           sec_                {*this},  
                            sec2align_          {*this},  
                            txtBx_ResultSec     {*this},  
                            txtBx_ResultSec2Align{*this};
    nana::checkbox          chkBx_Tm_save_asPCR {*this, STR("save")},   
                            chkBx_align         {*this, STR("align")},
                            chkBx_copy_rev      {*this, STR("rev")},    
                            chkBx_copy_compl    {*this, STR("cpl")};
    nana::button            run_                {*this, STR("Tm !")},
                            copy_f_s_2          {*this, STR("copy")},   
                            copy_s              {*this, STR("c")},
                            copy_s_a            {*this, STR("c")};      
    nana::label             error_              {*this, STR("no error")};
    nana::NumberBox         Tm_min_Up{*this}, Tm_Up{*this}, Tm_max_Up{*this} ,
                            Tm_min_Dw{*this}, Tm_Dw{*this}, Tm_max_Dw{*this} ,
                            Tm_min_In{*this}, Tm_In{*this}, Tm_max_In{*this} ,
                            G_min_Up{*this},   G_Up{*this},  G_max_Up{*this} ,
                            G_min_Dw{*this},   G_Dw{*this},  G_max_Dw{*this} ,
                            G_min_In{*this},   G_In{*this},  G_max_In{*this} ;

    BindGroup              _TmCalc;
public:     
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
	    _place.field("CopyBut"  )<< nana::vplace::room (copy_f_s_2, 1, 2)<< copy_s << copy_s_a ;
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
            (nana::msgbox(*this,STR("Error during Tm calculation !"), nana::msgbox::button_t::ok)<<e.what()) (  ) ;
		    return;
		}	 	        		 
        txtBx_ResultSec      .caption (nana::charset (_Pr._TmCal._AlignedSec        ));
        txtBx_ResultSec2Align.caption (nana::charset (_Pr._TmCal._AlignedSec2Align  ));
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

class RenameFrom : public nana::form, public EditableForm
{
    std::string     _name;
    bool            _renamed    {false};
    nana::textbox   edit        {*this};
    nana:: button   OK          {*this, STR("rename")}, 
                    Cancel      {*this, STR("abort" )};
    BindGroup       _bind     ;


  public:
    RenameFrom(nana::widget* owner, std::string name) : 
            _name(name),  
             nana::form  (nana::rectangle( nana::point(150,500), nana::size(300,50) )),
             EditableForm(owner, *this, STR("Rename") )     
        {
            edit.caption(nana::charset(_name) );
            InitMyLayout();
            OK.events().click([this]()
            {
                std::string  name=nana::charset(edit.caption()); 
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
        _DefLayout= "vertical gap=2 <Edit weigth=24> <weigth=24 <free_left><Buttons gap=10>>                   \n\t ";
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

class ThDyNanaForm : public nana::form, public EditableForm , public ThDyProject
{
    using tabbar = nana::tabbar<nana::string> ;
	tabbar                     tabbar_     {*this};
    SetupPage                  setup_      {*this};
    FindSondenPage             findSond_   {*this};
    MplexPCR                   mPCR_       {*this};
    uArray                     uArr_       {*this}; 
    TmCalcPage                 tmCalc_     {*this}; 
    nana::NumUnitUpDown        numUpDwMaxTgId  {*this, STR("Max. ident.:"        ), 99,  50 , 100 ,   "%"}, 
                               numUpDw_TgBeg   {*this, STR("Beg.:"               ),  0,   0 , 100000,"nt"},    /// rev !!
                               numUpDw_TgEnd   {*this, STR("End.:"               ),  0,   0 , 100000,"nt"},    /// rev !!	
                               numUpDw_SLenMin {*this, STR("Min.Len.:"           ),  0,   0 , 100000,"nt"},
                               numUpDw_SLenMax {*this, STR("Max.Len.:"           ),  0,   0 , 100000,"nt"};
    BindGroup                  _commPP     ;

  public:    
    std::vector<std::unique_ptr<nana::form>> _results;
    SeqExpl                         mExpl_      {*this};

    ThDyNanaForm (int argc, char *argv[])
                  :nana::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                   EditableForm    (nullptr, *this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")) 
   {
        //nana::API::zoom_window(*this, true);
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //background (0xEEEEEE);
        //foreground(1);
       
        add_page( setup_    ); // 0
        add_page( mExpl_    ); // 1
        add_page( findSond_ ); // 2
        add_page( mPCR_     ); // 3
        add_page( uArr_     ); // 4
        add_page( tmCalc_   ); // 5

        tabbar_.activate (1);

        setup_._proj.FileNameOnly(nana::charset ( ProjetFile()  ));
        try{ 
			    if ( argc > 1 )
				    setup_._proj.FileNameOpen(nana::charset( argv[1]) )   ;
			    else
				    load() ;						// cuando no existe Def Project: 1ra vez que se usa el prog??
		    }
    	catch ( std::exception& e )      // Por ejemplo cuando no existe Def Project: 1ra vez que se usa el prog.
		{   
            (nana::msgbox(*this, STR("Error during initial project load !\n\t"), nana::msgbox::button_t::ok)
                             .icon(nana::msgbox::icon_information )
                            << e.what()    << "\n\n A new Default Project will be created. "
                          ).show (  ) ;
		    save_defPr() ; 					                
        }

		//this->comBoxTAMeth->SelectedIndex  = SMStLucia;    
		_cp.Actualize_All_NNp();
        LoadSequences();
		this->_uArr._probesMS->CreateNonDegSetRec();
		this->_TmCal._probesMS->CreateNonDegSetRec();
		this->_mPCR._probesMS->CreateNonDegSetRec();
        mExpl_.InitTree();

        InitMyLayout();

        setup_.AddMenuItems (_menuBar.push_back(STR("P&roject")));     // 0
        mExpl_.AddMenuItems (_menuBar.push_back(STR("&Sequences")));   // 1 
        AddMenuProgram();                                              // 2
        
        SelectClickableWidget( _menuBar);

		//nana::fn_group<void(tabbar&, value_type&)> active;

		tabbar_.events().activated( [this]()// const tabbar & t, const tabbar::value_type& tab)
		{
			bool enable	= tabbar_.activated( )==1;
			auto &m		= _menuBar. at(1);
			auto sz		= m.size();

			for(int i=0; i< sz; ++i)
				m.enabled(i,enable);
		});


   }

    //~ThDyNanaForm();
    void SetDefLayout   () override
    {
        _DefLayout=
                "vertical      gap=2                   \n\t "
	            "vertical      gap=2                   	\n\t"
	            "	        <weight=25>                   	\n\t"
	            "	        <PagesTag    weight=23 >      	\n\t"
	            "	        <Pages       min=255   >      	\n\t"
	            "	        < <weight=30><TargetsOptions><weight=10> weight=23>      	\n\t"
	            "	        <weight=1 >                   	\n\t"
	            "	        < weight=23 <><Firma weight=280><> >                   	\n\t"
	            "	 	\n\t"
            ;

        numUpDwMaxTgId .ResetLayout (60,40,30 );  
        numUpDw_TgBeg  .ResetLayout (35,40,30 );  
        numUpDw_TgEnd  .ResetLayout (35,40,30 );  
        numUpDw_SLenMin.ResetLayout (60,40,30 );   
        numUpDw_SLenMax.ResetLayout (60,40,30 );  
    }
    void AsignWidgetToFields() override
    {
        _commPP  << link( _cp.MaxTgId                 ,       numUpDwMaxTgId  )
                 << link( _cp.SecLim         , numUpDw_TgBeg,  numUpDw_TgEnd  )
                 << link( _cp.SecLenLim   ,  numUpDw_SLenMin, numUpDw_SLenMax )
            ;
 
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
    void ShowExpl(){tabbar_.activate(1);}
};


#endif