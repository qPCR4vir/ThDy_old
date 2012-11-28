// Definiciones y declaraciones basicas. A usar por todos, user interface and programs. Primaria?, no depende? de nadie?.
#pragma unmanaged	
#ifndef _COMMON_BASIC_H
#define _COMMON_BASIC_H
#pragma warning( disable : 4996 )
//#include "matrix.h"


inline char *clone_c_str   (const char *str) {return strcpy (new char[strlen(str) +1] , str);}  // ;char *clone_c_str(const char *str)
inline char *clone_trim_str(const char *str)		// definida en    :   init_prog_param.cpp   -  elimina espacios al principio y al final
													//char *clone_trim_str(const char *str)
{	if(str && str[0]) 
	{							// llegado aqui - tiene al menos un char !
		size_t  f, i ;
		for ( i=0; str[i] && isspace(str[i]); ++i );   // salta espacios al principio
		for ( f=i; str[f]					; ++f );	// f=Len >0. llega al final (considerando que char(0) es el final !!!!! )
		for ( --f;  f>=i  && isspace(str[f]); --f );			// se come espacios al final
		size_t NewLen= f - i+1 ;

		if (NewLen>0)					// strncpy(NewStr, &str[i], NewLen) ; 		
		{
			char *NewStr= new char[NewLen+1];
			NewStr[NewLen]=0;
			while ( NewLen )
				NewStr[--NewLen]=str[f--];
			return NewStr ;
		}
	}
									// vacia o solo espacios. O sea: no char dist de espacio
	char *NewStr= new char[1]; 
	NewStr[0]=0;
	return NewStr;
}
 template<typename CharType>               //   elimina espacios al principio y al final
inline CharType *clone_trim(const CharType *str)		// definida en    :   init_prog_param.cpp   -  elimina espacios al principio y al final
													//char *clone_trim_str(const char *str)
{	if(str && str[0]) 
	{							// llegado aqui - tiene al menos un char !
		size_t  f, i ;
		for ( i=0; str[i] && isspace(str[i]); ++i );   // salta espacios al principio
		for ( f=i; str[f]					; ++f );	// f=Len >0. llega al final (considerando que char(0) es el final !!!!! )
		for ( --f;  f>=i  && isspace(str[f]); --f );			// se come espacios al final
		size_t NewLen= f - i+1 ;

		if (NewLen>0)					// strncpy(NewStr, &str[i], NewLen) ; 		
		{
			char *NewStr= new CharType[NewLen+1];
			NewStr[NewLen]=0;
			while ( NewLen )
				NewStr[--NewLen]=str[f--];
			return NewStr ;
		}
	}
									// vacia o solo espacios. O sea: no char dist de espacio
	char *NewStr= new CharType[1]; 
	NewStr[0]=CharType(0);
	return NewStr;
}
#include <string.h>


template<class _Elem,	class _Traits,	class _Alloc>               //   elimina espacios al principio y al final
inline std::basic_string<_Elem, _Traits, _Alloc> trim_string(const std::basic_string<_Elem, _Traits, _Alloc>& str)		
{	size_t i,l=str.length() ;
	if (l)
	{
		for ( i=0;  i<l  && isspace(str[i]); ++i );           // salta espacios al principio
		for ( --l; l>=i  && isspace(str[l]); --l );			// se come espacios al final
		size_t NewLen= l - i +1;
		if (NewLen>0)					// strncpy(NewStr, &str[i], NewLen) ; 		
			return str.substr(i,NewLen);
	}
	return std::basic_string<_Elem, _Traits, _Alloc>();
}

//		// TEMPLATE CLASS basic_string
//template<class _Elem,
//	class _Traits,
//	class _Alloc>
//	class basic_string
//		: public _String_alloc<!is_empty<_Alloc>::value,
//			_String_base_types<_Elem, _Alloc> >
//	{	// null-terminated transparent array of elements
//public:
//	typedef basic_string<_Elem, _Traits, _Alloc> _Myt;
//
//template<Class CharType>    bool isspace(  CharType _Ch,    const locale& _Loc   )




//	size_t l =strlen(str) , i ;
//	for ( i=0; i<l && isspace(str[i]); i++ );   // salta espacios al principio
//	for (    ; isspace(str[l]) ; l--);			// se come espacios al final
//	size_t NewLen= l - i+1 ;
//	char *NewStr= new char[NewLen+1];
//	strncpy(NewStr, &str[i], NewLen) ; 	NewStr[NewLen]=0;
//	return NewStr ;
//}




