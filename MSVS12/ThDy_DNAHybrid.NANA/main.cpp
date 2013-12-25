#include <iostream>    // temp, for debugging
#include <fstream>     // temp, for debugging
#include <filesystem>

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>

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
 
class TableRes  : public nana::gui::form, public EditableForm
{   
    using List  = nana::gui::listbox;
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
    int                    n_dec{ 1 },   n_len{ 6 };
    Tm  _Tm;
    G   _G;
    Pos _Pos;
    value                  *val { &_Tm} ;
 
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
    //class ListTableTmMaker : public ListTableMaker
    //{
    //   float value(index col,  index row)const override
    //   {
    //       return table(row,index(col)-1)._Tm;
    //   }
    //    
    // public:
    //    //using ListTableMaker::ListTableMaker;
    //    ListTableTmMaker( CTable<TmGPos> &table, int &dec , int &len) : ListTableMaker( table, dec , len){}
    //};
    //class ListTableGMaker : public ListTableMaker
    //{
    //   float value(index col,  index row)const override
    //   {
    //       return table(row,index(col)-1)._G;
    //   }
    //    
    // public:
    //    //using ListTableMaker::ListTableMaker;
    //    ListTableGMaker( CTable<TmGPos> &table, int &dec , int &len) : ListTableMaker( table, dec , len){}
    //};

    bool comp(index col, nana::any* row1_, nana::any*row2_, bool reverse)
    {
                float  v1{ (*val)(*row1_->get<index> (),col-1) }, 
                       v2{ (*val)(*row2_->get<index> (),col-1) };
                return reverse?  v2<v1 : v1<v2 ;
    }
    void SetDefLayout   () override
    {
        _DefLayout= 
            "vertical      gap=2             \n\t"
	        "  <_list  >       \n\t "
 
            ;
    }
    void AsignWidgetToFields() override
    {
 	    _place.field("_list"         )<<_list;
     }
 public:
     TableRes    (std::shared_ptr<CTable<TmGPos>> table)  : _table(table), _Tm{*table.get()}, _G{*table.get()}, _Pos{*table.get()},  
                nana::gui::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                EditableForm    (nullptr, *this, nana::charset( table->TitTable() ), STR("TableTm.lay.txt")) 
   {
        //nana::gui::API::zoom_window(*this, true);
        caption( nana::string(STR("Table Tm: ") +  _Titel));
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

        _menuProgram.append_splitter();
        
        _menuProgram.append(   STR("Show Tm")  ,   [&](nana::gui::menu::item_proxy& ip) 
        {
            SetFormat(1);
            SetValType(_Tm);
            caption( nana::string(STR("Table Tm: ") +  _Titel));

        });
        _menuProgram.check_style(_menuProgram.size()-1, nana::gui::menu::check_t::check_option);

        _menuProgram.append(   STR("Show delta G")  ,   [&](nana::gui::menu::item_proxy& ip) 
        {
            SetFormat(1);
            SetValType(_G);
            caption( nana::string(STR("Table G: ") +  _Titel));
        });
        _menuProgram.check_style(_menuProgram.size()-1, nana::gui::menu::check_t::check_option);
        _menuProgram.append(   STR("Show Pos")  ,   [&](nana::gui::menu::item_proxy& ip) 
        {
            SetFormat(0);
            SetValType(_Pos);
            caption( nana::string(STR("Table Pos: ") +  _Titel));
        });
        _menuProgram.check_style(_menuProgram.size()-1, nana::gui::menu::check_t::check_option);
        //_menuProgram.checked    (_menuProgram.size()-1, false );

        //MakeResponive();
    }
        void SetFormat(int dec=1 , int len=6){  n_len=len; n_dec=dec; }
};

class SetupPage : public CompoWidget
{
    ThDyProject        &_Pr;
    FilePickBox         _results    { *this, STR("Results:") } ;

