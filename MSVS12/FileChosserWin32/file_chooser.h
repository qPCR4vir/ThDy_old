#include "stdafx.h"
#include <ostream>
#include <vector>
#include <map>
#include <assert.h>

class CFileChosserAsk;

class CExtGUI
{public:
	CExtGUI (std::string Name="Universal",  CExtGUI *Def_GUI=nullptr, bool available_=true)	: name (Name)
	{   reguistre re={ this, Def_GUI, available_} ; 		GUI_list[name]= re;	}

	static CExtGUI *mountExtGUI(std::string Name) {return mountExtGUI ( GUI_list[Name]).GUI;}
	static CExtGUI *GUI(){return getGUI(_currGUI);}
	virtual CFileChosserAsk& FileChosserAsk();
private:
	std::string name ;
	struct reguistre	{	CExtGUI *GUI, *DefGUI ;	  bool available ;	
							reguistre():GUI(nullptr),DefGUI(nullptr), available(false){} } ;
	static std::map<std::string,reguistre> GUI_list ;
	static reguistre _currGUI, UniversalGUI;
	static reguistre mountExtGUI(reguistre newGUI)
	{	if(newGUI.GUI   && newGUI.available)	return _currGUI=newGUI;
		if(newGUI.DefGUI)						return mountExtGUI( GUI_list[newGUI.DefGUI->name] ) ;
		if(_currGUI.GUI && _currGUI.available)	return _currGUI;
		if(UniversalGUI.GUI && UniversalGUI.available) return _currGUI=UniversalGUI;
		assert(false);
	}
	CExtGUI *getGUI(reguistre reg)
	{ if(reg.GUI && reg.available)	 return reg.GUI;
	  if(reg.DefGUI) return reg.DefGUI;
	  return UniversalGUI.GUI;
	}
		
	static bool Available(CExtGUI *xGUI){ } 	
	void mount(){mountExtGUI(get_registre());};
	reguistre get_registre(){return GUI_list[name]; }



};


template<class StrType>
class CFileFiltre : public std::vector<StrType>
{public:
	CFileFiltre(	const StrType				&filtreName		): _Name(filtreName),_sep(StrType(";"))  {}

    // CFileFiltre<std::string> myTxtFiltre ("Plain txt",{"*.txt","*.ascii"}); 
	CFileFiltre(	const StrType				&filtreName, 
					const std::vector<StrType>	&filtres	
				): base(filtres),_Name(filtreName),_sep(StrType(";"))  {}


	 CFileFiltre( ){}
	~CFileFiltre( ){};

	 CFileFiltre& operator<<(const StrType& filtre) { push_back(filtre); return *this;}

	 StrType Name()const{return _Name;}
	 StrType Sep ()const{return _sep ;}
	 StrType Sep (const StrType &separator){StrType oldSep=_sep; _sep=separator; return oldSep;}
	 StrType ConcatenedFiltres(const StrType &separator=_sep) const
	{	if (empty()) return StrType ("");
		auto	fi	= begin();
		StrType conc= *fi; 
		while ( (++fi) != end() )  conc+=(separator+*fi);
		return conc;
	}


 private:
	StrType	_Name, _sep					/*=StrType(";")*/;
	typedef std::vector<StrType> base;
};


void fo();



template<class StrType>
class CFileChooser
{public:
	enum class FileChosserType {open,save,multiopen,dir,multidir,newdir} ; // more types?
	//CFileChooser<std::string> opTxtFile (CFileChooser<std::string>::open, "Chosse your txt file:", "../dir/default.txt",
	//											{    "Plain txt",{"*.txt","*.ascii"},   "All files",{*.*}     }); 
	
	CFileChooser(    FileChosserType	Type, 
					const StrType		&Title, 
					const StrType		InitialPath=StrType(""),
					const std::vector<CFileFiltre<StrType>> &filtres=filtres_t()
				)
				:_type(Type), _asked(false), _InitialPath(InitialPath), _filtres(filtres){}
	~CFileChooser(){}

	CFileChooser&			addFiltre(CFileFiltre<StrType> &ff){_filtres.push_back(ff);return *this;}
	StrType					AskOne(){return Ask()[0];}
	std::vector<StrType>	Ask()
	{	return CExtGUI::GUI()->FileChosserAsk(this);	}	
		/*fo(); return std::vector<StrType>(1);*/// possible more than one for multiopen type
	typedef std::vector<CFileFiltre<StrType>>	filtres_t;

private:
	FileChosserType						_type;
	std::vector<CFileFiltre<StrType>>	_filtres;
	bool								_asked/* =false*/;
	StrType								_InitialPath ;
};





class CFileChosserAsk
{ public: 
	CFileChosserAsk(){}
	std::vector<StrType> operator 
};