char *AttachToCharStr       (const char *CharStr   , const char *Attach); //defined in : sec.cpp: no olvide delete este pointer
char *ChangeCharStrAttaching(char *&CharStrToChange, const char *Attach);	// CharStrToChange : debe ser una cadena que se creo con new, 
char *ChangeCharStrAttaching(char *&CharStrToChange, const int   Attach);	// y que sera borrada y vuelta a crear !!!

inline char *AddFileExt(const char *FileName, const char *Ext="")	{return AttachToCharStr(FileName,Ext);}// no olvide delete este pointer

typedef unsigned char Base;
//char operator Base(){return }
Base *Generate_DegSec( const char *sec, bool rev, bool compl, long l=0) ;// , long l=0) ;
inline char *Generate_DegSec_char( const char *sec, bool rev, bool compl, long l=0){return (char *)Generate_DegSec(sec, rev,  compl,  l);};

//  Clase para lidiar con las string como file name y sec que deben ser copiadas y liberadas todo el tiempo. Copy() vs Take() !!!!
class C_str
{public:
/*explicit*/ C_str()													:_s(clone_c_str		("")){}
	explicit C_str(const char * s)										:_s(clone_c_str		(s )){}
	explicit C_str(const C_str& s)										:_s(clone_c_str		(s._s )){}  // Copy constructor


	char *Copy		(const char *s)	{ if (s!=_s) delete []_s; _s=clone_c_str	(s );				return _s		;}  // return correct ???
	C_str& Copy		(const C_str& s){ if (this!= &s) Copy(s._s);									return *this	;}	// "sinonimos"
	C_str& operator=(const C_str& s){ 																return Copy(s)	;}	// = return Copy(s) ;					
	char *CopyTrim	(const char *s)	{ if (s!=_s) delete []_s; _s=clone_trim_str	(s );				return _s		;}
	char *Trim		(			)	{ char *t=_s;  			_s=clone_trim_str	(_s); delete []t;	return _s		;}
	char *Take		(	   char *s) { if(s!=_s) delete []_s;										return _s=	(s );}
	char *TakeTrim	(	   char *s) { Take(s);														return Trim()	;}
	char *Get		()				{		/* PELIGROSO !!!! use release or Copy ?? */				return _s		;}
	const char *Get	()		const	{																return _s		;}
	char *release	()				{  char *t=_s;  	_s=clone_c_str		("")	;				return t		;}


	~C_str			()				{ delete []_s; }
private:
	char *_s;
};


template <class Num>
inline bool IsEq (Num f1, Num f2, float prec= 0.01f )	// ; bool	IsEq (float f1, float f2, float prec)   ----> dif/med < prec ????
{	float fm = float( abs(f1) + abs(f2) )/ 2.0f ;		if (fm == 0) return true ;
	return abs(float(f1-f2)) / fm  < prec 	;
}


extern char sep[];
enum SaltCorrection {NoSelect  =-1 , StLucia  =0 , Owczarzy  =1 }; // ya se puede usar StLucia inicializando todo en el constructor. Parcialmente implementado cambio de Conc
enum SaltCorrecP	{NoSMSelect=-1 , SMStLucia=0 , SMOwczarzy=1 }; // Unificar !!

enum AlignMeth		{TAMeth_Tm =0  , TAMeth_G    , TAMeth_Fract };

enum		 DNAStrand										{plus	, minus, direct	, rev	, compl, rev_compl	} ;
extern char *DNAStrandName[];	// char *DNAStrandName[]=	{""		, "(c)", ""		, "(r)"	, "(i)", "(c)"		} ; //defined in : sec.cpp: 

enum fileFormat {fasta =1 , csv =1<<1, f2 =1<<2, f3 =1<<3} ; // se pueden "OR" los formatos : OUTPUT !!!!!!


template <class Num>
class NumRang				// ------------------------------------------------------------------- NumRang<Numeric>	-----------------
{public: 
	NumRang(					  ):min(0 ),			max(0 ){} 
	NumRang(		Num av		  ):min(av),			max(av){}
	NumRang(Num mi,         Num ma):min(mi),			max(ma){}

