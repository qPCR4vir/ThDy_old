#ifndef _INIT_PROG_PARAM_IMPL_H
#define _INIT_PROG_PARAM_IMPL_H
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
//using namespace std ; 
#pragma warning( disable : 4996 )
#include "..\ThDySec\common_basics.h" 

class CProgParam ;
class IBParam
{
   std::string _Titel;
 public:
   IBParam (CProgParam *pp, const std::string &titel)      //  pp->?????????????
		    :_Titel(titel), ValueChanged(nullptr)
	        {         //  pp->_parametrs[_etiq]= *this;
	        }    
	 std::string Titel   ()const{return _Titel;}    // Human redeable
	 void        SetTitel(std::string titel){ _Titel=titel;}    // Human redeable


 virtual std::ostream &save	(std::ostream	&osPr) const
			            {   osPr<< ".\t"<<Titel()<<std::endl; 
							return osPr;
			            } 
 virtual bool   load	(std::istream   &isPr)  /*throw( std::out_of_range)  */              
			            {   return false;}   
 virtual bool   load	(std::string &etiq, std::istream &isPr) /*throw( std::out_of_range)*/
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
{    std::string _etiq, _unit;
 public:
	 IParam (CProgParam *pp, const std::string& titel, const std::string& etiq, const std::string& unit="") 
		    : IBParam(pp, titel), 
			  _etiq(etiq), _unit(unit)
	        {  
				assert (pp); 
				insertParam(pp); 
				if (_etiq=="")          // si no quieres introducir una etiq puedes usar el Titel !!
					_etiq=Titel();

	        }    
	 std::string Etiq()const{return _etiq;}      // semiHuman redeable and unic. Best with length 10
	 void SetEtiq(std::string etiq){ _etiq=etiq;}    // Human redeable
	
	 std::string Unit()const{return _unit;}      // Human redeable and optional

	 std::ostream	&save	(std::ostream	&osPr) const override
			            {   osPr<< _etiq << ": "; 
			                saveValue(osPr)<<" "<<_unit<<" ";
							IBParam::save(osPr)	; 
							return osPr;
			            } 
     bool       load	(std::istream   &isPr)  /* throw( std::out_of_range)    */      override         //    Asume etiqueta ya comprobada !!!!
			            {   return loadValue(isPr);}   
	 bool       load	(std::string		&etiq, std::istream &isPr) /*throw( std::out_of_range)   */  override
			            {   if (etiq!=_etiq) 
						        return false;
			                return load(isPr);
			            }  
	virtual std::ostream	    &saveValue	(std::ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
	                                {return osPr;} 
    virtual bool        loadValue	(std::istream   &isPr) /*throw( std::out_of_range)   */
	                                {return false;}         // =0;   ??    ?????????????????
    virtual void        insertParam (CProgParam *pp) override;

	  ~IParam()override{}
};

template <typename Num>
class CParamBNRange: public IParam, public NumRang<Num>
{
    Num  _v, &_value;
 public:
								/// Acepta un parametro y por tanto no usa _v. Por compatibilidad.
    CParamBNRange (CProgParam *pp, const std::string& titel, const std::string& etiq, Num &parRef, 
						Num min, Num max, Num defValue,
						const std::string& unit=""
					) : IParam (pp, titel, etiq, unit), 
					    NumRang<Num>(min,max), 
						_value(parRef)
	          { /*if (!inRang(defValue)) 
			        throw ParamOutOfNumRange(string("Default Value out of Range while trying to construct: ")+Titel() ); */
	             _value=defValue ;
	          }

								/// Num &parRef,   usa _v y por tanto no necesita un parametro externo
    CParamBNRange (CProgParam *pp, const std::string& titel, const std::string& etiq, 
						Num min, Num max, Num defValue,
						const std::string& unit=""
					) : IParam (pp, titel, etiq, unit), NumRang<Num>(min,max), _value(_v)
	          { /*if (!inRang(defValue)) 
			        throw OutOfNumRange(string("Default Value out of Range while trying to construct: ")+Titel() );*/
	             _value=defValue ; 
	          }
	void set(Num value){ if (!inRang(value)) /// TODO: Incluir value y rang que no concuerdan en mensaje to throw
		                    throw OutOfNumRange(std::string("Value out of Range while trying to set: ")+Titel(), value, *this );
	                     if (_value==value) return; 
						 _value=value ; 
						 changed();
	                    }
	Num get()const{return _value;}
	//virtual ostream	    &saveValue	(ostream	&osPr	) const  // =0;   ??No salva nada, no tiene "value" todavia
};


class CProgProject;
class CEspProgParam ;
/**   Para crear y anadir un nuevo programa:
 *		- crear interfase de usuario para tener idea de los parametros a usar
 *		- crear funcion del programa en su propio .cpp en el proyecto de prog
 *		- crear class tomando como base CEspProgParam o uno de sus derivados : anadir los paramet espec com se vio en la interfase e inicializarlos en el constr
 *		- implementar funciones de actualizar parametros <=> interfase de usuario : UpdateThDyP() & UpdateThDyForm()
 *		- implementar funciones load/save del project. Load: con etiqueta para cada param, "	<< boolalpha " para bool, 
 */
class CProgParam : public IBParam // -------	  Clase base "interfase" para param de prog.Solo salva/load project y run prog (virtual todo)   ----------
{   
 public:
    std::map<std::string,IParam*> _parametrs;
 
	CProgParam (const std::string& titel, CProgProject *proj=nullptr); /*:_Titel(titel){ if (proj) proj->_ProgList.push_back(this);}*/
	std::ofstream	&save		(std::ofstream	&osPr				 )  const
	                            {   osPr << std::endl <<"\t------\t"<<Titel()<<" "<<std::endl ;
									for (auto &par : _parametrs) 
								        par.second->save(osPr); 
	                                return osPr;
	                            }
	         bool	load		(std::string		&etiq, std::ifstream &isPr) 
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
/// How to use?  Each program's parameter have an unique identificator or etiquette.  
/// While loading, the text between the beginning of a line and the first : will be taken as 
/// an etiquette (discarding surrounding but not internal spaces). 
/// IF the etiquette is know (valid), the rest of the line will be use to deduce the value of the parameter. 
/// Some parameter (like file´s names) will assume this rest-line-text entirely as his valid value. 
/// For such parameter please, add any comment in the next line. 
/// Other parameter (like numeric or bool parameters) will only use the beginning of this rest-line-text and ignore the end. 
/// Any line without a valid etiquette will be ignore (they are comments!).
/// Only the last valid value of each parameter will be used
/// For not defined parameters, the previous value (from the previously active project or from the program´s default) will be use.
/// Direct questions please to ArielVina.Rodriguez@fli.bund.de
//;

/// derivar para concretar parametros comunes. Mantiene link a proj de los prog Espec que los usan
/// clase base para los parametros "Especificos" de programas "Especificos"
class CProgProject : public CProgParam
{
public:
	CProgProject(const std::string& titel, const char *prFname="", const char*defProFN="Def.Proj.txt")
		:CProgParam(titel), _defPr(defProFN)  ,
		_ProjetFileName(prFname)   
							
	        {  if (!prFname || !prFname[0]) 
			    _ProjetFileName.Copy(_defPr);   //    ???????????????????????????????????????ß
	        } 

		C_str					_defPr ;
		C_str					_ProjetFileName ;

   ~CProgProject()override { }

    //  Este es el verdadero save !!! El que abre el fichero y lo salva todo.
	std::ofstream	&saveToFile	(const char *ProjetFileName) const{	std::ofstream osPr(ProjetFileName);			return save_all(osPr);}

	std::ofstream	&save		()		const		{	return saveToFile(_ProjetFileName.Get())	;   }
	std::ofstream	&save_defPr	()                  {ProjetFile(_defPr.Get()); return save();	        }
	void	    ProjetFile	(const char *ProjetFileName){	_ProjetFileName.CopyTrim(	ProjetFileName);	}

	bool	    load		(); 

		// OJO !!!!!!!!!   las sig funciones se aduenan del pointer, y luego lo deletean    !!!!!!!!
	void	    SetProjetFile(char  *ProjetFileName)
	                    {   ProjetFile(ProjetFileName) ; 		
	                        delete []ProjetFileName ;				
	                    }
	std::ofstream &save	 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return save();			}
	bool	    load	 (char	*ProjetFileName){	SetProjetFile(ProjetFileName); return load();			}



    ///  Derivar para usar el MakeProgName de un CComP
	virtual std::ofstream &saveTMP() const            /// Reescribe el projecto actual. Pensar algo mejor? Preguntar al user? usar # conscuti?
	                            {	return save();	}

	//ofstream	&	save    (ofstream &osPr)	const	override	 ;//{  	return true ;    }
	//      bool	load    (string &etiq, ifstream &isPr);//{ 	if (true) return false ; return true ;}
	   std::ofstream&	save_all(std::ofstream &osPr)	const 			
	                        {   for(auto p : _ProgList) 
						            p->save(osPr) ;		
	                             CProgParam::save(osPr) ;   
	   
	   osPr<< std::endl<<std::endl<<
			 "How to use? \n Each program´s parameter have an unique identificator or etiquette. \n "
			 "While loading, the text between the beginning of a line and the first : will be taken as\n "
			 "an etiquette (discarding surrounding but not internal spaces). \n"
			 "IF the etiquette is know (valid), the rest of the line will be use to deduce the value of the parameter. \n"
			 "Some parameter (like file´s names) will assume this rest-line-text entirely as his valid value. \n"
			 "For such parameter please, add any comment in the next line. \n"
			 "Other parameter (like numeric or bool parameters) will only use the beginning of this rest-line-text and ignore the end. \n"
			 "Any line without a valid etiquette will be ignore (they are comments!).” \n"
			 "Only the last valid value of each parameter will be used\n"
			 "For not defined parameters, the previous value (from the previously active project or from the program´s default) will be use.\n"
			 "Direct questions please to ArielVina.Rodriguez@fli.bund.de\n"
			;

	   return (osPr) ;
	   
	   }   // por que solo funciona con el CProgParam:: ???
	bool	    load_all(std::string &etiq, std::ifstream &isPr)	//override
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
	std::vector<CProgParam*> _ProgList;
};
class CCommProgParam : public CProgParam 
{	CProgProject *_proj;
 public:	
	CCommProgParam  (const std::string& titel,       CProgProject *proj=nullptr)
		            : CProgParam(titel,proj),   _proj(proj) {}
	~CCommProgParam() override	{}

	std::ofstream	&save_all(std::ofstream	&osPr 				 ) const
	                    {   
							assert(("Atemt to use an unitialized project pointer in save_all",_proj));
							return _proj->save_all(osPr);
	                    }
	bool	    load_all(std::string     &etiq, std::ifstream &isPr)
	                    {
							assert(("Atemt to use an unitialized project pointer in load_all",_proj));
							return _proj->load_all(etiq,isPr);
	                    } 
	void        AddProgToProject(CProgParam *p)
	                    {
							assert(("Atemt to use an unitialized project pointer in AddProgToProject",_proj));
							_proj->AddProg(p);
	                    }
    virtual std::string  MakeRuningName()const {return "";}

};
class	CEspProgParam  : public CProgParam 
{public:										// Permite no duplicar los parametros comunes en los parametros especificos
	explicit CEspProgParam(const std::string& titel, CCommProgParam &commParam ) 
		                    : _cp(commParam), CProgParam(titel ) 
	                        { _cp.AddProgToProject(this);}
	CCommProgParam &_cp;
	std::ofstream	&save_all(std::ofstream &osPr)	const		   // Save all needed parametrs for this programm, not only the spesific ones
	                     {          _cp.save(osPr) ;			
	                         return     save(osPr) ;    }
	bool	    load_all(std::string &var, std::ifstream &isPr)	  // Usar estas dos funciones solo si se quiere save or load olny this program
	                    { 	if ( _cp.load(var, isPr))       // PAra salvar el projecto completo use el save_all del projecto
						        return true ;
							return load(var, isPr);					 }
} ;


#endif


