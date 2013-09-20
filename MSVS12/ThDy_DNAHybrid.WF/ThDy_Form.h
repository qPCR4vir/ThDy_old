#pragma once
#include "ThDy_DNAHybrid.VC-WF/th_dy_programs.h"  // aqui esta ya : // #include "..\ThDy_programs\init_prog_param.h"  ;    #include "..\ThDySec\common_basics.h" 
#include "ThDy_DNAHybrid.VC-WF/Results.h"
#include "ThDySec\matrix.h" 
#include "ThDySec\common_basics.h" 
#pragma managed
#include "SeqExpl.h" //"ThDy_DNAHybrid.VC-WF/Results.h"
#include "ThDy_DNAHybrid.VC-WF/TagBinding.h"

namespace ThDy_DNAHybridWF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace TagBindingNS ;
    //using namespace Microsoft::Office::Interop::Excel; 
    //using namespace Microsoft::Office::Tools::Excel;

	/// <summary>
	/// Summary for ThPr_Form
	/// </summary>
	
	public ref class ThPr_Form : public System::Windows::Forms::Form //  , public INotifyPropertyChanged
	{	
	private:		ThDyProject		&_Pr;
					TagBindGroup	^_CommThDyP, ^_uArrThDyP, ^_TmCalThDyP,  ^_mPCRThDyP ,  ^_SdDesThDyP		 ;
					SeqExpl			^_seqExpl;

	private: System::Windows::Forms::Button^  btPCRfiltrFile;
	private: System::Windows::Forms::TextBox^  txtBoxPCRfiltr;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groBox_ComUnic;
	private: System::Windows::Forms::RadioButton^  radBut_UnicSond;
	private: System::Windows::Forms::RadioButton^  radBut_CommSond;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  butSeqExplorer;
	private: System::Windows::Forms::OpenFileDialog^  opPCRfiltrFDlg;

			 void 			InitializeTagBindings();
	public:
		ThPr_Form() 
			try : _Pr(   *( new ThDyProject() )    )
		{			
			InitializeComponent();
			InitializeTagBindings();
           
			//
			//TODO: Add the constructor code here
			//
			this->comBoxSalMeth->SelectedIndex = TAMeth_Tm;    // 0    ???????
            try{ 
				    if (Environment::GetCommandLineArgs()->Length   > 1    )
                        _Pr.load( CreateCharFromManString(Environment::GetCommandLineArgs()[1]   ) );	
					else
						_Pr.load() ;						// cuando no existe Def Project: 1ra vez que se usa el prog??
		       }
		    catch ( std::exception& e )
		     {   MessageBox::Show ( gcnew String(e.what()) + "\n A new Default Project will be created. "  ) ;
                 try{                                       // char *defPr= clone_trim_str( _Pr._ProjetFileName) ;
				        UpdateThDyP();						// Esto para darle "prioridad" a los def parametros de la FORM 
		            }
		          catch ( ParamOutOfNumRange e)
		          { MessageBox::Show ( gcnew String(e.what())  ) ;
		          }
			    this->comBoxTAMeth->SelectedIndex  = SMStLucia;     // 0
		        _Pr.save_defPr() ; 					                // _Pr.save( _defPr ) ;  // _Pr.ProjetFile(_defPr);
		        this->textBoxPrFile->Update();		                //  crea el Def Project.
             }

			_seqExpl = gcnew SeqExpl(this->_Pr);

			this->comBoxTAMeth->SelectedIndex  = SMStLucia;         // 0
            UpdateThDyForm();
		}
		catch ( std::exception e)
		          { MessageBox::Show ( gcnew String(e.what())  ) ;
		          }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ThPr_Form()
		{
			if (components)
			{
				delete components;
			}
			ThDyProject *pr=&_Pr ;
			delete pr ;
		}

public: void UpdateThDyForm					()		//  ------------------------ UpdateThDyForm.  Todo el prog		---------------------
			{	UpdateCommThDyForm() ;
				_uArrThDyP->UpDateForm() ;
				_mPCRThDyP->UpDateForm () ;
				_SdDesThDyP->UpDateForm ();
				_TmCalThDyP->UpDateForm();		// UpDateForm	(txtBx_Sec);	UpDateForm	(txtBx_Sec2Align);

				this->Update(); 	
			}
public: void UpdateThDyP						()	{		UpdateCommThDyP() ;		}////				_uArrThDyP->UpDateP() ;			

private: void UpdateCommThDyForm				()		//  -------------------- UpdateCommThDyForm.  Parametros comunes		------
	{	_CommThDyP->UpDateForm();
		this->comBoxSalMeth->SelectedIndex	=			_Pr._cp._SaltCorr  ;			//this->comBoxSalMeth->Update(); 
		this->comBoxTAMeth->SelectedIndex	=			_Pr._cp._TAMeth  ;				//this->comBoxTAMeth->Update(); 
	}
private: void UpdateCommThDyP					()
	{		_Pr._cp._SaltCorr= SaltCorrection(this->comBoxSalMeth->SelectedIndex ) ;//	_CommThDyP->UpDateP();
			_Pr._cp._TAMeth	 = AlignMeth  (this->comBoxTAMeth->SelectedIndex ) ;
	}


private: int Run		(CEspProg& IPrgPar)											 //	  Run      CEspProg
	{		
		UpdateCommThDyP();
			
		int r= _Pr.Run(IPrgPar);	//			return IPrgPar->Run();  //   ????  return _Pr.Run(IPrgPar);
		return r;
	}	

private: System::Void commandSavePrFile			(System::Object^  sender, System::EventArgs^  e)					 // Save  Proj File
	{			 this->savePrFileDialog->FileName = this->textBoxPrFile->Text;
				 this->savePrFileDialog->ShowDialog();

				 this->textBoxPrFile->Text = this->savePrFileDialog->FileName;
				 this->textBoxPrFile->Update(); 
	}
private: System::Void commandLoadPrFile			(System::Object^  sender, System::EventArgs^  e)					 // Load  Proj File
	{			 this->loadPrFileDialog->FileName = this->textBoxPrFile->Text;
				 this->loadPrFileDialog->ShowDialog();

				 this->textBoxPrFile->Text = this->loadPrFileDialog->FileName;
				 this->textBoxPrFile->Update(); 
	}
private: System::Void SavePrFileDialog_FileOk	(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
	{			  try{                                       // char *defPr= clone_trim_str( _Pr._ProjetFileName) ;
				        UpdateThDyP();						// Esto para darle "prioridad" a los def parametros de la FORM 
		            }
		          catch ( ParamOutOfNumRange e)
		          { MessageBox::Show ( gcnew String(e.what())  ) ;
		          }
				_Pr.save( CreateCharFromManString(this->savePrFileDialog->FileName    ) );
	}
private: System::Void LoadPrFileDialog_FileOk	(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
	{			
		  try{      _Pr.load( CreateCharFromManString(this->loadPrFileDialog->FileName    ) );
		     }
		     catch ( ParamOutOfNumRange e)
		     { MessageBox::Show ( gcnew String(e.what())  ) ;
		     }
		 UpdateThDyForm();
	}
private: System::Void commandSetDefPr			(System::Object^  sender, System::EventArgs^  e) 	// crea el Def Project.	 // Set Deff  Proj File
	{			 try{                                       // char *defPr= clone_trim_str( _Pr._ProjetFileName) ;
				        UpdateThDyP();						// Esto para darle "prioridad" a los def parametros de la FORM 
		            }
		          catch ( ParamOutOfNumRange e)
		          { MessageBox::Show ( gcnew String(e.what())  ) ;
		          }
				char *pr = clone_c_str(_Pr._ProjetFileName.Get()) ;
													// _Pr.ProjetFile(_defPr);
				_Pr.save_defPr() ; 					// _Pr.save( _defPr ) ; 

				_Pr.SetProjetFile(pr);
				this->textBoxPrFile->Update();   // crea el Def Project.

	}
private: System::Void commandRestDefPr			(System::Object^  sender, System::EventArgs^  e)		// Restore (USE) Deff  Proj File
	{			_Pr.ProjetFile(_Pr._defPr.Get());
            try{
			        _Pr.load() ;						// cuando no existe Def Project: 1ra vez que se usa el prog??
		        }
		 catch ( std::exception& e )
		 { MessageBox::Show ( gcnew String(e.what())  ) ;	}			
		 UpdateThDyForm();
	}

private: System::Void commandLoadNNParamFile	(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->openFDial_NNParam->ShowDialog();
			 this->textBoxNNParamFile->Text = this->openFDial_NNParam->FileName;
			 this->textBoxNNParamFile->Update();
		 }
private: System::Void commandOpenTargetsSecFile4uA(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->openFileDialog_targets->ShowDialog();
			 this->textBoxTgScF4uA->Text = this->openFileDialog_targets->FileName;
			 this->textBoxTgScF4uA->Update(); 
		 }
private: System::Void commandOpenNonTargetsSecFile(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->openFileDialog_non_targets->ShowDialog();
			 this->textBoxNonTargetsFile->Text = this->openFileDialog_non_targets->FileName;
			 this->textBoxNonTargetsFile->Update();
		 }
private: System::Void commandSaveResultFile		(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->SaveResultFileDialog->ShowDialog();
			 this->textBoxSaveResultFile->Text = this->SaveResultFileDialog->FileName;
			 this->textBoxSaveResultFile->Update(); 
		 }
private: System::Void butSeqExplorer_Click		(System::Object^  sender, System::EventArgs^  e) 
		 {		  try{                                       
						 UpdateThDyP(); 
                         _seqExpl->Show();		 
		            }
		          catch ( std::exception& e)
		          { MessageBox::Show ( gcnew String(e.what())  ) ;
		          }
		 }
private: System::Void command_uArray			(System::Object^  sender, System::EventArgs^  e)  // Run       mArray
	{	_uArrThDyP->UpDateP() ;	
                                                    //	MessageBox::Show(L"Primero probemos esto");//_Pr._uArr.Udate = UI_uA_Create ;
	   try{                                   
		        Run(_Pr._uArr);	
		   }
		   catch ( std::exception& e)
		   { MessageBox::Show ( gcnew String(e.what())  ) ;
		    return;
		   }	 
		ShowResTbl(_Pr._uArr._rtbl );
		_Pr._uArr._rtbl = nullptr;
//using namespace Microsoft::Office::Tools;//::Excel;
//using namespace Microsoft::Office::Interop::Excel; 
//using namespace Microsoft::Office::Interop; 
//  Excel::Application^ xlApp =gcnew Excel::ApplicationClass();
// 
//  Excel::Workbook^ mybook = xlApp->Workbooks->Open("C:\\Liste.xlsx",Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing,Type::Missing, Type::Missing);
// 
//  xlApp->Visible = false;
// 
//  Excel::Worksheet^ mysheet= safe_cast<Worksheet^>(xlApp->ActiveSheet);
//  
//  mysheet = (Excel::Worksheet^)mybook->Worksheets->Item[1]; 
//  Excel::Range^ cell= safe_cast<Range^>(safe_cast<Worksheet^>(mysheet)->Cells[2,3]);
//  cell->Value2->ToString() ;
//  cell->Value[0]->ToString() ;
//
//  Convert/*  ->ToString();
//  String^*/ tmp=(String^)((safe_cast<Range^>(safe_cast<Worksheet^>(mysheet)->Cells->Item[2,3]))->Value);    ;
//  String^ tmp=(String^) (safe_cast<Range^>(safe_cast<Worksheet^>(mysheet)->Cells[2,3] ))->Value2     ;
//  String^ tmp=(String^) (safe_cast<Range^>(safe_cast<Worksheet^>(mysheet)->Cells[2,3] ) )  ;
//  String^ tmp=(String^) (                  safe_cast<Worksheet^>(mysheet)->Cells[2,3] )    ;
//  String^ tmp=(String^) (										 mysheet ->Cells[2,3] )    ;
//  String^ tmp=(String^) 										 mysheet ->Cells[2,3]     ;
//  //Convert::ToString((Decimal)safe_cast<Range^>(safe_cast<Worksheet^>(mysheet)->Cells[2,3] )->Value2 ).
//
//Excel::NamedRange 
   //     using namespace Microsoft::Office::Interop; 
   //Excel::ApplicationClass^ eac=  gcnew Excel::ApplicationClass;
   //Excel::_Application^ ea ;   Excel::Application;
   ////Excel::Workbook^ ew1 = Excel::ApplicationClass->;
   //Excel::Workbook^ ew =  eac->Workbooks->Item["na"]  ;
   //eac->Visible=true;
   //Excel::Range^ r=    eac->Range("names") ;
	}
void ShowResTbl(	CTable<TmGPos> *rtbl)
{
	if (!rtbl) return;
		Results^ rsTm	=gcnew Results	(	/**_Pr._uArr._tlTm*/	);
		Results^ rsG	=gcnew Results	(	/**_Pr._uArr._tlG	*/	);
		Results^ rsPos	=gcnew Results	(	/**_Pr._uArr._tlPos	*/	);

		CTable<TmGPos> &tb = *rtbl ; // Alias para _Pr._uArr._rtbl 

		String^ wname = gcnew String( tb.TitTable().c_str() ) + L"  ( " + gcnew String ( _Pr._cp._OutputFile.Get() ) + L" )";

		rsTm->Text		= L" Tm: " + wname;
		rsG->Text		= L"  G: " + wname;
		rsPos->Text		= L"pos: " + wname;



		rsTm->dg->ColumnCount  = tb.totalCol();		
		rsG->dg->ColumnCount   = tb.totalCol();		
		rsPos->dg->ColumnCount = tb.totalCol();		// create columns

		for (int c=0 ; c < tb.totalCol() ; c++)											// fill columns headers
		{	String^ tCol= gcnew String( tb.TitColumn(c).c_str() ); 

			rsTm->dg->Columns[c]->HeaderText	= tCol ;	
			rsG->dg->Columns[c]->HeaderText		= tCol ;	
			rsPos->dg->Columns[c]->HeaderText	= tCol ;

			rsTm->dg->Columns[c]->Name	= 	tCol ;
			rsG->dg->Columns[c]->Name	= 	tCol ;		
			rsPos->dg->Columns[c]->Name =	tCol ;
			//Column->ToolTipText = L"Se puede anadir Tips"; Column->Width	= 23; 
		}

		rsTm->dg->RowCount	= tb.totalRow();		 
		rsG->dg->RowCount	= tb.totalRow();		 
		rsPos->dg->RowCount	= tb.totalRow();				// create rows

		for (int r=0 ; r < tb.totalRow() ; r++)
		{	String^ tRow=gcnew String( tb.TitRow(r).c_str() );	

			rsTm->dg->Rows[r]->HeaderCell->Value =   tRow;	 
			rsG->dg->Rows[r]->HeaderCell->Value =    tRow;	
			rsPos->dg->Rows[r]->HeaderCell->Value =  tRow;		// fill columns headers
			
			for (int c=0 ; c < tb.totalCol() ; c++)	
			{	rsTm->dg[c,r]->Value	= gcnew Decimal(	tb(r,c)._Tm );	
				rsG->dg[c,r]->Value		= gcnew Decimal(	tb(r,c)._G  );	
				rsPos->dg[c,r]->Value	= gcnew Decimal(	tb(r,c)._Pos);		// fill cells
			}
		}
		rsTm-> dg->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
		rsG->  dg->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
		rsPos->dg->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders);
		rsTm-> dg->AutoResizeColumns();
		rsG->  dg->AutoResizeColumns();
		rsPos->dg->AutoResizeColumns();
		rsTm-> dg->DefaultCellStyle->Format =L"N1";
		rsG->  dg->DefaultCellStyle->Format =L"N2";
		rsPos->dg->DefaultCellStyle->Format =L"N0";

			rsTm->Show(this);
			rsG->Show(this);
			rsPos->Show(this);

			delete rtbl  ; //_Pr._uArr._rtbl  =nullptr ;		// seguro ????? quien y donde hacerlo??????
			//delete _Pr._uArr._tlTm  ; _Pr._uArr._tlTm  =nullptr ;		// seguro ????? quien y donde hacerlo??????
			//delete _Pr._uArr._tlG   ; _Pr._uArr._tlG   =nullptr  ;		// seguro ????? quien y donde hacerlo??????
			//delete _Pr._uArr._tlPos ; _Pr._uArr._tlPos =nullptr ;		// seguro ????? quien y donde hacerlo??????

	}
private: System::Void commandopenSondeFile		(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->openSondesFileDialog->ShowDialog();
			 this->textBoxSondesFile->Text = this->openSondesFileDialog->FileName;
			 this->textBoxSondesFile->Update();
		 }
private: System::Void but_uArrExp_Click			(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->oFileExp->ShowDialog();
			 this->txtBoxExp->Text = this->oFileExp->FileName;
			 this->txtBoxExp->Update();
			 }

private: System::Void buttPCR_Click				(System::Object^  sender, System::EventArgs^  e) //	  Run      _IPrgPar_mPCR
			 {	_mPCRThDyP->UpDateP()	;				
		           try{                                   
		                    Run(_Pr._mPCR);	
		           }
		           catch ( std::exception& e)
		           { MessageBox::Show ( gcnew String(e.what())  ) ;
		            return;
		           }
		ShowResTbl(_Pr._mPCR._rtbl );
		_Pr._uArr._rtbl = nullptr;

		ShowResTbl(_Pr._mPCR._rtbl_self );
		_Pr._mPCR._rtbl_self = nullptr;

		    }
private: System::Void butSdSecFilePCR_Click		(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->openSondesFileDialog->ShowDialog();
			 this->textBoxSdSecFilePCR->Text = this->openSondesFileDialog->FileName;
			 this->textBoxSdSecFilePCR->Update();
		 }

private: System::Void commandDesign				(System::Object^  sender, System::EventArgs^  e) //    Run      Sonde design
		{	_Pr._SdDes._design	 = true ;		
		 
		           try{                                   
		                  Run(_Pr._SdDes);	
		           }
		           catch ( std::exception& e)
		           { MessageBox::Show ( gcnew String(e.what())  ) ;
		            return;
		           }	 	        		 

		 	}                       //Update_SdDesThDyP()	;
private: System::Void but_Compare_Click			(System::Object^  sender, System::EventArgs^  e) //    Run      Sonde design/ COMPARE
		 {	_Pr._SdDes._design	 = false ;				 
		           try{                                   
		                  Run(_Pr._SdDes);	
		           }
		           catch ( std::exception& e)
		           { MessageBox::Show ( gcnew String(e.what())  ) ;
		            return;
		           }	 	        		 

		 	}                       //Update_SdDesThDyP()	;
private: System::Void but_TmCalc_Click			(System::Object^  sender, System::EventArgs^  e)	//    Run      TmCalc 
		{	
		           try{                                   
		                	Run(_Pr._TmCal);		
		           }
		           catch ( std::exception& e)
		           { MessageBox::Show ( gcnew String(e.what())  ) ;
		            return;
		           }	 	        		 
			Update_TmCalThDyForm_Results				();
		}
private: void Update_TmCalThDyForm_Results				()
	{	dGVw_TmCalcRes[0,0]->Value	= gcnew Decimal( _Pr._TmCal._TmS.Min() );
		dGVw_TmCalcRes[1,0]->Value	= gcnew Decimal( _Pr._TmCal._TmS.Ave() );
		dGVw_TmCalcRes[2,0]->Value	= gcnew Decimal( _Pr._TmCal._TmS.Max() );

		dGVw_TmCalcRes[0,1]->Value	= gcnew Decimal( _Pr._TmCal._Tm2A.Min() );
		dGVw_TmCalcRes[1,1]->Value	= gcnew Decimal( _Pr._TmCal._Tm2A.Ave() );
		dGVw_TmCalcRes[2,1]->Value	= gcnew Decimal( _Pr._TmCal._Tm2A.Max() );

		dGVw_TmCalcRes[0,2]->Value	= gcnew Decimal( _Pr._TmCal._TmHy.Min() );
		dGVw_TmCalcRes[1,2]->Value	= gcnew Decimal( _Pr._TmCal._TmHy.Ave() );
		dGVw_TmCalcRes[2,2]->Value	= gcnew Decimal( _Pr._TmCal._TmHy.Max() );

		dGVw_TmCalcRes[3,0]->Value	= gcnew Decimal( _Pr._TmCal._GS.Min() );
		dGVw_TmCalcRes[4,0]->Value	= gcnew Decimal( _Pr._TmCal._GS.Ave() );
		dGVw_TmCalcRes[5,0]->Value	= gcnew Decimal( _Pr._TmCal._GS.Max() );

		dGVw_TmCalcRes[3,1]->Value	= gcnew Decimal( _Pr._TmCal._G2A.Min() );
		dGVw_TmCalcRes[4,1]->Value	= gcnew Decimal( _Pr._TmCal._G2A.Ave() );
		dGVw_TmCalcRes[5,1]->Value	= gcnew Decimal( _Pr._TmCal._G2A.Max() );

		dGVw_TmCalcRes[3,2]->Value	= gcnew Decimal( _Pr._TmCal._GHy.Min() );
		dGVw_TmCalcRes[4,2]->Value	= gcnew Decimal( _Pr._TmCal._GHy.Ave() );
		dGVw_TmCalcRes[5,2]->Value	= gcnew Decimal( _Pr._TmCal._GHy.Max() );

		this->txtBx_ResultSec->Text			= gcnew String(_Pr._TmCal._AlignedSec.Get()		) ; 
		this->txtBx_ResultSec2Align->Text	= gcnew String(_Pr._TmCal._AlignedSec2Align.Get() ) ; 
	}
private: System::Void txtBx_Sec_TextChanged		(System::Object^  sender, System::EventArgs^  e) 		//  cambiar Sec2Align por lo corresp de Sec
	{   bool rev  =  this->chkBx_rev->Checked, 
		     compl=  this->chkBx_compl->Checked;	

		_Pr._TmCal.Update_Sec_Sec2Align	(rev, compl) ;
		UpDateForm	(txtBx_Sec2Align);			
		this->txtBx_Sec2Align->Update();			
	}
private: System::Void but_SecUpd_Click			(System::Object^  sender, System::EventArgs^  e) 	//  cambiar Sec por lo corresp de Sec
	{	bool rev  = this->chkBx_rev->Checked, 
			 compl= this->chkBx_compl->Checked;	

		_Pr._TmCal.Update_Sec(rev, compl) ;
		UpDateForm	(txtBx_Sec);	
		this->txtBx_Sec->Update();		
	}
private: System::Void but_Sec2AlignUpd_Click	(System::Object^  sender, System::EventArgs^  e) 	//  cambiar Sec2Align por lo corresp de Sec2Align
	{	bool rev  = this->chkBx_rev->Checked, 
			 compl= this->chkBx_compl->Checked;	

		_Pr._TmCal.Update_Sec2Align(rev, compl) ;
		UpDateForm	(txtBx_Sec2Align);		
		this->txtBx_Sec2Align->Update();	
	}
	private: System::Windows::Forms::Button^  btPCRfiltrFile;
	private: System::Windows::Forms::TextBox^  txtBoxPCRfiltr;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groBox_ComUnic;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  butSeqExplorer;
    private: System::Windows::Forms::CheckBox^  chkBx_unique;
    private: System::Windows::Forms::CheckBox^  chkBx_common;
    private: System::Windows::Forms::NumericUpDown^  numUpDw_MinTargCov;
    private: System::Windows::Forms::OpenFileDialog^  opPCRfiltrFDlg;


	private: System::Windows::Forms::DataGridView^  dGVw_TmCalcRes;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Tm_min;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Tm;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Tm_max;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  G_min;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  G;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  G_max;
private: System::Windows::Forms::CheckBox^		ckBx_savExportTarg;
private: System::Windows::Forms::CheckBox^		ckBx_savExportSond;
private: System::Windows::Forms::CheckBox^		ckBx_savLog;
private: System::Windows::Forms::CheckBox^		ckBx_savNNParam;
private: System::Windows::Forms::TextBox^		textBoxNNParamFile;
private: System::Windows::Forms::Button^		butNNParamFile;
private: System::Windows::Forms::Label^			lab_MinLen;
private: System::Windows::Forms::NumericUpDown^ numUpDw_MinLen;
private: System::Windows::Forms::GroupBox^  grBoxTargets;



private: System::Windows::Forms::TextBox^		txtBx_ResultSec2Align;
private: System::Windows::Forms::TextBox^		txtBx_ResultSec;
private: System::Windows::Forms::CheckBox^		ckBx_loadNNParam;
	private: System::Windows::Forms::CheckBox^			checkBox1;
	private: System::Windows::Forms::OpenFileDialog^	openFDial_NNParam;
	private: System::Windows::Forms::NumericUpDown^		numUpDwMxGrDeg;
	private: System::Windows::Forms::Label^				labMxGrDeg;
	private: System::Windows::Forms::GroupBox^			groupBox2;
private: System::Windows::Forms::CheckBox^		ckBx_savTm;
private: System::Windows::Forms::CheckBox^		ckBx_savPos;
private: System::Windows::Forms::CheckBox^		ckBx_savG;
private: System::Windows::Forms::CheckBox^		ckBx_savG_Plasm;
private: System::Windows::Forms::CheckBox^		ckBx_savTm_Plasm;
private: System::Windows::Forms::CheckBox^		ckBx_savAlign;
	private: System::Windows::Forms::TextBox^		txtBxErrorMsg;
	private: System::Windows::Forms::TextBox^		txtBx_Sec2Align;
	private: System::Windows::Forms::TextBox^		txtBx_Sec;
	private: System::Windows::Forms::Button^		but_TmCalc;
	private: System::Windows::Forms::CheckBox^		chkBx_rev;
	private: System::Windows::Forms::CheckBox^		chkBx_compl;

	private: System::Windows::Forms::CheckBox^		chkBx_Tm_save_asPCR;
	private: System::Windows::Forms::CheckBox^		chkBx_align;
	private: System::Windows::Forms::Button^		but_Sec_Sec2AlignUpd;
	private: System::Windows::Forms::Button^		but_SecUpd;
	private: System::Windows::Forms::Button^		but_Sec2AlignUpd;
	private: System::Windows::Forms::Button^		but_Compare;
	private: System::Windows::Forms::Label^			lab_sG;
	private: System::Windows::Forms::NumericUpDown^  nUpDw_MaxSd_G;
	private: System::Windows::Forms::NumericUpDown^  nUpDw_MinSd_G;

	private: System::Windows::Forms::Label^			lab_sMax;
	private: System::Windows::Forms::GroupBox^		groupBox_Sd_Min_Max;
	private: System::Windows::Forms::Label^		labMinSelfG;
	private: System::Windows::Forms::Label^		label3;
	private: System::Windows::Forms::Label^		labelMinSd_nonTgG;
	private: System::Windows::Forms::Label^		labelMaxSd_TgG;
	private: System::Windows::Forms::Label^		labelMaxSd_nonTgTm;
	private: System::Windows::Forms::NumericUpDown^  nUpDowMinSd_nonTgG;
	private: System::Windows::Forms::NumericUpDown^  nUpDowMaxSd_TgG;
	private: System::Windows::Forms::NumericUpDown^  nUpDowMaxSd_nonTgTm;

	private: System::Windows::Forms::GroupBox^		groupBox_Sd_selft;
	private: System::Windows::Forms::NumericUpDown^  numUpDwMinSelfG;
	private: System::Windows::Forms::GroupBox^		groupBox_Sd_nTg;
	private: System::Windows::Forms::GroupBox^		groupBox_SdTg;
	private: System::Windows::Forms::Label^			lab_please;
private: System::Windows::Forms::CheckBox^		ckBx_savProj;
	private: System::Windows::Forms::OpenFileDialog^	openFileDialog_targets;
	private: System::Windows::Forms::Button^			Design;
	private: System::Windows::Forms::OpenFileDialog^	openFileDialog_exe;
	private: System::Windows::Forms::Button^			buttonOpenNonTargetsSecFile;
	private: System::Windows::Forms::TextBox^			textBoxNonTargetsFile;

	private: System::Windows::Forms::OpenFileDialog^	openFileDialog_non_targets;
	private: System::Windows::Forms::Label^				label_MinSdTm;
	private: System::Windows::Forms::Label^				lab_sMin;

	private: System::Windows::Forms::Label^				label_MinPrimerLength;

	private: System::Windows::Forms::NumericUpDown^		nUpDowMinSdTm;
	private: System::Windows::Forms::NumericUpDown^		nUpDw_MinSdLength;
	private: System::Windows::Forms::NumericUpDown^		nUpDw_MaxSdLength;

	private: System::Windows::Forms::NumericUpDown^  nUpDowMaxSdTm;
private: System::Windows::Forms::NumericUpDown^  numUpDw_MaxTargCov;




	private: System::Windows::Forms::TabControl^  tabControl;

	private: System::Windows::Forms::TabPage^  tabPagFindSonden;

	private: System::Windows::Forms::TabPage^  microArray;
	private: System::Windows::Forms::Button^  buttonTargetsFile;

	private: System::Windows::Forms::Button^  buttonRun_uArray;
 
	private: System::Windows::Forms::Button^  buttonSondeFile;
	private: System::Windows::Forms::TextBox^  textBoxSondesFile;
	private: System::Windows::Forms::TextBox^  textBoxTgScF4uA;

	private: System::Windows::Forms::OpenFileDialog^  openSondesFileDialog;
	private: System::Windows::Forms::Button^  buttonSaveResultFile;
	private: System::Windows::Forms::TextBox^  textBoxSaveResultFile;
	private: System::Windows::Forms::SaveFileDialog^  SaveResultFileDialog;

private: System::Windows::Forms::NumericUpDown^  numUpDowSdConc;
private: System::Windows::Forms::Label^  labSdConc;
private: System::Windows::Forms::NumericUpDown^  numUpDowTgConc;
private: System::Windows::Forms::Label^  labTgConc;
private: System::Windows::Forms::NumericUpDown^  numUpDowTa;
private: System::Windows::Forms::Label^  labTa;
private: System::Windows::Forms::NumericUpDown^  numUpDowSalConc;
private: System::Windows::Forms::Label^  labSalConc;

private: System::Windows::Forms::ComboBox^  comBoxSalMeth;
private: System::Windows::Forms::Label^  labSalMeth;
private: System::Windows::Forms::Label^  labelMinSd_TgTm;
private: System::Windows::Forms::NumericUpDown^  nUpDowMinSd_TgTm;

private: System::Windows::Forms::RichTextBox^  richTextBox1;
private: System::Windows::Forms::TabPage^  tabPagPCRDesing;
private: System::Windows::Forms::NumericUpDown^  numericUpDown_MaxProdLength;
private: System::Windows::Forms::NumericUpDown^  numericUpDown_MinProdLength;
private: System::Windows::Forms::Label^  label_MinProdLength;
private: System::Windows::Forms::Label^  label_MaxProdLength;
private: System::Windows::Forms::TabPage^  tabPagMultiplexPCR;
private: System::Windows::Forms::TabPage^  tabPagTmCalc;
private: System::Windows::Forms::RichTextBox^  richTextBox2;
private: System::Windows::Forms::Button^  buttPCR;
private: System::Windows::Forms::Button^  butSdSecFilePCR;
private: System::Windows::Forms::TextBox^  textBoxSdSecFilePCR;
private: System::Windows::Forms::Label^  labTAMeth;
private: System::Windows::Forms::ComboBox^  comBoxTAMeth;
private: System::Windows::Forms::ToolTip^  toolTip;
private: System::Windows::Forms::Label^  labMaxTgId;
private: System::Windows::Forms::NumericUpDown^  numUpDwMaxTgId;
private: System::Windows::Forms::NumericUpDown^  numUpDw_TgBeg;
private: System::Windows::Forms::Label^  labTgBeg;
private: System::Windows::Forms::Label^  labTgEnd;
private: System::Windows::Forms::NumericUpDown^  numUpDw_TgEnd;
private: System::Windows::Forms::Label^  labMaxSelfTm;
private: System::Windows::Forms::NumericUpDown^  numUpDwMaxSelfTm;
private: System::Windows::Forms::Button^  buttPrSave;
private: System::Windows::Forms::Button^  buttPrLoad;
private: System::Windows::Forms::TextBox^  textBoxPrFile;
private: System::Windows::Forms::SaveFileDialog^  savePrFileDialog;
private: System::Windows::Forms::OpenFileDialog^  loadPrFileDialog;
private: System::Windows::Forms::TabPage^  tabPagSetup;
private: System::Windows::Forms::Button^  butNoProcess;

private: System::Windows::Forms::Button^  butCreateDir;

private: System::Windows::Forms::Button^  butRestDefPr;

private: System::Windows::Forms::Button^  butSetDefPr;

private: System::Windows::Forms::Button^  but_uArrExp;

private: System::Windows::Forms::CheckBox^  checkBox15;
private: System::Windows::Forms::CheckBox^  checkBox14;
private: System::Windows::Forms::CheckBox^  checkBox13;
private: System::Windows::Forms::CheckBox^  checkBox12;
private: System::Windows::Forms::CheckBox^  checkBox11;

private: System::Windows::Forms::TextBox^  txtBoxExp;

private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::CheckBox^  checkBox16;
private: System::Windows::Forms::TextBox^  textBox2;
private: System::Windows::Forms::OpenFileDialog^  oFileExp;
private: System::Windows::Forms::RadioButton^  radioButton2;
private: System::Windows::Forms::RadioButton^  radioButton1;
private: System::Windows::Forms::NumericUpDown^  numericUpDown5;

private: System::Windows::Forms::NumericUpDown^  numericUpDown4;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Label^  label7;

private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::CheckBox^  checkBox17;
private: System::Windows::Forms::CheckBox^  checkBox18;
private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
private: System::Windows::Forms::Label^  label9;


private: System::ComponentModel::IContainer^  components;

		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>

		 void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle8 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle9 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle7 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->openFileDialog_targets = (gcnew System::Windows::Forms::OpenFileDialog());
			this->Design = (gcnew System::Windows::Forms::Button());
			this->openFileDialog_exe = (gcnew System::Windows::Forms::OpenFileDialog());
			this->buttonOpenNonTargetsSecFile = (gcnew System::Windows::Forms::Button());
			this->textBoxNonTargetsFile = (gcnew System::Windows::Forms::TextBox());
			this->openFileDialog_non_targets = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label_MinSdTm = (gcnew System::Windows::Forms::Label());
			this->lab_sMin = (gcnew System::Windows::Forms::Label());
			this->label_MinPrimerLength = (gcnew System::Windows::Forms::Label());
			this->nUpDowMinSdTm = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDw_MinSdLength = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDw_MaxSdLength = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDowMaxSdTm = (gcnew System::Windows::Forms::NumericUpDown());
			this->numUpDw_MaxTargCov = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->tabPagFindSonden = (gcnew System::Windows::Forms::TabPage());
			this->but_Compare = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lab_sG = (gcnew System::Windows::Forms::Label());
			this->nUpDw_MaxSd_G = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDw_MinSd_G = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox_Sd_Min_Max = (gcnew System::Windows::Forms::GroupBox());
			this->lab_sMax = (gcnew System::Windows::Forms::Label());
			this->groBox_ComUnic = (gcnew System::Windows::Forms::GroupBox());
			this->numUpDw_MinTargCov = (gcnew System::Windows::Forms::NumericUpDown());
			this->chkBx_unique = (gcnew System::Windows::Forms::CheckBox());
			this->chkBx_common = (gcnew System::Windows::Forms::CheckBox());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->groupBox_SdTg = (gcnew System::Windows::Forms::GroupBox());
			this->nUpDowMinSd_TgTm = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDowMaxSd_TgG = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelMinSd_TgTm = (gcnew System::Windows::Forms::Label());
			this->labelMaxSd_TgG = (gcnew System::Windows::Forms::Label());
			this->groupBox_Sd_nTg = (gcnew System::Windows::Forms::GroupBox());
			this->nUpDowMaxSd_nonTgTm = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUpDowMinSd_nonTgG = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelMaxSd_nonTgTm = (gcnew System::Windows::Forms::Label());
			this->labelMinSd_nonTgG = (gcnew System::Windows::Forms::Label());
			this->groupBox_Sd_selft = (gcnew System::Windows::Forms::GroupBox());
			this->numUpDwMinSelfG = (gcnew System::Windows::Forms::NumericUpDown());
			this->numUpDwMaxSelfTm = (gcnew System::Windows::Forms::NumericUpDown());
			this->labMinSelfG = (gcnew System::Windows::Forms::Label());
			this->labMaxSelfTm = (gcnew System::Windows::Forms::Label());
			this->microArray = (gcnew System::Windows::Forms::TabPage());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->buttonRun_uArray = (gcnew System::Windows::Forms::Button());
			this->but_uArrExp = (gcnew System::Windows::Forms::Button());
			this->buttonSondeFile = (gcnew System::Windows::Forms::Button());
			this->checkBox15 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox14 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox13 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox12 = (gcnew System::Windows::Forms::CheckBox());
			this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->checkBox16 = (gcnew System::Windows::Forms::CheckBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->checkBox11 = (gcnew System::Windows::Forms::CheckBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->txtBoxExp = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSondesFile = (gcnew System::Windows::Forms::TextBox());
			this->tabPagPCRDesing = (gcnew System::Windows::Forms::TabPage());
			this->lab_please = (gcnew System::Windows::Forms::Label());
			this->numericUpDown_MaxProdLength = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown_MinProdLength = (gcnew System::Windows::Forms::NumericUpDown());
			this->label_MinProdLength = (gcnew System::Windows::Forms::Label());
			this->label_MaxProdLength = (gcnew System::Windows::Forms::Label());
			this->tabPagMultiplexPCR = (gcnew System::Windows::Forms::TabPage());
			this->checkBox17 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox18 = (gcnew System::Windows::Forms::CheckBox());
			this->buttPCR = (gcnew System::Windows::Forms::Button());
			this->butSdSecFilePCR = (gcnew System::Windows::Forms::Button());
			this->textBoxSdSecFilePCR = (gcnew System::Windows::Forms::TextBox());
			this->tabPagTmCalc = (gcnew System::Windows::Forms::TabPage());
			this->dGVw_TmCalcRes = (gcnew System::Windows::Forms::DataGridView());
			this->Tm_min = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Tm = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Tm_max = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->G_min = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->G = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->G_max = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->chkBx_align = (gcnew System::Windows::Forms::CheckBox());
			this->chkBx_Tm_save_asPCR = (gcnew System::Windows::Forms::CheckBox());
			this->but_Sec2AlignUpd = (gcnew System::Windows::Forms::Button());
			this->but_SecUpd = (gcnew System::Windows::Forms::Button());
			this->but_Sec_Sec2AlignUpd = (gcnew System::Windows::Forms::Button());
			this->but_TmCalc = (gcnew System::Windows::Forms::Button());
			this->chkBx_rev = (gcnew System::Windows::Forms::CheckBox());
			this->chkBx_compl = (gcnew System::Windows::Forms::CheckBox());
			this->txtBxErrorMsg = (gcnew System::Windows::Forms::TextBox());
			this->txtBx_Sec2Align = (gcnew System::Windows::Forms::TextBox());
			this->txtBx_ResultSec2Align = (gcnew System::Windows::Forms::TextBox());
			this->txtBx_Sec = (gcnew System::Windows::Forms::TextBox());
			this->txtBx_ResultSec = (gcnew System::Windows::Forms::TextBox());
			this->tabPagSetup = (gcnew System::Windows::Forms::TabPage());
			this->ckBx_savNNParam = (gcnew System::Windows::Forms::CheckBox());
			this->numUpDwMxGrDeg = (gcnew System::Windows::Forms::NumericUpDown());
			this->labMxGrDeg = (gcnew System::Windows::Forms::Label());
			this->butNoProcess = (gcnew System::Windows::Forms::Button());
			this->ckBx_loadNNParam = (gcnew System::Windows::Forms::CheckBox());
			this->butCreateDir = (gcnew System::Windows::Forms::Button());
			this->textBoxNNParamFile = (gcnew System::Windows::Forms::TextBox());
			this->butRestDefPr = (gcnew System::Windows::Forms::Button());
			this->butSetDefPr = (gcnew System::Windows::Forms::Button());
			this->butNNParamFile = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->ckBx_savExportTarg = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savExportSond = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savTm = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savPos = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savG = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savG_Plasm = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savTm_Plasm = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savAlign = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savProj = (gcnew System::Windows::Forms::CheckBox());
			this->ckBx_savLog = (gcnew System::Windows::Forms::CheckBox());
			this->buttonTargetsFile = (gcnew System::Windows::Forms::Button());
			this->buttonSaveResultFile = (gcnew System::Windows::Forms::Button());
			this->textBoxSaveResultFile = (gcnew System::Windows::Forms::TextBox());
			this->textBoxTgScF4uA = (gcnew System::Windows::Forms::TextBox());
			this->openSondesFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SaveResultFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->numUpDowSdConc = (gcnew System::Windows::Forms::NumericUpDown());
			this->labSdConc = (gcnew System::Windows::Forms::Label());
			this->numUpDowTgConc = (gcnew System::Windows::Forms::NumericUpDown());
			this->labTgConc = (gcnew System::Windows::Forms::Label());
			this->numUpDowTa = (gcnew System::Windows::Forms::NumericUpDown());
			this->labTa = (gcnew System::Windows::Forms::Label());
			this->numUpDowSalConc = (gcnew System::Windows::Forms::NumericUpDown());
			this->labSalConc = (gcnew System::Windows::Forms::Label());
			this->comBoxSalMeth = (gcnew System::Windows::Forms::ComboBox());
			this->labSalMeth = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->labTAMeth = (gcnew System::Windows::Forms::Label());
			this->comBoxTAMeth = (gcnew System::Windows::Forms::ComboBox());
			this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->labMaxTgId = (gcnew System::Windows::Forms::Label());
			this->numUpDwMaxTgId = (gcnew System::Windows::Forms::NumericUpDown());
			this->labTgEnd = (gcnew System::Windows::Forms::Label());
			this->numUpDw_TgEnd = (gcnew System::Windows::Forms::NumericUpDown());
			this->lab_MinLen = (gcnew System::Windows::Forms::Label());
			this->numUpDw_MinLen = (gcnew System::Windows::Forms::NumericUpDown());
			this->numUpDw_TgBeg = (gcnew System::Windows::Forms::NumericUpDown());
			this->labTgBeg = (gcnew System::Windows::Forms::Label());
			this->buttPrSave = (gcnew System::Windows::Forms::Button());
			this->buttPrLoad = (gcnew System::Windows::Forms::Button());
			this->textBoxPrFile = (gcnew System::Windows::Forms::TextBox());
			this->savePrFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->loadPrFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->grBoxTargets = (gcnew System::Windows::Forms::GroupBox());
			this->btPCRfiltrFile = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->txtBoxPCRfiltr = (gcnew System::Windows::Forms::TextBox());
			this->openFDial_NNParam = (gcnew System::Windows::Forms::OpenFileDialog());
			this->oFileExp = (gcnew System::Windows::Forms::OpenFileDialog());
			this->opPCRfiltrFDlg = (gcnew System::Windows::Forms::OpenFileDialog());
			this->butSeqExplorer = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSdTm))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MinSdLength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MaxSdLength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSdTm))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_MinTargCov))->BeginInit();
			this->tabControl->SuspendLayout();
			this->tabPagFindSonden->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MaxSd_G))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MinSd_G))->BeginInit();
			this->groupBox_Sd_Min_Max->SuspendLayout();
			this->groBox_ComUnic->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->groupBox_SdTg->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSd_TgTm))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSd_TgG))->BeginInit();
			this->groupBox_Sd_nTg->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSd_nonTgTm))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSd_nonTgG))->BeginInit();
			this->groupBox_Sd_selft->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMinSelfG))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMaxSelfTm))->BeginInit();
			this->microArray->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			this->tabPagPCRDesing->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_MaxProdLength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_MinProdLength))->BeginInit();
			this->tabPagMultiplexPCR->SuspendLayout();
			this->tabPagTmCalc->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dGVw_TmCalcRes))->BeginInit();
			this->tabPagSetup->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMxGrDeg))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowSdConc))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowTgConc))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowTa))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowSalConc))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMaxTgId))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_TgEnd))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_MinLen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_TgBeg))->BeginInit();
			this->grBoxTargets->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog_targets
			// 
			this->openFileDialog_targets->Filter = L"fasta|*.fas;*.fasta|NCBI BLAST|*-Alignment.xml|GB|*.gb;*-sequence.xml|Text|*.txt|" 
				L"All file|*.*";
			this->openFileDialog_targets->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\targets";
			this->openFileDialog_targets->Title = L"Open Targets File";
			// 
			// Design
			// 
			this->Design->Location = System::Drawing::Point(42, 226);
			this->Design->Name = L"Design";
			this->Design->Size = System::Drawing::Size(122, 23);
			this->Design->TabIndex = 2;
			this->Design->Text = L"Only 100% common !";
			this->Design->UseVisualStyleBackColor = true;
			this->Design->Click += gcnew System::EventHandler(this, &ThPr_Form::commandDesign);
			// 
			// openFileDialog_exe
			// 
			this->openFileDialog_exe->DefaultExt = L"exe";
			this->openFileDialog_exe->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\";
			this->openFileDialog_exe->ShowReadOnly = true;
			this->openFileDialog_exe->SupportMultiDottedExtensions = true;
			this->openFileDialog_exe->Title = L"Select exe file";
			// 
			// buttonOpenNonTargetsSecFile
			// 
			this->buttonOpenNonTargetsSecFile->Enabled = false;
			this->buttonOpenNonTargetsSecFile->Location = System::Drawing::Point(3, 7);
			this->buttonOpenNonTargetsSecFile->Name = L"buttonOpenNonTargetsSecFile";
			this->buttonOpenNonTargetsSecFile->Size = System::Drawing::Size(116, 23);
			this->buttonOpenNonTargetsSecFile->TabIndex = 0;
			this->buttonOpenNonTargetsSecFile->Text = L"NonTargetsSecFile";
			this->buttonOpenNonTargetsSecFile->UseVisualStyleBackColor = true;
			this->buttonOpenNonTargetsSecFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandOpenNonTargetsSecFile);
			// 
			// textBoxNonTargetsFile
			// 
			this->textBoxNonTargetsFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxNonTargetsFile->Enabled = false;
			this->textBoxNonTargetsFile->Location = System::Drawing::Point(123, 9);
			this->textBoxNonTargetsFile->Name = L"textBoxNonTargetsFile";
			this->textBoxNonTargetsFile->Size = System::Drawing::Size(419, 20);
			this->textBoxNonTargetsFile->TabIndex = 1;
			this->textBoxNonTargetsFile->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// openFileDialog_non_targets
			// 
			this->openFileDialog_non_targets->Filter = L"fasta|*.fas|fasta|*.fasta|Text|*.txt|All file|*.*";
			this->openFileDialog_non_targets->Title = L"Open Non-Targets Sec File";
			// 
			// label_MinSdTm
			// 
			this->label_MinSdTm->AutoSize = true;
			this->label_MinSdTm->Location = System::Drawing::Point(61, 90);
			this->label_MinSdTm->Name = L"label_MinSdTm";
			this->label_MinSdTm->Size = System::Drawing::Size(22, 13);
			this->label_MinSdTm->TabIndex = 3;
			this->label_MinSdTm->Text = L"Tm";
			// 
			// lab_sMin
			// 
			this->lab_sMin->AutoSize = true;
			this->lab_sMin->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->lab_sMin->Location = System::Drawing::Point(97, 12);
			this->lab_sMin->Name = L"lab_sMin";
			this->lab_sMin->Size = System::Drawing::Size(27, 13);
			this->lab_sMin->TabIndex = 3;
			this->lab_sMin->Text = L"Min.";
			// 
			// label_MinPrimerLength
			// 
			this->label_MinPrimerLength->AutoSize = true;
			this->label_MinPrimerLength->Location = System::Drawing::Point(52, 116);
			this->label_MinPrimerLength->Name = L"label_MinPrimerLength";
			this->label_MinPrimerLength->Size = System::Drawing::Size(40, 13);
			this->label_MinPrimerLength->TabIndex = 3;
			this->label_MinPrimerLength->Text = L"Length";
			// 
			// nUpDowMinSdTm
			// 
			this->nUpDowMinSdTm->DecimalPlaces = 2;
			this->nUpDowMinSdTm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMinSdTm->Location = System::Drawing::Point(90, 88);
			this->nUpDowMinSdTm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {90, 0, 0, 0});
			this->nUpDowMinSdTm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->nUpDowMinSdTm->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMinSdTm->Name = L"nUpDowMinSdTm";
			this->nUpDowMinSdTm->Size = System::Drawing::Size(52, 20);
			this->nUpDowMinSdTm->TabIndex = 4;
			this->nUpDowMinSdTm->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {57, 0, 0, 0});
			// 
			// nUpDw_MinSdLength
			// 
			this->nUpDw_MinSdLength->Location = System::Drawing::Point(99, 114);
			this->nUpDw_MinSdLength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			this->nUpDw_MinSdLength->Name = L"nUpDw_MinSdLength";
			this->nUpDw_MinSdLength->Size = System::Drawing::Size(43, 20);
			this->nUpDw_MinSdLength->TabIndex = 4;
			this->nUpDw_MinSdLength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			// 
			// nUpDw_MaxSdLength
			// 
			this->nUpDw_MaxSdLength->Location = System::Drawing::Point(153, 114);
			this->nUpDw_MaxSdLength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->nUpDw_MaxSdLength->Name = L"nUpDw_MaxSdLength";
			this->nUpDw_MaxSdLength->Size = System::Drawing::Size(43, 20);
			this->nUpDw_MaxSdLength->TabIndex = 4;
			this->nUpDw_MaxSdLength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {35, 0, 0, 0});
			// 
			// nUpDowMaxSdTm
			// 
			this->nUpDowMaxSdTm->DecimalPlaces = 2;
			this->nUpDowMaxSdTm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMaxSdTm->Location = System::Drawing::Point(153, 88);
			this->nUpDowMaxSdTm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {90, 0, 0, 0});
			this->nUpDowMaxSdTm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->nUpDowMaxSdTm->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMaxSdTm->Name = L"nUpDowMaxSdTm";
			this->nUpDowMaxSdTm->Size = System::Drawing::Size(52, 20);
			this->nUpDowMaxSdTm->TabIndex = 4;
			this->nUpDowMaxSdTm->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {63, 0, 0, 0});
			// 
			// numUpDw_MaxTargCov
			// 
			this->numUpDw_MaxTargCov->DecimalPlaces = 1;
			this->numUpDw_MaxTargCov->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->numUpDw_MaxTargCov->Location = System::Drawing::Point(222, 40);
			this->numUpDw_MaxTargCov->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDw_MaxTargCov->Name = L"numUpDw_MaxTargCov";
			this->numUpDw_MaxTargCov->Size = System::Drawing::Size(52, 20);
			this->numUpDw_MaxTargCov->TabIndex = 4;
			this->numUpDw_MaxTargCov->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// tabControl
			// 
			this->tabControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl->Controls->Add(this->tabPagFindSonden);
			this->tabControl->Controls->Add(this->microArray);
			this->tabControl->Controls->Add(this->tabPagPCRDesing);
			this->tabControl->Controls->Add(this->tabPagMultiplexPCR);
			this->tabControl->Controls->Add(this->tabPagTmCalc);
			this->tabControl->Controls->Add(this->tabPagSetup);
			this->tabControl->Location = System::Drawing::Point(1, 35);
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(556, 281);
			this->tabControl->TabIndex = 5;
			// 
			// tabPagFindSonden
			// 
			this->tabPagFindSonden->Controls->Add(this->but_Compare);
			this->tabPagFindSonden->Controls->Add(this->Design);
			this->tabPagFindSonden->Controls->Add(this->nUpDw_MaxSdLength);
			this->tabPagFindSonden->Controls->Add(this->label1);
			this->tabPagFindSonden->Controls->Add(this->label3);
			this->tabPagFindSonden->Controls->Add(this->lab_sG);
			this->tabPagFindSonden->Controls->Add(this->label_MinSdTm);
			this->tabPagFindSonden->Controls->Add(this->nUpDw_MinSdLength);
			this->tabPagFindSonden->Controls->Add(this->nUpDw_MaxSd_G);
			this->tabPagFindSonden->Controls->Add(this->nUpDowMaxSdTm);
			this->tabPagFindSonden->Controls->Add(this->label_MinPrimerLength);
			this->tabPagFindSonden->Controls->Add(this->nUpDw_MinSd_G);
			this->tabPagFindSonden->Controls->Add(this->nUpDowMinSdTm);
			this->tabPagFindSonden->Controls->Add(this->buttonOpenNonTargetsSecFile);
			this->tabPagFindSonden->Controls->Add(this->textBoxNonTargetsFile);
			this->tabPagFindSonden->Controls->Add(this->groupBox_Sd_Min_Max);
			this->tabPagFindSonden->Controls->Add(this->groBox_ComUnic);
			this->tabPagFindSonden->Controls->Add(this->flowLayoutPanel1);
			this->tabPagFindSonden->Location = System::Drawing::Point(4, 22);
			this->tabPagFindSonden->Name = L"tabPagFindSonden";
			this->tabPagFindSonden->Padding = System::Windows::Forms::Padding(3);
			this->tabPagFindSonden->Size = System::Drawing::Size(548, 255);
			this->tabPagFindSonden->TabIndex = 0;
			this->tabPagFindSonden->Text = L"Find probes";
			this->tabPagFindSonden->UseVisualStyleBackColor = true;
			// 
			// but_Compare
			// 
			this->but_Compare->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->but_Compare->Location = System::Drawing::Point(380, 225);
			this->but_Compare->Name = L"but_Compare";
			this->but_Compare->Size = System::Drawing::Size(83, 23);
			this->but_Compare->TabIndex = 2;
			this->but_Compare->Text = L"Find !";
			this->but_Compare->UseVisualStyleBackColor = true;
			this->but_Compare->Click += gcnew System::EventHandler(this, &ThPr_Form::but_Compare_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(214, 65);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"  Tm ( °C )";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(214, 47);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(71, 13);
			this->label3->TabIndex = 3;
			this->label3->Text = L"G ( kcal/mol )";
			// 
			// lab_sG
			// 
			this->lab_sG->AutoSize = true;
			this->lab_sG->Location = System::Drawing::Point(67, 65);
			this->lab_sG->Name = L"lab_sG";
			this->lab_sG->Size = System::Drawing::Size(15, 13);
			this->lab_sG->TabIndex = 3;
			this->lab_sG->Text = L"G";
			// 
			// nUpDw_MaxSd_G
			// 
			this->nUpDw_MaxSd_G->DecimalPlaces = 2;
			this->nUpDw_MaxSd_G->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDw_MaxSd_G->Location = System::Drawing::Point(153, 63);
			this->nUpDw_MaxSd_G->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
			this->nUpDw_MaxSd_G->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->nUpDw_MaxSd_G->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDw_MaxSd_G->Name = L"nUpDw_MaxSd_G";
			this->nUpDw_MaxSd_G->Size = System::Drawing::Size(52, 20);
			this->nUpDw_MaxSd_G->TabIndex = 4;
			this->nUpDw_MaxSd_G->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			// 
			// nUpDw_MinSd_G
			// 
			this->nUpDw_MinSd_G->DecimalPlaces = 2;
			this->nUpDw_MinSd_G->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDw_MinSd_G->Location = System::Drawing::Point(90, 63);
			this->nUpDw_MinSd_G->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
			this->nUpDw_MinSd_G->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->nUpDw_MinSd_G->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDw_MinSd_G->Name = L"nUpDw_MinSd_G";
			this->nUpDw_MinSd_G->Size = System::Drawing::Size(52, 20);
			this->nUpDw_MinSd_G->TabIndex = 4;
			this->nUpDw_MinSd_G->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, System::Int32::MinValue});
			// 
			// groupBox_Sd_Min_Max
			// 
			this->groupBox_Sd_Min_Max->Controls->Add(this->lab_sMax);
			this->groupBox_Sd_Min_Max->Controls->Add(this->lab_sMin);
			this->groupBox_Sd_Min_Max->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_Sd_Min_Max->Location = System::Drawing::Point(8, 33);
			this->groupBox_Sd_Min_Max->Name = L"groupBox_Sd_Min_Max";
			this->groupBox_Sd_Min_Max->Size = System::Drawing::Size(200, 111);
			this->groupBox_Sd_Min_Max->TabIndex = 5;
			this->groupBox_Sd_Min_Max->TabStop = false;
			this->groupBox_Sd_Min_Max->Text = L"Probe (PM)";
			// 
			// lab_sMax
			// 
			this->lab_sMax->AutoSize = true;
			this->lab_sMax->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->lab_sMax->Location = System::Drawing::Point(154, 12);
			this->lab_sMax->Name = L"lab_sMax";
			this->lab_sMax->Size = System::Drawing::Size(30, 13);
			this->lab_sMax->TabIndex = 3;
			this->lab_sMax->Text = L"Max.";
			// 
			// groBox_ComUnic
			// 
			this->groBox_ComUnic->Controls->Add(this->numUpDw_MinTargCov);
			this->groBox_ComUnic->Controls->Add(this->chkBx_unique);
			this->groBox_ComUnic->Controls->Add(this->numUpDw_MaxTargCov);
			this->groBox_ComUnic->Controls->Add(this->chkBx_common);
			this->groBox_ComUnic->Location = System::Drawing::Point(8, 148);
			this->groBox_ComUnic->Name = L"groBox_ComUnic";
			this->groBox_ComUnic->Size = System::Drawing::Size(286, 67);
			this->groBox_ComUnic->TabIndex = 6;
			this->groBox_ComUnic->TabStop = false;
			this->groBox_ComUnic->Text = L"Find probes";
			// 
			// numUpDw_MinTargCov
			// 
			this->numUpDw_MinTargCov->DecimalPlaces = 1;
			this->numUpDw_MinTargCov->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->numUpDw_MinTargCov->Location = System::Drawing::Point(222, 16);
			this->numUpDw_MinTargCov->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDw_MinTargCov->Name = L"numUpDw_MinTargCov";
			this->numUpDw_MinTargCov->Size = System::Drawing::Size(52, 20);
			this->numUpDw_MinTargCov->TabIndex = 7;
			// 
			// chkBx_unique
			// 
			this->chkBx_unique->AutoSize = true;
			this->chkBx_unique->Checked = true;
			this->chkBx_unique->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBx_unique->Location = System::Drawing::Point(11, 18);
			this->chkBx_unique->Name = L"chkBx_unique";
			this->chkBx_unique->Size = System::Drawing::Size(208, 17);
			this->chkBx_unique->TabIndex = 8;
			this->chkBx_unique->Text = L"Unique, with Max Target Coverage (%)";
			this->toolTip->SetToolTip(this->chkBx_unique, L"Probes with hybrid in one target but in not more than in a % of the others target"
				L"s");
			this->chkBx_unique->UseVisualStyleBackColor = true;
			// 
			// chkBx_common
			// 
			this->chkBx_common->AutoSize = true;
			this->chkBx_common->Checked = true;
			this->chkBx_common->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBx_common->Location = System::Drawing::Point(11, 41);
			this->chkBx_common->Name = L"chkBx_common";
			this->chkBx_common->Size = System::Drawing::Size(212, 17);
			this->chkBx_common->TabIndex = 6;
			this->chkBx_common->Text = L"Common, with Min Target Coverage (%)";
			this->toolTip->SetToolTip(this->chkBx_common, L"Probes with hybrid in one target and at last in a % of the others targets");
			this->chkBx_common->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->flowLayoutPanel1->Controls->Add(this->groupBox_SdTg);
			this->flowLayoutPanel1->Controls->Add(this->groupBox_Sd_nTg);
			this->flowLayoutPanel1->Controls->Add(this->groupBox_Sd_selft);
			this->flowLayoutPanel1->Location = System::Drawing::Point(302, 25);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(241, 194);
			this->flowLayoutPanel1->TabIndex = 16;
			// 
			// groupBox_SdTg
			// 
			this->groupBox_SdTg->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox_SdTg->Controls->Add(this->nUpDowMinSd_TgTm);
			this->groupBox_SdTg->Controls->Add(this->nUpDowMaxSd_TgG);
			this->groupBox_SdTg->Controls->Add(this->labelMinSd_TgTm);
			this->groupBox_SdTg->Controls->Add(this->labelMaxSd_TgG);
			this->groupBox_SdTg->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_SdTg->Location = System::Drawing::Point(1, 1);
			this->groupBox_SdTg->Margin = System::Windows::Forms::Padding(1);
			this->groupBox_SdTg->Name = L"groupBox_SdTg";
			this->groupBox_SdTg->Size = System::Drawing::Size(237, 61);
			this->groupBox_SdTg->TabIndex = 5;
			this->groupBox_SdTg->TabStop = false;
			this->groupBox_SdTg->Text = L"Probe-target";
			this->toolTip->SetToolTip(this->groupBox_SdTg, L"Only sonde with stronger interaction with target will be \"include\"");
			// 
			// nUpDowMinSd_TgTm
			// 
			this->nUpDowMinSd_TgTm->DecimalPlaces = 2;
			this->nUpDowMinSd_TgTm->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			this->nUpDowMinSd_TgTm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMinSd_TgTm->Location = System::Drawing::Point(172, 36);
			this->nUpDowMinSd_TgTm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {70, 0, 0, 0});
			this->nUpDowMinSd_TgTm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {273, 0, 0, System::Int32::MinValue});
			this->nUpDowMinSd_TgTm->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMinSd_TgTm->Name = L"nUpDowMinSd_TgTm";
			this->nUpDowMinSd_TgTm->Size = System::Drawing::Size(52, 20);
			this->nUpDowMinSd_TgTm->TabIndex = 4;
			this->toolTip->SetToolTip(this->nUpDowMinSd_TgTm, L"Only sonde with longer sonde-target Tm will \"include\"");
			this->nUpDowMinSd_TgTm->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			// 
			// nUpDowMaxSd_TgG
			// 
			this->nUpDowMaxSd_TgG->DecimalPlaces = 2;
			this->nUpDowMaxSd_TgG->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			this->nUpDowMaxSd_TgG->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMaxSd_TgG->Location = System::Drawing::Point(172, 13);
			this->nUpDowMaxSd_TgG->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->nUpDowMaxSd_TgG->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->nUpDowMaxSd_TgG->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMaxSd_TgG->Name = L"nUpDowMaxSd_TgG";
			this->nUpDowMaxSd_TgG->Size = System::Drawing::Size(52, 20);
			this->nUpDowMaxSd_TgG->TabIndex = 4;
			this->toolTip->SetToolTip(this->nUpDowMaxSd_TgG, L"Only sonde with stronger interaction with target  (smaller G by selected Ta) will" 
				L" be \"include\"");
			this->nUpDowMaxSd_TgG->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// labelMinSd_TgTm
			// 
			this->labelMinSd_TgTm->AutoSize = true;
			this->labelMinSd_TgTm->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold));
			this->labelMinSd_TgTm->Location = System::Drawing::Point(119, 38);
			this->labelMinSd_TgTm->Name = L"labelMinSd_TgTm";
			this->labelMinSd_TgTm->Size = System::Drawing::Size(48, 15);
			this->labelMinSd_TgTm->TabIndex = 3;
			this->labelMinSd_TgTm->Text = L"Min Tm";
			this->toolTip->SetToolTip(this->labelMinSd_TgTm, L"Only sonde with longer sonde-target Tm will \"include\"");
			// 
			// labelMaxSd_TgG
			// 
			this->labelMaxSd_TgG->AutoSize = true;
			this->labelMaxSd_TgG->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold));
			this->labelMaxSd_TgG->Location = System::Drawing::Point(122, 15);
			this->labelMaxSd_TgG->Name = L"labelMaxSd_TgG";
			this->labelMaxSd_TgG->Size = System::Drawing::Size(42, 15);
			this->labelMaxSd_TgG->TabIndex = 3;
			this->labelMaxSd_TgG->Text = L"Max G";
			this->toolTip->SetToolTip(this->labelMaxSd_TgG, L"Only sonde with stronger interaction with target  (smaller G by selected Ta) will" 
				L" be \"include\"");
			// 
			// groupBox_Sd_nTg
			// 
			this->groupBox_Sd_nTg->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox_Sd_nTg->Controls->Add(this->nUpDowMaxSd_nonTgTm);
			this->groupBox_Sd_nTg->Controls->Add(this->nUpDowMinSd_nonTgG);
			this->groupBox_Sd_nTg->Controls->Add(this->labelMaxSd_nonTgTm);
			this->groupBox_Sd_nTg->Controls->Add(this->labelMinSd_nonTgG);
			this->groupBox_Sd_nTg->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_Sd_nTg->Location = System::Drawing::Point(1, 64);
			this->groupBox_Sd_nTg->Margin = System::Windows::Forms::Padding(1);
			this->groupBox_Sd_nTg->Name = L"groupBox_Sd_nTg";
			this->groupBox_Sd_nTg->Size = System::Drawing::Size(237, 62);
			this->groupBox_Sd_nTg->TabIndex = 5;
			this->groupBox_Sd_nTg->TabStop = false;
			this->groupBox_Sd_nTg->Text = L"Probe-non-target";
			this->toolTip->SetToolTip(this->groupBox_Sd_nTg, L"Only sonde with weak interaction with non-target will be \"include\"");
			// 
			// nUpDowMaxSd_nonTgTm
			// 
			this->nUpDowMaxSd_nonTgTm->DecimalPlaces = 2;
			this->nUpDowMaxSd_nonTgTm->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			this->nUpDowMaxSd_nonTgTm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMaxSd_nonTgTm->Location = System::Drawing::Point(175, 36);
			this->nUpDowMaxSd_nonTgTm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {70, 0, 0, 0});
			this->nUpDowMaxSd_nonTgTm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {273, 0, 0, System::Int32::MinValue});
			this->nUpDowMaxSd_nonTgTm->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMaxSd_nonTgTm->Name = L"nUpDowMaxSd_nonTgTm";
			this->nUpDowMaxSd_nonTgTm->Size = System::Drawing::Size(52, 20);
			this->nUpDowMaxSd_nonTgTm->TabIndex = 4;
			this->nUpDowMaxSd_nonTgTm->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// nUpDowMinSd_nonTgG
			// 
			this->nUpDowMinSd_nonTgG->DecimalPlaces = 2;
			this->nUpDowMinSd_nonTgG->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			this->nUpDowMinSd_nonTgG->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUpDowMinSd_nonTgG->Location = System::Drawing::Point(175, 13);
			this->nUpDowMinSd_nonTgG->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->nUpDowMinSd_nonTgG->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->nUpDowMinSd_nonTgG->MinimumSize = System::Drawing::Size(30, 0);
			this->nUpDowMinSd_nonTgG->Name = L"nUpDowMinSd_nonTgG";
			this->nUpDowMinSd_nonTgG->Size = System::Drawing::Size(52, 20);
			this->nUpDowMinSd_nonTgG->TabIndex = 4;
			this->toolTip->SetToolTip(this->nUpDowMinSd_nonTgG, L"Only sonde with weak interaction with non-target (larger G by selected Ta) will b" 
				L"e \"include\"");
			this->nUpDowMinSd_nonTgG->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			// 
			// labelMaxSd_nonTgTm
			// 
			this->labelMaxSd_nonTgTm->AutoSize = true;
			this->labelMaxSd_nonTgTm->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold));
			this->labelMaxSd_nonTgTm->Location = System::Drawing::Point(116, 38);
			this->labelMaxSd_nonTgTm->Name = L"labelMaxSd_nonTgTm";
			this->labelMaxSd_nonTgTm->Size = System::Drawing::Size(52, 15);
			this->labelMaxSd_nonTgTm->TabIndex = 3;
			this->labelMaxSd_nonTgTm->Text = L"Max Tm";
			// 
			// labelMinSd_nonTgG
			// 
			this->labelMinSd_nonTgG->AutoSize = true;
			this->labelMinSd_nonTgG->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold));
			this->labelMinSd_nonTgG->Location = System::Drawing::Point(130, 15);
			this->labelMinSd_nonTgG->Name = L"labelMinSd_nonTgG";
			this->labelMinSd_nonTgG->Size = System::Drawing::Size(38, 15);
			this->labelMinSd_nonTgG->TabIndex = 3;
			this->labelMinSd_nonTgG->Text = L"Min G";
			this->toolTip->SetToolTip(this->labelMinSd_nonTgG, L"Only sonde with weak interaction with non-target \r\n(larger G by selected Ta) will" 
				L" be \"include\"");
			// 
			// groupBox_Sd_selft
			// 
			this->groupBox_Sd_selft->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox_Sd_selft->Controls->Add(this->numUpDwMinSelfG);
			this->groupBox_Sd_selft->Controls->Add(this->numUpDwMaxSelfTm);
			this->groupBox_Sd_selft->Controls->Add(this->labMinSelfG);
			this->groupBox_Sd_selft->Controls->Add(this->labMaxSelfTm);
			this->groupBox_Sd_selft->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_Sd_selft->Location = System::Drawing::Point(1, 128);
			this->groupBox_Sd_selft->Margin = System::Windows::Forms::Padding(1);
			this->groupBox_Sd_selft->Name = L"groupBox_Sd_selft";
			this->groupBox_Sd_selft->Size = System::Drawing::Size(237, 62);
			this->groupBox_Sd_selft->TabIndex = 5;
			this->groupBox_Sd_selft->TabStop = false;
			this->groupBox_Sd_selft->Text = L"Probe-selft ";
			this->toolTip->SetToolTip(this->groupBox_Sd_selft, L"Only sonde with weaksecundary structur will be \"include\"");
			// 
			// numUpDwMinSelfG
			// 
			this->numUpDwMinSelfG->DecimalPlaces = 2;
			this->numUpDwMinSelfG->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->numUpDwMinSelfG->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDwMinSelfG->Location = System::Drawing::Point(175, 11);
			this->numUpDwMinSelfG->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->numUpDwMinSelfG->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->numUpDwMinSelfG->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDwMinSelfG->Name = L"numUpDwMinSelfG";
			this->numUpDwMinSelfG->Size = System::Drawing::Size(52, 20);
			this->numUpDwMinSelfG->TabIndex = 4;
			this->numUpDwMinSelfG->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// numUpDwMaxSelfTm
			// 
			this->numUpDwMaxSelfTm->DecimalPlaces = 1;
			this->numUpDwMaxSelfTm->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numUpDwMaxSelfTm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDwMaxSelfTm->Location = System::Drawing::Point(175, 35);
			this->numUpDwMaxSelfTm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {70, 0, 0, 0});
			this->numUpDwMaxSelfTm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {273, 0, 0, System::Int32::MinValue});
			this->numUpDwMaxSelfTm->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDwMaxSelfTm->Name = L"numUpDwMaxSelfTm";
			this->numUpDwMaxSelfTm->Size = System::Drawing::Size(52, 20);
			this->numUpDwMaxSelfTm->TabIndex = 4;
			this->numUpDwMaxSelfTm->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// labMinSelfG
			// 
			this->labMinSelfG->AutoSize = true;
			this->labMinSelfG->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labMinSelfG->Location = System::Drawing::Point(115, 16);
			this->labMinSelfG->Name = L"labMinSelfG";
			this->labMinSelfG->Size = System::Drawing::Size(38, 15);
			this->labMinSelfG->TabIndex = 3;
			this->labMinSelfG->Text = L"Min G";
			// 
			// labMaxSelfTm
			// 
			this->labMaxSelfTm->AutoSize = true;
			this->labMaxSelfTm->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold));
			this->labMaxSelfTm->Location = System::Drawing::Point(109, 39);
			this->labMaxSelfTm->Name = L"labMaxSelfTm";
			this->labMaxSelfTm->Size = System::Drawing::Size(52, 15);
			this->labMaxSelfTm->TabIndex = 3;
			this->labMaxSelfTm->Text = L"Max Tm";
			this->toolTip->SetToolTip(this->labMaxSelfTm, L"Max. Tm for sonde secundary structure");
			// 
			// microArray
			// 
			this->microArray->Controls->Add(this->radioButton2);
			this->microArray->Controls->Add(this->radioButton1);
			this->microArray->Controls->Add(this->buttonRun_uArray);
			this->microArray->Controls->Add(this->but_uArrExp);
			this->microArray->Controls->Add(this->buttonSondeFile);
			this->microArray->Controls->Add(this->checkBox15);
			this->microArray->Controls->Add(this->checkBox14);
			this->microArray->Controls->Add(this->checkBox13);
			this->microArray->Controls->Add(this->checkBox12);
			this->microArray->Controls->Add(this->numericUpDown5);
			this->microArray->Controls->Add(this->numericUpDown4);
			this->microArray->Controls->Add(this->numericUpDown3);
			this->microArray->Controls->Add(this->numericUpDown2);
			this->microArray->Controls->Add(this->checkBox16);
			this->microArray->Controls->Add(this->label10);
			this->microArray->Controls->Add(this->checkBox11);
			this->microArray->Controls->Add(this->label7);
			this->microArray->Controls->Add(this->label9);
			this->microArray->Controls->Add(this->label5);
			this->microArray->Controls->Add(this->label6);
			this->microArray->Controls->Add(this->label4);
			this->microArray->Controls->Add(this->textBox2);
			this->microArray->Controls->Add(this->txtBoxExp);
			this->microArray->Controls->Add(this->textBoxSondesFile);
			this->microArray->Location = System::Drawing::Point(4, 22);
			this->microArray->Name = L"microArray";
			this->microArray->Padding = System::Windows::Forms::Padding(3);
			this->microArray->Size = System::Drawing::Size(548, 255);
			this->microArray->TabIndex = 1;
			this->microArray->Text = L"µArray";
			this->microArray->ToolTipText = L"An virtual microarray experiment - check all target aganist all sonde";
			this->microArray->UseVisualStyleBackColor = true;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Checked = true;
			this->radioButton2->Location = System::Drawing::Point(3, 230);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(14, 13);
			this->radioButton2->TabIndex = 17;
			this->radioButton2->TabStop = true;
			this->radioButton2->UseVisualStyleBackColor = true;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(3, 8);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(14, 13);
			this->radioButton1->TabIndex = 17;
			this->radioButton1->UseVisualStyleBackColor = true;
			// 
			// buttonRun_uArray
			// 
			this->buttonRun_uArray->Location = System::Drawing::Point(40, 177);
			this->buttonRun_uArray->Name = L"buttonRun_uArray";
			this->buttonRun_uArray->Size = System::Drawing::Size(70, 30);
			this->buttonRun_uArray->TabIndex = 12;
			this->buttonRun_uArray->Text = L"Hybrid !";
			this->buttonRun_uArray->UseVisualStyleBackColor = true;
			this->buttonRun_uArray->Click += gcnew System::EventHandler(this, &ThPr_Form::command_uArray);
			// 
			// but_uArrExp
			// 
			this->but_uArrExp->Location = System::Drawing::Point(21, 3);
			this->but_uArrExp->Name = L"but_uArrExp";
			this->but_uArrExp->Size = System::Drawing::Size(94, 23);
			this->but_uArrExp->TabIndex = 5;
			this->but_uArrExp->Text = L"Exp Data";
			this->but_uArrExp->UseVisualStyleBackColor = true;
			this->but_uArrExp->Click += gcnew System::EventHandler(this, &ThPr_Form::but_uArrExp_Click);
			// 
			// buttonSondeFile
			// 
			this->buttonSondeFile->Enabled = false;
			this->buttonSondeFile->Location = System::Drawing::Point(21, 225);
			this->buttonSondeFile->Name = L"buttonSondeFile";
			this->buttonSondeFile->Size = System::Drawing::Size(94, 23);
			this->buttonSondeFile->TabIndex = 5;
			this->buttonSondeFile->Text = L"Probes SecFile";
			this->buttonSondeFile->UseVisualStyleBackColor = true;
			this->buttonSondeFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandopenSondeFile);
			// 
			// checkBox15
			// 
			this->checkBox15->AutoSize = true;
			this->checkBox15->Checked = true;
			this->checkBox15->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox15->Enabled = false;
			this->checkBox15->Location = System::Drawing::Point(92, 209);
			this->checkBox15->Name = L"checkBox15";
			this->checkBox15->Size = System::Drawing::Size(34, 17);
			this->checkBox15->TabIndex = 15;
			this->checkBox15->Text = L"G";
			this->checkBox15->UseVisualStyleBackColor = true;
			// 
			// checkBox14
			// 
			this->checkBox14->AutoSize = true;
			this->checkBox14->Enabled = false;
			this->checkBox14->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox14->Location = System::Drawing::Point(47, 208);
			this->checkBox14->Name = L"checkBox14";
			this->checkBox14->Size = System::Drawing::Size(31, 18);
			this->checkBox14->TabIndex = 15;
			this->checkBox14->Text = L"I";
			this->checkBox14->UseVisualStyleBackColor = true;
			// 
			// checkBox13
			// 
			this->checkBox13->AutoSize = true;
			this->checkBox13->Enabled = false;
			this->checkBox13->Location = System::Drawing::Point(87, 51);
			this->checkBox13->Name = L"checkBox13";
			this->checkBox13->Size = System::Drawing::Size(72, 17);
			this->checkBox13->TabIndex = 15;
			this->checkBox13->Text = L"Normalize";
			this->checkBox13->UseVisualStyleBackColor = true;
			// 
			// checkBox12
			// 
			this->checkBox12->AutoSize = true;
			this->checkBox12->Enabled = false;
			this->checkBox12->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox12->Location = System::Drawing::Point(23, 66);
			this->checkBox12->Name = L"checkBox12";
			this->checkBox12->Size = System::Drawing::Size(62, 18);
			this->checkBox12->TabIndex = 15;
			this->checkBox12->Text = L"I x I - d";
			this->checkBox12->UseVisualStyleBackColor = true;
			// 
			// numericUpDown5
			// 
			this->numericUpDown5->DecimalPlaces = 1;
			this->numericUpDown5->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown5->Location = System::Drawing::Point(103, 151);
			this->numericUpDown5->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->numericUpDown5->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->numericUpDown5->MinimumSize = System::Drawing::Size(30, 0);
			this->numericUpDown5->Name = L"numericUpDown5";
			this->numericUpDown5->Size = System::Drawing::Size(43, 20);
			this->numericUpDown5->TabIndex = 4;
			this->numericUpDown5->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->DecimalPlaces = 1;
			this->numericUpDown4->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown4->Location = System::Drawing::Point(103, 105);
			this->numericUpDown4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->numericUpDown4->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
			this->numericUpDown4->MinimumSize = System::Drawing::Size(30, 0);
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(43, 20);
			this->numericUpDown4->TabIndex = 4;
			this->numericUpDown4->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, System::Int32::MinValue});
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->DecimalPlaces = 2;
			this->numericUpDown3->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numericUpDown3->Location = System::Drawing::Point(32, 151);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {87, 0, 0, 131072});
			this->numericUpDown3->MinimumSize = System::Drawing::Size(30, 0);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(43, 20);
			this->numericUpDown3->TabIndex = 4;
			this->numericUpDown3->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->DecimalPlaces = 2;
			this->numericUpDown2->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numericUpDown2->Location = System::Drawing::Point(32, 105);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown2->MinimumSize = System::Drawing::Size(30, 0);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(43, 20);
			this->numericUpDown2->TabIndex = 4;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {87, 0, 0, 131072});
			// 
			// checkBox16
			// 
			this->checkBox16->AutoSize = true;
			this->checkBox16->Enabled = false;
			this->checkBox16->Location = System::Drawing::Point(23, 31);
			this->checkBox16->Name = L"checkBox16";
			this->checkBox16->Size = System::Drawing::Size(63, 17);
			this->checkBox16->TabIndex = 15;
			this->checkBox16->Text = L"exclude";
			this->checkBox16->UseVisualStyleBackColor = true;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(88, 153);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(15, 13);
			this->label10->TabIndex = 3;
			this->label10->Text = L"G";
			// 
			// checkBox11
			// 
			this->checkBox11->AutoSize = true;
			this->checkBox11->Enabled = false;
			this->checkBox11->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox11->Location = System::Drawing::Point(23, 51);
			this->checkBox11->Name = L"checkBox11";
			this->checkBox11->Size = System::Drawing::Size(47, 18);
			this->checkBox11->TabIndex = 15;
			this->checkBox11->Text = L"I x I";
			this->checkBox11->UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(88, 107);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(15, 13);
			this->label7->TabIndex = 3;
			this->label7->Text = L"G";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(17, 153);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(12, 14);
			this->label9->TabIndex = 3;
			this->label9->Text = L"I";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(55, 138);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(54, 13);
			this->label5->TabIndex = 3;
			this->label5->Text = L"Sensitivity";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(17, 107);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(12, 14);
			this->label6->TabIndex = 3;
			this->label6->Text = L"I";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(55, 92);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(55, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Saturation";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(84, 28);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(35, 20);
			this->textBox2->TabIndex = 9;
			this->textBox2->Text = L"x";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// txtBoxExp
			// 
			this->txtBoxExp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBoxExp->Location = System::Drawing::Point(130, 5);
			this->txtBoxExp->Name = L"txtBoxExp";
			this->txtBoxExp->Size = System::Drawing::Size(411, 20);
			this->txtBoxExp->TabIndex = 9;
			this->txtBoxExp->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\exp\\";
			this->txtBoxExp->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBoxSondesFile
			// 
			this->textBoxSondesFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSondesFile->Enabled = false;
			this->textBoxSondesFile->Location = System::Drawing::Point(130, 227);
			this->textBoxSondesFile->Name = L"textBoxSondesFile";
			this->textBoxSondesFile->Size = System::Drawing::Size(411, 20);
			this->textBoxSondesFile->TabIndex = 9;
			this->textBoxSondesFile->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\sonden\\";
			this->textBoxSondesFile->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// tabPagPCRDesing
			// 
			this->tabPagPCRDesing->BackColor = System::Drawing::Color::LightGray;
			this->tabPagPCRDesing->Controls->Add(this->lab_please);
			this->tabPagPCRDesing->Controls->Add(this->numericUpDown_MaxProdLength);
			this->tabPagPCRDesing->Controls->Add(this->numericUpDown_MinProdLength);
			this->tabPagPCRDesing->Controls->Add(this->label_MinProdLength);
			this->tabPagPCRDesing->Controls->Add(this->label_MaxProdLength);
			this->tabPagPCRDesing->Location = System::Drawing::Point(4, 22);
			this->tabPagPCRDesing->Name = L"tabPagPCRDesing";
			this->tabPagPCRDesing->Padding = System::Windows::Forms::Padding(3);
			this->tabPagPCRDesing->Size = System::Drawing::Size(548, 255);
			this->tabPagPCRDesing->TabIndex = 2;
			this->tabPagPCRDesing->Text = L"PCR Design";
			// 
			// lab_please
			// 
			this->lab_please->AutoEllipsis = true;
			this->lab_please->AutoSize = true;
			this->lab_please->Location = System::Drawing::Point(27, 7);
			this->lab_please->Name = L"lab_please";
			this->lab_please->Size = System::Drawing::Size(223, 13);
			this->lab_please->TabIndex = 9;
			this->lab_please->Text = L"Please, first fill in \"Find probes\" parametr page";
			// 
			// numericUpDown_MaxProdLength
			// 
			this->numericUpDown_MaxProdLength->Enabled = false;
			this->numericUpDown_MaxProdLength->Location = System::Drawing::Point(360, 117);
			this->numericUpDown_MaxProdLength->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->numericUpDown_MaxProdLength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {70, 0, 0, 0});
			this->numericUpDown_MaxProdLength->Name = L"numericUpDown_MaxProdLength";
			this->numericUpDown_MaxProdLength->Size = System::Drawing::Size(43, 20);
			this->numericUpDown_MaxProdLength->TabIndex = 7;
			this->numericUpDown_MaxProdLength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {400, 0, 0, 0});
			// 
			// numericUpDown_MinProdLength
			// 
			this->numericUpDown_MinProdLength->Enabled = false;
			this->numericUpDown_MinProdLength->Location = System::Drawing::Point(173, 117);
			this->numericUpDown_MinProdLength->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->numericUpDown_MinProdLength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			this->numericUpDown_MinProdLength->Name = L"numericUpDown_MinProdLength";
			this->numericUpDown_MinProdLength->Size = System::Drawing::Size(43, 20);
			this->numericUpDown_MinProdLength->TabIndex = 8;
			this->numericUpDown_MinProdLength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {40, 0, 0, 0});
			// 
			// label_MinProdLength
			// 
			this->label_MinProdLength->AutoSize = true;
			this->label_MinProdLength->Enabled = false;
			this->label_MinProdLength->Location = System::Drawing::Point(67, 119);
			this->label_MinProdLength->Name = L"label_MinProdLength";
			this->label_MinProdLength->Size = System::Drawing::Size(100, 13);
			this->label_MinProdLength->TabIndex = 5;
			this->label_MinProdLength->Text = L"Min Product Length";
			// 
			// label_MaxProdLength
			// 
			this->label_MaxProdLength->AutoSize = true;
			this->label_MaxProdLength->Enabled = false;
			this->label_MaxProdLength->Location = System::Drawing::Point(251, 119);
			this->label_MaxProdLength->Name = L"label_MaxProdLength";
			this->label_MaxProdLength->Size = System::Drawing::Size(103, 13);
			this->label_MaxProdLength->TabIndex = 6;
			this->label_MaxProdLength->Text = L"Max Product Length";
			// 
			// tabPagMultiplexPCR
			// 
			this->tabPagMultiplexPCR->Controls->Add(this->checkBox17);
			this->tabPagMultiplexPCR->Controls->Add(this->checkBox18);
			this->tabPagMultiplexPCR->Controls->Add(this->buttPCR);
			this->tabPagMultiplexPCR->Controls->Add(this->butSdSecFilePCR);
			this->tabPagMultiplexPCR->Controls->Add(this->textBoxSdSecFilePCR);
			this->tabPagMultiplexPCR->Location = System::Drawing::Point(4, 22);
			this->tabPagMultiplexPCR->Name = L"tabPagMultiplexPCR";
			this->tabPagMultiplexPCR->Padding = System::Windows::Forms::Padding(3);
			this->tabPagMultiplexPCR->Size = System::Drawing::Size(548, 255);
			this->tabPagMultiplexPCR->TabIndex = 3;
			this->tabPagMultiplexPCR->Text = L"Multiplex PCR";
			this->tabPagMultiplexPCR->ToolTipText = L"Check all primers and complementary primers against selft and against all others " 
				L"primers and targets";
			this->tabPagMultiplexPCR->UseVisualStyleBackColor = true;
			// 
			// checkBox17
			// 
			this->checkBox17->AutoSize = true;
			this->checkBox17->Checked = true;
			this->checkBox17->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox17->Enabled = false;
			this->checkBox17->Location = System::Drawing::Point(86, 205);
			this->checkBox17->Name = L"checkBox17";
			this->checkBox17->Size = System::Drawing::Size(34, 17);
			this->checkBox17->TabIndex = 17;
			this->checkBox17->Text = L"G";
			this->checkBox17->UseVisualStyleBackColor = true;
			// 
			// checkBox18
			// 
			this->checkBox18->AutoSize = true;
			this->checkBox18->Enabled = false;
			this->checkBox18->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 8.249999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox18->Location = System::Drawing::Point(41, 204);
			this->checkBox18->Name = L"checkBox18";
			this->checkBox18->Size = System::Drawing::Size(31, 18);
			this->checkBox18->TabIndex = 16;
			this->checkBox18->Text = L"I";
			this->checkBox18->UseVisualStyleBackColor = true;
			// 
			// buttPCR
			// 
			this->buttPCR->Location = System::Drawing::Point(41, 228);
			this->buttPCR->Name = L"buttPCR";
			this->buttPCR->Size = System::Drawing::Size(75, 23);
			this->buttPCR->TabIndex = 15;
			this->buttPCR->Text = L"PCR !";
			this->buttPCR->UseVisualStyleBackColor = true;
			this->buttPCR->Click += gcnew System::EventHandler(this, &ThPr_Form::buttPCR_Click);
			// 
			// butSdSecFilePCR
			// 
			this->butSdSecFilePCR->Enabled = false;
			this->butSdSecFilePCR->Location = System::Drawing::Point(7, 42);
			this->butSdSecFilePCR->Name = L"butSdSecFilePCR";
			this->butSdSecFilePCR->Size = System::Drawing::Size(116, 23);
			this->butSdSecFilePCR->TabIndex = 13;
			this->butSdSecFilePCR->Text = L"Probe SecFile";
			this->butSdSecFilePCR->UseVisualStyleBackColor = true;
			this->butSdSecFilePCR->Click += gcnew System::EventHandler(this, &ThPr_Form::butSdSecFilePCR_Click);
			// 
			// textBoxSdSecFilePCR
			// 
			this->textBoxSdSecFilePCR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSdSecFilePCR->Enabled = false;
			this->textBoxSdSecFilePCR->Location = System::Drawing::Point(129, 44);
			this->textBoxSdSecFilePCR->Name = L"textBoxSdSecFilePCR";
			this->textBoxSdSecFilePCR->Size = System::Drawing::Size(412, 20);
			this->textBoxSdSecFilePCR->TabIndex = 14;
			this->textBoxSdSecFilePCR->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\sonden\\";
			this->textBoxSdSecFilePCR->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// tabPagTmCalc
			// 
			this->tabPagTmCalc->BackColor = System::Drawing::Color::White;
			this->tabPagTmCalc->Controls->Add(this->dGVw_TmCalcRes);
			this->tabPagTmCalc->Controls->Add(this->chkBx_align);
			this->tabPagTmCalc->Controls->Add(this->chkBx_Tm_save_asPCR);
			this->tabPagTmCalc->Controls->Add(this->but_Sec2AlignUpd);
			this->tabPagTmCalc->Controls->Add(this->but_SecUpd);
			this->tabPagTmCalc->Controls->Add(this->but_Sec_Sec2AlignUpd);
			this->tabPagTmCalc->Controls->Add(this->but_TmCalc);
			this->tabPagTmCalc->Controls->Add(this->chkBx_rev);
			this->tabPagTmCalc->Controls->Add(this->chkBx_compl);
			this->tabPagTmCalc->Controls->Add(this->txtBxErrorMsg);
			this->tabPagTmCalc->Controls->Add(this->txtBx_Sec2Align);
			this->tabPagTmCalc->Controls->Add(this->txtBx_ResultSec2Align);
			this->tabPagTmCalc->Controls->Add(this->txtBx_Sec);
			this->tabPagTmCalc->Controls->Add(this->txtBx_ResultSec);
			this->tabPagTmCalc->Location = System::Drawing::Point(4, 22);
			this->tabPagTmCalc->Name = L"tabPagTmCalc";
			this->tabPagTmCalc->Padding = System::Windows::Forms::Padding(3);
			this->tabPagTmCalc->Size = System::Drawing::Size(548, 255);
			this->tabPagTmCalc->TabIndex = 4;
			this->tabPagTmCalc->Text = L"Tm Calc";
			// 
			// dGVw_TmCalcRes
			// 
			this->dGVw_TmCalcRes->AllowUserToAddRows = false;
			this->dGVw_TmCalcRes->AllowUserToDeleteRows = false;
			this->dGVw_TmCalcRes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dGVw_TmCalcRes->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this->dGVw_TmCalcRes->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dGVw_TmCalcRes->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {this->Tm_min, 
				this->Tm, this->Tm_max, this->G_min, this->G, this->G_max});
			dataGridViewCellStyle8->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle8->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle8->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle8->Format = L"N1";
			dataGridViewCellStyle8->NullValue = nullptr;
			dataGridViewCellStyle8->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle8->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle8->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->dGVw_TmCalcRes->DefaultCellStyle = dataGridViewCellStyle8;
			this->dGVw_TmCalcRes->Location = System::Drawing::Point(94, 78);
			this->dGVw_TmCalcRes->Name = L"dGVw_TmCalcRes";
			this->dGVw_TmCalcRes->ReadOnly = true;
			dataGridViewCellStyle9->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle9->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle9->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle9->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle9->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle9->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dGVw_TmCalcRes->RowHeadersDefaultCellStyle = dataGridViewCellStyle9;
			this->dGVw_TmCalcRes->RowHeadersWidth = 80;
			this->dGVw_TmCalcRes->Size = System::Drawing::Size(434, 95);
			this->dGVw_TmCalcRes->TabIndex = 20;
			// 
			// Tm_min
			// 
			this->Tm_min->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
			this->Tm_min->DefaultCellStyle = dataGridViewCellStyle2;
			this->Tm_min->HeaderText = L"min-";
			this->Tm_min->Name = L"Tm_min";
			this->Tm_min->ReadOnly = true;
			// 
			// Tm
			// 
			this->Tm->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			this->Tm->DefaultCellStyle = dataGridViewCellStyle3;
			this->Tm->HeaderText = L"Tm(°C)";
			this->Tm->Name = L"Tm";
			this->Tm->ReadOnly = true;
			// 
			// Tm_max
			// 
			this->Tm_max->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			this->Tm_max->DefaultCellStyle = dataGridViewCellStyle4;
			this->Tm_max->HeaderText = L"-max";
			this->Tm_max->Name = L"Tm_max";
			this->Tm_max->ReadOnly = true;
			// 
			// G_min
			// 
			this->G_min->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
			this->G_min->DefaultCellStyle = dataGridViewCellStyle5;
			this->G_min->HeaderText = L"min-";
			this->G_min->Name = L"G_min";
			this->G_min->ReadOnly = true;
			// 
			// G
			// 
			this->G->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle6->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			this->G->DefaultCellStyle = dataGridViewCellStyle6;
			this->G->HeaderText = L"G( kJ)";
			this->G->Name = L"G";
			this->G->ReadOnly = true;
			// 
			// G_max
			// 
			this->G_max->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle7->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			this->G_max->DefaultCellStyle = dataGridViewCellStyle7;
			this->G_max->HeaderText = L"-max";
			this->G_max->Name = L"G_max";
			this->G_max->ReadOnly = true;
			// 
			// chkBx_align
			// 
			this->chkBx_align->AutoSize = true;
			this->chkBx_align->Checked = true;
			this->chkBx_align->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkBx_align->Location = System::Drawing::Point(9, 120);
			this->chkBx_align->Name = L"chkBx_align";
			this->chkBx_align->Size = System::Drawing::Size(48, 17);
			this->chkBx_align->TabIndex = 19;
			this->chkBx_align->Text = L"align";
			this->chkBx_align->UseVisualStyleBackColor = true;
			// 
			// chkBx_Tm_save_asPCR
			// 
			this->chkBx_Tm_save_asPCR->AutoSize = true;
			this->chkBx_Tm_save_asPCR->Location = System::Drawing::Point(9, 103);
			this->chkBx_Tm_save_asPCR->Name = L"chkBx_Tm_save_asPCR";
			this->chkBx_Tm_save_asPCR->Size = System::Drawing::Size(49, 17);
			this->chkBx_Tm_save_asPCR->TabIndex = 19;
			this->chkBx_Tm_save_asPCR->Text = L"save";
			this->chkBx_Tm_save_asPCR->UseVisualStyleBackColor = true;
			// 
			// but_Sec2AlignUpd
			// 
			this->but_Sec2AlignUpd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->but_Sec2AlignUpd->Location = System::Drawing::Point(502, 31);
			this->but_Sec2AlignUpd->Margin = System::Windows::Forms::Padding(0);
			this->but_Sec2AlignUpd->Name = L"but_Sec2AlignUpd";
			this->but_Sec2AlignUpd->Size = System::Drawing::Size(20, 21);
			this->but_Sec2AlignUpd->TabIndex = 18;
			this->but_Sec2AlignUpd->Text = L"copy";
			this->but_Sec2AlignUpd->UseVisualStyleBackColor = true;
			this->but_Sec2AlignUpd->Click += gcnew System::EventHandler(this, &ThPr_Form::but_Sec2AlignUpd_Click);
			// 
			// but_SecUpd
			// 
			this->but_SecUpd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->but_SecUpd->Location = System::Drawing::Point(502, 7);
			this->but_SecUpd->Margin = System::Windows::Forms::Padding(0);
			this->but_SecUpd->Name = L"but_SecUpd";
			this->but_SecUpd->Size = System::Drawing::Size(20, 21);
			this->but_SecUpd->TabIndex = 18;
			this->but_SecUpd->Text = L"copy";
			this->but_SecUpd->UseVisualStyleBackColor = true;
			this->but_SecUpd->Click += gcnew System::EventHandler(this, &ThPr_Form::but_SecUpd_Click);
			// 
			// but_Sec_Sec2AlignUpd
			// 
			this->but_Sec_Sec2AlignUpd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->but_Sec_Sec2AlignUpd->Location = System::Drawing::Point(482, 6);
			this->but_Sec_Sec2AlignUpd->Margin = System::Windows::Forms::Padding(0);
			this->but_Sec_Sec2AlignUpd->Name = L"but_Sec_Sec2AlignUpd";
			this->but_Sec_Sec2AlignUpd->Size = System::Drawing::Size(18, 47);
			this->but_Sec_Sec2AlignUpd->TabIndex = 18;
			this->but_Sec_Sec2AlignUpd->Text = L"copy";
			this->but_Sec_Sec2AlignUpd->UseVisualStyleBackColor = true;
			this->but_Sec_Sec2AlignUpd->Click += gcnew System::EventHandler(this, &ThPr_Form::txtBx_Sec_TextChanged);
			// 
			// but_TmCalc
			// 
			this->but_TmCalc->Location = System::Drawing::Point(13, 74);
			this->but_TmCalc->Name = L"but_TmCalc";
			this->but_TmCalc->Size = System::Drawing::Size(75, 23);
			this->but_TmCalc->TabIndex = 18;
			this->but_TmCalc->Text = L"Tm !";
			this->but_TmCalc->UseVisualStyleBackColor = true;
			this->but_TmCalc->Click += gcnew System::EventHandler(this, &ThPr_Form::but_TmCalc_Click);
			// 
			// chkBx_rev
			// 
			this->chkBx_rev->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->chkBx_rev->AutoSize = true;
			this->chkBx_rev->Location = System::Drawing::Point(443, 55);
			this->chkBx_rev->Name = L"chkBx_rev";
			this->chkBx_rev->Size = System::Drawing::Size(41, 17);
			this->chkBx_rev->TabIndex = 17;
			this->chkBx_rev->Text = L"rev";
			this->chkBx_rev->UseVisualStyleBackColor = true;
			// 
			// chkBx_compl
			// 
			this->chkBx_compl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->chkBx_compl->AutoSize = true;
			this->chkBx_compl->Location = System::Drawing::Point(484, 55);
			this->chkBx_compl->Name = L"chkBx_compl";
			this->chkBx_compl->Size = System::Drawing::Size(40, 17);
			this->chkBx_compl->TabIndex = 16;
			this->chkBx_compl->Text = L"cpl";
			this->chkBx_compl->UseVisualStyleBackColor = true;
			// 
			// txtBxErrorMsg
			// 
			this->txtBxErrorMsg->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtBxErrorMsg->Font = (gcnew System::Drawing::Font(L"Lucida Sans Unicode", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtBxErrorMsg->ForeColor = System::Drawing::Color::Red;
			this->txtBxErrorMsg->Location = System::Drawing::Point(26, 55);
			this->txtBxErrorMsg->MaxLength = 1000;
			this->txtBxErrorMsg->Name = L"txtBxErrorMsg";
			this->txtBxErrorMsg->Size = System::Drawing::Size(351, 17);
			this->txtBxErrorMsg->TabIndex = 1;
			this->txtBxErrorMsg->Text = L"no error";
			// 
			// txtBx_Sec2Align
			// 
			this->txtBx_Sec2Align->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBx_Sec2Align->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtBx_Sec2Align->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtBx_Sec2Align->Location = System::Drawing::Point(13, 32);
			this->txtBx_Sec2Align->MaxLength = 1000;
			this->txtBx_Sec2Align->Name = L"txtBx_Sec2Align";
			this->txtBx_Sec2Align->Size = System::Drawing::Size(466, 21);
			this->txtBx_Sec2Align->TabIndex = 1;
			// 
			// txtBx_ResultSec2Align
			// 
			this->txtBx_ResultSec2Align->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBx_ResultSec2Align->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtBx_ResultSec2Align->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->txtBx_ResultSec2Align->Location = System::Drawing::Point(13, 222);
			this->txtBx_ResultSec2Align->MaxLength = 1000;
			this->txtBx_ResultSec2Align->Name = L"txtBx_ResultSec2Align";
			this->txtBx_ResultSec2Align->ReadOnly = true;
			this->txtBx_ResultSec2Align->Size = System::Drawing::Size(528, 21);
			this->txtBx_ResultSec2Align->TabIndex = 1;
			// 
			// txtBx_Sec
			// 
			this->txtBx_Sec->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBx_Sec->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtBx_Sec->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtBx_Sec->Location = System::Drawing::Point(13, 6);
			this->txtBx_Sec->MaxLength = 1000;
			this->txtBx_Sec->Name = L"txtBx_Sec";
			this->txtBx_Sec->Size = System::Drawing::Size(466, 21);
			this->txtBx_Sec->TabIndex = 1;
			this->txtBx_Sec->Tag = L"";
			// 
			// txtBx_ResultSec
			// 
			this->txtBx_ResultSec->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBx_ResultSec->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtBx_ResultSec->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtBx_ResultSec->Location = System::Drawing::Point(13, 196);
			this->txtBx_ResultSec->MaxLength = 1000;
			this->txtBx_ResultSec->Name = L"txtBx_ResultSec";
			this->txtBx_ResultSec->ReadOnly = true;
			this->txtBx_ResultSec->Size = System::Drawing::Size(529, 21);
			this->txtBx_ResultSec->TabIndex = 1;
			// 
			// tabPagSetup
			// 
			this->tabPagSetup->BackColor = System::Drawing::Color::White;
			this->tabPagSetup->Controls->Add(this->ckBx_savNNParam);
			this->tabPagSetup->Controls->Add(this->numUpDwMxGrDeg);
			this->tabPagSetup->Controls->Add(this->labMxGrDeg);
			this->tabPagSetup->Controls->Add(this->butNoProcess);
			this->tabPagSetup->Controls->Add(this->ckBx_loadNNParam);
			this->tabPagSetup->Controls->Add(this->butCreateDir);
			this->tabPagSetup->Controls->Add(this->textBoxNNParamFile);
			this->tabPagSetup->Controls->Add(this->butRestDefPr);
			this->tabPagSetup->Controls->Add(this->butSetDefPr);
			this->tabPagSetup->Controls->Add(this->butNNParamFile);
			this->tabPagSetup->Controls->Add(this->groupBox2);
			this->tabPagSetup->Location = System::Drawing::Point(4, 22);
			this->tabPagSetup->Name = L"tabPagSetup";
			this->tabPagSetup->Padding = System::Windows::Forms::Padding(3);
			this->tabPagSetup->Size = System::Drawing::Size(548, 255);
			this->tabPagSetup->TabIndex = 5;
			this->tabPagSetup->Text = L"Setup";
			// 
			// ckBx_savNNParam
			// 
			this->ckBx_savNNParam->AutoSize = true;
			this->ckBx_savNNParam->Location = System::Drawing::Point(474, 237);
			this->ckBx_savNNParam->Name = L"ckBx_savNNParam";
			this->ckBx_savNNParam->Size = System::Drawing::Size(49, 17);
			this->ckBx_savNNParam->TabIndex = 15;
			this->ckBx_savNNParam->Text = L"save";
			this->toolTip->SetToolTip(this->ckBx_savNNParam, L"Save used NN paramter table (*.ThDyParam.csv)");
			this->ckBx_savNNParam->UseVisualStyleBackColor = true;
			// 
			// numUpDwMxGrDeg
			// 
			this->numUpDwMxGrDeg->Enabled = false;
			this->numUpDwMxGrDeg->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->numUpDwMxGrDeg->Location = System::Drawing::Point(89, 150);
			this->numUpDwMxGrDeg->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numUpDwMxGrDeg->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numUpDwMxGrDeg->Name = L"numUpDwMxGrDeg";
			this->numUpDwMxGrDeg->Size = System::Drawing::Size(52, 20);
			this->numUpDwMxGrDeg->TabIndex = 12;
			this->numUpDwMxGrDeg->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->numUpDwMxGrDeg->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {300, 0, 0, 0});
			// 
			// labMxGrDeg
			// 
			this->labMxGrDeg->AutoSize = true;
			this->labMxGrDeg->Enabled = false;
			this->labMxGrDeg->Location = System::Drawing::Point(23, 152);
			this->labMxGrDeg->Name = L"labMxGrDeg";
			this->labMxGrDeg->Size = System::Drawing::Size(66, 13);
			this->labMxGrDeg->TabIndex = 9;
			this->labMxGrDeg->Text = L"Max. gr. deg";
			// 
			// butNoProcess
			// 
			this->butNoProcess->Enabled = false;
			this->butNoProcess->Location = System::Drawing::Point(18, 108);
			this->butNoProcess->Name = L"butNoProcess";
			this->butNoProcess->Size = System::Drawing::Size(123, 23);
			this->butNoProcess->TabIndex = 5;
			this->butNoProcess->Text = L"# de proces";
			this->butNoProcess->UseVisualStyleBackColor = true;
			this->butNoProcess->Click += gcnew System::EventHandler(this, &ThPr_Form::commandSavePrFile);
			// 
			// ckBx_loadNNParam
			// 
			this->ckBx_loadNNParam->AutoSize = true;
			this->ckBx_loadNNParam->Location = System::Drawing::Point(474, 221);
			this->ckBx_loadNNParam->Name = L"ckBx_loadNNParam";
			this->ckBx_loadNNParam->Size = System::Drawing::Size(46, 17);
			this->ckBx_loadNNParam->TabIndex = 15;
			this->ckBx_loadNNParam->Text = L"load";
			this->toolTip->SetToolTip(this->ckBx_loadNNParam, L"Load NN paramter table (*.ThDyParam.csv)");
			this->ckBx_loadNNParam->UseVisualStyleBackColor = true;
			// 
			// butCreateDir
			// 
			this->butCreateDir->Enabled = false;
			this->butCreateDir->Location = System::Drawing::Point(18, 79);
			this->butCreateDir->Name = L"butCreateDir";
			this->butCreateDir->Size = System::Drawing::Size(123, 23);
			this->butCreateDir->TabIndex = 5;
			this->butCreateDir->Text = L"Create dir.";
			this->butCreateDir->UseVisualStyleBackColor = true;
			this->butCreateDir->Click += gcnew System::EventHandler(this, &ThPr_Form::commandSavePrFile);
			// 
			// textBoxNNParamFile
			// 
			this->textBoxNNParamFile->Location = System::Drawing::Point(75, 229);
			this->textBoxNNParamFile->Name = L"textBoxNNParamFile";
			this->textBoxNNParamFile->Size = System::Drawing::Size(387, 20);
			this->textBoxNNParamFile->TabIndex = 14;
			this->textBoxNNParamFile->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// butRestDefPr
			// 
			this->butRestDefPr->Location = System::Drawing::Point(18, 50);
			this->butRestDefPr->Name = L"butRestDefPr";
			this->butRestDefPr->Size = System::Drawing::Size(123, 23);
			this->butRestDefPr->TabIndex = 5;
			this->butRestDefPr->Text = L"Restore Def. project";
			this->butRestDefPr->UseVisualStyleBackColor = true;
			this->butRestDefPr->Click += gcnew System::EventHandler(this, &ThPr_Form::commandRestDefPr);
			// 
			// butSetDefPr
			// 
			this->butSetDefPr->Location = System::Drawing::Point(18, 21);
			this->butSetDefPr->Name = L"butSetDefPr";
			this->butSetDefPr->Size = System::Drawing::Size(123, 23);
			this->butSetDefPr->TabIndex = 5;
			this->butSetDefPr->Text = L"Set as Def. project";
			this->butSetDefPr->UseVisualStyleBackColor = true;
			this->butSetDefPr->Click += gcnew System::EventHandler(this, &ThPr_Form::commandSetDefPr);
			// 
			// butNNParamFile
			// 
			this->butNNParamFile->Location = System::Drawing::Point(7, 227);
			this->butNNParamFile->Name = L"butNNParamFile";
			this->butNNParamFile->Size = System::Drawing::Size(67, 23);
			this->butNNParamFile->TabIndex = 5;
			this->butNNParamFile->Text = L"NN param.";
			this->butNNParamFile->UseVisualStyleBackColor = true;
			this->butNNParamFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandLoadNNParamFile);
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::Transparent;
			this->groupBox2->Controls->Add(this->ckBx_savExportTarg);
			this->groupBox2->Controls->Add(this->ckBx_savExportSond);
			this->groupBox2->Controls->Add(this->ckBx_savTm);
			this->groupBox2->Controls->Add(this->ckBx_savPos);
			this->groupBox2->Controls->Add(this->ckBx_savG);
			this->groupBox2->Controls->Add(this->ckBx_savG_Plasm);
			this->groupBox2->Controls->Add(this->ckBx_savTm_Plasm);
			this->groupBox2->Controls->Add(this->ckBx_savAlign);
			this->groupBox2->Controls->Add(this->ckBx_savProj);
			this->groupBox2->Controls->Add(this->ckBx_savLog);
			this->groupBox2->Location = System::Drawing::Point(446, 7);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(79, 207);
			this->groupBox2->TabIndex = 16;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"save result";
			// 
			// ckBx_savExportTarg
			// 
			this->ckBx_savExportTarg->AutoSize = true;
			this->ckBx_savExportTarg->Enabled = false;
			this->ckBx_savExportTarg->Location = System::Drawing::Point(7, 187);
			this->ckBx_savExportTarg->Name = L"ckBx_savExportTarg";
			this->ckBx_savExportTarg->Size = System::Drawing::Size(65, 17);
			this->ckBx_savExportTarg->TabIndex = 15;
			this->ckBx_savExportTarg->Text = L"Exp targ";
			this->ckBx_savExportTarg->UseVisualStyleBackColor = true;
			// 
			// ckBx_savExportSond
			// 
			this->ckBx_savExportSond->AutoSize = true;
			this->ckBx_savExportSond->Enabled = false;
			this->ckBx_savExportSond->Location = System::Drawing::Point(7, 168);
			this->ckBx_savExportSond->Name = L"ckBx_savExportSond";
			this->ckBx_savExportSond->Size = System::Drawing::Size(70, 17);
			this->ckBx_savExportSond->TabIndex = 15;
			this->ckBx_savExportSond->Text = L"Exp sond";
			this->ckBx_savExportSond->UseVisualStyleBackColor = true;
			// 
			// ckBx_savTm
			// 
			this->ckBx_savTm->AutoSize = true;
			this->ckBx_savTm->Checked = true;
			this->ckBx_savTm->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savTm->Location = System::Drawing::Point(7, 16);
			this->ckBx_savTm->Name = L"ckBx_savTm";
			this->ckBx_savTm->Size = System::Drawing::Size(41, 17);
			this->ckBx_savTm->TabIndex = 15;
			this->ckBx_savTm->Text = L"Tm";
			this->toolTip->SetToolTip(this->ckBx_savTm, L"Save Tm table (*.Tm.csv)");
			this->ckBx_savTm->UseVisualStyleBackColor = true;
			// 
			// ckBx_savPos
			// 
			this->ckBx_savPos->AutoSize = true;
			this->ckBx_savPos->Checked = true;
			this->ckBx_savPos->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savPos->Location = System::Drawing::Point(7, 35);
			this->ckBx_savPos->Name = L"ckBx_savPos";
			this->ckBx_savPos->Size = System::Drawing::Size(44, 17);
			this->ckBx_savPos->TabIndex = 15;
			this->ckBx_savPos->Text = L"Pos";
			this->toolTip->SetToolTip(this->ckBx_savPos, L"Save position table (*.Pos.csv)");
			this->ckBx_savPos->UseVisualStyleBackColor = true;
			// 
			// ckBx_savG
			// 
			this->ckBx_savG->AutoSize = true;
			this->ckBx_savG->Checked = true;
			this->ckBx_savG->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savG->Location = System::Drawing::Point(7, 54);
			this->ckBx_savG->Name = L"ckBx_savG";
			this->ckBx_savG->Size = System::Drawing::Size(34, 17);
			this->ckBx_savG->TabIndex = 15;
			this->ckBx_savG->Text = L"G";
			this->toolTip->SetToolTip(this->ckBx_savG, L"Save G table (*.G.csv)");
			this->ckBx_savG->UseVisualStyleBackColor = true;
			// 
			// ckBx_savG_Plasm
			// 
			this->ckBx_savG_Plasm->AutoSize = true;
			this->ckBx_savG_Plasm->Checked = true;
			this->ckBx_savG_Plasm->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savG_Plasm->Location = System::Drawing::Point(7, 111);
			this->ckBx_savG_Plasm->Name = L"ckBx_savG_Plasm";
			this->ckBx_savG_Plasm->Size = System::Drawing::Size(65, 17);
			this->ckBx_savG_Plasm->TabIndex = 15;
			this->ckBx_savG_Plasm->Text = L"G-Plasm";
			this->ckBx_savG_Plasm->UseVisualStyleBackColor = true;
			// 
			// ckBx_savTm_Plasm
			// 
			this->ckBx_savTm_Plasm->AutoSize = true;
			this->ckBx_savTm_Plasm->Checked = true;
			this->ckBx_savTm_Plasm->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savTm_Plasm->Location = System::Drawing::Point(7, 130);
			this->ckBx_savTm_Plasm->Name = L"ckBx_savTm_Plasm";
			this->ckBx_savTm_Plasm->Size = System::Drawing::Size(72, 17);
			this->ckBx_savTm_Plasm->TabIndex = 15;
			this->ckBx_savTm_Plasm->Text = L"Tm-Plasm";
			this->ckBx_savTm_Plasm->UseVisualStyleBackColor = true;
			// 
			// ckBx_savAlign
			// 
			this->ckBx_savAlign->AutoSize = true;
			this->ckBx_savAlign->Checked = true;
			this->ckBx_savAlign->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savAlign->Location = System::Drawing::Point(7, 73);
			this->ckBx_savAlign->Name = L"ckBx_savAlign";
			this->ckBx_savAlign->Size = System::Drawing::Size(49, 17);
			this->ckBx_savAlign->TabIndex = 15;
			this->ckBx_savAlign->Text = L"Align";
			this->toolTip->SetToolTip(this->ckBx_savAlign, L"Save Align table (*.Al.csv)");
			this->ckBx_savAlign->UseVisualStyleBackColor = true;
			// 
			// ckBx_savProj
			// 
			this->ckBx_savProj->AutoSize = true;
			this->ckBx_savProj->Checked = true;
			this->ckBx_savProj->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckBx_savProj->Location = System::Drawing::Point(7, 92);
			this->ckBx_savProj->Name = L"ckBx_savProj";
			this->ckBx_savProj->Size = System::Drawing::Size(44, 17);
			this->ckBx_savProj->TabIndex = 15;
			this->ckBx_savProj->Text = L"Proj";
			this->toolTip->SetToolTip(this->ckBx_savProj, L"Save program Project file (*.ThDy.txt)");
			this->ckBx_savProj->UseVisualStyleBackColor = true;
			// 
			// ckBx_savLog
			// 
			this->ckBx_savLog->AutoSize = true;
			this->ckBx_savLog->Enabled = false;
			this->ckBx_savLog->Location = System::Drawing::Point(7, 149);
			this->ckBx_savLog->Name = L"ckBx_savLog";
			this->ckBx_savLog->Size = System::Drawing::Size(40, 17);
			this->ckBx_savLog->TabIndex = 15;
			this->ckBx_savLog->Text = L"log";
			this->ckBx_savLog->UseVisualStyleBackColor = true;
			// 
			// buttonTargetsFile
			// 
			this->buttonTargetsFile->Location = System::Drawing::Point(4, 319);
			this->buttonTargetsFile->Name = L"buttonTargetsFile";
			this->buttonTargetsFile->Size = System::Drawing::Size(89, 35);
			this->buttonTargetsFile->TabIndex = 7;
			this->buttonTargetsFile->Text = L"TargetsSecFile (Proben)";
			this->buttonTargetsFile->UseVisualStyleBackColor = true;
			this->buttonTargetsFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandOpenTargetsSecFile4uA);
			// 
			// buttonSaveResultFile
			// 
			this->buttonSaveResultFile->Location = System::Drawing::Point(6, 465);
			this->buttonSaveResultFile->Name = L"buttonSaveResultFile";
			this->buttonSaveResultFile->Size = System::Drawing::Size(116, 23);
			this->buttonSaveResultFile->TabIndex = 5;
			this->buttonSaveResultFile->Text = L"Save Result Files";
			this->buttonSaveResultFile->UseVisualStyleBackColor = true;
			this->buttonSaveResultFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandSaveResultFile);
			// 
			// textBoxSaveResultFile
			// 
			this->textBoxSaveResultFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSaveResultFile->Location = System::Drawing::Point(128, 467);
			this->textBoxSaveResultFile->Name = L"textBoxSaveResultFile";
			this->textBoxSaveResultFile->Size = System::Drawing::Size(425, 20);
			this->textBoxSaveResultFile->TabIndex = 9;
			this->textBoxSaveResultFile->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\results\\";
			this->textBoxSaveResultFile->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBoxTgScF4uA
			// 
			this->textBoxTgScF4uA->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxTgScF4uA->Location = System::Drawing::Point(84, 13);
			this->textBoxTgScF4uA->Name = L"textBoxTgScF4uA";
			this->textBoxTgScF4uA->Size = System::Drawing::Size(449, 20);
			this->textBoxTgScF4uA->TabIndex = 8;
			this->textBoxTgScF4uA->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\targets\\";
			this->textBoxTgScF4uA->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// openSondesFileDialog
			// 
			this->openSondesFileDialog->DefaultExt = L"txt";
			this->openSondesFileDialog->Filter = L"fasta|*.fas;*.fasta|Text|*.txt|All file|*.*";
			this->openSondesFileDialog->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\sonden";
			this->openSondesFileDialog->Title = L"Open Sondes File";
			// 
			// SaveResultFileDialog
			// 
			this->SaveResultFileDialog->AddExtension = false;
			this->SaveResultFileDialog->Filter = L"All file|*.*|Excel file|*.csv";
			this->SaveResultFileDialog->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\results";
			this->SaveResultFileDialog->Title = L"Save Result File";
			this->SaveResultFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ThPr_Form::LoadPrFileDialog_FileOk);
			// 
			// numUpDowSdConc
			// 
			this->numUpDowSdConc->DecimalPlaces = 4;
			this->numUpDowSdConc->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDowSdConc->Location = System::Drawing::Point(128, 493);
			this->numUpDowSdConc->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numUpDowSdConc->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 262144});
			this->numUpDowSdConc->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDowSdConc->Name = L"numUpDowSdConc";
			this->numUpDowSdConc->Size = System::Drawing::Size(78, 20);
			this->numUpDowSdConc->TabIndex = 4;
			this->numUpDowSdConc->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->numUpDowSdConc->ThousandsSeparator = true;
			this->numUpDowSdConc->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// labSdConc
			// 
			this->labSdConc->AutoSize = true;
			this->labSdConc->Location = System::Drawing::Point(17, 495);
			this->labSdConc->Name = L"labSdConc";
			this->labSdConc->Size = System::Drawing::Size(108, 13);
			this->labSdConc->TabIndex = 3;
			this->labSdConc->Text = L"Probe Concentr. (nM)";
			// 
			// numUpDowTgConc
			// 
			this->numUpDowTgConc->DecimalPlaces = 4;
			this->numUpDowTgConc->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDowTgConc->Location = System::Drawing::Point(128, 518);
			this->numUpDowTgConc->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numUpDowTgConc->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 262144});
			this->numUpDowTgConc->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDowTgConc->Name = L"numUpDowTgConc";
			this->numUpDowTgConc->Size = System::Drawing::Size(78, 20);
			this->numUpDowTgConc->TabIndex = 4;
			this->numUpDowTgConc->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->numUpDowTgConc->ThousandsSeparator = true;
			this->numUpDowTgConc->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// labTgConc
			// 
			this->labTgConc->AutoSize = true;
			this->labTgConc->Location = System::Drawing::Point(17, 520);
			this->labTgConc->Name = L"labTgConc";
			this->labTgConc->Size = System::Drawing::Size(111, 13);
			this->labTgConc->TabIndex = 3;
			this->labTgConc->Text = L"Target Concentr. (nM)";
			// 
			// numUpDowTa
			// 
			this->numUpDowTa->DecimalPlaces = 2;
			this->numUpDowTa->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDowTa->Location = System::Drawing::Point(128, 577);
			this->numUpDowTa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->numUpDowTa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {27315, 0, 0, -2147352576});
			this->numUpDowTa->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDowTa->Name = L"numUpDowTa";
			this->numUpDowTa->Size = System::Drawing::Size(66, 20);
			this->numUpDowTa->TabIndex = 4;
			this->numUpDowTa->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->toolTip->SetToolTip(this->numUpDowTa, L"Temp. to calculate G");
			this->numUpDowTa->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {55, 0, 0, 0});
			// 
			// labTa
			// 
			this->labTa->AutoSize = true;
			this->labTa->Location = System::Drawing::Point(20, 579);
			this->labTa->Name = L"labTa";
			this->labTa->Size = System::Drawing::Size(104, 13);
			this->labTa->TabIndex = 3;
			this->labTa->Text = L" Temp. Anneling (°C)";
			this->toolTip->SetToolTip(this->labTa, L"Temp. to calculate G");
			// 
			// numUpDowSalConc
			// 
			this->numUpDowSalConc->DecimalPlaces = 4;
			this->numUpDowSalConc->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numUpDowSalConc->Location = System::Drawing::Point(345, 493);
			this->numUpDowSalConc->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numUpDowSalConc->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 262144});
			this->numUpDowSalConc->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDowSalConc->Name = L"numUpDowSalConc";
			this->numUpDowSalConc->Size = System::Drawing::Size(78, 20);
			this->numUpDowSalConc->TabIndex = 4;
			this->numUpDowSalConc->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->numUpDowSalConc->ThousandsSeparator = true;
			this->numUpDowSalConc->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// labSalConc
			// 
			this->labSalConc->AutoSize = true;
			this->labSalConc->Location = System::Drawing::Point(222, 495);
			this->labSalConc->Name = L"labSalConc";
			this->labSalConc->Size = System::Drawing::Size(117, 13);
			this->labSalConc->TabIndex = 3;
			this->labSalConc->Text = L"Salt Conc.(cations, µM)";
			// 
			// comBoxSalMeth
			// 
			this->comBoxSalMeth->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comBoxSalMeth->Enabled = false;
			this->comBoxSalMeth->FormattingEnabled = true;
			this->comBoxSalMeth->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Santa Lucia", L"Owczarzy"});
			this->comBoxSalMeth->Location = System::Drawing::Point(345, 517);
			this->comBoxSalMeth->Name = L"comBoxSalMeth";
			this->comBoxSalMeth->Size = System::Drawing::Size(96, 21);
			this->comBoxSalMeth->TabIndex = 7;
			// 
			// labSalMeth
			// 
			this->labSalMeth->AutoSize = true;
			this->labSalMeth->Enabled = false;
			this->labSalMeth->Location = System::Drawing::Point(235, 520);
			this->labSalMeth->Name = L"labSalMeth";
			this->labSalMeth->Size = System::Drawing::Size(104, 13);
			this->labSalMeth->TabIndex = 3;
			this->labSalMeth->Text = L"Salt Correct. Method";
			// 
			// richTextBox1
			// 
			this->richTextBox1->BackColor = System::Drawing::SystemColors::Info;
			this->richTextBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextBox1->Cursor = System::Windows::Forms::Cursors::No;
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Gisha", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBox1->Location = System::Drawing::Point(343, 619);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->richTextBox1->Size = System::Drawing::Size(203, 16);
			this->richTextBox1->TabIndex = 10;
			this->richTextBox1->Text = L"     ArielVina.Rodriguez@fli.bund.de  ";
			// 
			// richTextBox2
			// 
			this->richTextBox2->BackColor = System::Drawing::SystemColors::Info;
			this->richTextBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextBox2->Cursor = System::Windows::Forms::Cursors::No;
			this->richTextBox2->Font = (gcnew System::Drawing::Font(L"Lucida Handwriting", 11, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBox2->Location = System::Drawing::Point(343, 597);
			this->richTextBox2->Multiline = false;
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->richTextBox2->Size = System::Drawing::Size(203, 18);
			this->richTextBox2->TabIndex = 10;
			this->richTextBox2->Text = L"        INNT - FLI .    ";
			// 
			// labTAMeth
			// 
			this->labTAMeth->AutoSize = true;
			this->labTAMeth->Location = System::Drawing::Point(235, 541);
			this->labTAMeth->Name = L"labTAMeth";
			this->labTAMeth->Size = System::Drawing::Size(101, 13);
			this->labTAMeth->TabIndex = 3;
			this->labTAMeth->Text = L"ThDy Align. Method";
			// 
			// comBoxTAMeth
			// 
			this->comBoxTAMeth->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comBoxTAMeth->FormattingEnabled = true;
			this->comBoxTAMeth->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Tm", L"G", L"Fract"});
			this->comBoxTAMeth->Location = System::Drawing::Point(345, 538);
			this->comBoxTAMeth->Name = L"comBoxTAMeth";
			this->comBoxTAMeth->Size = System::Drawing::Size(96, 21);
			this->comBoxTAMeth->TabIndex = 7;
			this->toolTip->SetToolTip(this->comBoxTAMeth, L"Method used to Align");
			// 
			// toolTip
			// 
			this->toolTip->IsBalloon = true;
			// 
			// labMaxTgId
			// 
			this->labMaxTgId->AutoSize = true;
			this->labMaxTgId->Location = System::Drawing::Point(1, 40);
			this->labMaxTgId->Name = L"labMaxTgId";
			this->labMaxTgId->Size = System::Drawing::Size(67, 13);
			this->labMaxTgId->TabIndex = 3;
			this->labMaxTgId->Text = L"Max. ident,%";
			this->toolTip->SetToolTip(this->labMaxTgId, L"Very simple filter for max identity");
			// 
			// numUpDwMaxTgId
			// 
			this->numUpDwMaxTgId->DecimalPlaces = 1;
			this->numUpDwMaxTgId->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->numUpDwMaxTgId->Location = System::Drawing::Point(67, 38);
			this->numUpDwMaxTgId->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {80, 0, 0, 0});
			this->numUpDwMaxTgId->MinimumSize = System::Drawing::Size(30, 0);
			this->numUpDwMaxTgId->Name = L"numUpDwMaxTgId";
			this->numUpDwMaxTgId->Size = System::Drawing::Size(43, 20);
			this->numUpDwMaxTgId->TabIndex = 4;
			this->toolTip->SetToolTip(this->numUpDwMaxTgId, L"Very simple filter for max identity");
			this->numUpDwMaxTgId->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			// 
			// labTgEnd
			// 
			this->labTgEnd->AutoSize = true;
			this->labTgEnd->Location = System::Drawing::Point(237, 40);
			this->labTgEnd->Name = L"labTgEnd";
			this->labTgEnd->Size = System::Drawing::Size(26, 13);
			this->labTgEnd->TabIndex = 9;
			this->labTgEnd->Text = L"End";
			this->toolTip->SetToolTip(this->labTgEnd, L"Set to 0 for full seq.");
			// 
			// numUpDw_TgEnd
			// 
			this->numUpDw_TgEnd->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->numUpDw_TgEnd->Location = System::Drawing::Point(262, 38);
			this->numUpDw_TgEnd->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->numUpDw_TgEnd->Name = L"numUpDw_TgEnd";
			this->numUpDw_TgEnd->Size = System::Drawing::Size(52, 20);
			this->numUpDw_TgEnd->TabIndex = 12;
			this->numUpDw_TgEnd->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->toolTip->SetToolTip(this->numUpDw_TgEnd, L"Set to 0 for full seq.");
			// 
			// lab_MinLen
			// 
			this->lab_MinLen->AutoSize = true;
			this->lab_MinLen->Location = System::Drawing::Point(423, 40);
			this->lab_MinLen->Name = L"lab_MinLen";
			this->lab_MinLen->Size = System::Drawing::Size(45, 13);
			this->lab_MinLen->TabIndex = 9;
			this->lab_MinLen->Text = L"Min.Len";
			this->toolTip->SetToolTip(this->lab_MinLen, L"Filter for min. seq length. \r\nSet to 0 for no limit.");
			// 
			// numUpDw_MinLen
			// 
			this->numUpDw_MinLen->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->numUpDw_MinLen->Location = System::Drawing::Point(466, 38);
			this->numUpDw_MinLen->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->numUpDw_MinLen->Name = L"numUpDw_MinLen";
			this->numUpDw_MinLen->Size = System::Drawing::Size(52, 20);
			this->numUpDw_MinLen->TabIndex = 12;
			this->numUpDw_MinLen->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->toolTip->SetToolTip(this->numUpDw_MinLen, L"Filter for min. seq length. \r\nSet to 0 for no limit.");
			// 
			// numUpDw_TgBeg
			// 
			this->numUpDw_TgBeg->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->numUpDw_TgBeg->Location = System::Drawing::Point(184, 38);
			this->numUpDw_TgBeg->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->numUpDw_TgBeg->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numUpDw_TgBeg->Name = L"numUpDw_TgBeg";
			this->numUpDw_TgBeg->Size = System::Drawing::Size(52, 20);
			this->numUpDw_TgBeg->TabIndex = 12;
			this->numUpDw_TgBeg->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->numUpDw_TgBeg->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// labTgBeg
			// 
			this->labTgBeg->AutoSize = true;
			this->labTgBeg->Location = System::Drawing::Point(160, 40);
			this->labTgBeg->Name = L"labTgBeg";
			this->labTgBeg->Size = System::Drawing::Size(26, 13);
			this->labTgBeg->TabIndex = 9;
			this->labTgBeg->Text = L"Beg";
			// 
			// buttPrSave
			// 
			this->buttPrSave->Location = System::Drawing::Point(1, 6);
			this->buttPrSave->Name = L"buttPrSave";
			this->buttPrSave->Size = System::Drawing::Size(78, 23);
			this->buttPrSave->TabIndex = 5;
			this->buttPrSave->Text = L"Save project";
			this->buttPrSave->UseVisualStyleBackColor = true;
			this->buttPrSave->Click += gcnew System::EventHandler(this, &ThPr_Form::commandSavePrFile);
			// 
			// buttPrLoad
			// 
			this->buttPrLoad->Location = System::Drawing::Point(85, 6);
			this->buttPrLoad->Name = L"buttPrLoad";
			this->buttPrLoad->Size = System::Drawing::Size(43, 23);
			this->buttPrLoad->TabIndex = 5;
			this->buttPrLoad->Text = L"Load";
			this->buttPrLoad->UseVisualStyleBackColor = true;
			this->buttPrLoad->Click += gcnew System::EventHandler(this, &ThPr_Form::commandLoadPrFile);
			// 
			// textBoxPrFile
			// 
			this->textBoxPrFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxPrFile->Location = System::Drawing::Point(133, 8);
			this->textBoxPrFile->Name = L"textBoxPrFile";
			this->textBoxPrFile->Size = System::Drawing::Size(424, 20);
			this->textBoxPrFile->TabIndex = 14;
			this->textBoxPrFile->Text = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\Def.ThDy.txt";
			this->textBoxPrFile->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// savePrFileDialog
			// 
			this->savePrFileDialog->AddExtension = false;
			this->savePrFileDialog->DefaultExt = L"ThDy.txt";
			this->savePrFileDialog->FileName = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\Def.ThDy.txt";
			this->savePrFileDialog->Filter = L"All file|*.*|ThDy project|*.ThDy.txt";
			this->savePrFileDialog->FilterIndex = 2;
			this->savePrFileDialog->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec";
			this->savePrFileDialog->SupportMultiDottedExtensions = true;
			this->savePrFileDialog->Title = L"Save Project File";
			this->savePrFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ThPr_Form::SavePrFileDialog_FileOk);
			// 
			// loadPrFileDialog
			// 
			this->loadPrFileDialog->DefaultExt = L"ThDy.txt";
			this->loadPrFileDialog->FileName = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\Def.ThDy.txt";
			this->loadPrFileDialog->Filter = L"All file|*.*|ThDy project|*.ThDy.txt";
			this->loadPrFileDialog->FilterIndex = 2;
			this->loadPrFileDialog->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec";
			this->loadPrFileDialog->SupportMultiDottedExtensions = true;
			this->loadPrFileDialog->Title = L"Load Project File";
			this->loadPrFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ThPr_Form::LoadPrFileDialog_FileOk);
			// 
			// grBoxTargets
			// 
			this->grBoxTargets->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->grBoxTargets->Controls->Add(this->numUpDw_TgEnd);
			this->grBoxTargets->Controls->Add(this->numUpDw_MinLen);
			this->grBoxTargets->Controls->Add(this->numUpDw_TgBeg);
			this->grBoxTargets->Controls->Add(this->btPCRfiltrFile);
			this->grBoxTargets->Controls->Add(this->checkBox1);
			this->grBoxTargets->Controls->Add(this->labTgEnd);
			this->grBoxTargets->Controls->Add(this->lab_MinLen);
			this->grBoxTargets->Controls->Add(this->labTgBeg);
			this->grBoxTargets->Controls->Add(this->txtBoxPCRfiltr);
			this->grBoxTargets->Controls->Add(this->textBoxTgScF4uA);
			this->grBoxTargets->Controls->Add(this->numUpDwMaxTgId);
			this->grBoxTargets->Controls->Add(this->labMaxTgId);
			this->grBoxTargets->Location = System::Drawing::Point(13, 320);
			this->grBoxTargets->Name = L"grBoxTargets";
			this->grBoxTargets->Size = System::Drawing::Size(540, 106);
			this->grBoxTargets->TabIndex = 15;
			this->grBoxTargets->TabStop = false;
			// 
			// btPCRfiltrFile
			// 
			this->btPCRfiltrFile->Enabled = false;
			this->btPCRfiltrFile->Location = System::Drawing::Point(11, 64);
			this->btPCRfiltrFile->Name = L"btPCRfiltrFile";
			this->btPCRfiltrFile->Size = System::Drawing::Size(69, 34);
			this->btPCRfiltrFile->TabIndex = 7;
			this->btPCRfiltrFile->Text = L"PCR filtrFile (Primers)";
			this->btPCRfiltrFile->UseVisualStyleBackColor = true;
			this->btPCRfiltrFile->Click += gcnew System::EventHandler(this, &ThPr_Form::commandOpenTargetsSecFile4uA);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Enabled = false;
			this->checkBox1->Location = System::Drawing::Point(317, 39);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(59, 17);
			this->checkBox1->TabIndex = 15;
			this->checkBox1->Text = L"no frag";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// txtBoxPCRfiltr
			// 
			this->txtBoxPCRfiltr->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtBoxPCRfiltr->Enabled = false;
			this->txtBoxPCRfiltr->Location = System::Drawing::Point(82, 72);
			this->txtBoxPCRfiltr->Name = L"txtBoxPCRfiltr";
			this->txtBoxPCRfiltr->Size = System::Drawing::Size(451, 20);
			this->txtBoxPCRfiltr->TabIndex = 8;
			this->txtBoxPCRfiltr->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// openFDial_NNParam
			// 
			this->openFDial_NNParam->DefaultExt = L"ThDyParam.csv";
			this->openFDial_NNParam->Filter = L"ThDyParam|*.ThDyParam.csv|Text|*.txt|All file|*.*";
			this->openFDial_NNParam->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\";
			this->openFDial_NNParam->Title = L"Open NN ThermoDynamic Parametr File";
			// 
			// oFileExp
			// 
			this->oFileExp->DefaultExt = L"*.csv";
			this->oFileExp->Filter = L"CSV file|*.csv|All file|*.*";
			this->oFileExp->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\";
			this->oFileExp->Title = L"Open NN ThermoDynamic Parametr File";
			// 
			// opPCRfiltrFDlg
			// 
			this->opPCRfiltrFDlg->DefaultExt = L"txt";
			this->opPCRfiltrFDlg->Filter = L"fasta|*.fas;*.fasta|Text|*.txt|All file|*.*";
			this->opPCRfiltrFDlg->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\sonden";
			this->opPCRfiltrFDlg->Title = L"Open Sondes File";
			// 
			// butSeqExplorer
			// 
			this->butSeqExplorer->Location = System::Drawing::Point(179, 432);
			this->butSeqExplorer->Name = L"butSeqExplorer";
			this->butSeqExplorer->Size = System::Drawing::Size(83, 21);
			this->butSeqExplorer->TabIndex = 16;
			this->butSeqExplorer->Text = L"Seq Explorer";
			this->butSeqExplorer->UseVisualStyleBackColor = true;
			this->butSeqExplorer->Click += gcnew System::EventHandler(this, &ThPr_Form::butSeqExplorer_Click);
			// 
			// ThPr_Form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(564, 642);
			this->Controls->Add(this->butSeqExplorer);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->textBoxPrFile);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->buttonTargetsFile);
			this->Controls->Add(this->comBoxTAMeth);
			this->Controls->Add(this->comBoxSalMeth);
			this->Controls->Add(this->buttPrLoad);
			this->Controls->Add(this->buttPrSave);
			this->Controls->Add(this->buttonSaveResultFile);
			this->Controls->Add(this->labTgConc);
			this->Controls->Add(this->textBoxSaveResultFile);
			this->Controls->Add(this->labTAMeth);
			this->Controls->Add(this->numUpDowTgConc);
			this->Controls->Add(this->labSalMeth);
			this->Controls->Add(this->labSalConc);
			this->Controls->Add(this->numUpDowSalConc);
			this->Controls->Add(this->labSdConc);
			this->Controls->Add(this->numUpDowSdConc);
			this->Controls->Add(this->labTa);
			this->Controls->Add(this->numUpDowTa);
			this->Controls->Add(this->tabControl);
			this->Controls->Add(this->grBoxTargets);
			this->Name = L"ThPr_Form";
			this->Text = L"Thermo Dynamic DNA Hybridization";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSdTm))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MinSdLength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MaxSdLength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSdTm))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_MinTargCov))->EndInit();
			this->tabControl->ResumeLayout(false);
			this->tabPagFindSonden->ResumeLayout(false);
			this->tabPagFindSonden->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MaxSd_G))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDw_MinSd_G))->EndInit();
			this->groupBox_Sd_Min_Max->ResumeLayout(false);
			this->groupBox_Sd_Min_Max->PerformLayout();
			this->groBox_ComUnic->ResumeLayout(false);
			this->groBox_ComUnic->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numUpDw_MinTargCov))->EndInit();
			this->flowLayoutPanel1->ResumeLayout(false);
			this->groupBox_SdTg->ResumeLayout(false);
			this->groupBox_SdTg->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSd_TgTm))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSd_TgG))->EndInit();
			this->groupBox_Sd_nTg->ResumeLayout(false);
			this->groupBox_Sd_nTg->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMaxSd_nonTgTm))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUpDowMinSd_nonTgG))->EndInit();
			this->groupBox_Sd_selft->ResumeLayout(false);
			this->groupBox_Sd_selft->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMinSelfG))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMaxSelfTm))->EndInit();
			this->microArray->ResumeLayout(false);
			this->microArray->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			this->tabPagPCRDesing->ResumeLayout(false);
			this->tabPagPCRDesing->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_MaxProdLength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_MinProdLength))->EndInit();
			this->tabPagMultiplexPCR->ResumeLayout(false);
			this->tabPagMultiplexPCR->PerformLayout();
			this->tabPagTmCalc->ResumeLayout(false);
			this->tabPagTmCalc->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dGVw_TmCalcRes))->EndInit();
			this->tabPagSetup->ResumeLayout(false);
			this->tabPagSetup->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMxGrDeg))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowSdConc))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowTgConc))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowTa))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDowSalConc))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDwMaxTgId))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_TgEnd))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_MinLen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUpDw_TgBeg))->EndInit();
			this->grBoxTargets->ResumeLayout(false);
			this->grBoxTargets->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		 }
#pragma endregion


};
}

