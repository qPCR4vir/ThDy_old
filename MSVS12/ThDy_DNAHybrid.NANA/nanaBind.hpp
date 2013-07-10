#ifndef nanaBind_H
#define nanaBind_H

#include <memory>
#include <vector>
 
//#include "ThDySec\matrix.h" 
#include "ThDySec\common_basics.h" 
#include "ThDy_programs/init_prog_param.h" 
#include "ThDy_programs/ParamGUIBind.hpp" 
#include <../temp/EditableForm.hpp>
#include <nana/gui/widgets/checkbox.hpp>

namespace ParamGUIBind 
{
class nanaWidgetBind : public virtual IParBind 
{
  protected:
    nana::gui::widget& _w;
public: 
    nanaWidgetBind (nana::gui::widget& w): _w(w)
    {
        _w.make_event <nana::gui::events::focus>([&](const nana::gui::eventinfo& ei)
        {  if (!ei.focus.getting) 
                UpDateProg ();
        });
    }
    void          updateForm(nana::string val){ _w.caption (val); }
    nana::string  getFormVal(       ){ return   _w.caption (   ); }
};

class Bind_checkbox : public nanaWidgetBind  
{ 	
 public:				
    Bind_checkbox ( nana::gui::checkbox& c):nanaWidgetBind(c){} 

    void updateForm(bool val){  static_cast <nana::gui::checkbox&>(_w).check  (val); }
    bool getFormVal(){  return  static_cast <nana::gui::checkbox&>(_w).checked(   ); }
};
class Bind_UnitUpDw : public nanaWidgetBind  
{ 	
 public:				
    Bind_UnitUpDw ( nana::gui::NumUnitUpDown & c):nanaWidgetBind(c){} 

    void   updateForm(double val){  static_cast <nana::gui::NumUnitUpDown&>(_w).Value  (val); }
    double getFormVal(  ){  return  static_cast <nana::gui::NumUnitUpDown&>(_w).Value  (   ); }
    void   updateForm(double val, const CUnit::unit_name &un){  static_cast <nana::gui::NumUnitUpDown&>(_w).Value  (val, un); }
    double getFormVal(const CUnit::unit_name &un    ){  return  static_cast <nana::gui::NumUnitUpDown&>(_w).Value  (un     ); }
};
class Bind_NumUpDw : public nanaWidgetBind  
{ 	
 public:				
    Bind_NumUpDw ( nana::gui::NumerUpDown & c):nanaWidgetBind(c){} 

    void   updateForm(double val){  static_cast <nana::gui::NumerUpDown&>(_w).Value  (val); }
    double getFormVal(  ){  return  static_cast <nana::gui::NumerUpDown&>(_w).Value  (   ); }
};



class Bind_CParamC_str_widget : public nanaWidgetBind, public Bind_CParamC_str  
{ 	
 public:				
    Bind_CParamC_str_widget (CParamC_str &p, nana::gui::widget& c):Bind_CParamC_str(p),nanaWidgetBind(c){SetDef();} 

    void UpDateForm()override { updateForm(             nana::charset ( getProgVal() ))         ;}
	void UpDateProg()override { updateProg(std::string( nana::charset ( getFormVal() )).c_str());}
};
class BindBool   : public Bind_checkbox, public Bind_CParamBool  
{ 	
 public:				
    BindBool (CParamBool &p, nana::gui::checkbox& c):Bind_CParamBool(p),Bind_checkbox(c){SetDef();} 

    void	UpDateForm(	 )	override {         updateForm(getProgVal()); }
	void	UpDateProg(	 )	override {         updateProg(getFormVal()); }
};


upPbind link(CParamC_str &p, nana::gui::widget&  w)
{
    return  upPbind(new Bind_CParamC_str_widget (p, w));
}
upPbind link(CParamC_str &p, FilePickBox&        w)
{
    return  link(p, w._fileName);
}
upPbind link(CParamBool &p, nana::gui::checkbox& c)
{
    return  upPbind(new BindBool (p, c));
}






            template <class Bind_Param,class Bind_Widget> 
class Bind_Param_Widget : public     Bind_Param, public Bind_Widget
{
    //Bind_Param  & _p;
    //Bind_Widget & _w;
public:
    template <class Param,class Widget> 
    Bind_Param_Widget (Param &p, Widget& w):Bind_Param(p),Bind_Widget(w){SetDef();} 

