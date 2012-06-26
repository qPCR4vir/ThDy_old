#pragma once
#include < vcclr.h >
//#include "ThDy_DNAHybrid.VC-WF/StdAfx.h"

#include "ThDySec\matrix.h" 
#include "ThDySec\common_basics.h" 
#pragma managed
namespace TagBindingNS 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

char *CreateCharFromManString(System::String ^Text);		//char *CreateTrimCharFromManString(System::String ^Text);
	

void			UpDateP		(Control^  control);
void			UpDateForm	(Control^  control);
System::Void	Validated_TB(System::Object^  senderControl, System::EventArgs^  e);


enum  PriorizeDefault {Form, Parametr, NoDef};
ref		  class TagBindGroup;
interface class IUpDatable 						{	void UpDateP() ,			UpDateForm() ;	
													static  PriorizeDefault _def=/*PriorizeDefault::*/Form;		};
interface class ITagBinding : public IUpDatable	{	Object^ get(); 	void	set(Object^ o);	 };

ITagBinding^	isTagBinding	(Control^ c);
ITagBinding^	getTagBinding	(Control^ c);
				

ref		  class TagBinding : public ITagBinding //  crear y dejar instalado las especializadas.
{ protected:	Control^	_c;
				Object^		_OldTag;

				TagBinding(Control^ c)		: _c(c),  _OldTag(c->Tag)		
					{	_c->Tag= this;
						_c->Validated += gcnew System::EventHandler( Validated_TB) ;		
					}
				void SetDef(void){  switch (IUpDatable::_def){	case /*PriorizeDefault::*/Form:			UpDateP();		break;		
																case /*PriorizeDefault::*/Parametr:		UpDateForm();	break;
																case /*PriorizeDefault::*/NoDef:						default:	;	
								 }						     }
public:
				~TagBinding() { _c->Tag = _OldTag ; }


	ITagBinding^		getTagBinding()	{ return	TagBindingNS::getTagBinding(_c)		;}	
	Control^			getControl()	{ return	_c									;}

	virtual void		set(Object^ s)	{			getTagBinding()->set(s)			;}
	virtual Object^		get(		 )	{ return	getTagBinding()->get( )			;}

	virtual void		UpDateForm(	 )	{			getTagBinding()->UpDateForm(	 )	;}
	virtual void		UpDateP(	 )	{			getTagBinding()->UpDateP(	 )		;}

};

ref		  class TagBindGroup : public IUpDatable // Clase base para vincular una variable al WindowsControlForm de la User Interfase que lo colecta usando el Tag del Control
{	private: 	List<IUpDatable^> ^_tb ;
	public:		TagBindGroup() : _tb( gcnew List<IUpDatable^>() ){}
				TagBindGroup^	operator<<	(IUpDatable^ tbg){		Add( tbg); return %*this;	 }
				TagBindGroup^	operator<<	(TagBinding%  tb){		Add( tb ); return %*this;	 }
				TagBindGroup^	operator<<	(PriorizeDefault  d){	IUpDatable::_def=d   ; return %*this;	 }

				void			Add			(IUpDatable^ tbg){ _tb->Add( tbg);				 }
				void			Add			(TagBinding%  tb){ _tb->Add( tb.getTagBinding());}	

	virtual void		UpDateForm(	 )	{ for each(IUpDatable^ tb in _tb)	tb->UpDateForm(	 );}
	virtual void		UpDateP(	 )	{ for each(IUpDatable^ tb in _tb)	tb->UpDateP   (	 );}
};



ref		  class TagBinding_C_str_b : public TagBinding //    Bind a Control.Text with a C_str variable ---- TagBinding_C_str    :
{ 	protected:	C_str	&_p ;
	public:				TagBinding_C_str_b (C_str &s, Control^ c):TagBinding(c)	, _p(s)	{} 

	virtual void		set(Object^ s)	override{						_p.Take (  CreateCharFromManString((String^ )s)	) ; }
	virtual Object^		get(		 )	override{ return  gcnew String(	_p.Get()								) ; }
	virtual void		UpDateForm(	 )	override{ _c->Text = (String^)get()	;}
	virtual void		UpDateP(	 )	override{ set (_c->Text)				;}
};
ref		  class TagBinding_C_str    : public TagBinding_C_str_b  //    Bind a Control.Text with a C_str variable using Trim ---- TagBinding_strTrim    :
{ 	public:				TagBinding_C_str   (C_str &s, Control^ c):TagBinding_C_str_b(s,c)		{SetDef()		;}
};
ref		  class TagBinding_strTrim  : public TagBinding_C_str_b  //    Bind a Control.Text with a C_str variable using Trim ---- TagBinding_strTrim    :
{ 	public:				TagBinding_strTrim (C_str &s, Control^ c):TagBinding_C_str_b(s,c)		{SetDef()		;}
	virtual void		set(Object^ s)	override{						_p.TakeTrim (  CreateCharFromManString((String^ )s)	) ; }
};