    FilePickBox         _targets    { *this, STR("Targets:") }  ;
    nana::gui::checkbox _chkTargRecDir    { *this, STR("Targets - Recur Dir") },
                        _chkTargOnlyStruct{ *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _nTsec      {*this, STR("Non template seq:"),STR("FindSonden-OSB.NonTarg.lay.txt")};
    nana::gui::checkbox _chk_nTgRecDir    { *this, STR("Non Targets - Recur Dir") },
                        _chk_nTgOnlyStruct{ *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _PCRfiltre  { *this, STR("PCR-filtre:")};

    FilePickBox         _PrimersFilePCR{*this, STR("Primers seq. file:") };
    nana::gui::checkbox _chkPrimRecDir    { *this, STR("Primers - Recur Dir") },
                        _chkPrOnlyStruct{ *this, STR("Only reproduce Dir Structure") };

    FilePickBox         _Prob_uArr{*this, STR("Probes seq. file:") };
    nana::gui::checkbox _chkProbRecDir    { *this, STR("Probes - Recur Dir") },
                        _chkProbOnlyStruct{ *this, STR("Only reproduce Dir Structure") };

    OpenSaveBox         _NNParamFile {*this, STR("NN param:")};

    nana::gui::combox               comBoxSalMeth   {*this}, 
                                    comBoxTAMeth    {*this};
    nana::gui::NumUnitUpDown        numUpDowTgConc  {*this, STR("Target Conctr:"      ), 50, 0.1 , 1000,  "µM"}, 
                                    numUpDowSalConc {*this, STR("Salt Conc [Cations]:"), 50, 0.1 , 10000000,"µM"} , 
                                    numUpDowTa      {*this, STR("Temp. Anneling:"     ), 55,  40 , 75,    "°C"},  
                                    numUpDowSdConc  {*this, STR("Sonde Conctr:"       ), 50, 0.1 , 1000,  "µM"}  ;
    nana::gui::button  _set_def_proj    {*this,STR("Set as Def. project") },
                       _load_def_proj   {*this,STR("ReLoad Def. project") };
    BindGroup          _setup;

    void  SetDefLayout   () override
    {
        _DefLayout =
	"vertical      gap=2         		\n\t"
	"	   < weight=400   <weight=2><vertical min=50    max=800 gap=2 		\n\t"
	"	               		                   <<Project     >      weight=23 >      			\n\t"
	"			                                   <<Results     >      weight=23 >      			\n\t"
	"		                                       <    _targets         weight=50      gap=2   vertical <gap=10 <weight=10%><TargOpt ><weight=10%> >  >       		\n\t"
	"		                                       <    _nTsec        weight=50     gap=2   vertical <gap=10 <weight=10%><nTargOpt><weight=10%> >  >       		\n\t"
	"		                                       <    _PCRfiltre      weight=50     gap=2   vertical <gap=10 <weight=10%><_PCRfiltreOpt><weight=10%> >  >       		\n\t"
	"		                                       <    _PrimersFilePCR weight=50     gap=2   vertical <gap=10 <weight=10%><_PrimersFilePCROpt><weight=10%> >  >       		\n\t"
	"		                                       <    _Prob_uArr      weight=50     gap=2   vertical <gap=10 <weight=10%><_Prob_uArrOpt><weight=10%> >  >       		\n\t"
	"		                                       <<NN_param  >     weight=23 >      			\n\t"
	"			                                     <min=50 <weight=2>  <vertical min=50 max=200 gap=2 buttons>  <>  >		\n\t"
	"	                                                                                                                                                                                                     >      <weight=120 checks>   	>		\n\t"
	"					\n\t"
	"	   < weight=46  gap=2  <>  <vertical ConcST   weight=200  gap=2>  		\n\t"
	"	                                         <>  <vertical ConcSaltTa   weight=230  gap=2>   		\n\t"
	"	                                         <>  <vertical   weight=250 <SMeth gap=2>          			\n\t"
	"			                                                                                <AMeth gap=2>   > 		\n\t"
	"	                                         <>  >      			\n\t"
	"			\n\t"
	"		\n\t"
    
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
    void  AsignWidgetToFields() override
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
	    _place.field("checks"   )    << "save result" ;

	    _place.field("ConcST"  )        << numUpDowSdConc   
                                        << numUpDowTgConc ;
	    _place.field("ConcSaltTa"  )    << numUpDowSalConc 
                                        << numUpDowTa ;
	    _place.field("SMeth"  )         << " Salt Correct. Method:"	   <<  comBoxSalMeth;
	    _place.field("AMeth"  )         << " ThDy Align. Method"       <<  comBoxTAMeth ;
    }
    void  MakeResponive()
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

        _set_def_proj .make_event	<nana::gui::events::click> ([&](){ setAsDefProject() ;} );
        _load_def_proj.make_event	<nana::gui::events::click> ([&](){ RestDefPr      () ;} );
    }
    void  SaveProj()
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
    void  setAsDefProject()
    {
		string caption = "Set current setting as Default project";
		string message = std::string("This will overwrite the current Default Project." )        + "\n\n"
						+  "Are you sure?"   + "\n\n"
						+ "\tYes:  The default project will be overwrited. " + "\n"
						+ "\tNo:  No action will be taken. " + "\n"
						;
		switch ( (nana::gui::msgbox(  *this, nana::charset (caption) , nana::gui::msgbox::yes_no )
                        <<  message
                    ).icon(nana::gui::msgbox::icon_question ) .show (  ))
		{
			case  nana::gui::msgbox::pick_yes :  
                                    _Pr.save_asDefPr() ; 					 // crea el Def Project.
				return;

			case  nana::gui::msgbox::pick_no:    
            default:;
        }
    }
    void  RestDefPr	 ( )		// Restore (USE) Deff  Proj File
	{		 
        try{
		       _Pr.load_defPr() ;			// cuando no existe Def Project: 1ra vez que se usa el prog??
		    }
		catch ( std::exception& e)
		{ 
			(nana::gui::msgbox ( STR("Error loading Def Project" ) )<< e.what()).show() ;
 		}		 
	}

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
    static nana::gui::filebox::filtres FastaFiltre( )
    {
        return nana::gui::filebox::filtres{ {STR("fasta")       , STR("*.fas;*.fasta"     ) },
                                            {STR("NCBI BLAST")  , STR("*-Alignment.xml"   ) },
                                            {STR("GB"        )  , STR("*.gb;*-sequence.xml")},
                                            {STR("Text"      )  , STR("*.txt"             ) },
                                            {STR("All sequences"), STR("*.fas;*.fasta;*.txt;*-Alignment.xml;*.gb;*-sequence.xml")},
                                            {STR("All files" )  , STR("*.*"               ) }
                                          } ;
    }


    void AddMenuItems(nana::gui::menu& menu)
    {
  //      		_menuFile.append  (STR("&Open..."   ),[this](nana::gui::menu::item_proxy& ip){ this->_OSbx.open(nana::string(nana::charset(this->_textBox.filename())));this->OpenFile()  ;}                );
  //      _menuFile.append  (STR("&Save"      ),[&](nana::gui::menu::item_proxy& ip){  ForceSave(nana::string(nana::charset(_textBox.filename())) ) ;}   );
		//_menuFile.append  (STR("Save &As..."),[&](nana::gui::menu::item_proxy& ip){ _OSbx.save(nana::string(nana::charset(_textBox.filename())));SaveFile() ;} );

        menu.append(STR("New"    )  , [&](nana::gui::menu::item_proxy& ip)  {  ;  } );
        menu.append(STR("Open...")  , [&](nana::gui::menu::item_proxy& ip)  
        { 
            _proj.open(_proj.FileName()); /*OpenProj() ;*/   
            if (!_proj.Canceled())
                LoadProject( nana::charset(_proj.FileName()));
            //this->_OSbx.open(nana::string(nana::charset(this->_textBox.filename())));this->OpenFile()  ;}                );

        } );
        menu.append(STR("Save...")  , [&](nana::gui::menu::item_proxy& ip)  
        { 
            _proj.save(_proj.FileName()); 
            if( ! _proj.Canceled () )   
               _Pr.save (nana::charset ( _proj.FileName())); 

            //SaveProj() ;  
        } );
        menu.append_splitter();
        menu.append(STR("Set as deffault") , [&](nana::gui::menu::item_proxy& ip)  {;  });
        menu.append(STR("Restore deffault"), [&](nana::gui::menu::item_proxy& ip)  {;  });
        menu.append_splitter();
        menu.append(STR("Exit"    )  , [&](nana::gui::menu::item_proxy& ip)  {  ;  } );

    }
    void LoadProjectAndReg(nana::string file)
    {
        LoadProject(file);
 		_proj.FileName(file  );
   }
    void LoadProject(nana::string file)
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
			switch ( (nana::gui::msgbox(  *this, nana::charset (caption) , nana::gui::msgbox::yes_no_cancel )
                            <<  message
                        ).icon(nana::gui::msgbox::icon_error) .show (  ))
			{
				case  nana::gui::msgbox::pick_yes :  
					    _Pr.load_defPr();
                        _proj.FileName(nana::charset ( _Pr.ProjetFile ()  ));
					return;

				case  nana::gui::msgbox::pick_no:    
                        _proj.open (nana::charset (file));
                        if ( !  _proj.Canceled() )
                                LoadProject(nana::charset (  _proj.FileName()));
                        return;
			}
		}
	}
};

