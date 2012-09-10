
#include "stdafx.h"
#include "file_chooser.h"
#include <Windows.h>
#include <Shobjidl.h>
HRESULT BasicFileOpen()
{
    // CoCreate the File Open Dialog object.
    IFileDialog *pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, 
                      NULL, 
                      CLSCTX_LOCAL_SERVER, /*CLSCTX_INPROC_SERVER, */
                      IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        // Create an event handling object, and hook it up to the dialog.
        // IFileDialogEvents *pfde = NULL;
        //hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            // Hook up the event handler.
            // DWORD dwCookie;
            //hr = pfd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                // Set the options on the dialog.
                DWORD dwFlags;

                // Before setting, always get the options first in order 
                // not to override existing options.
                hr = pfd->GetOptions(&dwFlags);
                if (SUCCEEDED(hr))
                {
                    // In this case, get shell items only for file system items.
                    hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                    if (SUCCEEDED(hr))
                    {
                        // Set the file types to display only. 
                        // Notice that this is a 1-based array.

					/*	typedef struct _COMDLG_FILTERSPEC {
						  LPCWSTR pszName;
						  LPCWSTR pszSpec;
						} COMDLG_FILTERSPEC;	*/				

						COMDLG_FILTERSPEC rgSpec[] =
						{ 
							{ L"JPG", L"*.jpg;*.jpeg" },
							{ L"BMP", L"*.bmp" },
							{ L"ALL", L"*.*" },
						};


                        hr = pfd->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
                        if (SUCCEEDED(hr))
                        {
                            // Set the selected file type index to Word Docs for this example.
                            hr = pfd->SetFileTypeIndex(UINT (1)/*INDEX_WORDDOC*/);
                            if (SUCCEEDED(hr))
                            {
                                // Set the default extension to be ".doc" file.
                                hr = pfd->SetDefaultExtension(L"doc;docx");
                                if (SUCCEEDED(hr))
                                {
                                    // Show the dialog
                                    hr = pfd->Show(NULL);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Obtain the result once the user clicks 
                                        // the 'Open' button.
                                        // The result is an IShellItem object.
                                        IShellItem *psiResult;
                                        hr = pfd->GetResult(&psiResult);
                                        if (SUCCEEDED(hr))
                                        {
                                            // We are just going to print out the 
                                            // name of the file for sample sake.
                                            PWSTR pszFilePath = NULL;
                                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, 
                                                               &pszFilePath);
                                            if (SUCCEEDED(hr))
                                            {
                                                //TaskDialog(NULL,
                                                //           NULL,
                                                //           L"CommonFileDialogApp",
                                                //           pszFilePath,
                                                //           NULL,
                                                //           TDCBF_OK_BUTTON,
                                                //           TD_INFORMATION_ICON,
                                                //           NULL);
                                                CoTaskMemFree(pszFilePath);
                                            }
                                            psiResult->Release();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // Unhook the event handler.
                //pfd->Unadvise(dwCookie);
            }
            //pfde->Release();
        }
        pfd->Release();
    }
    return hr;
}
void fo(){BasicFileOpen();}
//template<class StrType>
//void CFileChooser<class StrType>::Ask()
//{BasicFileOpen();
//}