template<typename Num>
ref		  class TagBinding_Dec    : public TagBinding //    Bind a NumericUpDown.Value with a float variable ---- TagBinding_Dec    :
{ 	protected:	Num		&_p; 
				float	_k ;
	public:				TagBinding_Dec (Num &p, NumericUpDown^ c, float k):TagBinding(c), _p(p), _k(k)	{SetDef()		;}

	virtual void		set(Object^ f)	override{	_p	 = (Num)(_k * Decimal::ToSingle( *(Decimal^)(f) )); }
	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p / _k) ; }
	virtual void		UpDateForm(	 )	override{	   ((NumericUpDown^)(_c))->Value =  *(Decimal^) get()	;}
	virtual void		UpDateP(	 )	override{ set( ((NumericUpDown^)(_c))->Value )  					;}	
};

	template<typename Num>
ref		  class TagBinding_Rang_Min_b    : public TagBinding //    Bind a NumericUpDown.Value with a min of NumRang float variable ---- TagBinding_Rang_Min_b    :
{ 	protected:	float			_k ;
				NumRang<Num> &_p;

	public:				TagBinding_Rang_Min_b (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding(c)	, _p(p), _k(k){ }

	virtual void		set(Object^ f)	override{	_p.SetMin( (Num)(_k * Decimal::ToSingle( *(Decimal^)(f) ))); }
	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p.Min() / _k) ; }
	virtual void		UpDateForm(	 )	override{	   ((NumericUpDown^)(_c))->Value =  *(Decimal^) get()	;}
	virtual void		UpDateP(	 )	override{ set( ((NumericUpDown^)(_c))->Value )  					;}	
};
	template<typename Num>
ref		  class TagBinding_Rang_Min    : public TagBinding_Rang_Min_b<Num> //    Bind a NumericUpDown.Value with a a min of NumRang float variable ---- TagBinding_Rang_Min
{ 	public:				TagBinding_Rang_Min (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding_Rang_Min_b<Num>(p,c,k)	{ SetDef()		;}
};
	template<typename Num>
ref		  class TagBinding_Rang_Max    : public TagBinding_Rang_Min_b<Num> //    Bind a NumericUpDown.Value with a a max of NumRang float variable ---- TagBinding_Rang_Max
{ 	public:				TagBinding_Rang_Max (NumRang<Num> &p, NumericUpDown^ c, float k)	:TagBinding_Rang_Min_b<Num>(p,c,k)	{ SetDef()		;}
	virtual void		set(Object^ f)	override{	_p.SetMax((Num)(_k * Decimal::ToSingle( *(Decimal^)(f) ))); }
	virtual Object^		get(		 )	override{ return  gcnew Decimal(	_p.Max() / _k) ; }
};
	template<typename Num>
ref		  class TagBinding_Rang    : public TagBindGroup //    Bind a NumericUpDown.Value with a float variable ---- TagBinding_Dec    :
{ 	public:				TagBinding_Rang (NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p, float k)
							{   Add( gcnew TagBinding_Rang_Min<Num>(p,cmin,k));
								Add( gcnew TagBinding_Rang_Max<Num>(p,cmax,k));}
};

ref		  class TagBinding_bool : public TagBinding //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
{ 	protected:	bool	&_p ;
	public:				TagBinding_bool (bool &p, CheckBox^ c):TagBinding(c), _p(p){SetDef()		;} 

	virtual void		set(bool p	 )			{						_p= p		; }// OJO  nuevas get y set !!! no las virtuales heredadas
	virtual	bool		get(		 )	new		{ return  _p						; }
	virtual void		UpDateForm(	 )	override{ ((CheckBox^ )_c)->Checked = get()	;}
	virtual void		UpDateP(	 )	override{ set (((CheckBox^ )_c)->Checked)	;}
};

					TagBinding_C_str^		TagBind		(Control^	c	, C_str &s							);
					TagBinding_strTrim^		TagBind_Trim(Control^	c	, C_str &s							);
template<class Num>	TagBinding_Dec<Num>^	TagBind(NumericUpDown^ c, Num &p, float k						){ return gcnew TagBinding_Dec<Num>(p,c,k)	;}	
template<class Num>	TagBinding_Dec<Num>^	TagBind(NumericUpDown^ c, Num &p								){ return TagBind<Num>(c,p,1)	;}	
template<class Num>	TagBinding_Rang<Num>^	TagBind(NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p, float k){ return gcnew TagBinding_Rang<Num>(cmin, cmax,p,k)	;}	
template<class Num>	TagBinding_Rang<Num>^	TagBind(NumericUpDown^ cmin, NumericUpDown^ cmax, NumRang<Num> &p		  ){ return TagBind<Num>(cmin, cmax,p,1)	;}	

TagBinding_bool^	TagBind(CheckBox^	  c , bool  &p			);

}

