#pragma warning( disable : 4996 )
#include <stdio.h>
#include <windows.h>
#include <string>

template <typename cT>
inline cT* copyAs(const cT* str, size_t len=0)      /*<cT_to,cT_from>*/ //    (A) 
{   if (! str ) 
        len =1;
    else  
	    if(len) ++len;
        else    while (str[len++]);

	cT_to *res=new cT_to[len--];
    res[len]= cT_to(0);
    while (len--) 
		res[len]=str[len];	 
	return res;			// espero que el compilador no permita convertir de const en no const. O sea, cT_to debe incluir const
}
    //{    cT  new cT_to(0);returncT_to(0);


template <typename cT_to, typename cT_from>                             //      (B)
inline cT_to copyAs(const cT_from str, size_t len=0)
{   static_assert(false);
	return /*cT_to*/(str);			// espero que el compilador no permita convertir de const en no const. O sea, cT_to debe incluir const
}

template <>                                                                                 //     Full especialize B 
inline LPCOLESTR copyAs(const OLECHAR* str, size_t len)      /*<cT_to,cT_from>*/
{return copyAs<OLECHAR>(str, len) ;     /*<cT_to,cT_from>*/                                 //   ...and call A
}
		template <>                                                                                  //     Full especialize B                   
inline char* copyAs(const wchar_t* wstr, size_t len)  //<char*,const wchar_t*>
{	
    std::mbstate_t state = std::mbstate_t();
	if (!len)
		len =std::wcsrtombs(NULL, &wstr, 0, &state);		/* +1  */
	std::auto_ptr<char> mbstr(new char[len+1]);				//    std::vector<char> mbstr(len);
    std::wcsrtombs(mbstr.get(), &wstr, len+1, &state);	
	return mbstr.release();	
}

		template <>                                                                           //     Full especialize B 
inline wchar_t* copyAs/*<wchar_t*,const char*>*/(const char* mbstr, size_t len)
{	
    std::mbstate_t state = std::mbstate_t();
	if (!len)
		len =std::mbsrtowcs(NULL, &mbstr, 0, &state);/* +1  */
	std::auto_ptr<wchar_t> wstr(new wchar_t[len+1]);      // std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(wstr.get(), &mbstr, len+1, &state);
	return wstr.release();	
}
		template <typename Str>
inline std::wstring StrTo_wstring(Str str)
{
	return str;
}

		template <>
inline std::wstring StrTo_wstring(std::string str)
{
    std::mbstate_t state = std::mbstate_t();
	const char *mbstr= str.c_str();
    int len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
	std::auto_ptr<wchar_t> wstr(new wchar_t[len]);      // std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(&wstr.get()[0], &mbstr, len, &state);
	std::wstring ws(wstr.get());
	return ws;
}
		template <typename Str>
inline std::string StrTo_string(Str str)
{
	return str;
}

		template <>
inline std::string  StrTo_string(std::wstring str)
{	
    std::mbstate_t state = std::mbstate_t();
	const wchar_t *wstr= str.c_str();
    int len = 1 + std::wcsrtombs(NULL, &wstr, 0, &state);
	std::auto_ptr<char> mbstr(new char[len]);      //    std::vector<char> mbstr(len);
    std::wcsrtombs(&mbstr.get()[0], &wstr, len, &state);	
	std::string mbs(mbstr.get());
	return mbs;	
}

void f()
{
	LPCOLESTR t=L"";                    //typedef /* [string] */  __RPC_string const OLECHAR *LPCOLESTR;
                                        //typedef WCHAR OLECHAR;
                                        //#define OLESTR(str) L##str
                                        //typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

	LPCOLESTR s=copyAs<LPCOLESTR>(t);
	LPCOLESTR s=copyAs<OLECHAR>(t);
	unsigned char *s=copyAs<unsigned char*>(t);      //  ???
	double *s=copyAs<double*>(t);      //  ???


	LPCOLESTR a=copyAs<OLECHAR*>(L"kk");
	LPCOLESTR a=copyAs<wchar_t>(L"kk");
	LPCOLESTR b=copyAs<OLECHAR*>("kk");   //  ???
	LPCOLESTR b=copyAs<wchar_t*>("kk");   //  ???
	char *c=copyAs<char*>(L"kk");
	char *d=copyAs<char>("kk");
	char *e=copyAs<char*>(t);
	char *f=copyAs<char*>(t);
	//int   i=charTo<int>(t);
}





	//auto_ptr<wchar_t> res(new wchar_t[str.length()+1]);
	//mbstowcs(res.get(),str.c_str(),str.length()); 
	//std::wstring ws(res.get());
	//return ws;

//void print_as_wide(const char* mbstr)	//http://en.cppreference.com/w/cpp/string/multibyte/mbsrtowcs
//{
//    std::mbstate_t state = std::mbstate_t();
//    int len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
//    std::vector<wchar_t> wstr(len);
//    std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
//    std::wcout << "Wide string: " << &wstr[0] << '\n'
//               << "The length, including '\\0': " << wstr.size() << '\n';
//}
//void print_wide(const wchar_t* wstr)    // http://en.cppreference.com/w/cpp/string/multibyte/wcsrtombs
//{   
//    std::mbstate_t state = std::mbstate_t();
//    int len = 1 + std::wcsrtombs(NULL, &wstr, 0, &state);
//    std::vector<char> mbstr(len);
//    std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
//    std::cout << "multibyte string: " << &mbstr[0] << '\n'
//              << "Length, including '\\0': " << mbstr.size() << '\n';
//}


//char *CreateCharFromManString(String ^Text)	
//{
//	size_t					convertedChars	= 0;
//	size_t					sizeInBytes		= ((Text.length() + 1) * 2);
//	errno_t					err				= 0;
//	char					*CharText		= new char [sizeInBytes];
//	//pin_ptr<const wchar_t>  wchText			= PtrToStringChars(Text);
//	
//	err = wcstombs_s ( &convertedChars, 
//						CharText, 
//						sizeInBytes,
//						wchText, 
//						sizeInBytes);
//	return CharText	;
//}

//template <typename cT_to, typename cT_from>
//inline cT_to* copyAs/*<cT_to*,cT_from*>*/(const cT_from* str, size_t len=0)      /*<cT_to,cT_from>*/
//{   if (! str || !*str ) 
//        return new cT_to(0);
//	if(len) ++len;
//    else    while (str[len++]);
//	cT_to *res=new cT_to[len--];
//    res[len]= cT_to(0);
//    while (len--) 
//		res[len]=str[len];	 
//	return res;			// espero que el compilador no permita convertir de const en no const. O sea, cT_to debe incluir const
//}
//template <typename cT_from>
//inline LPCOLESTR copyAs<LPCOLESTR,cT_from*>(const cT_from* str, size_t len=0)      /*<cT_to,cT_from>*/
//{return charTo<OLECHAR>(str, len) ;     /*<cT_to,cT_from>*/
//}
