//#include "StdAfx.h"
#pragma unmanaged
#include "thdy_programs\init_thdy_prog_param.h"
#include "ThDy_programs\prog_comm_functions.h"

ThDyCommProgParam::~ThDyCommProgParam(void)        {/*delete []_ProgList;*/}

CProgParam_microArray::~CProgParam_microArray()		{ /*delete _tlTm;*/}

CMultSec* ThDyCommProgParam::CreateRoot	() 
{
	return new CMultSec("All seq");
}
CMultSec* ThDyCommProgParam::AddSeqGroup		(CMultSec   *parentGr, const std::string&     Name)
{
	if (! _pSaltCorrNNp)
		_pSaltCorrNNp=Create_NNpar(*this);			//Comprobar que no ha cambiado????????
	CMultSec *sG=new CMultSec(Name);                // Revisar esto    !!!!!!!!!!!!!!!!
	sG->_NNPar= _pSaltCorrNNp ;
	if(parentGr)
		parentGr->AddMultiSec(sG);
	return sG;
}

CMultSec* ThDyCommProgParam::AddSeqFromFile(CMultSec   *parentGr, const std::string& FileName)
{
	if (! _pSaltCorrNNp)
		_pSaltCorrNNp=Create_NNpar(*this);			//Comprobar que no ha cambiado????????

	CMultSec *sG=new CMultSec (FileName.c_str(), _pSaltCorrNNp);
	if(parentGr)
		parentGr->AddMultiSec(sG);
	return sG;
}
void CProgParam_microArray::RenameSondesMS(const std::string& name)
{
    _probesMS->_name=name;
}


CProgParam_uArrExp::CProgParam_uArrExp (const string& titel, ThDyCommProgParam &commThDyParam):	
	                _exclSd(false),		
					_IxI(true),		_IxI_d(true),			
					_Normalize(true), 
					_Isat(Energy(0.87f)),_Isen(Energy(0.01f)),	_Gsat(Energy(-2.0f)),	_Gsen(Energy(2.0f)),
					CProgParam_microArray (titel,commThDyParam) 
					{  _probesMS->_name="Probes of Exp uArr";
					} 
CProgParam_MultiplexPCR::CProgParam_MultiplexPCR(const string& titel, ThDyCommProgParam &commThDyParam) 
	: CProgParam_microArray(titel,commThDyParam), _rtbl_self(nullptr)
	{	_InputSondeFile.SetTitel("Imput file for primers"); 
		_InputSondeFile.SetEtiq("iSonde_PCR"); 
        _probesMS->_name="Primers of Multiplex PCR";
	}


//#include "ThDySec\sec.h"

