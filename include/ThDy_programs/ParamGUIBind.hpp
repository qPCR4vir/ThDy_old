#ifndef ProgParamGUIBind_H
#define ProgParamGUIBind_H

#include <memory>
#include <vector>
 
//#include "ThDySec\matrix.h" 
#include "ThDySec\common_basics.h" 
#include "init_prog_param.h" 

namespace ParamGUIBind 
{

//
//void			UpDateP		(Control^  control);          // Posible because in .NET all control have a Tag^
//void			UpDateForm	(Control^  control);          // and there we keep a ref to the "users" parametr
//System::Void	Validated_TB(System::Object^  senderControl, System::EventArgs^  e);
//ref		  class TagBindGroup;
//interface class ITagBinding : public IUpDatable	{	Object^ get(); 	void	set(Object^ o);	 };
//
//ITagBinding^	isTagBinding	(Control^ c);
//ITagBinding^	getTagBinding	(Control^ c);
				//TagBindGroup^	operator<<	(TagBinding%  tb){		Add( tb ); return %*this;	 }
				//TagBindGroup^	operator<<	(PriorizeDefault  d){	IUpDatable::_def=d   ; return %*this;	 }
				//void			Add			(IUpDatable^ tbg){ _tb->Add( tbg);				 }
				//void			Add			(TagBinding%  tb){ _tb->Add( tb.getTagBinding());}	
   //BindGroup()  {}
//class TagBinding : public ITagBinding //  crear y dejar instalado las especializadas.
//{ protected:	Control^	_c;
//				Object^		_OldTag;
//
//				TagBinding(Control^ c)		: _c(c),  _OldTag(c->Tag)		
//					{	_c->Tag= this;
//						_c->Validated += gcnew System::EventHandler( Validated_TB) ;		
//					}
//				void SetDef(void){  switch (IUpDatable::_def){	case /*PriorizeDefault::*/Form:			UpDateP();		break;		
//																case /*PriorizeDefault::*/Parametr:		UpDateForm();	break;
//																case /*PriorizeDefault::*/NoDef:						default:	;	
//								 }						     }
//public:
//				~TagBinding() { _c->Tag = _OldTag ; }
//
//
//	ITagBinding^		getTagBinding()	{ return	TagBindingNS::getTagBinding(_c)		;}	
//	Control^			getControl()	{ return	_c									;}
//
//	virtual void		set(Object^ s)	{			getTagBinding()->set(s)			;}
//	virtual Object^		get(		 )	{ return	getTagBinding()->get( )			;}
//
//	virtual void		UpDateForm(	 )	{			getTagBinding()->UpDateForm(	 )	;}
//	virtual void		UpDateP(	 )	{			getTagBinding()->UpDateP(	 )		;}
//
//};
//
//
//ref		  class TagBinding_C_str_b : public TagBinding //    Bind a Control.Text with a C_str variable ---- TagBinding_C_str    :
//{ 	protected:	C_str	&_p ;
//	public:				TagBinding_C_str_b (C_str &s, Control^ c):TagBinding(c)	, _p(s)	{} 
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

                            /// from the user code (the "Program") or the GUI layout?
enum  class PriorizeDefault {Form, Parametr, NoDef};          

class IParBind	
{	
	static  PriorizeDefault _def;   /*=PriorizeDefault::Form;*/	
  public: 
    virtual void    UpDateProg()=0, UpDateForm()=0 ;	
    virtual ~IParBind()    {}
    static void SetDef(PriorizeDefault def){_def=def;}
    void SetDef(void)
    {  switch (_def)
       {
           case PriorizeDefault::Form:			UpDateProg();	break;		
	       case PriorizeDefault::Parametr:		UpDateForm();	break;
	       case PriorizeDefault::NoDef:						
           default:	;	
       }
    }				
};
typedef std::unique_ptr<IParBind> upPbind;

                 /// A container that will be a member of a form, and will collect all the other ParBind
class BindGroup : public IParBind 
{ private: 	
   std::vector<upPbind>  _pb ;

  public:		
     BindGroup&	operator<<	(upPbind&& pb){  _pb.push_back(std::move(pb)) ;	  return *this;	 }
     ~BindGroup();

	void	UpDateForm(	 )	override { for(upPbind& pb : _pb)	pb->UpDateForm(	 );}
	void	UpDateProg(	 )	override { for(upPbind& pb : _pb)	pb->UpDateProg(	 );}
};

class ProgPBind : public virtual IParBind 
{
  protected:
      IParam& _p;
  public:
      ProgPBind(IParam& p):_p(p)
      {
          _p.ValueChanged =[&](){UpDateForm();
          };
      }
};

class Bind_CParamBool : public ProgPBind  
{ 	
 public:				
    Bind_CParamBool (CParamBool &p ):ProgPBind(p){} 

    void updateProg(bool val){ static_cast <CParamBool& >(_p).set  (val); }
    bool getProgVal(){ return  static_cast <CParamBool& >(_p).get  () ; }
};

class Bind_CParamString : public ProgPBind  
{ 	
 public:				
    Bind_CParamString (CParamString &p ):ProgPBind(p){} 

    void        updateProg(const std::string&  val){ static_cast <CParamString& >(_p).set (val); }
    std::string getProgVal(               ){ return  static_cast <CParamString& >(_p).get (  ) ; }
};
class Bind_CParamC_str : public ProgPBind  
{ 	
 public:				
    Bind_CParamC_str (CParamC_str &p ):ProgPBind(p){} 

    void        updateProg(const char*  val){ static_cast <CParamC_str& >(_p).Copy     (val); 
    }
    const char* getProgVal(        ){ return  static_cast <CParamC_str& >(_p).Get      (  ) ; 
    }
};
class Bind_CParamC_str_TRIM : public Bind_CParamC_str  
{ 	
 public:				
    Bind_CParamC_str_TRIM (CParamC_str &p ):Bind_CParamC_str(p){} 

    void        updateProg(const char*  val){ static_cast <CParamC_str& >(_p).CopyTrim (val); }
};


template <class Num>
class Bind_CParamRang  : public ProgPBind  
{ 	
 public:				
    Bind_CParamRang (CParamNumRange<Num> &p ):ProgPBind(p){} 

    void updateProg (Num val) { static_cast <CParamNumRange<Num>& >(_p).set  (val); 
    }
    Num  getProgVal (){ return  static_cast <CParamNumRange<Num>& >(_p).get  () ; 
    }
};

template <class Num>
class Bind_CParamRang_Min  : public ProgPBind //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
{ 	
 public:				
    Bind_CParamRang_Min (CParamNumMinMax &p ):ProgPBind(p){} 

    void updateProg(bool val){ static_cast <CParamBool&         >(_p).set    (val); }
    bool getProgVal(){ return  static_cast <CParamBool&         >(_p).get    () ; }
};




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
//ref		  class TagBinding_bool : public TagBinding //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
//{ 	protected:	bool	&_p ;
//	public:				TagBinding_bool (bool &p, CheckBox^ c):TagBinding(c), _p(p){SetDef()		;} 
//
//	virtual void		set(bool p	 )			{						_p= p		; }// OJO  nuevas get y set !!! no las virtuales heredadas
//	virtual	bool		get(		 )	new		{ return  _p						; }
//	virtual void		UpDateForm(	 )	override{ ((CheckBox^ )_c)->Checked = get()	;}
//	virtual void		UpDateP(	 )	override{ set (((CheckBox^ )_c)->Checked)	;}
//};
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
}
#endif