    void	UpDateForm(	 )	override {         updateForm(getProgVal()); }
	void	UpDateProg(	 )	override {         updateProg(getFormVal()); }

};
            template <class Bind_Param,class Bind_Widget,class Param,class Widget> 
upPbind link( Param &p,  Widget&  w)
{
    return  upPbind(new Bind_Param_Widget<Bind_Param,Bind_Widget> (p, w));
}

}
#endif

//class Bind_C_str_txtbox : public Bind_txtbox, public Bind_C_str //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
//{ 	
// public:				
//    Bind_C_str_txtbox (CParamC_str &p, nana::gui::textbox& c):Bind_C_str(p),Bind_txtbox(c){SetDef();} 
//
//    void UpDateForm()override { updateForm(             nana::charset ( getProgVal() ))         ;}
//	void UpDateProg()override { updateProg(std::string( nana::charset ( getFormVal() )).c_str());}
//};
//class PrgPrmNanaBind : public nanaWidgetBind, public ProgPBind
//{
//  public:
//      PrgPrmNanaBind(IParam& p,nana::gui::widget& w):ProgPBind(p), nanaWidgetBind(w){}
//
//};
//class Bind_bool : public PrgPrmNanaBind //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
//{ 	
// public:				
//    Bind_bool (CParamBool &p, nana::gui::checkbox& c):PrgPrmNanaBind(p,c){} 
//
//    void updateForm(bool val){ static_cast <nana::gui::checkbox&>(_w).check  (val); }
//    void updateProg(bool val){ static_cast <CParamBool&         >(_p).set    (val); }
//    bool getProgVal(){ return  static_cast <CParamBool&         >(_p).get    () ; }
//    bool getFormVal(){ return  static_cast <nana::gui::checkbox&>(_w).checked() ; }
//    
//    void	UpDateForm(	 )	override { updateForm(getProgVal()); }
//	void	UpDateProg(	 )	override { updateProg(getFormVal()); }
//};
//class Bind_txtbox : public nanaWidgetBind  
//{ 	
// public:				
//    Bind_txtbox ( nana::gui::textbox& c):nanaWidgetBind(c){} 
//
//    //void          updateForm(nana::string val){ static_cast <nana::gui::textbox&>(_w).caption (val); }
//    //nana::string  getFormVal(       ){ return   static_cast <nana::gui::textbox&>(_w).caption (   ); }
//};
     //BindGroup&	operator<<	(upPbind&& pb){  _pb.push_back(std::move(pb)) ;	  return *this;	 }
//upPbind link(CParamC_str &p, nana::gui::textbox& c)
//{
//    return  upPbind(new Bind_C_str_txtbox (p, c));
//}