class SeqExpl : public CompoWidget
{
    using Tree = nana::gui::treebox;
    using Node = Tree::item_proxy;
    using List = nana::gui::listbox;

    ThDyNanaForm       &_Pr;
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

public:
    SeqExpl(ThDyNanaForm& tdForm);
    void ShowFindedProbes_in_mPCR(bool show_=true);
    void RefreshProbes_mPCR(bool show_=true);
    void AddMenuItems(nana::gui::menu& menu);
    void InitTree();
};

class FindSondenPage : public CompoWidget
{    
    ThDyProject &_Pr;
    BindGroup   _findSond;
    nana::gui::NumUnitUpDown _Gmin     {*this, STR("G :"    ), -5, -10 , 10,"kcal/mol"},   _Gmax   {*this, STR(""), -1, -10, 10, "kcal/mol"}, 
                             _Tmmin    {*this, STR("Tm :"   ), 57,  40 , 60,"°C"      },  _Tmmax   {*this, STR(""), 63,  45, 75, "°C"      }, 
                             _Lengthmin{*this, STR("Length:"), 20,  15 , 35,"nt"      }, _Lengthmax{*this, STR(""), 35,  15, 40, "nt"      },
                             _MaxG     {*this, STR("Max G" ), 10, -10, 30, "kcal/mol" },  _MinTm   {*this, STR("Tm :"  ), 30,  10 , 60,"°C"}, 
                             _MinG     {*this, STR("Min G" ), 15, -10 , 30,"kcal/mol" }, _MaxTm    {*this, STR("Max Tm"), 10, -10, 75, "°C"}, 
                             _MinSelfG {*this, STR("Min G" ), 10, -10 , 30,"kcal/mol" }, _MaxSelfTm{*this, STR("Max Tm"), 10, -10, 75, "°C"}, 	
                             numUpDw_MinTargCov{ *this, STR("Min. target coverage:"), 100.0, 0.0 , 100.0,"%" }, 
                             numUpDw_MaxTargCov{ *this, STR("Max. target coverage:"),   0.0, 0.0 , 100.0,"%" } ;

