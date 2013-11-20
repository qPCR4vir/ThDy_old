
#include <stdio.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <iomanip>
#include <memory>
#include "strTo.h"
#include <comdef.h>		//_com_error :  *Native C++ compiler COM support - main definitions header*/

//#include <ComAdmin.h>	//http://msdn.microsoft.com/en-us/library/windows/desktop/ee309561(v=vs.85).aspx
		//Initiates a session to do programmatic COM+ administration, access collections in the catalog, install COM+ applications and components, 
		//	start and stop services, and connect to remote servers. ICOMAdminCatalog provides access to the COM+ catalog data store.
//#include <combaseapi.h>   //  Contents:   Base Component Object Model defintions.
//#include <comcat.h>// OLE Componet Categories Interfaces.
//#include <comdefsp.h>  //               Native C++ compiler COM support - smart pointer support header
//#include <comip.h>  //Native C++ compiler COM support - COM interface pointers header
//#include <commapi.h> //  ApiSet Contract for api-ms-win-core-comm-l1   

//using namespace std;


		
inline IErrorInfo *GetCOM_ErrorInfo()
{
	IErrorInfo* perrinfo= nullptr;
	GetErrorInfo(0,&perrinfo);  //http://msdn.microsoft.com/en-us/library/ms221268(v=vs.85).aspx
	return perrinfo;
}
inline void Throw_COM_error(HRESULT hr)
{
	throw(	_com_error( hr, GetCOM_ErrorInfo() )  ); 
}

class COM_exeption :  public _com_error, public std::runtime_error 
{
public:
	COM_exeption(HRESULT hr, const std::string& what_arg ):_com_error(hr,GetCOM_ErrorInfo()), std::runtime_error (what_arg)
	{}

	virtual const char* what() const override
	{
		return (std::string(  std::runtime_error::what()) 
								+ "\nCOM throws the error: "	+ StrTo_string(ErrorMessage())
								+ "\nDescription: "				+ StrTo_string(Description())
								+ "\nHelp file: "				+ StrTo_string(HelpFile())
								+ "\nSource: "					+ StrTo_string(Source())
								).c_str(); 
	}
	virtual const wchar_t* w_what() const 
	{
		return (std::wstring(  StrTo_wstring(std::runtime_error::what()) )
								+ L"\nCOM throws the error: "	+ StrTo_wstring(ErrorMessage())
								+ L"\nDescription: "			+ StrTo_wstring(Description())
								+ L"\nHelp file: "				+ StrTo_wstring(HelpFile())
								+ L"\nSource: "					+ StrTo_wstring(Source())
								).c_str(); 
	}
};

class IBUnknown 
{
public:
	std::shared_ptr<IUnknown> pIUnk;

	static IUnknown *GetActiveObject_IUnk(REFCLSID rclsid)
	{	IUnknown *ppunk;
		HRESULT hr=	 ::GetActiveObject( rclsid, nullptr, &ppunk	  );
		if (FAILED(hr)) 	
			throw(	COM_exeption(hr,"Error getting active object of...\n")  ); //throw
		return ppunk;
	}

	//pIUnk->QueryInterface(



};


class IBDispatch : public IBUnknown
{
public:
	//wstring Name;
	//HRESULT _hr;
	std::shared_ptr<IDispatch> pIDisp;

	IBDispatch(				) 	{}

	IBDispatch( CLSID ClsID	) : pIDisp(CoCreateInstance(ClsID) )	{}
		template <typename Str>
	IBDispatch( Str progID	) : pIDisp(CoCreateInstance(progID) )	{}	

	IDispatch *CoCreateInstance(CLSID ClsID)
	{	IDispatch *pIDispatch ;
		HRESULT hr = ::CoCreateInstance(	ClsID,      nullptr,	CLSCTX_LOCAL_SERVER,  IID_PPV_ARGS(&pIDispatch));
		if (FAILED(hr)) 					
			throw(COM_exeption(hr,"Error creating COM instance of ... \n")); 
		return pIDispatch;
	}
		template <typename Str>
	IDispatch *CoCreateInstance(Str progID) 	{	return CoCreateInstance( CLSIDFromProgID(progID));	}

