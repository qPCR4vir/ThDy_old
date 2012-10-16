#ifndef _INIT_PROG_PARAM_H
#define _INIT_PROG_PARAM_H
#pragma unmanaged
#include <iostream>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <assert.h>
#include <time.h>
#include <map>
#include <vector>

//#include "..\ThDySec\matrix.h" 
// TODO:  PROBLEMA : como organizar estos parametros si usamos procesos? Hacer copia de ellos !!!!!!!!?
// Definiciones y declaraciones para user interface. A usar tambien por programs. Casi Primaria, depende solo de Common basics.
using namespace std ; 
#pragma warning( disable : 4996 )
#include "..\ThDySec\common_basics.h" 





class CProgParam ;
class IBParam
{
    string _Titel;
 public:
	 IBParam (CProgParam *pp, const string& titel) 
		    :_Titel(titel), ValueChanged(nullptr)
	        {         //  pp->_parametrs[_etiq]= *this;
	        }    
	 string Titel()const{return _Titel;}    // Human redeable
	 void SetTitel(string titel){ _Titel=titel;}    // Human redeable


 virtual ostream &save	(ostream	&osPr) const
			            {   osPr<< ".\t"<<Titel()<<endl; 
							return osPr;
			            } 
 virtual bool   load	(istream   &isPr)  /*throw( std::out_of_range)  */              
			            {   return false;}   
 virtual bool   load	(string		&etiq, istream &isPr) /*throw( std::out_of_range)*/
			            {   return false;}   

	 virtual ~IBParam(){}
     void (*ValueChanged)(IBParam& param) ;
 protected: 
	void changed()
	            {   if(ValueChanged) 
				        ValueChanged(*this);
	            }
 virtual void insertParam(CProgParam *pp){}
};
   // ifstream& operator >>(ifstream& ifs,IParam& p);                             //    ?????????????????
   // ofstream& operator <<(ofstream& ofs,const IParam& p){ p.save(ofs); return ofs;};  //    ?????????????????

class IParam : public IBParam
{    string _etiq, _unit;
 public:
	 IParam (CProgParam *pp, const string& titel, const string& etiq, const string& unit="") 
		    : IBParam(pp, titel), 
			  _etiq(etiq), _unit(unit)
	        {  
				assert (pp); 
				insertParam(pp); 
				if (_etiq=="")          // si no quieres introducir una etiq puedes usar el Titel !!
					_etiq=Titel();

	        }    
	 string Etiq()const{return _etiq;}      // semiHuman redeable and unic. Best with length 10
	 void SetEtiq(string etiq){ _etiq=etiq;}    // Human redeable
	
	 string Unit()const{return _unit;}      // Human redeable and optional

	 ostream	&save	(ostream	&osPr) const override
			            {   osPr<< _etiq << ": "; 
			                saveValue(osPr)<<" "<<_unit<<" ";
							IBParam::save(osPr)	; 
							return osPr;
			            } 
     bool       load	(istream   &isPr)  /* throw( std::out_of_range)    */      override         //    Asume etiqueta ya comprobada !!!!
			            {   return loadValue(isPr);}   
	 bool       load	(string		&etiq, istream &isPr) /*throw( std::out_of_range)   */  override
			            {   if (etiq!=_etiq) 
						        return false;
			                return load(isPr);
			            }  
	virtual ostream	    &saveValue	(ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
	                                {return osPr;} 
    virtual bool        loadValue	(istream   &isPr) /*throw( std::out_of_range)   */
	                                {return false;}         // =0;   ??    ?????????????????
    virtual void        insertParam (CProgParam *pp) override;

	  ~IParam()override{}
};

class CParamBool: public IParam
{    bool  _v, &_value;
 public:
						// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamBool (CProgParam *pp,  const string& titel, const string& etiq, bool &parRef,    
		            bool defValue
					) : IParam (pp, titel, etiq), _value(parRef)            {  _value= defValue;	          }

					   //  no necesita un parametro externo
    CParamBool (CProgParam *pp,  const string& titel, const string& etiq, 
		            bool defValue
					) : IParam (pp, titel, etiq), _v(defValue) , _value(_v) {            }

	void set(bool value){ if (value == _value) return; _value = value;  changed();    }
	bool get()const{ return _value;    }

