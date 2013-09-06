//#include "ThDy_DNAHybrid.VC-WF/StdAfx.h"
#include "StdAfx.h"
#include "ThDy_DNAHybrid.VC-WF/TagBinding.h"
//using namespace ParamF;
namespace TagBindingNS 
{
ITagBinding^	isTagBinding	(Control^ c){ return  getTagBinding	(c)					;}
ITagBinding^	getTagBinding	(Control^ c){ return  dynamic_cast<ITagBinding^>(c->Tag);}

void			UpDateP		(Control^  control){ if(isTagBinding( control ))  getTagBinding( control )->UpDateP   ()  ;}
void			UpDateForm	(Control^  control){ if(isTagBinding( control ))  getTagBinding( control )->UpDateForm()  ;}

System::Void	Validated_TB(System::Object^  senderControl, System::EventArgs^  e) {  UpDateP((Control^)senderControl) ;	}	

TagBinding_C_str^	TagBind		(Control^	c	, C_str &s							){ return gcnew TagBinding_C_str(s,c)	;}
TagBinding_strTrim^	TagBind_Trim(Control^	c	, C_str &s							){ return gcnew TagBinding_strTrim(s,c)	;}

TagBinding_bool^	TagBind(CheckBox^	  c , bool  &p			){ return gcnew TagBinding_bool(p,c)	;}


//using namespace System;   // http://msdn.microsoft.com/en-us/library/system.runtime.interopservices.marshal.stringtohglobalansi.aspx
//using namespace System::Runtime::InteropServices;
char *CreateCharFromManString(String ^text)   //   ---> StringToHGlobalAnsi : http://msdn.microsoft.com/en-us/library/system.runtime.interopservices.marshal.stringtohglobalansi.aspx
{	size_t					convertedChars	= 0;
	size_t					sizeInBytes		= ((text->Length + 1) * 2);
	errno_t					err				= 0;
	char					*CharText		= new char [sizeInBytes];
	pin_ptr<const wchar_t>  wchText			= PtrToStringChars(text);
	
	err = wcstombs_s ( &convertedChars, 
						CharText, 
						sizeInBytes,
						wchText, 
						sizeInBytes);
	return CharText	;

	//char *CharText=(char*) Marshal::StringToHGlobalAnsi(Text ).ToPointer();
	//char *r=strcpy(new char[strlen(CharText)+1],CharText);
	//Marshal::FreeHGlobal(IntPtr(CharText));
	//return r;
//


}
std::string CreateStdFromManString (String ^Text)   //   ---> StringToHGlobalAnsi : http://msdn.microsoft.com/en-us/library/system.runtime.interopservices.marshal.stringtohglobalansi.aspx
{	
	char		*CharText= CreateCharFromManString(Text) 	;
    std::string  res(CharText);
    delete [] CharText;
    return res;
}


}


//using namespace System;   // http://msdn.microsoft.com/en-us/library/system.runtime.interopservices.marshal.stringtohglobalansi.aspx
//using namespace System::Runtime::InteropServices;
//
//#include <iostream>                                                 // for printf 
//
//
//int main()
//{
//    // Create a managed string.
//    String^ managedString = "Hello unmanaged world (from the managed world).";
//
//    // Marshal the managed string to unmanaged memory. 
//    char* stringPointer = (char*) Marshal::StringToHGlobalAnsi(managedString ).ToPointer();
//
//    printf("stringPointer = %s\n", stringPointer);
//
//    // Always free the unmanaged string.
//    Marshal::FreeHGlobal(IntPtr(stringPointer));
//
//    return 0;
//}


//  //    http://msdn.microsoft.com/en-us/library/s7wzt4be(v=vs.110).aspx    
//errno_t wcstombs_s(					// Zero if successful, an error code on failure.
//   size_t			*pReturnValue,	// [out] The number of characters converted.
//   char				*mbstr,			// [out] The address of a buffer for the resulting converted multibyte character string.
//   size_t			sizeInBytes,	// The size in bytes of the mbstr buffer.
//   const wchar_t	*wcstr,			// Points to the wide character string to be converted.
//   size_t			count			// The maximum number of wide characters to be stored in the mbstr buffer, 
//									// not including the terminating null character, or _TRUNCATE.
//);
//errno_t _wcstombs_s_l(
//   size_t			*pReturnValue,
//   char				*mbstr,
//   size_t			sizeInBytes,
//   const wchar_t	*wcstr,
//   size_t			count,
//   _locale_t		locale			// The locale to use.
//);
//template <size_t size>
//errno_t wcstombs_s(
//   size_t			*pReturnValue,
//   char				(&mbstr)[size],
//   const wchar_t	*wcstr,
//   size_t			count 
//); // C++ only
//template <size_t size>
//errno_t _wcstombs_s_l(
//   size_t			*pReturnValue,
//   char				(&mbstr)[size],
//   const wchar_t	*wcstr,
//   size_t			count,
//   _locale_t		locale
//); // C++ only
//        