	void Set(					   ){min=0 			;max=0	;}
	void Set(		Num av		   ){min=av			;max=av ;}
	void Set(Num mi,         Num ma){min=mi			;max=ma ;}
	void Set(     NumRang    R     ){min=R.min		;max=R.max;}

	void SetMin(Num mi			   ){min=mi;			    }
	void SetMax(			 Num ma){				max=ma ;}

	Num  Ave() const {return (min+max)/2 ;}				// Lineal aritmetical Average "by default"
	Num  Min() const {return min			;}			Num& Min()		 {return min			;}
	Num  Max() const {return max			;}			Num& Max()       {return max			;}
   	Num length()const{ return Max() - Min() ;}          // throw execption "negative length ??????


	bool inRang  ( Num		X)   const {return (min<= X			&& X<=max)		;}
	bool inRang  ( NumRang	R)   const {return ( R.inRang(min) && R.inRang(max));}
	bool isExtrem( Num		X)   const {return (min== X			|| X==max)		;}
	bool isExtrem( NumRang	R)   const {return (min== R.Min()	|| R.Max()==max);}

	void Expand(Num num)			 { if ( num<min ) min=num  ; if(  num>max)  max=num  ;}
	void Expand(Num mi,       Num ma){ if (   mi<min) min=mi   ; if(   ma>max)  max=ma   ;}
	bool Expand(NumRang R)			 { bool e; if (e=(R.min<min)) min=R.min; if(R.max>max)  {max=R.max;return true;} return e;}
    void Shrink(NumRang R)			 { if (R.min>min) min=R.min; if(R.max<max)  max=R.max;}
	Num  Intersection(NumRang R){return R.Shrink(this);}	//Num  Union(NumRang R)	    {return R.Expand(this);}


	bool UpdateMin(Num mi){ return (mi<min) ? min=mi, true : false ;}
	bool UpdateMax(Num ma){ return (ma>max) ? max=ma, true : false ;}
private: 
	Num min,max; 
};

template <class Num>
class NumRangA	: public NumRang<Num>	// ------------------------------------------ NumRangA<Numeric> 	with seteable average-----------------
{public: 
	NumRangA(					   ):					ave(0)				{} 
	NumRangA(		 Num av		   ):NumRang(av),		ave(av)				{}
	NumRangA(Num mi,         Num ma):NumRang(mi,ma),	ave(NumRang::Ave()) {}
	NumRangA(Num mi, Num av, Num ma):NumRang(mi,ma),	ave(av)				{}

	void Set(					   ){NumRang::Set(  )   ;ave=0				;}
	void Set(		Num av		   ){NumRang::Set(av)   ;ave=av				;}
	void Set(Num mi,         Num ma){NumRang::Set(mi,ma);ave=NumRang::Ave() ;}
	void Set(Num mi, Num av, Num ma){NumRang::Set(mi,ma);ave=av				;}

	void SetAve(     Num av        ){	    ave=av;					 }

	Num  Ave() const {return ave ;}		Num&  Ave()  {return ave ;}

private: 
	Num ave; 
};


template <class Num>
inline	bool inRang(const Num X, const NumRang<Num> R)  {return  R.inRang(X) ;}

typedef float	Energy  ;		// ----------------------  typedef : 	Energy	--	Entropy	--	Temperature	--	SecPos --	LonSecPos and Rangs
typedef Energy	Entropy ;
typedef float	Temperature  ;
typedef int		SecPos ;
typedef long	LonSecPos ;

typedef NumRang<Energy>			EnergyRang  ;
typedef NumRang<Entropy>		EntropyRang	 ;
typedef NumRang<Temperature>	TemperatureRang	  ;
typedef NumRang<SecPos>			SecPosRang		 ;
typedef NumRang<LonSecPos>		LonSecPosRang	 ;

struct TmGPos {Temperature _Tm; Energy _G; SecPos _Pos; 
				TmGPos(Temperature Tm, Energy G, SecPos Pos):_Tm(Tm), _G(G), _Pos(Pos) {} 
				TmGPos() {} 
			  };

extern const 	EnergyRang		G_def  ;	//  G_def (-5,-1), 
extern const 	TemperatureRang	Tm_def ;	//  Tm_def(57,63) ;
extern const 	SecPosRang		L_def  ;	//  L_def (20,35) ;