	ostream	    &saveValue	(ostream	&osPr) const override   
	                        {   osPr << boolalpha << _value ;     
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (istream   &isPr) override   // Descarta el Titel que queda detras del ultimo tab
	                    {   isPr>>boolalpha>>_value;
							return true;
	                    } 
};

class CParamString: public IParam
{    string  _v, &_value;
 public:
								 // Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamString (CProgParam *pp,  const string& titel, const string& etiq, string &parRef,  
		            const string defValue
					) : IParam (pp, titel, etiq), _value(parRef)            {  _value= defValue;	          }

							     //  no necesita un parametro externo
    CParamString (CProgParam *pp,      const string& titel, const string& etiq, 
		            const string defValue
					) : IParam (pp, titel, etiq), _v(defValue) , _value(_v) {            }

	void set(const string& value){ if (value == _value) return; _value == value;  changed();    }
	string get()const{ return _value;    }

	virtual ostream	    &saveValue	(ostream	&osPr) const override   
	                        {   osPr << _value << endl <<"\t\t\t\t" ;     
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (istream   &isPr) override   // Descarta el Titel que queda detras del ultimo tab
	                    {   string t; getline(isPr,t); 
	                        _value=trim_string( t.substr(0, t.rfind("\t")).c_str()   );
							return true;
	                    } 
};
class CParamC_str: public IParam, public C_str
{public:
    //CParamC_str (CProgParam *pp, C_str &parRef,    // Acepta un parametro y por tanto no usa _v. Por compatibilidad.
		  //          const string& titel, const string& etiq, 
		  //          const char * defValue
				//	) : IParam (pp, titel, etiq), _value(parRef)            {  _value.Copy( defValue) ;	          }
    CParamC_str (CProgParam *pp,                  //  no necesita un parametro externo
		            const string& titel, const string& etiq, 
		            const char * defValue
					) : IParam (pp, titel, etiq), C_str(defValue)                {            }
    CParamC_str (CProgParam *pp,                  //  no necesita un parametro externo
		            const string& titel, const string& etiq, 
		            const C_str& defValue
					) : IParam (pp, titel, etiq), C_str(defValue)                {            }

	void set(const C_str& value){ if (! strcmp (Get(),value.Get())) return; Copy(value) ;  changed();    }
	void set(const char * value){ if (! strcmp (Get(),value))       return; Copy(value) ;  changed();    }

	ostream	    &saveValue	(ostream	&osPr) const override   
	                        {   osPr << Get() << endl <<"\t\t\t\t" ;     // Por que no se puede poner el return directo????
	                            return osPr;        // sera solo un problema de IntelliSense ??
	                        } 

	bool        loadValue (istream   &isPr) override   // Descarta el Titel que queda detras del ultimo tab
	                    {   string t; getline(isPr,t); 
	                        CopyTrim( t.substr(0, t.rfind("\t")).c_str()   );
							return true;
	                    } 
};
template <typename Num>
class CParamBNRange: public IParam, public NumRang<Num>
{
    Num  _v, &_value;
 public:
									// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamBNRange (CProgParam *pp, const string& titel, const string& etiq, Num &parRef, 
						Num min, Num max, Num defValue,
						const string& unit=""
					) : IParam (pp, titel, etiq, unit), 
					    NumRang<Num>(min,max), 
						_value(parRef)
	          { /*if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(string("Default Value out of Range while trying to construct: ")+Titel() ); */
	             _value=defValue ;
	          }

								// Num &parRef,   usa _v y por tanto no necesita un parametro externo
    CParamBNRange (CProgParam *pp, const string& titel, const string& etiq, 
						Num min, Num max, Num defValue,
						const string& unit=""
					) : IParam (pp, titel, etiq, unit), NumRang<Num>(min,max), _value(_v)
	          { /*if (!inRang(defValue)) 
			        throw OutOfNumRange(string("Default Value out of Range while trying to construct: ")+Titel() );*/
	             _value=defValue ; 
	          }
	void set(Num value){ if (!inRang(value)) // TODO: Incluir value y rang que no concuerdan en mensaje to throw
		                    throw OutOfNumRange(string("Value out of Range while trying to set: ")+Titel(), value, *this );
	                     if (_value==value) return; 
						 _value=value ; 
						 changed();
	                    }
	Num get()const{return _value;}
	//virtual ostream	    &saveValue	(ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
};


