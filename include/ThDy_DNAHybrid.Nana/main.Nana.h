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
#include <nana/gui/widgets/group.hpp>

#include <../../nana.ext/include/EditableForm.hpp>
#include <../../nana.ext/include/Numer.hpp>
#include "../../nana.ext/include/nanaBind.hpp"

#include "thdy_programs\init_thdy_prog_param.h"
#include "matrix.h" 
#include "common_basics.h" 


using namespace ParamGUIBind;

class ThDyNanaForm ;
 
using List = nana::listbox;

class TableRes  : public nana::form, public EditableForm
{   
    using Table = CTable<TmGPos> ;
    using index = Table::index;
    struct value
    {
        Table  *table;
        int      n_len{ 6 }, n_dec{ 1 }  ;

        virtual ~value(        ){}
        value (Table &t) :table {&t}{}

        virtual float val  (index row,  index col)const =0 ;
                float operator()(index row,  index col){return val(row,col);}
        
        virtual nana::string str(index row,  index col) const
        {
            nana::string s (n_len, 0);

            auto l=swprintf((wchar_t*)s.data(), n_len+1 , STR("% *.*f"), n_len, n_dec, val(row,col) );
            //s.resize(l);
            return s;
        }

        virtual bool         return_bg(){return false;}
        virtual nana::color bg_color(index row,  index col)
        {return List::scheme_type().header_bgcolor.get_color(); }
    }   ;
    struct Tm : value
    {
        float val(index row,  index col) const override
        {
            return table->at(row,col )._Tm;
        }
        Tm(Table &t) :value {t}{};
        bool return_bg() override {return true;}
        nana::color bg_color(index row,  index col) override 
        {
            Temperature t=val(row,col);

            Temperature min=20.0, max=63.0;
            double fade_rate=  t<min? 0.0 : t>max? 1.0 : (t-min)/(max-min);
            nana::color bgc(nana::colors::red);
            return bgc.blend(nana::colors::blue, fade_rate) ;
        }
    };
    struct G : value
    {
        float val(index row,  index col) const override
        {
            return table->at(row,col )._G;
        }
        G(Table &t) :value {t}{};
    };
    struct Pos : value
    {
        float val(index row,  index col)const override
        {
            return table->at(row,col )._Pos;
        }
        Pos(Table &t) :value {t} {n_dec=0;};
   };

    std::shared_ptr<Table> _table;
    List                   _list { *this };

