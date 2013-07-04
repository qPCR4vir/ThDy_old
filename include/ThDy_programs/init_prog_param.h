#ifndef _INIT_PROG_PARAM_H
#define _INIT_PROG_PARAM_H
#include "ThDy_programs/init_prog_param_impl.h"

class CParamBool: public IParam
{    bool  _v, &_value;
 public:
						// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamBool (CProgParam *pp,  const std::string& titel, const std::string& etiq, bool &parRef,    
		            bool defValue
					) : IParam (pp, titel, etiq), _value(parRef)            {  _value= defValue;	          }

					   //  no necesita un parametro externo
    CParamBool (CProgParam *pp,  const std::string& titel, const std::string& etiq, 
		            bool defValue
					) : IParam (pp, titel, etiq), _v(defValue) , _value(_v) {            }

	void set(bool value){ if (value == _value) return; _value = value;  changed();    }
	bool get()const{ return _value;    }

	std::ostream	    &saveValue	(std::ostream	&osPr) const override   
	                        {   osPr << std::boolalpha << _value ;     
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (std::istream   &isPr) override   // Descarta el Titel que queda detras del ultimo tab
	                    {   isPr>>std::boolalpha>>_value;
							return true;
	                    } 
};

class CParamString: public IParam
{    std::string  _v, &_value;
 public:
    /// Link to an existing parameter. Do not use the internal string _v. For compatibility.
    CParamString (CProgParam *pp,  const std::string& titel, const std::string& etiq, std::string &parRef,  
		            const std::string defValue
					) : IParam (pp, titel, etiq), _value(parRef)            {  _value= defValue;	          }

	/// Use the internal parameter and dont need an external one
    CParamString (CProgParam *pp,      const std::string& titel, const std::string& etiq, 
		            const std::string defValue
					) : IParam (pp, titel, etiq), _v(defValue) , _value(_v) {            }

	void set(const std::string& value){ if (value == _value) return; _value = value;  changed();    }
	std::string get()const{ return _value;    }

	virtual std::ostream	    &saveValue	(std::ostream	&osPr) const override   
	                        {   osPr << _value << std::endl <<"\t\t\t\t" ;     
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (std::istream   &isPr) override   /// Descarta el Titel que queda detras del ultimo tab
	                    {   std::string t; getline(isPr,t); 
	                        _value=trim_string( t.substr(0, t.rfind("\t"))   );
							return true;
	                    } 
};
class CParamC_str: public IParam, public C_str
{public:
    //CParamC_str (CProgParam *pp, C_str &parRef,    // Acepta un parametro y por tanto no usa _v. Por compatibilidad.
		  //          const string& titel, const string& etiq, 
		  //          const char * defValue
				//	) : IParam (pp, titel, etiq), _value(parRef)            {  _value.Copy( defValue) ;	          }
    CParamC_str (CProgParam *pp,                  ///  no necesita un parametro externo
		            const std::string& titel, const std::string& etiq, 
		            const char * defValue
					) : IParam (pp, titel, etiq), C_str(defValue)                {            }
    CParamC_str (CProgParam *pp,                  ///  no necesita un parametro externo
		            const std::string& titel, const std::string& etiq, 
		            const C_str& defValue
					) : IParam (pp, titel, etiq), C_str(defValue)                {            }

	void set(const C_str& value){ if (! strcmp (Get(),value.Get())) return; Copy(value) ;  changed();    }
	void set(const char * value){ if (! strcmp (Get(),value))       return; Copy(value) ;  changed();    }

	std::ostream &saveValue	(std::ostream	&osPr) const override   
	                        {   osPr << Get() << std::endl <<"\t\t\t\t" ;     // Por que no se puede poner el return directo????
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (std::istream   &isPr) override   // Descarta el Titel que queda detras del ultimo tab
	                    {   std::string t; std::getline(isPr,t); 
	                        CopyTrim( t.substr(0, t.rfind("\t")).c_str()   );
							return true;
	                    } 
};