class ParamOutOfNumRange : public OutOfNumRange 
{ public: 
	explicit ParamOutOfNumRange ( const std::string& what_arg ): OutOfNumRange(what_arg ){}
		template<typename Num>
	ParamOutOfNumRange ( const std::string& what_arg, Num invalidValue, NumRang<Num>& NR ) 
		: OutOfNumRange (  what_arg ,  invalidValue,  NR ) 
		{}
};
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
    CParamNumRange (CProgParam *pp, const string& titel, const string& etiq, Num &parRef, 
						Num min, Num max, Num defValue,
						const string& unit=""
					) : CParamBNRange (pp, titel, etiq, parRef,min,  max,  defValue,unit)
	          { if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }
								// Num &parRef,   usa _v y por tanto no necesita un parametro externo
    CParamNumRange (CProgParam *pp, const string& titel, const string& etiq, 
						Num min, Num max, Num defValue,
						const string& unit=""
					) : CParamBNRange (pp, titel, etiq,min,  max,  defValue,unit)
	          { if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }
	ostream	&saveValue	(ostream	&osPr) const override   
	                        {return osPr<<get();} 
    bool        loadValue   (istream   &isPr) /*throw( ParamOutOfNumRange) */  override         
	                        {   Num t; 
								isPr>>t; 
	                            set(t);
								return true;
	                        } 
	void set(Num value){ if (!inRang(value)) 
		                    throw ParamOutOfNumRange(string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")"
													 , value, *this );
						  CParamBNRange::set(value) ; 
	                    }

};

template <typename enumType>
class CParamEnumRange: public CParamBNRange<enumType>
{
   map<string, enumType> _StrValues;
   map<int, enumType>	 _IntValues;

 public:
	 void AddEnumValues(					enumType eTy )		{ _IntValues[int(eTy)]=eTy;}
	 void AddStrValues (const string& strV, enumType eTy )		{ _StrValues[strV]    =eTy;
																	AddEnumValues(eTy );	}
	 string ToString(enumType v)const
		{ for (auto p : _StrValues) 
			if (p.second==v)
				return p.first;
		  throw OutOfNumRange(string("Value out of Range while trying to set: ")+Titel() );
		}
	 string StringEnumerate()const 
	 {	std::ostringstream result;
		for (auto p : _StrValues)
			result <<  p.first << "(" << int(p.second) <<"), ";
		return result.str();
	 }
	 string ToString()const {return ToString(get());}
	 bool exist(int v)		const{		return _IntValues.end()!=_IntValues.find(v); }
	 bool exist(string v)	const{		return _StrValues.end()!=_StrValues.find(v); }


									// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
	 CParamEnumRange (CProgParam *pp, const string& titel, const string& etiq, enumType &parRef, 
						enumType min, enumType max, enumType defValue,
						const string& unit=""
					 ) : CParamBNRange<enumType> (pp,  titel,  etiq, parRef, min, max, defValue, unit	)
	          { if (!inRang(defValue)) 

			    throw ParamOutOfEnumRange(string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }									
											// enumType &parRef,   usa _v y por tanto no necesita un parametro externo
	 CParamEnumRange (CProgParam *pp, const string& titel, const string& etiq, 
						enumType min, enumType max, enumType defValue,
						const string& unit=""
					 ) : CParamBNRange<enumType> (pp,  titel,  etiq, min, max, defValue, unit	)
	          { if (!inRang(defValue)) 
			    throw ParamOutOfEnumRange(string("Error contructing parametr: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", tryin to set the default value " ,
												defValue , *this  );
	          }								
	void set(int value){	if (_IntValues.empty())
								set(enumType(value));
							else{
								auto p=_IntValues.find(value);
								if (p==_IntValues.end())	// TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with know values "  + StringEnumerate() ,
												value , *this  );
								CParamBNRange::set(enumType(p->second));
							}
	                    }
	void set(string value){		auto p=_StrValues.find(value);
								if (p==_StrValues.end())	// TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(string( "Value \"" ) + value + "\" out of Range while trying to modify: \"" 
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with have know values: "  + StringEnumerate()  );
								CParamBNRange::set(p->second);
								set(p->second);
	                    }
	void set(enumType value){ if (!inRang(value)) // TODO: Incluir value y rang que no concuerdan en mensaje to throw
									throw ParamOutOfEnumRange(string("Value out of Range while trying to modify: \"")
												     + Titel() 
												     + "\" ("+ Etiq() + ")" + ", with know values "  + StringEnumerate() ,
												value , *this  );
							 CParamBNRange::set(value) ; 
	                    }

	//virtual ostream	    &saveValue	(ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
	ostream	&saveValue	(ostream	&osPr) const override   
	                        {	for (auto p : _StrValues) 
									if (p.second==get())
										return osPr<<p.first;
								return osPr<< int(get());
							} 
    bool        loadValue   (istream   &isPr) override         
	                        {   string t; 
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
    CParamNumMinMax (CProgParam *pp, const string& titel, NumRang<Num> &parRef ,// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
		            const string& titelmin, const string& etiqmin, Num minmin, Num maxmin, Num defValuemin,
		            const string& titelmax, const string& etiqmax, Num minmax, Num maxmax, Num defValuemax,
		            const string& unit=""
					) : IBParam (pp, titel), 
					    min(pp, titel+". "+titelmin, etiqmin,  parRef.Min(), minmin, maxmin, defValuemin, unit),
					    max(pp, titel+". "+titelmax, etiqmax,  parRef.Max(), minmax, maxmax, defValuemax, unit)
	          { 
	          }
    CParamNumMinMax (CProgParam *pp, const string& titel, //CNumRang<Num> &parRef ,//   usa _v y por tanto no necesita un parametro externo
		            const string& titelmin, const string& etiqmin, Num minmin, Num maxmin, Num defValuemin,
		            const string& titelmax, const string& etiqmax, Num minmax, Num maxmax, Num defValuemax,
		            const string& unit=""
					) : IBParam (pp, titel), 
					    min(pp, titel+". "+titelmin, etiqmin,  _v.Min(), minmin, maxmin, defValuemin, unit),
					    max(pp, titel+". "+titelmax, etiqmax,  _v.Max(), minmax, maxmax, defValuemax, unit)
	          { 
	          }
};


/*   Para crear y anadir un nuevo programa:
		- crear interfase de usuario para tener idea de los parametros a usar
		- crear funcion del programa en su propio .cpp en el proyecto de prog
		- crear class tomando como base CEspProgParam o uno de sus derivados : anadir los paramet espec com se vio en la interfase e inicializarlos en el constr
		- implementar funciones de actualizar parametros <=> interfase de usuario : UpdateThDyP() & UpdateThDyForm()
		- implementar funciones load/save del project. Load: con etiqueta para cada param, "	<< boolalpha " para bool, 
*/

class CProgProject;
class CEspProgParam ;

class CProgParam : public IBParam // -------	  Clase base "interfase" para param de prog.Solo salva/load project y run prog (virtual todo)   ----------
{   
 public:
    map<string,IParam*> _parametrs;
 
	CProgParam (const string& titel, CProgProject *proj=nullptr); /*:_Titel(titel){ if (proj) proj->_ProgList.push_back(this);}*/
	ofstream	    &save		(ofstream	&osPr				 )  const
	                            {   osPr << endl <<"\t------\t"<<Titel()<<" "<<endl ;
									for (auto &par : _parametrs) 
								        par.second->save(osPr); 
	                                return osPr;
	                            }
	         bool	load		(string		&etiq, ifstream &isPr) 
	                            {   auto p=_parametrs.find(etiq); 
	                                if (p==_parametrs.end()) 
										return false;
	                                return p->second->load(isPr); //throw execption if false ????
	                            }
	virtual	int		Run			(CProgParam &prog				){return prog.Run();}       //  ???????
	virtual int		Run			(		void					)
	                            {   for(int WorkToDo=Initialize(); WorkToDo>0 ; WorkToDo=Continue()) 
										CallBack(WorkToDo); 
									return Finalize();			 
	                            } 
	virtual int		Initialize	(		void					){ return 0;} 
	virtual int		Continue	(		void					){ return false;}
	virtual int		Finalize	(		void					){ return 0;} 
	virtual void	CallBack	(		int WorkToDo			){}
	virtual ~CProgParam() override{}
};	

typedef CEspProgParam *pCEspProgParam ;

// derivar para concretar parametros comunes. Mantiene link a proj de los prog Espec que los usan

// clase base para los parametros "Especificos" de programas "Especificos"

class CProgProject : public CProgParam
{
public:
	CProgProject(const string& titel, const char *prFname="", const char*defProFN="Def.Proj.txt")
		:CProgParam(titel), _ProjetFileName(prFname)   ,
							_defPr(defProFN)  
	        {  if (!prFname || !prFname[0]) 
			    _ProjetFileName.Copy(_defPr);   //    ???????????????????????????????????????ß
	        } 

		C_str						_defPr ;
		C_str						_ProjetFileName ;

   ~CProgProject()override { }

    //  Este es el verdadero save !!! El que abre el fichero y lo salva todo.
	ofstream	&saveToFile	(const char *ProjetFileName) const{	ofstream osPr(ProjetFileName);			return save_all(osPr);}

	ofstream	&save		()			const			{	return saveToFile(_ProjetFileName.Get())	;   }
	ofstream	&save_defPr	()                          {ProjetFile(_defPr.Get()); return save();	        }
	void	    ProjetFile	(const char *ProjetFileName){	_ProjetFileName.CopyTrim(	ProjetFileName);	}

	bool	    load		(); 

		// OJO !!!!!!!!!   las sig funciones se aduenan del pointer, y luego lo deletean    !!!!!!!!
	void	    SetProjetFile(char  *ProjetFileName)
	                    {   ProjetFile(ProjetFileName) ; 		
	                        delete []ProjetFileName ;				
	                    }
	ofstream	&save	 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return save();			}
	bool	    load	 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return load();			}



