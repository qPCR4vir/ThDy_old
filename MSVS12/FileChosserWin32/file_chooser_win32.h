#include "stdafx.h"
#include <vector>
template<class StrType>
class CFileFiltre
{public:
     CFileFiltre( const StrType &filtreName, const StrType &separator=StrType(";")	): _Name(filtreName),_sep(separator)  {}
	 ~CFileFiltre(){};

	CFileFiltre& operator<<(const StrType& filtre) { _filtres.add(filtre); return *this;}

private:
	StrType					_Name, _sep /*=StrType(";")*/;
	std::vector<StrType>	_filtres;

};

void fo();

template<class StrType>
class CFileChooser
{
public:
	enum class FileChosserType: bool {open,save} ;
	
	CFileChooser(FileChosserType Type, const StrType &Title):_typ(Type), _asked(false){}

	CFileChooser& addFiltre(CFileFiltre<StrType> &ff){_filtres.push_back(ff);return *this;}

			//, const StrType &separator=StrType(";")
			//void Separator(const StrType separator){_sep=separator;}
	StrType ConcatenedFiltres(const StrType &separator=_sep);

	~CFileChooser(){}
	void Ask(){fo();}

private:
	FileChosserType						_typ;
	std::vector<CFileFiltre<StrType>>	_filtres;
	bool								_asked/* =false*/;
};

template<class StrType>
StrType CFileChooser<StrType>::ConcatenedFiltres(const StrType &separator)//StrType(";")
{	if (_filtres.empty()) return StrType("");
		_sep=separator;
		auto	fi	= _filtre.begin();
		StrType conc= *fi; 
		while ( (++fi) != _filtre.end() )  conc+=(-sep+*fi);
		return conc;
}
