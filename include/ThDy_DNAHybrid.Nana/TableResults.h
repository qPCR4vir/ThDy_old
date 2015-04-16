/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\TableResults.h
*
* @brief 
*/

#ifndef TableResults_H
#define TableResults_H

#include <nana/gui/widgets/listbox.hpp>
//#include <nana/gui/tooltip.hpp>

//#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/widgets/toolbar.hpp>

#include <../../nana.ext/include/EditableForm.hpp>

#include "thdy_programs\init_thdy_prog_param.h"
#include "matrix.h" 

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

#endif