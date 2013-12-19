//#include "StdAfx.h"
#pragma unmanaged
#include "thdy_programs\init_thdy_prog_param.h"
#include "ThDy_programs\prog_comm_functions.h"
#include "ThDySec/sec.h"


//ThDyCommProgParam::~ThDyCommProgParam(void)        {/*delete []_ProgList;*/}

CProgParam_microArray::~CProgParam_microArray()		{ /*delete _tlTm;*/}

CMultSec* ThDyCommProgParam::CreateRoot	() 
{
	if (! _pSaltCorrNNp)
		_pSaltCorrNNp=Create_NNpar( );			//Comprobar que no ha cambiado????????  revisar     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return new CMultSec("All seq");
}
CMultSec* ThDyCommProgParam::AddSeqGroup		(CMultSec   *parentGr, const std::string&     Name)
{
	CMultSec *sG=new CMultSec(Name);                // Revisar esto    !!!!!!!!!!!!!!!!
	if(parentGr)
	{
        parentGr->AddMultiSec(sG);
	    if (  parentGr->_NNPar ) 
	    {
            sG->_NNPar= parentGr->_NNPar ;     //  revisar     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            return sG;
        }
    }
	if ( _pSaltCorrNNp)
            sG->_NNPar= _pSaltCorrNNp ;
    return sG;
}

CMultSec* ThDyCommProgParam::AddSeqFromFile(CMultSec   *parentGr, const std::string& FileName, bool all_in_dir)
{
    if (! _pSaltCorrNNp )     Actualice_NNp ();

	CMultSec *sG=new CMultSec (FileName.c_str(), _pSaltCorrNNp, all_in_dir,
								    _MaxTgId,
								    _SecLim ,
                                    _SecLenLim);
	if(parentGr)
		parentGr->AddMultiSec(sG);
	return sG;
}
CMultSec* ThDyCommProgParam::CopyStructFromDir	(CMultSec *parentGr, const std::string& FileName)
{
    if (! _pSaltCorrNNp )     Actualice_NNp ();

	CMultSec *sG=new CMultSec (FileName.c_str(), _pSaltCorrNNp, true,
								    _MaxTgId,
								    _SecLim ,
                                    _SecLenLim,false);
	if(parentGr)
		parentGr->AddMultiSec(sG);
	return sG;
}

void CProgParam_microArray::RenameSondesMS(const std::string& name)
{
    _probesMS->_name=name;
}


//CProgParam_uArrExp::CProgParam_uArrExp (const string& titel, ThDyCommProgParam &commThDyParam):	
//	                _exclSd(false),		
//					_IxI(true),		_IxI_d(true),			
//					_Normalize(true), 
//					_Isat(Energy(0.87f)),_Isen(Energy(0.01f)),	_Gsat(Energy(-2.0f)),	_Gsen(Energy(2.0f)),
//					CProgParam_microArray (titel,commThDyParam) 
//					{  _probesMS->_name="Probes of Exp uArr";
//					} 
CProgParam_MultiplexPCR::CProgParam_MultiplexPCR(const string& titel, ThDyCommProgParam &commThDyParam) 
	: CProgParam_microArray(titel,commThDyParam), _rtbl_self(nullptr)
	{	
        _InputSondeFile.SetTitel("Imput file for primers"); 
		_InputSondeFile.SetEtiq("iSonde_PCR", this); 
        _probesMS->_name="Primers of Multiplex PCR";

        _PrRecurDir.SetTitel("Recursively add all primers seq-files from all dir"); 
		_PrRecurDir.SetEtiq("PrimRecDir", this); 

        _PrDirStrOnly.SetTitel("Reproduce only the dir struct in primers"); 
		_PrDirStrOnly.SetEtiq("PrimDirStr", this); 

 
	}


//#include "ThDySec\sec.h"

