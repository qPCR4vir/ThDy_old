

#include "ThDy_Form.h"

#pragma region Import the type libraries

#import "C:\Program Files (x86)\Common Files\microsoft shared\OFFICE12\MSO.DLL" \
    rename("RGB", "MSORGB") \
    rename("DocumentProperties", "MSODocumentProperties")
using namespace Office;

#import "C:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\VBE6EXT.OLB"
using namespace VBIDE;

#import "C:\Program Files (x86)\Microsoft Office\Office12\EXCEL.EXE" \
    rename("DialogBox", "ExcelDialogBox") \
    rename("RGB", "ExcelRGB") \
    rename("CopyFile", "ExcelCopyFile") \
    rename("ReplaceText", "ExcelReplaceText") \
    no_auto_exclude

#pragma endregion

using namespace std;
int main()
{ f();
try{
	string t;
    cout<< "CoInitialize\n";
    cin>> t; 

	CoInitialize(nullptr);
	Excel::_ApplicationPtr App ;
	HRESULT hr  = App.GetActiveObject(__uuidof(Excel::Application));
    if (FAILED(hr))
    {	hr = App.CreateInstance(__uuidof(Excel::Application), nullptr,CLSCTX_ALL);
		if (FAILED(hr))
		{
				wprintf(L"ActivateMicrosoftApp and CreateInstance failed w/err 0x%08lx ", hr);
				return 1;
		}
    }

    cout<<"\nExcel.Application is started\n";

		cout<< " Make Excel visible. ";
		cin>> t; 
        App->Visible[0] = VARIANT_TRUE;			//#define VARIANT_TRUE ((VARIANT_BOOL)-1)
													//#define VARIANT_FALSE ((VARIANT_BOOL)0)

	cout<< " get Workbooks.";
		cin>> t; 
 //       // Create a new Workbook. (i.e. Application.Workbooks.Add)


        Excel::WorkbooksPtr spXlBooks = App->Workbooks;
		bool ready(false);
	//	//if (! App->Ready )
	//	//{	cout<< "Are you editing? Please, give me a chanse!";
	//	//	cin>>t;
	//	//	while (! App->Ready);
	//	//}
		ready=(true);
	cout<< " get Ranges.";
		cin>> t; 
		Excel::_WorkbookPtr spXlBook;
		cin>> t; 
		/*spXlBook->*/
		Excel::RangePtr G_Table= App->Names->Item(L"G_tbl")->RefersToRange ;
		cin>> t; 
		G_Table->Copy();
		cin>> t; 
		Excel::RangePtr temp1  = App->Names->Item(L"temp_1")->RefersToRange ;
		temp1->Copy();
		cin>> t; 
		Excel::RangePtr temp2  = App->Names->Item(L"temp_2")->RefersToRange ;
		temp2->Copy();
		cin>> t; 
		Excel::RangePtr temp3  = App->Names->Item(L"temp_2")->RefersToRange ;
		temp3->Copy();
		cin>> t; 

		G_Table->Copy(temp2.GetInterfacePtr() );
		cin>> t; 
		temp3->Value2 = 13;
		cin>> t; 

		temp1->GetItem(1,2) =15 ;   //->Offset[1][2] = 1;
		cin>> t; 
		//temp1[0]=2;
		//G_Table->Cells[0];
		cin>> t; 


		//cin>> t; 


		G_Table->Copy();
		cin>> t; 

		//Excel::RangePtr G_Table= App->get_Range(L"G_tbl") ;
		//while (ready)
		//{	try
		//	{
		//		spXlBook = spXlBooks->Add();
		//		break;
		//	}
		//	catch(_com_error &err)
		//	{
		//		wprintf(L"Excel throws the error: %s\n", err.ErrorMessage());
		//		wprintf(L"Description: %s\n", (LPCWSTR) err.Description());
		//		wprintf(L"Description: "+ err.Description()+L"Help file"+ err.HelpFile() + L"Source"+ err.Source() );
		//		cout<< "\nAre you editing? Please, give me a chanse!";
		//		cin>> t; 
		//	}
		//}

		int i,n=spXlBooks->Count; 
  //      _putws(L"A new workbook is created");
		_putws(L"(Names of Worbooks)" );
		cout<< "n ="<<",   "<<n;
		cin>> t; 
		for(i=1; i<=n ; i++)
			_putws(spXlBooks->Item[i]->Name + L"(Name of Worbook)" );
			 
	cout<< " Close Workbook.";
		cin>> t; 
        //spXlBook->Close();

        //// Quit the Excel application. (i.e. Application.Quit)
        //_putws(L"Quit the Excel application");
        //App->Quit();

        // Release the COM objects.
        // Releasing the references is not necessary for the smart pointers
        // ...
        // spXlApp.Release();
        // ...

    }
    catch (_com_error &err)
    {
        wprintf(L"Excel throws the error: %s\n", err.ErrorMessage());
        wprintf(L"Description: "+ err.Description()+L"Help file"+ err.HelpFile() + L"Source"+ err.Source() );
		string t;
		cin>> t; 

    }
		

    CoUninitialize();

}
