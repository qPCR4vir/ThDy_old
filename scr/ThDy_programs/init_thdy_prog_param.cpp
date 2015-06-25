//#include "StdAfx.h"
#pragma unmanaged
#include "thdy_programs\init_thdy_prog_param.h"
#include "ThDy_programs\prog_comm_functions.h"
#include "ThDySec/sec.h"
#include <assert.h>


//ThDyCommProgParam::~ThDyCommProgParam(void)        {/*delete []_ProgList;*/}

CProgParam_microArray::~CProgParam_microArray()		{ /*delete _tlTm;*/}

CMultSec* ThDyCommProgParam::CreateRoot	() 
{
	return new CMultSec(_pSaltCorrNNp,"All seq");
}

CMultSec* ThDyCommProgParam::AddSeqGroup		(CMultSec   *parentGr, const std::string&     Name)
{
	if(parentGr)
	    if (  parentGr->_NNPar ) 
            return parentGr->AddMultiSec(new CMultSec(parentGr->_NNPar, Name));
		else
            return parentGr->AddMultiSec(new CMultSec(_pSaltCorrNNp, Name));

    return new CMultSec(_pSaltCorrNNp, Name);
}

CMultSec* ThDyCommProgParam::AddSeqFromFile(CMultSec *parentGr, const std::string& FileName, bool recursive/*=false*/, bool onlyStructure/*=false*/)
{
	auto pNN=_pSaltCorrNNp;
	if(parentGr && parentGr->_NNPar)
        pNN=parentGr->_NNPar ;
	CMultSec *sG=new CMultSec ( FileName , 
                                pNN, 
                                recursive,
							   _MaxTgId,
							   _SecLim ,
                               _SecLenLim, 
                               !onlyStructure);
	if(parentGr)
    {
		parentGr->AddMultiSec(sG);
        std::string parent_path = filesystem::path(sG->_Path).remove_filename().string();
        if (parentGr->_Local._NMSec == 1)
            parentGr->_Path= parent_path ;
        else if (parentGr->_Path != parent_path)
                parentGr->_Path.clear();
    }
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