    nana::button           _bTm  {*this,STR("Tm" )},       //nana::toolbar     _tbar { *this };
                           _bG   {*this,STR("G"  )},   
                           _bPos {*this,STR("Pos")},
                           _mix  {*this, STR("Consolide")}; 

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
            i.resolve_from(i.value<Index>());
    }

    bool comp(index col, nana::any* row1_, nana::any*row2_, bool reverse)
    {
                float  v1{ val->val( row1_->get<Index>()->row , col-1) }, 
                       v2{ val->val( row2_->get<Index>()->row , col-1) };
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
     TableRes    (std::shared_ptr<CTable<TmGPos>> table)  : 
                            _table(table), 
                            _Tm{*table.get()}, _G{*table.get()}, _Pos{*table.get()},  
                            nana::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                            EditableForm    (nullptr, *this, nana::charset( table->TitTable() ), STR("TableTm.lay.txt")) 
   {
        //nana::API::zoom_window(*this, true);
        caption( nana::string(STR("Table Tm: ")) +  _Titel);
        //_tbar.append(STR("Tm"));
        //_tbar.append(STR("G"));
        //_tbar.append(STR("Pos"));

        InitMyLayout();
        SelectClickableWidget( _list);
        SelectClickableWidget( *this);

        _list.auto_draw(false);
                
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
            _list.at(0).append( Index{this,row}, true );

        _list.auto_draw(true);

        //MakeResponive();
        _bTm .events().click([this]()
                        {
                            SetFormat(1);
                            SetValType(_Tm);
                            caption( nana::string(STR("Table Tm: ")) +  _Titel);
                            _bTm .pushed(true);
                            _bG  .pushed(false);
                            _bPos.pushed(false);
                            _menuProgram.checked(mTm, true);
                        });
        _bG  .events().click([this]()
                        {
                            SetFormat(1);
                            SetValType(_G);
                            caption( nana::string(STR("Table G: ")) +  _Titel);
                            _bTm .pushed(false);
                            _bG  .pushed(true);
                            _bPos.pushed(false);
                            _menuProgram.checked(mG, true);
                        });
        _bPos.events().click([this]()
                        {
                            SetFormat(0);
                            SetValType(_Pos);
                            caption( nana::string(STR("Table Pos: ")) +  _Titel);
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
    void SetFormat(int dec=1 , int len=6)  // ??
    {  
        _Tm.n_len=_G.n_len=len; _Tm.n_dec=_G.n_dec=dec;
    }

    friend struct Index;

    struct Index
    {
        TableRes* table;
        index       row;

        friend List::oresolver& operator<<(List::oresolver& ores, const TableRes::Index& i)
        {
            auto &t = *i.table->_table.get();
            auto &v = *i.table->val;
            ores<< t.TitRow(i.row)   ;
                
            if  (v.return_bg() )
                for (int col=0; col< t.totalCol() ; ++col)
                    ores<< List::cell{ v.str     (i.row, col),
                                       v.bg_color(i.row, col),
                                       nana::colors::white};
            else 
                for (int col=0; col< t.totalCol() ; ++col)
                    ores<< v.str(i.row, col)  ;

            return ores;
        }
    };
    friend List::oresolver& operator<<(List::oresolver& ores, const TableRes::Index& i);

};
       
class SetupPage : public CompoWidget
{
    ThDyNanaForm       &_Pr;
    nana::group         _gr_dir  {*this, STR(" <bold=true> Directories: </>"          ), true};
    FilePickBox         _results            { _gr_dir.inner(), STR("Results: ") } ;

    nana::group         _gr_seq         {_gr_dir.inner(), STR(" <bold=true> Sequences </>File or Directory"      ), true};
    nana::group         _gr_targ        {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _targets            { _gr_targ.inner(), STR("Targets: ") }  ;
    nana::checkbox      _chkTargRecDir      { _gr_targ.inner(), STR("Targets - Recur Dir") },
                        _chkTargOnlyStruct  { _gr_targ.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_ntarg       {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _nTsec              { _gr_ntarg.inner(), STR("Non targets: "),STR("FindSonden-OSB.NonTarg.lay.txt")};
    nana::checkbox      _chk_nTgRecDir      { _gr_ntarg.inner(), STR("Non Targets - Recur Dir") },
                        _chk_nTgOnlyStruct  { _gr_ntarg.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_PCRfiltre       {_gr_seq.inner(), STR(""                                             ), true};
    FilePickBox         _PCRfiltre          { _gr_PCRfiltre.inner(), STR("PCR-filtre: ")};

    nana::group         _gr_PrimersFilePCR  {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _PrimersFilePCR     { _gr_PrimersFilePCR.inner(), STR("Primers: ") };
    nana::checkbox      _chkPrimRecDir      { _gr_PrimersFilePCR.inner(), STR("Primers - Recur Dir") },
                        _chkPrOnlyStruct    { _gr_PrimersFilePCR.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_uArr        {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _Prob_uArr          { _gr_uArr.inner(), STR("Probes: ") };
    nana::checkbox      _chkProbRecDir      { _gr_uArr.inner(), STR("Probes - Recur Dir") },
                        _chkProbOnlyStruct  { _gr_uArr.inner(), STR("Only reproduce Dir Structure") };

    OpenSaveBox         _NNParamFile        { _gr_dir.inner(), STR("NN param: ")};

    nana::group         _gr_salt {*this, STR(" <bold=true> Input & analisis parameters: </>"                 ), true};
    nana::combox        comBoxSalMeth       { _gr_salt.inner()}, 
                        comBoxTAMeth        { _gr_salt.inner()};
    nana::NumUnitUpDown numUpDowTgConc      { _gr_salt.inner(), STR("Target Conctr:"      ), 50, 0.1    , 1000000,  "nM"}, 
                        numUpDowSalConc     { _gr_salt.inner(), STR("Salt Conc [Cations]:"), 50, 0.0001 , 10000,    "mM"} , 
                        numUpDowTa          { _gr_salt.inner(), STR("Temp. Anneling:"     ), 55, 40     , 75,       "�C"},  
                        numUpDowSdConc      { _gr_salt.inner(), STR("Sonde Conctr:"       ), 0.8, 0.001 , 1000,     "�M"}  ;

    nana::button        _set_def_proj       { *this,STR("Set as Def. project") },
                        _load_def_proj      { *this,STR("ReLoad Def. project") };

    nana::group         _gr_checks {*this, STR(" <bold=true> Save in results: </>"                 ), true};
    nana::checkbox      ckBx_savTm          { _gr_checks.inner(), STR("Tm"    ) },
                        ckBx_savPos         { _gr_checks.inner(), STR("Pos"   ) },
                        ckBx_savG           { _gr_checks.inner(), STR("G"     ) },
                        ckBx_savAlign       { _gr_checks.inner(), STR("Align" ) },
                        ckBx_savProj        { _gr_checks.inner(), STR("Proj"  ) },
                        ckBx_savG_Plasm     { _gr_checks.inner(), STR("G->Plasmid") },
                        ckBx_savTm_Plasm    { _gr_checks.inner(), STR("Tm->Plasmid") },
                        ckBx_savLog         { _gr_checks.inner(), STR("log"     ) },
                        ckBx_savExportSond  { _gr_checks.inner(), STR("Exp. probes" ) },
                        ckBx_savExportTarg  { _gr_checks.inner(), STR("Exp. targets") },
                        ckBx_loadNNParam    { _gr_dir.inner()   , STR("load at start") },
                        ckBx_savNNParam     { _gr_checks.inner(), STR("save NNparam") }/*,*/
                        ;

    BindGroup          _setup;

    void  SetDefLayout   () override
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

        _gr_dir .plc().div(" vert "
                         "  <weight=26  Results  >       \n\t"
				         "  <min=280 margin=5 seq>    	    \n\t"
	 		             "  <weight=26  <NN_param><weight=80 ckBx_loadNNParam> >       \n\t" );

        _gr_seq .plc().div(" vert "
                         "  <weight=62  margin=[0,5,0,5] _targets       >    		         \n\t"
				         "  <weight=62  margin=[0,5,0,5] _nTsec         >    		         \n\t"
				         "  <weight=50  margin=[0,5,0,5] _PCRfiltre     >    		         \n\t"
				         "  <weight=62  margin=[0,5,0,5] _PrimersFilePCR>    		         \n\t"
				         "  <weight=67  margin=[0,5,5,5] _Prob_uArr     >    		         \n\t");

        _gr_targ .plc().div(" vert "
                          "<weight=22 dir>                                           \n\t"
                          "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                          "<>                                                        \n\t");

        _gr_ntarg .plc().div( " vert "
                            "<weight=22 dir>                                           \n\t"
                            "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                            "<>                                                        \n\t");

        _gr_PCRfiltre .plc().div(" vert "
                               "<weight=22 dir>                                           \n\t"
                               "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                               "<>                                                        \n\t");

        _gr_PrimersFilePCR .plc().div(" vert "
                                    "<weight=22 dir>                                           \n\t"
                                    "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                                    "<>                                                        \n\t");

        _gr_uArr .plc().div(" vert "
                          "<weight=22 dir>                                           \n\t"
                          "<weight=23 gap=10 <weight=10%>< Opt ><weight=10%>    >   \n\t"
                          "<>                                                        \n\t");

        _gr_checks .plc().div(	" <vertical weight=210 checks>     				\n\t");

        _gr_salt .plc().div(  
            "     <horizontal <> 		                                    \n\t" 
	        "                 <weight=200 vertical ConcST        gap=2>     \n\t"
	        "                 <> 		                                    \n\t" 
	        "		          <weight=230 vertical ConcSaltTa    gap=2>     \n\t"
	        "                 <> 		                                    \n\t" 
            "		          <weight=250 vertical gap=5  <weight=23 SMeth >        	\n\t"
            "				                              <weight=23 AMeth >  >       \n\t"
	        "                 <>  		 >                                  \n\t");
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
 	    _place.field("toolbar") << L"   Files:"  << _loadFile << _re_loadFile   
                               /* << 10  */         << _paste           
                                << L"      Dir:" << _loadDir  << _re_loadDir  << _scanDir  
                               /* << 10 */          << _cut      << _del      
                                << L"      Seq:" << _show_locals_s  << _show_filt_s 
								/*<< 10*/           << _cutSec   << _delSec
                                ;
        _place.field("Tree"   ) << _tree;
        _place.field("List"   ) << _list;
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
                                              (s->Filtered()     ?   0xFF00FF   ///\todo: use codigo
                                                                :   0x0   )  ));//nana::color::gray_border );
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
public:
    SeqExpl(ThDyNanaForm& tdForm);
    void ShowFindedProbes_in_mPCR(bool show_=true);
    void RefreshProbes_mPCR(bool show_=true);
    void AddMenuItems(nana::menu& menu);
    void InitTree();
};

List::oresolver& operator<<(List::oresolver & ores, CSec * const sec )
{
    static const long    blen{ 50 }, slen{ 1000 };
    nana::char_t val[blen];

    swprintf(val,blen,     STR("%*d")  , 6,           sec->Len()       );
    ores <<  sec->Name()  <<  val  ;                                    // col 0: name     // col 1: len

	Temperature t=KtoC( sec->NonDegSet() ? sec->NonDegSet()->_Local._Tm.Ave() : sec->_Tm.Ave());
	swprintf(val,blen, STR("% *.*f �C"), 6, 1,   t );
    Temperature min=57.0, max=63.0;
    double fade_rate=  t<min? 0.0 : t>max? 1.0 : (t-min)/(max-min);
    nana::color tc{static_cast<nana::color_rgb>(0xFFFFFFFF)} , 
                bc = nana::color(nana::colors::red).blend( nana::colors::blue, fade_rate); 
    ores <<  List::cell {val, bc , tc};                                                 //case 2: Tm 

    swprintf(val,blen,     STR("%*d")  , 5,           sec->Degeneracy());            // case 3: deg     // case 4: descr   // sec
    ores <<  val  << sec->Description() <<  (char *)(  sec->Sequence().substr (1, std::min( sec->Len(), slen)).c_str()    );                                                          

    return ores;
}


class FindSondenPage : public CompoWidget
{    
    ThDyProject &_Pr;
    BindGroup   _findSond;
    nana::group         _gr_probes  {*this, STR("<bold=true> Probes: </>"          ), true},
                        _gr_prob_tg {*this, STR("<bold=true> Probe-target: </>"    ), true},
                        _gr_prob_ntg{*this, STR("<bold=true> Probe-non-target: </>"), true},
                        _gr_find_prb{*this, STR(           " Find Probes "         ), true},
                        _gr_probself{*this, STR("<bold=true> Probe-self: </>"      ), true};
    nana::NumUnitUpDown _Gmin     {_gr_probes.inner(), STR("G : "    ), -5, -10 , 10,"kcal/mol"},   _Gmax   {_gr_probes.inner(), STR(""), -1, -10, 10, "kcal/mol"}, 
                        _Tmmin    {_gr_probes.inner(), STR("Tm : "   ), 57,  40 , 60,"�C"      },  _Tmmax   {_gr_probes.inner(), STR(""), 63,  45, 75, "�C"      }, 
                        _Lengthmin{_gr_probes.inner(), STR("Length: "), 20,  15 , 35,"nt"      }, _Lengthmax{_gr_probes.inner(), STR(""), 35,  15, 40, "nt"      },
                        _MaxG     {_gr_prob_tg.inner(), STR("Max G: " ), 10, -10, 30, "kcal/mol" },  _MinTm   {_gr_prob_tg.inner(), STR("Tm: "  ), 30,  10 , 60,"�C"}, 
                        _MinG     {_gr_prob_ntg.inner(), STR("Min G: " ), 15, -10 , 30,"kcal/mol" }, _MaxTm   {_gr_prob_ntg.inner(), STR("Max Tm: "), 10, -10, 75, "�C"}, 
                        _MinSelfG {_gr_probself.inner(), STR("Min G: " ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{_gr_probself.inner(), STR("Max Tm: "), 10, -10, 75, "�C"}, 	
                        numUpDw_MinTargCov{ _gr_find_prb.inner(), STR("max."),   0.0, 0.0 , 100.0,"%" }, 
                        numUpDw_MaxTargCov{ _gr_find_prb.inner(), STR("min."), 100.0, 0.0 , 100.0,"%" } ;
    nana::tooltip _Gmintt     {_Gmin, STR("Only probes with stronger interaction with target (smaller G by selected Ta) will be included"    ) }/*,   _Gmax   {*this, STR(""), -1, -10, 10, "kcal/mol"}, 
                             _Tmmin    {*this, STR("Tm :"   ), 57,  40 , 60,"�C"      },  _Tmmax   {*this, STR(""), 63,  45, 75, "�C"      }, 
                             _Lengthmin{*this, STR("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, STR(""), 35,  15, 40, "nt"      },
                             _MaxG     {*this, STR("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, STR("Tm :"  ), 30,  10 , 60,"�C"}, 
                             _MinG     {*this, STR("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, STR("Max Tm"), 10, -10, 75, "�C"}, 
                             _MinSelfG {*this, STR("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, STR("Max Tm"), 10, -10, 75, "�C"}, 	
                             numUpDw_MinTargCov{ *this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" }, 
                             numUpDw_MaxTargCov{ *this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" }*/ ;

    nana::button        _design {*this, STR("Design !" )}, 
                        _compare{*this, STR("Compare !")};

    nana::checkbox      chkBx_unique{_gr_find_prb.inner(), STR("Unique, with target coverage ")}, 
                        chkBx_common{_gr_find_prb.inner(), STR("Common, with target coverage ")}, 
                        chkBx_showFindedProbes{*this, STR("Show Finded Probes")};
	nana::tooltip       chkBx_uniqueTT{chkBx_unique, STR("For each target seq, probes with hybrid on it, AND maximum on a given percent of the OTHER targets will be reported")};
	nana::tooltip       chkBx_commonTT{chkBx_common, STR("All probes with hybrid on at laest the given percent of targets will be reported")};
public: 
    FindSondenPage(ThDyNanaForm& tdForm);
    void SetDefLayout   () override
    {
      _DefLayout=  
	"vertical   gap=2    margin=5                    			\n\t"
	"		<weight=10     >       			\n\t"
	"	   <weight=260 gap=8 <weight=5> <weight=388 vertical   		                      	\n\t"
	"	                                              <weight=115 <weight=388 Sonde  > >		             	\n\t"
	"		                                           <weight=10>			\n\t"
	"			                                       <weight=72 TargCov        >    			\n\t"
	"		                                           <weight=10> 			\n\t"
	"			                                       <weight=40 <   <> <weight=300   gap=20 Run>  <>     > >    			\n\t"
	"			                                       <weight=10>                                 			\n\t"
	"			                         >   <> <weight=230 gap=1 vertical  options> <weight=5>                          			\n\t"
	"	   >   			\n\t"
	"		<weight=23   <weight=140> <Output>   <> >       		          	\n\t"
	"			 			\n\t"
	"		\n\t"
        ;


    _gr_probes .plc().div("vert < Sonde  margin=2 gap= 2 grid=[2,4]  	    \n\t"
	"					                                    		\n\t"
	"						                                  >	\n\t");

    _gr_prob_tg .plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_prob_ntg.plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_probself.plc().div("<  margin=2 gap= 2 vertical   options>");
    _gr_find_prb.plc().div("<  margin=5 gap= 2 TargCov grid=[2,2]>");

         _Gmin.ResetLayout     (45,40,55 );   _Gmax.ResetLayout     (1,40,75 );
        _Tmmin.ResetLayout     (45,40,55 );  _Tmmax.ResetLayout     (1,40,75 );
        _Lengthmin.ResetLayout (45,40,55 );  _Lengthmax.ResetLayout (1,40,75 );
 
        _MaxG.ResetLayout     (110,45,50 );   
        _MinTm.ResetLayout    (110,45,50 );  

        _MinG.ResetLayout     (110,45,50 );   
        _MaxTm.ResetLayout    (110,45,50 );   

        _MinSelfG.ResetLayout  (110,45,50);   
        _MaxSelfTm.ResetLayout (110,45,50 );  

        numUpDw_MinTargCov.ResetLayout (30,40,40 );  
        numUpDw_MaxTargCov.ResetLayout (30,40,40 );  
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
        
        /// Use room (wd,w,h) in combination with a <Table grid=[W,H]>
	            _place["Sonde"]    << _gr_probes ;
                _place["TargCov"]  << _gr_find_prb ;     
        _place.field("Run"     )   << _design	<< _compare	;
	    _place.field ("options" )  << _gr_prob_tg <<  _gr_prob_ntg     << _gr_probself;
        _place.field("Output"  )   << chkBx_showFindedProbes;

          _gr_probes.plc()["Sonde"]  << STR("                               Min." )<< STR("           Max."  ) 
                                   <<   _Gmin     <<   _Gmax  
                                   <<   _Tmmin    <<   _Tmmax  
                                   << _Lengthmin  <<   _Lengthmax  ; 

        _gr_find_prb.plc()["TargCov"]<< chkBx_unique << numUpDw_MinTargCov       
                                   << chkBx_common << numUpDw_MaxTargCov     	;
                 
	    _gr_prob_tg .plc()["options"] <<   _MaxG     << _MinTm;
        _gr_prob_ntg.plc()["options"] <<   _MinG     << _MaxTm;
        _gr_probself.plc()["options"] <<   _MinSelfG << _MaxSelfTm    ;


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
    nana::group             primers             {*this, STR("<bold=true> Primers: </>" ), true}, 
                            interaction         {*this, STR("<bold=true> Interaction: </>" ), true},
                            align               {*this, STR("<bold=true> Alignment: </>" ), true}; 
    nana::textbox           sec_                {primers.inner()},  
                            sec2align_          {primers.inner()},  
                            txtBx_ResultSec     {align.inner()},  
                            txtBx_ResultSec2Align{align.inner()};
    nana::checkbox          chkBx_Tm_save_asPCR {*this, STR("save")},   
                            chkBx_align         {*this, STR("align")},
                            chkBx_copy_rev      {primers.inner(), STR("rev")},    
                            chkBx_copy_compl    {primers.inner(), STR("cpl")};
    nana::button            run_                {*this, STR("Tm !")},
                            copy_f_s_2          {primers.inner(), STR("copy")},   
                            copy_s              {primers.inner(), STR("c")},
                            copy_s_a            {primers.inner(), STR("c")};      
    nana::label             error_              {primers.inner(), STR("no error")};
    nana::NumberBox         Tm_min_Up{interaction.inner()}, Tm_Up{interaction.inner()}, Tm_max_Up{interaction.inner()} ,
                            Tm_min_Dw{interaction.inner()}, Tm_Dw{interaction.inner()}, Tm_max_Dw{interaction.inner()} ,
                            Tm_min_In{interaction.inner()}, Tm_In{interaction.inner()}, Tm_max_In{interaction.inner()} ,
                            G_min_Up {interaction.inner()},  G_Up{interaction.inner()},  G_max_Up{interaction.inner()} ,
                            G_min_Dw {interaction.inner()},  G_Dw{interaction.inner()},  G_max_Dw{interaction.inner()} ,
                            G_min_In {interaction.inner()},  G_In{interaction.inner()},  G_max_In{interaction.inner()} ;

    BindGroup              _TmCalc;
public:     
    TmCalcPage (ThDyNanaForm& tdForm);

    void SetDefLayout   () override
    {
        _DefLayout= 	
	"vertical      gap=8  min=150    margin=5                              		\n\t"
	"		       < weight=95  primers  >                                               		\n\t"
	"		        < weight=95 horizontal gap=2  <weight=120 vertical gap=2  margin=[15,45,0,15]   Left  >              	\n\t"
	"                                                                       <weight=320   Table     >  >           		\n\t"
	"		        < weight=70 vertical  ResAlign>    		\n\t"
	"		\n\t"
            ;

         primers.plc().div("vert <weight=50 horizontal margin=[0,5,0,5] <min=100 vertical gap=2 InputSec>                            "  
                        "                                       <weight=50 gap=1 CopyBut grid=[2,2]  collapse(0,0,1,2)> > \n\t "
                        "<weight=23 horizontal  <weight=20>"
                        "                       <min=50    error     > "
                        "                       <weight=80 rev_compl >     >         \n\t  ");

         interaction.plc().div("vert <min=280    margin=[0,5,5,5] Table    grid=[7,4]    >                "  );

         align.plc().div("vert < weight=50 vertical margin=[0,5,0,5]  ResAlign  >                            " );


    }
    void AsignWidgetToFields() override
    {
	    _place.field("primers"  )<< primers    ;
	    _place.field("Left"     )<< run_        << chkBx_align;
	    _place.field("Table"    )<< interaction;
	    _place.field("ResAlign" )<< align      ;

	    primers.plc()["InputSec" ]<< sec_          << sec2align_ ;
	    primers.plc()["CopyBut"  ]<<  copy_f_s_2   << copy_s      << copy_s_a ;
	    primers.plc()["error"    ]<< error_        ;
	    primers.plc()["rev_compl"]<< chkBx_copy_rev << chkBx_copy_compl ;

	    interaction.plc()["Table" ]<< L""          << L"   min-" << L"Tm(�C)"   << L"-max" << L"   min-"  << L"G(kJ)"   << L"-max   "   
	                             << L"Up"        << Tm_min_Up << Tm_Up        << Tm_max_Up<<G_min_Up   <<  G_Up      <<  G_max_Up   
	                             << L"Down"      << Tm_min_Dw << Tm_Dw        << Tm_max_Dw<<G_min_Dw   <<  G_Dw      <<  G_max_Dw   
	                             << L"Interact"  << Tm_min_In << Tm_In        << Tm_max_In<<G_min_In   <<  G_In      <<  G_max_In  ;


        align.plc()["ResAlign" ]  << txtBx_ResultSec << txtBx_ResultSec2Align ;
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
        txtBx_ResultSec      .caption (nana::string(nana::charset(_Pr._TmCal._AlignedSec )       ));
        txtBx_ResultSec2Align.caption (nana::string(nana::charset(_Pr._TmCal._AlignedSec2Align  )));
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
    RenameFrom(nana::window owner, std::string name) : 
            _name(name),  
             nana::form  (nana::rectangle( nana::point(150,500), nana::size(300,50) )),
             EditableForm(owner, *this, STR("Rename") )     
        {
            edit.caption(nana::string(nana::charset(_name) ));
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
    nana::label                _firma     {*this, STR("ArielVina.Rodriguez@fli.bund.de")};
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
        //bgcolor (nana::color_rgb( 0xEEEEEE));  ///\todo: use codigo
        //foreground(1);
       //this->scheme().background(col)
        //this->b
        add_page( setup_    );// setup_.ReCollocate(); // 0 
        add_page( mExpl_    ); //mExpl_.ReCollocate();// 1
        add_page( findSond_ );// findSond_.ReCollocate();// 2
        add_page( mPCR_     ); //mPCR_.ReCollocate();// 3
        add_page( uArr_     );// uArr_.ReCollocate();// 4
        add_page( tmCalc_   );// tmCalc_.ReCollocate();// 5

        tabbar_.activate (1);

        setup_._proj.FileNameOnly(nana::charset ( ProjetFile()  ));
        try{ 
			    if ( argc > 1 )
				    setup_._proj.FileNameOpen(nana::charset( argv[1]) )   ;
			    else
				    load() ;						
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
        try{ 
		        _cp.Actualize_All_NNp();
                LoadSequences();
		    }
    	catch ( std::exception& e )      //  
		{   
            (nana::msgbox(*this, STR("Error during sequence or NN parametr load !\n\t"), nana::msgbox::button_t::ok)
                             .icon(nana::msgbox::icon_information )
                            << e.what()     
                          ).show (  ) ;
		    save_defPr() ; 					                
        }
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
	        "vertical      gap=2                   	\n\t"
	        "	 vertical      gap=2                   		\n\t"
	        "		        <weight=25>                   		\n\t"
	        "		        <PagesTag    weight=23 >      		\n\t"
	        "		        <Pages       min=255   >      		\n\t"
	        "		        < <weight=30><TargetsOptions min=640 max=1000><weight=10> weight=23>      		\n\t"
	        "		        <weight=1 >                   		\n\t"
	        "		        < weight=21 <><Firma weight=280> <weight=3 > >                   		\n\t"
	        "		 		\n\t"
	        "		\n\t"
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
	    _place.field("Firma"  )         << STR(" ArielVina.Rodriguez@fli.bund.de")
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