    /// To be trow
class ParamOutOfNumRange : public OutOfNumRange 
{ public: 
	explicit ParamOutOfNumRange ( const std::string& what_arg ): OutOfNumRange(what_arg ){}
		template<typename Num>
	ParamOutOfNumRange ( const std::string& what_arg, Num invalidValue, NumRang<Num>& NR ) 
		: OutOfNumRange (  what_arg ,  invalidValue,  NR ) 
		{}
};
    /// To be trow
class ParamOutOfEnumRange : public ParamOutOfNumRange 
{ public: 
	explicit ParamOutOfEnumRange ( const std::string& what_arg ): ParamOutOfNumRange(what_arg ){}
		template<typename Num>
	ParamOutOfEnumRange ( const std::string& what_arg, Num invalidValue, NumRang<Num>& NR ) 
		: ParamOutOfNumRange (  what_arg , int(invalidValue),  NumRang<int>(   int( NR.Min() )   ,   int( NR.Max() )    )    )
		{}
		template<typename Num>
	ParamOutOfEnumRange ( const std::string& what_arg, int invalidValue, NumRang<Num>& NR ) 
		: ParamOutOfNumRange (  what_arg , int(invalidValue),  NumRang<int>(   int( NR.Min() )   ,   int( NR.Max() )    )    )
		{}
};	 


template <typename Num>
class CParamNumRange: public CParamBNRange<Num>
{
    
