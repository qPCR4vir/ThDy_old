/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDySec\sec_basic.cpp
*
* @brief 
*/

#ifdef WINDOWS_FORM_GUI
#include "stdafx.h"
#pragma unmanaged
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <math.h>
#include <list>
#include <stack>

using namespace std ; 

#include "ThDySec/sec_mult.h"
#include "ThDySec/common.h" 
using namespace DegCod;

//#define SEQUENCES_MAX_SIZE 100000
char *DNAStrandName[]=	{""		, "(c)", ""		, "(r)"	, "(i)", "(c)"		} ;
// enum DNAStrand		{plus	, minus, direct	, rev	, compl, rev_compl	} ;


CSecBasInfo::~CSecBasInfo()
{
	if (_NonDegSet) 
	     if (!_NonDegSet->Prev() && !_NonDegSet->Next()) 
			 delete _NonDegSet;
	// en otro caso, donde borrar _NonDegSet ????. Lo borra la lista en la que esta insertado
}
	 
std::string& CSecBasInfo::Copy_Seq  	(std::string &SecHier,  long InicBase, long EndBase, DNAStrand strnd) const
{	
	if ( EndBase< 1 || Len() <EndBase ) EndBase= Len(); 
	long l=EndBase-InicBase+1 ;  
	if (l>=0) 	//assert(l>=0);
	{
		SecHier.clear();
		SecHier.reserve(l);
		switch (strnd)
		{	case DNAStrand::plus :
			case DNAStrand::direct:		for(long p=InicBase;  p<=EndBase;    p++) 	SecHier.push_back ( _c[p] );  				    break;
			case DNAStrand::compl:		for(long p=InicBase;  p<=EndBase;    p++) 	SecHier.push_back ( c_degbase[_c[p]]);      	break;
			case DNAStrand::rev:		for(long p=EndBase ;  p>=InicBase;   p--)	SecHier.push_back ( _c[p] );  				    break;
			case DNAStrand::minus:
			case DNAStrand::rev_compl:	for(long p=EndBase ;  p>=InicBase;   p--)	SecHier.push_back ( c_degbase[_c[p]]);      	break;
 		}
	}
	return  SecHier ;
}

/// Best just return sequence
//Base  *	CSecBasInfo::Copy_charSec(Base *charSecHier,long InicBase, long EndBase, DNAStrand strnd)//DNAStrand strnd=direct)
//{	if ( EndBase< 1 || Len() <EndBase ) EndBase= Len(); 
//	long l=EndBase-InicBase+1 ; charSecHier[l]=0 ;
//	if (l>=0) 
//	//assert(l>=0);
//	switch (strnd)
//	{	case DNAStrand::plus :
//		case DNAStrand::direct:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=_c[p];				break;
//		case DNAStrand::compl:		for(long i=0,   p=InicBase;  p<=EndBase;    i++, p++) 	charSecHier[i]=c_degbase[_c[p]];	break;
//		case DNAStrand::rev:		for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=_c[p];				break;
//		case DNAStrand::minus:
//		case DNAStrand::rev_compl:	for(long i=l-1, p=InicBase;  p<=EndBase;    i--, p++)	charSecHier[i]=c_degbase[_c[p]];	break;
//
//		default : return 0;
//	}
//	return charSecHier ;
//}
//Base  *	CSecBasInfo::GetCopy_charSec(DNAStrand strnd)
//{	return GetCopy_charSec(1, Len(), strnd);
//}
//Base  *	CSecBasInfo::GetCopy_charSec(long InicBase, long EndBase, DNAStrand strnd)       // recuerde los $...$, aqui se cuentan, 
//{	if ( InicBase< 1 )					InicBase=1;
//	if ( EndBase< 1 || Len() <EndBase )	EndBase=Len();
//	long l=EndBase-InicBase+1 ;
//	assert(l>=0);
//	Base *charSecHier=new Base[l+1];						 // asi como InicBase y EndBase inclusive!!
//	assert(charSecHier);	
//
//	return Copy_charSec(charSecHier, InicBase, EndBase, strnd)  ;
//}