		template <typename Str>
    CLSID CLSIDFromProgID(const Str& progID) const
	{
		CLSID ClsID	;
		HRESULT hr = ::CLSIDFromProgID( StrTo_wstring(progID).c_str(), &clsid);
		if (FAILED(hr)) 					
			throw(COM_exeption(hr,std::string("Error deducing the COM's CldID of")+StrTo_string(progID)+ ".\n")); 
		return ClsID;

	}
		static IUnknown *GetActiveObject_IUnk(REFCLSID rclsid)
	{	IUnknown *ppunk;
		HRESULT hr=	 ::GetActiveObject( rclsid, nullptr, &ppunk	  );
		if (FAILED(hr)) 	
			throw(	COM_exeption(hr,"Error getting active object of...\n")  ); //throw
		return ppunk;
	}
    //HRESULT GetActiveObject(const CLSID& rclsid) throw()
    //{
    //    _Release();

    //    IUnknown* pIUnknown;

    //    HRESULT hr = ::GetActiveObject(rclsid, NULL, &pIUnknown);

    //    if (SUCCEEDED(hr)) {
    //        hr = pIUnknown->QueryInterface(GetIID(), reinterpret_cast<void**>(&m_pInterface));

    //        pIUnknown->Release();
    //    }

    //    if (FAILED(hr)) {
    //        // just in case refcount = 0 and dtor gets called
    //        m_pInterface = NULL;
    //    }

    //    return hr;
    //}
		template <typename Str>
	DISPID GetIDOfName( Str fetName)
	{	
		DISPID rgDispId;
		HRESULT hr = pIDisp->GetIDsOfNames( IID_NULL,(LPOLESTR *)(StrTo_wstring(fetName.c_str())), 1, LOCALE_USER_DEFAULT, &rgDispId);

		if (FAILED(hr)) 					
			throw(COM_exeption(hr,std::string("Error deducing the COM's dispach-ID of")+StrTo_string(fetName)+ ".\n")); 
		return rgDispId;
	}

	//HRESULT GetActiveObject(    const CLSID& rclsid  ) throw( ); _ClsID       LPCOLESTR progID = L"Excel.Application";

   
	virtual const CLSID& ClsID()const {return CLSID_NULL;}
	//virtual CLSID ClsID()const{return CLSID_NULL;}
};

class CExcelApp : public IBDispatch
{public:
	CExcelApp (bool NewInst=false) 
	{ 
		if(NewInst)
		{	
			pIDisp.reset(  CoCreateInstance(ClsID()) );
		}
		else
		{
			pIUnk.reset(  GetActiveObject_IUnk( ClsID()  )  );
			//pIDisp.reset( pIUnk->QueryInterface( ( ClsID()  ))  );

		}

	}
	virtual const CLSID& ClsID()const override{static const CLSID ClsID=CLSIDFromProgID(std::wstring(L"Excel.Application"));return ClsID;}
	//virtual const CLSID& ClsID()const {return CLSID_NULL;}
	//CLSID ClsID_ExcelApp(){static CLSID ExApp_ClsID=CLSIDFromProgID(L"Excel.Application");return ExApp_ClsID;}
	//static CLSID _ClsID; 
};
//CLSID CExcelApp::_ClsID=CLSID_NULL; 



	//IBDispatch(wstring progID) : pIDisp(CoCreateInstance(L"Excel.Application") )
	//{}
 //   LPCOLESTR progID = L"Excel.Application";
    //HRESULT CComPtrBase::CoCreateInstance(   LPCOLESTR szProgID,   LPUNKNOWN pUnkOuter = NULL,   DWORD dwClsContext = CLSCTX_ALL ) throw( ); 	
	//HRESULT CComPtrBase::CoCreateInstance(    REFCLSID rclsid,     LPUNKNOWN pUnkOuter = NULL,   DWORD dwClsContext = CLSCTX_ALL ) throw( );
	//HRESULT GetErrorInfo(  _In_   ULONG dwReserved,  _Out_  IErrorInfo **pperrinfo);http://msdn.microsoft.com/en-us/library/ms221032(v=vs.85).aspx