 public:
									// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamNumRange (CProgParam *pp, const std::string& titel, const std::string& etiq, Num &parRef, 
						Num min, Num max, Num defValue,
						const std::string& unit=""
					) : CParamBNRange (pp, titel, etiq, parRef,min,  max,  defValue,unit)
	          { if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(std::string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }
								// Num &parRef,   usa _v y por tanto no necesita un parametro externo
    CParamNumRange (CProgParam *pp, const std::string& titel, const std::string& etiq, 
						Num min, Num max, Num defValue,
						const std::string& unit=""
					) : CParamBNRange (pp, titel, etiq,min,  max,  defValue,unit)
	          { if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(std::string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }
	std::ostream	&saveValue	(std::ostream	&osPr) const override   
	                        {return osPr<<get();} 
    bool        loadValue   (std::istream   &isPr) /*throw( ParamOutOfNumRange) */  override         
	                        {   Num t; 
								isPr>>t; 
	                            set(t);
								return true;
	                        } 
	void set(Num value){ if (!inRang(value)) 
		                    throw ParamOutOfNumRange(std::string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")"
													 , value, *this );
						  CParamBNRange::set(value) ; 
	                    }

};

template <typename enumType>
class CParamEnumRange: public CParamBNRange<enumType>
{
   std::map<std::string, enumType> _StrValues;
   std::map<int, enumType>	 _IntValues;

 public:
	 void AddEnumValues(					enumType eTy )		{ _IntValues[int(eTy)]=eTy;}
	 void AddStrValues (const std::string& strV, enumType eTy )		{ _StrValues[strV]    =eTy;
																	AddEnumValues(eTy );	}
	 std::string ToString(enumType v)const
		{ for (auto p : _StrValues) 
			if (p.second==v)
				return p.first;
		  throw OutOfNumRange(std::string("Value out of Range while trying to set: ")+Titel() );
		}
	 std::string StringEnumerate()const 
	 {	std::ostringstream result;
		for (auto p : _StrValues)
			result <<  p.first << "(" << int(p.second) <<"), ";
		return result.str();
	 }
	 std::string ToString()const {return ToString(get());}
	 bool exist(int v)		const{		return _IntValues.end()!=_IntValues.find(v); }
	 bool exist(std::string v)	const{		return _StrValues.end()!=_StrValues.find(v); }


									/// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
	 CParamEnumRange (CProgParam *pp, const std::string& titel, const std::string& etiq, enumType &parRef, 
						enumType min, enumType max, enumType defValue,
						const std::string& unit=""
					 ) : CParamBNRange<enumType> (pp,  titel,  etiq, parRef, min, max, defValue, unit	)
	          { if (!inRang(defValue)) 

			    throw ParamOutOfEnumRange(std::string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }									
											// enumType &parRef,   usa _v y por tanto no necesita un parametro externo
	 CParamEnumRange (CProgParam *pp, const std::string& titel, const std::string& etiq, 
						enumType min, enumType max, enumType defValue,
						const std::string& unit=""
					 ) : CParamBNRange<enumType> (pp,  titel,  etiq, min, max, defValue, unit	)
	          { if (!inRang(defValue)) 
			    throw ParamOutOfEnumRange(std::string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }								
	void set(int value){	if (_IntValues.empty())
								set(enumType(value));
							else{
								auto p=_IntValues.find(value);
								if (p==_IntValues.end())	// TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(std::string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with know values "  + StringEnumerate() ,
												value , *this  );
								CParamBNRange::set(enumType(p->second));
							}
	                    }
	void set(std::string value){		auto p=_StrValues.find(value);
								if (p==_StrValues.end())	// TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(std::string( "Value \"" ) + value + "\" out of Range while trying to modify: \"" 
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with have know values: "  + StringEnumerate()  );
								CParamBNRange::set(p->second);
								set(p->second);
	                    }
	void set(enumType value){ if (!inRang(value)) // TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(std::string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with know values "  + StringEnumerate() ,
												value , *this  );
							 CParamBNRange::set(value) ; 
	                    }

	//virtual ostream	    &saveValue	(ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
	std::ostream &saveValue	(std::ostream	&osPr) const override   
	                        {	for (auto p : _StrValues) 
									if (p.second==get())
										return osPr<<p.first;
								return osPr<< int(get());
							} 
    bool        loadValue   (std::istream   &isPr) override         
	                        {   std::string t; 
								 //try
									{	isPr>>t; 
										if (exist(t))
										{	set(t);
											return true;
										}// if(!_StrValues.empty()) return false;
										set (stoi(t));
										return true;
									} //catch (OutOfNumRange) {}
								 return false;
							}
};

template <typename Num>
class CParamNumMinMax: public IBParam
{   NumRang<Num>       _v ;
    CParamNumRange<Num> min, max; 
 public:
    CParamNumMinMax (CProgParam *pp, const std::string& titel, NumRang<Num> &parRef ,// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
		            const std::string& titelmin, const std::string& etiqmin, Num minmin, Num maxmin, Num defValuemin,
		            const std::string& titelmax, const std::string& etiqmax, Num minmax, Num maxmax, Num defValuemax,
		            const std::string& unit=""
					) : IBParam ( titel),
					    min(pp, titel+". "+titelmin, etiqmin,  parRef.Min(), minmin, maxmin, defValuemin, unit),
					    max(pp, titel+". "+titelmax, etiqmax,  parRef.Max(), minmax, maxmax, defValuemax, unit)
	          { 
	          }
    CParamNumMinMax (CProgParam *pp, const std::string& titel, //CNumRang<Num> &parRef ,//   usa _v y por tanto no necesita un parametro externo
		            const std::string& titelmin, const std::string& etiqmin, Num minmin, Num maxmin, Num defValuemin,
		            const std::string& titelmax, const std::string& etiqmax, Num minmax, Num maxmax, Num defValuemax,
		            const std::string& unit=""
					) : IBParam (titel),
					    min(pp, titel+". "+titelmin, etiqmin,  _v.Min(), minmin, maxmin, defValuemin, unit),
					    max(pp, titel+". "+titelmax, etiqmax,  _v.Max(), minmax, maxmax, defValuemax, unit)
	          { 
	          }
};

#endif