    //  Derivar para usar el MakeProgName de un CComP
	virtual ofstream &saveTMP() const            // Reescribe el projecto actual. Pensar algo mejor? Preguntar al user? usar # conscuti?
	                            {	return save();	}

	//ofstream	&	save    (ofstream &osPr)	const	override	 ;//{  	return true ;    }
	//      bool	load    (string &etiq, ifstream &isPr);//{ 	if (true) return false ; return true ;}
	   ofstream	&	save_all(ofstream &osPr)	const 			
	                        {   for(auto p : _ProgList) 
						            p->save(osPr) ;		
	                             CProgParam::save(osPr) ;   
	   
	   osPr<< endl<<endl<<
			 "How to use? \n Each program´s parameter have an unique identificator or etiquette. \n "
			 "While loading, the text between the beginning of a line and the first : will be taken as\n "
			 "an etiquette (discarding surrounding but not internal spaces). \n"
			 "IF the etiquette is know (valid), the rest of the line will be use to deduce the value of the parameter. \n"
			 "Some parameter (like file´s names) will assume this rest-line-text entirely as his valid value. \n"
			 "For such parameter please, add any comment in the next line. \n"
			 "Other parameter (like numeric or bool parameters) will only use the beginning of this rest-line-text and discard the end. \n"
			 "Any line without a valid etiquette will be discarded (they are comments!).” \n"
			 "Only the last valid value of each parameter will be used\n"
			 "For not defined parameters, the previous value (from the previously active project or from the program´s default) will be use.\n"
			 "Direct questions please to ArielVina.Rodriguez@fli.bund.de\n"
			;

	   return (osPr) ;
	   
	   }   // por que solo funciona con el CProgParam:: ???
	bool	    load_all(string &etiq, ifstream &isPr)	//override
	                    {   for(auto p : _ProgList)	
					            if ( p->load(etiq, isPr)) 
								    return true ;
						    return CProgParam::load(etiq, isPr);					 }
	int		Run (CProgParam &prog)	override                    //   ??????
	                 {	saveTMP( ) ; 
	                    return prog.Run();
	                 }

	void AddProg (CProgParam* par) {_ProgList.push_back(par);}
private:
	vector<CProgParam*> _ProgList;
};
class CCommProgParam : public CProgParam 
{	CProgProject *_proj;
 public:	
	CCommProgParam  (const string& titel,       CProgProject *proj=nullptr)
		            : CProgParam(titel,proj),   _proj(proj) {}
	~CCommProgParam() override	{}

	ofstream	&save_all(ofstream	&osPr 				 ) const
	                    {   return _proj->save_all(osPr);
	                    }
	bool	    load_all(string     &etiq, ifstream &isPr)
	                    {return _proj->load_all(etiq,isPr);
	                    } 
	void        AddProgToProject(CProgParam *p)
	                    {_proj->AddProg(p);}
    virtual string  MakeRuningName()const {return "";}

};
class	CEspProgParam  : public CProgParam 
{public:										// Permite no duplicar los parametros comunes en los parametros especificos
	explicit CEspProgParam(const string& titel, CCommProgParam &commParam ) 
		                    : _cp(commParam), CProgParam(titel ) 
	                        { _cp.AddProgToProject(this);}
	CCommProgParam &_cp;
	ofstream	&save_all(ofstream &osPr)	const		   // Save all needed parametrs for this programm, not only the spesific ones
	                     {          _cp.save(osPr) ;			
	                         return     save(osPr) ;    }
	bool	    load_all(string &var, ifstream &isPr)	  // Usar estas dos funciones solo si se quiere save or load olny this program
	                    { 	if ( _cp.load(var, isPr))       // PAra salvar el projecto completo use el save_all del projecto
						        return true ;
							return load(var, isPr);					 }
} ;


#endif