    nana::gui::button        _design{*this, STR("Design !" )}, 
                            _compare{*this, STR("Compare !")};
    nana::gui::checkbox      chkBx_unique{*this, STR("Report unique probes, ")}, 
                             chkBx_common{*this, STR("Report cariel.rodriguezommon probes, ")}, 
                             chkBx_showFindedProbes{*this, STR("Show Finded Probes")};
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

class MplexPCR : public CompoWidget
{ public: 
    ThDyNanaForm      &_Pr;
    nana::gui::button  _do_mPCR{*this, STR(" PCR ! ")};
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

class ThDyNanaForm : public nana::gui::form, public EditableForm , public ThDyProject
{
    nana::gui::tabbar<nana::string> tabbar_     {*this};
    SetupPage                       setup_      {*this};
    FindSondenPage                  findSond_   {*this};
    MplexPCR                        mPCR_       {*this};
    TmCalcPage                      tmCalc_     {*this}; 
    nana::gui::NumUnitUpDown        numUpDwMaxTgId  {*this, STR("Max. ident.:"        ), 99,  50 , 100 ,   "%"}, 
                                    numUpDw_TgBeg   {*this, STR("Beg.:"               ),  0,   0 , 100000,"nt"},    /// rev !!
                                    numUpDw_TgEnd   {*this, STR("End.:"               ),  0,   0 , 100000,"nt"},    /// rev !!	
                                    numUpDw_SLenMin {*this, STR("Min.Len.:"           ),  0,   0 , 100000,"nt"},
                                    numUpDw_SLenMax {*this, STR("Max.Len.:"           ),  0,   0 , 100000,"nt"};
    BindGroup                       _commPP     ;