class SondeLimits		// ------------------------------------------------------------------	SondeLimits	-------------------------------
{public: 
	EnergyRang		_G;
	TemperatureRang	_Tm ;
	SecPosRang		_L ;
	SondeLimits():_G(G_def),_Tm(Tm_def),_L(L_def){}
	SondeLimits(EnergyRang G, TemperatureRang Tm, SecPosRang L):_G(G),_Tm(Tm), _L(L){}
};
inline Temperature	  CtoK	(Temperature Tm)	{return Tm + Temperature(273.15f) ;}
inline Temperature	  KtoC	(Temperature Tm)	{return Tm - Temperature(273.15f) ;}

inline TemperatureRang CtoK	( TemperatureRang T  ) {	     T.Set( CtoK (     T.Min()) , CtoK (     T.Max()) ) ; return T ;}
inline TemperatureRang KtoC	( TemperatureRang T  ) {	     T.Set( KtoC (     T.Min()) , KtoC (     T.Max()) ) ; return T ;}

inline SondeLimits    CtoK	( SondeLimits    sL ) { sL._Tm.Set( CtoK (sL._Tm.Min()) , CtoK (sL._Tm.Max()) ) ; return sL;}
inline SondeLimits    KtoC	( SondeLimits    sL ) { sL._Tm.Set( KtoC (sL._Tm.Min()) , KtoC (sL._Tm.Max()) ) ; return sL;}
inline SondeLimits    convCtoK_ctok	( SondeLimits    sL ) { sL._Tm.Set( CtoK (sL._Tm.Min()) , CtoK (sL._Tm.Max()) )		;	
															sL._G.Set ( sL._G.Min()*1000    , sL._Tm.Max()*1000   )		; return sL;}
inline SondeLimits    convKtoC_ktoc	( SondeLimits    sL ) { sL._Tm.Set( KtoC (sL._Tm.Min()) , KtoC (sL._Tm.Max()) )		;	
															sL._G.Set ( sL._G.Min()/1000    , sL._Tm.Max()/1000   )		; return sL;}

class DegRes			//------------------------------------------------------------	DegRes	-------------------------------------
{public: float min,ave,max; 
	DegRes(							   ):min(0 ),ave(0 )		,max(0 ){}; 
	DegRes(			 float av		   ):min(av),ave(av)        ,max(av){};
	DegRes(float mi,           float ma):min(mi),ave((mi+ma)/2) ,max(ma){};
	DegRes(float mi, float av, float ma):min(mi),ave(av)        ,max(ma){};

	void Set(float mi, float av, float ma){min=mi; ave=av       ; max=ma;}
	void Set(float mi,           float ma){min=mi; ave=(mi+ma)/2; max=ma;}
	void Set(          float av          ){min=av; ave=av       ; max=av;}
	void Set(                            ){        ave=(min+max)/2      ;}
};


#include <string.h>
#include <sstream> 

		template <typename Num>
 std::string toString_Val_in_Range(Num val, NumRang<Num> &NR)
{     
	std::ostringstream result;
	result << val << " in range [" << NR.Min() <<", "<< NR.Max() << "]";
	return result.str();
}
		template <typename Num>
 std::string toString_Range(NumRang<Num> &NR)
{     
	std::ostringstream result;
	result << " [" << NR.Min() <<", "<< NR.Max() << "]";
	return result.str();
}



#include <stdexcept>

class OutOfNumRange : public std::out_of_range 
{ public: 
	explicit OutOfNumRange ( const std::string& what_arg ) : out_of_range(what_arg + " (Out of numeric rang)" ){}

		template<typename Num>
	explicit OutOfNumRange ( const std::string& what_arg, Num invalidValue, NumRang<Num>& NR ) 
		: out_of_range(what_arg + " (Out of numeric rang, trying to set " + toString_Val_in_Range(invalidValue, NR) + ")"   ) 
		{}
};


//class OutOfNumRangeT : public std::out_of_range 
//{ public: 
//	explicit OutOfNumRangeT ( const std::string& what_arg ) : out_of_range(what_arg + " (Out of numeric rang)" ){}
//
//		template<typename Num>
//	explicit OutOfNumRangeT ( const std::string& what_arg, Num invalidValue, NumRang<Num>& NR ) 
//		: out_of_range(what_arg + " (Out of numeric rang, trying to set " + toString_Val_in_Range(invalidValue, NR) + ")"   ) 
//		{}
//};


#endif 