//class Bind_C_str_b : public PrgPrmNanaBind //    Bind a Control.Text with a C_str variable ---- TagBinding_C_str    :
//{ 	protected:	 ;
//	public:				Bind_C_str_b (C_str &s, Control^ c):TagBinding(c)	, _p(s)	{} 
//
//	virtual void		set(Object^ s)	override{						_p.Take (  CreateCharFromManString((String^ )s)	) ; }
//	virtual Object^		get(		 )	override{ return  gcnew String(	_p.Get()								) ; }
//	virtual void		UpDateForm(	 )	override{ _c->Text = (String^)get()	;}
//	virtual void		UpDateP(	 )	override{ set (_c->Text)				;}
//};
//ref		  class TagBinding_C_str    : public TagBinding_C_str_b  //    Bind a Control.Text with a C_str variable using Trim ---- TagBinding_strTrim    :
//{ 	public:				TagBinding_C_str   (C_str &s, Control^ c):TagBinding_C_str_b(s,c)		{SetDef()		;}
//};
//ref		  class TagBinding_strTrim  : public TagBinding_C_str_b  //    Bind a Control.Text with a C_str variable using Trim ---- TagBinding_strTrim    :
//{ 	public:				TagBinding_strTrim (C_str &s, Control^ c):TagBinding_C_str_b(s,c)		{SetDef()		;}
//	virtual void		set(Object^ s)	override{						_p.TakeTrim (  CreateCharFromManString((String^ )s)	) ; }
//};
//
//template<typename Num>
//ref		  class TagBinding_Dec    : public TagBinding //    Bind a NumericUpDown.Value with a float variable ---- TagBinding_Dec    :
//{ 	protected:	Num		&_p; 
//				float	_k ;
//	public:				TagBinding_Dec (Num &p, NumericUpDown^ c, float k):TagBinding(c), _p(p), _k(k)	{SetDef()		;}
//
//	virtual void		set(Object^ f)	override{	_p	 = (Num)(_k * Decimal::ToSingle( *(Decimal^)(f) )); }
//	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p / _k) ; }
//	virtual void		UpDateForm(	 )	override{	   ((NumericUpDown^)(_c))->Value =  *(Decimal^) get()	;}
//	virtual void		UpDateP(	 )	override{ set( ((NumericUpDown^)(_c))->Value )  					;}	
//};
//
//	template<typename Num>
//ref		  class TagBinding_Rang_Min_b    : public TagBinding //    Bind a NumericUpDown.Value with a min of NumRang float variable ---- TagBinding_Rang_Min_b    :
//{ 	protected:	float			_k ;
//				NumRang<Num> &_p;
//
//	public:				TagBinding_Rang_Min_b (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding(c)	, _p(p), _k(k){ }
//
//	virtual void		set(Object^ f)	override{	_p.SetMin( (Num)(_k * Decimal::ToSingle( *(Decimal^)(f) ))); }
//	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p.Min() / _k) ; }
//	virtual void		UpDateForm(	 )	override{	   ((NumericUpDown^)(_c))->Value =  *(Decimal^) get()	;}
//	virtual void		UpDateP(	 )	override{ set( ((NumericUpDown^)(_c))->Value )  					;}	
//};
//	template<typename Num>
//ref		  class TagBinding_Rang_Min    : public TagBinding_Rang_Min_b<Num> //    Bind a NumericUpDown.Value with a a min of NumRang float variable ---- TagBinding_Rang_Min
//{ 	public:				TagBinding_Rang_Min (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding_Rang_Min_b<Num>(p,c,k)	{ SetDef()		;}
//};
//	template<typename Num>
//ref		  class TagBinding_Rang_Max    : public TagBinding_Rang_Min_b<Num> //    Bind a NumericUpDown.Value with a a max of NumRang float variable ---- TagBinding_Rang_Max
//{ 	public:				TagBinding_Rang_Max (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding_Rang_Min_b<Num>(p,c,k)	{ SetDef()		;}
//	virtual void		set(Object^ f)	override{	_p.SetMax((Num)(_k * Decimal::ToSingle( *(Decimal^)(f) ))); }
//	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p.Max() / _k) ; }
//};
//	template<typename Num>
//ref		  class TagBinding_Rang    : public TagBindGroup //    Bind a NumericUpDown.Value with a float variable ---- TagBinding_Dec    :
//{ 	public:				TagBinding_Rang (NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p, float k)
//							{   Add( gcnew TagBinding_Rang_Min<Num>(p,cmin,k));
//								Add( gcnew TagBinding_Rang_Max<Num>(p,cmax,k));}
//};
//
//
//					TagBinding_C_str^		TagBind		(Control^	c	, C_str &s							);
//					TagBinding_strTrim^		TagBind_Trim(Control^	c	, C_str &s							);
//template<class Num>	TagBinding_Dec<Num>^	TagBind(NumericUpDown^ c, Num &p, float k						){ return gcnew TagBinding_Dec<Num>(p,c,k)	;}	
//template<class Num>	TagBinding_Dec<Num>^	TagBind(NumericUpDown^ c, Num &p								){ return TagBind<Num>(c,p,1)	;}	
//template<class Num>	TagBinding_Rang<Num>^	TagBind(NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p, float k){ return gcnew TagBinding_Rang<Num>(cmin, cmax,p,k)	;}	
//template<class Num>	TagBinding_Rang<Num>^	TagBind(NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p		  ){ return TagBind<Num>(cmin, cmax,p,1)	;}	
//
//TagBinding_bool^	TagBind(CheckBox^	  c , bool  &p			);
//
//}
//