  public:    
    std::vector<std::unique_ptr<nana::gui::form>> _results;
    SeqExpl                         mExpl_      {*this};

    ThDyNanaForm (int argc, char *argv[])
                  :nana::gui::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                   EditableForm    (nullptr, *this, STR("ThDy DNA Hybrid"), STR("ThDy.lay.txt")) 
   {
        nana::gui::API::zoom_window(*this, true);
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

        setup_._proj.FileNameOnly(nana::charset ( ProjetFile()  ));
        try{ 
			    if ( argc > 1 )
				    setup_._proj.FileNameOpen(nana::charset( argv[1]) )   ;
			    else
				    load() ;						// cuando no existe Def Project: 1ra vez que se usa el prog??
		    }
    	catch ( std::exception& e )      // Por ejemplo cuando no existe Def Project: 1ra vez que se usa el prog.
		{   
            (nana::gui::msgbox(*this, STR("Error during initial project load !\n\t"), nana::gui::msgbox::button_t::ok)
                             .icon(nana::gui::msgbox::icon_information )
                            << e.what()    << "\n\n A new Default Project will be created. "
                          ).show (  ) ;
		    save_defPr() ; 					                
        }

		//this->comBoxTAMeth->SelectedIndex  = SMStLucia;    

        LoadSequences();
        mExpl_.InitTree();

        InitMyLayout();

        setup_.AddMenuItems (_menuBar.push_back(STR("P&roject")));
        mExpl_.AddMenuItems (_menuBar.push_back(STR("&Sequences")));
        AddMenuProgram();
        
        SelectClickableWidget( _menuBar);

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
    void ShowExpl(){tabbar_.active(1);}
};

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
            Run_Design(true );  });    
        _compare.make_event<nana::gui::events::click>([&]() 
        {
            Run_Design(false);  });  
     }
   MplexPCR::MplexPCR            (ThDyNanaForm& tdForm)
        : _Pr             (tdForm), 
          CompoWidget     (tdForm, STR("MplexPCR"), STR("MplexPCR.lay.txt"))
    {

        _do_mPCR      .make_event <nana::gui::events::click>([&](){buttPCR_Click ();});

        InitMyLayout();
        SelectClickableWidget( *this);
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

        menu.append(STR("Add a new, empty, group for sequences")  , [&](nana::gui::menu::item_proxy& ip) {  AddNewSeqGr(_tree.selected());    } );
        menu.append(STR("Add a group of sequences from a file..."), [&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb .add_filter ( SetupPage::FastaFiltre( )                   )
               .title      ( STR("Add a group of sequences from a file") );

            if (fb()) 
               AddMSeqFiles(nana::charset(fb.file()), false);
        });
        menu.append(STR("Add a tree of groups of sequences from a directory..."),[&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb .add_filter ( SetupPage::FastaFiltre( )                   )
               .title(STR("Add a tree of groups of sequences from a directory"));
            if (fb()) 
                AddMSeqFiles(nana::charset(fb.file()), true);
        });

        menu.append_splitter();

        menu.append(STR("Reproduce only the structure of directory..."),[&](nana::gui::menu::item_proxy& ip) 
        {
            nana::gui::filebox  fb{ *this, true };
            fb .add_filter ( SetupPage::FastaFiltre( )                   )
               .title(STR("Reproduce the structure of directory..."));
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
    void SeqExpl::InitTree()
    {
        _list.auto_draw(false);
        _tree.auto_draw(false);

        CMultSec *ms=_Pr._cp._pSeqTree.get();
        for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
			populate( AddRoot( ms->CurMSec())) ;

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

