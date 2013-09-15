#pragma once
#include "ThDy_DNAHybrid.VC-WF/th_dy_programs.h"  // aqui esta ya : // #include "..\ThDy_programs\init_prog_param.h"  ;    #include "..\ThDySec\common_basics.h" 
#include "ThDySec\sec.h"
#pragma managed
#include "util.h"
#include "ThDy_DNAHybrid.VC-WF/TagBinding.h"
namespace ThDy_DNAHybridWF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace TagBindingNS ;


	/// <summary>
	/// Summary for SeqExpl
	/// </summary>
	public ref class SeqExpl : public System::Windows::Forms::Form
	{
	public:

			ThDyProject				&_Pr;
			 bool					_showAllseq, _showFiltered;
             System::Drawing::Font	^seqFont;
			 TreeNode^				_DropTargetNode;
			 ItemCheckedEventHandler ^SecChecked;
	public: 

		SeqExpl(ThDyProject& Pr) : _Pr(Pr), _showAllseq (true), _showFiltered(true), seqFont(gcnew System::Drawing::Font("Lucida Console", 10))
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			SecChecked= gcnew System::Windows::Forms::ItemCheckedEventHandler(this, &SeqExpl::listV_Seq_ItemChecked);
            this->SuspendLayout();

			FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
			listV_Seq->Tag=treeV_Seq->Nodes[0];
            toolStripLabel_show_seq->Text=TextShowAllSeq();
			 RefreshSeqList      ();

			 this->ResumeLayout();
			this->listV_Seq->ItemChecked +=SecChecked;

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SeqExpl()
		{
			if (components)
			{
				delete components;
			}
		}
private: System::Void   tStripLab_showFiltered_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
             _showFiltered = ! _showFiltered  ; 
	        if(_showFiltered)
				tStripLab_showFiltered->Text= gcnew String(L"Show filtered seq");
	        else
                tStripLab_showFiltered->Text= gcnew String(L"Hide filtered seq");
            RefreshSeqList();
		 }
void					SetDropTargetNode   (TreeNode^ targetNode)
		{
            if (_DropTargetNode==targetNode)
                return;
            if (_DropTargetNode)
                _DropTargetNode->BackColor = treeV_Seq->BackColor;
            _DropTargetNode = targetNode ;
            _DropTargetNode->BackColor = Color::Yellow;
		}
String                  ^TextShowAllSeq     ()
{
	if(_showAllseq)
        return gcnew String(L"Show all seq");
	else
        return gcnew String(L"Show only local seq");
}
void                    RefreshSeqList      ()
	{
			this->listV_Seq->ItemChecked -=SecChecked;
            this->SuspendLayout();

		try{ 
			System::Windows::Forms::TreeNode^  tn = safe_cast<TreeNode^> (listV_Seq->Tag);
			CMultSec     *ms= safe_cast<CMSref^> (tn->Tag)->ms;
			listV_Seq->Items->Clear();
            AddSeqToListRecur(ms, _showAllseq);
		}
        catch(InvalidCastException^ e) 
		{   
			MessageBox::Show (  gcnew String( "Caught expected exception. \n After select ms: Tag is not CMref. \n") + e->Message ) ;
        }
		catch ( std::exception& e)
		{ MessageBox::Show ( gcnew String(e.what())  ) ;
		}

			 this->ResumeLayout();
		this->listV_Seq->ItemChecked +=SecChecked;
    }
void                    RefreshStatusInfo   (CMultSec *ms)
{
		toolStripStatusLabel1->Text= L"Local: #seq "     + Decimal(ms->_Local._NSec).ToString() 
			                             +L", #groups "  + Decimal(ms->_Local._NMSec).ToString()
                                         + (ms->_Local._NSec ? 
										  L", Len range ["+ Decimal(ms->_Local._Len.Min()).ToString()
										       + L", "  + Decimal(ms->_Local._Len.Max()).ToString() + L"]"
	+ String::Format(L", Tm range [{0:F1} °C, {1:F1} °C]", Decimal(KtoC(ms->_Local._Tm.Min())),
										                   Decimal(KtoC(ms->_Local._Tm.Max())))          
														   : L"");

  toolStripStatusLabel1->Text+=L".    Global: #seq "     + Decimal(ms->_Global._NSec).ToString() 
			                             +L", #groups "  + Decimal(ms->_Global._NMSec).ToString()
                                         + (ms->_Global._NSec ? 
										  L", Len range ["+ Decimal(ms->_Global._Len.Min()).ToString()
										       + L", "  + Decimal(ms->_Global._Len.Max()).ToString() + L"]"
	+ String::Format(L", Tm range [{0:F1} °C, {1:F1} °C]", Decimal(KtoC(ms->_Global._Tm.Min())),
										                   Decimal(KtoC(ms->_Global._Tm.Max()))     )            
														   : L"");
}

private: System::Void   toolStripLabel_show_seq_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
             _showAllseq = ! _showAllseq  ;       
			 toolStripLabel_show_seq->Text=TextShowAllSeq();
            RefreshSeqList();
		 }
void                    AddSeqToList        (CMultSec     *ms)
		{
			for ( ms->goFirstSec() ;  ms->NotEndSec() ; ms->goNextSec() )
			{
				if (!_showFiltered && ms->CurSec()->Filtered() ) continue;
				ListViewItem^ lvI=listV_Seq->Items->Add(     gcnew String(ms->CurSec()->Name().c_str())       );
 				lvI->Tag = gcnew CSref(ms->CurSec());
                lvI->Checked= ms->CurSec()->Selected();
                if ( ms->CurSec()->Filtered() )
                    lvI->ForeColor= Color::Gray;
				lvI->Tag= gcnew CSref(ms->CurSec());
				lvI->SubItems->Add(      ( Decimal ( ms->CurSec()->Len())).ToString()     );
				lvI->SubItems->Add(      ( Decimal ( KtoC( ms->CurSec()->_Tm.Ave()) )).ToString("F1")     );
				lvI->SubItems->Add(      ( Decimal ( ms->CurSec()->Degeneracy())).ToString()     );
				lvI->SubItems->Add(    gcnew String(ms->CurSec()->Description().c_str())  );
                lvI->UseItemStyleForSubItems = false;

				lvI->SubItems->Add(    gcnew String(ms->CurSec()->Get_charSec() )  )->Font= seqFont;
			}
		}
void                    AddSeqToListRecur   (CMultSec     *ms, bool rec)
		{
			AddSeqToList(ms);
            if (rec)
		        for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
                        AddSeqToListRecur(ms->CurMSec(), rec);
		}
private: System::Void   treeV_Seq_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
	{	
			this->listV_Seq->ItemChecked -=SecChecked;
            this->SuspendLayout();
		try{ 
			    System::Windows::Forms::TreeNode^ tn= e->Node;	
				if (!tn || tn->Equals( listV_Seq->Tag))
                    return;
			    listV_Seq->Items->Clear();
                listV_Seq->Tag = tn;
			    CMultSec     *ms= safe_cast<CMSref^> (tn->Tag)->ms;
                AddSeqToListRecur(ms, _showAllseq);
                RefreshStatusInfo(ms);
		    }
            catch(InvalidCastException^ e) 
			{   
				MessageBox::Show (  gcnew String( "Caught expected exception. \n After select ms: Tag is not CMref. \n") + e->Message ) ;
            }
			catch ( std::exception& e)
		    { MessageBox::Show ( gcnew String(e.what())  ) ;
		    }	
			 this->ResumeLayout();
			this->listV_Seq->ItemChecked +=SecChecked;
	}
void                    FillOneLevelDown    (TreeNodeCollection ^Nodes, CMultSec     *ms)
{
        Nodes->Clear();
		for ( ms->goFirstMSec() ;  ms->NotEndMSec() ; ms->goNextMSec() )
		{
			CMultSec *msi=ms->CurMSec();
			TreeNode^ tn= Nodes->Add(gcnew String(msi->_name.c_str()) );
			tn->Tag= gcnew CMSref(msi);
			for ( msi->goFirstMSec() ;  msi->NotEndMSec() ; msi->goNextMSec() )
			{
				CMultSec *msii=msi->CurMSec();
				TreeNode^ tnn= tn->Nodes->Add(gcnew String(msii->_name.c_str()) );
				tnn->Tag= gcnew CMSref(msii);
			}
		}
}
private: System::Void treeV_Seq_BeforeExpand(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) 
		 {      
            try {
			         if(!e || !e->Node || !e->Node->Tag )
                         return ;
					RefreshNode  (e->Node);
								//	CMultSec     *ms= safe_cast<CMSref^> (e->Node->Tag)->ms;
								//  FillOneLevelDown(e->Node->Nodes, ms);
                } 
            catch(InvalidCastException^ e) 
			{   
				MessageBox::Show (  gcnew String( "Caught expected exception. \n Before expand: Tag is not CMref. \n") + e->Message ) ;
            }
		 }
void					RefreshNode			(TreeNode^ tn) 
		 {      
			if(!tn || !tn->Tag )
                    return ;					// execption ????
		    CMultSec     *ms= safe_cast<CMSref^> (tn->Tag)->ms;
            FillOneLevelDown(tn->Nodes, ms);
		 }
TreeNode^				NodeOf				(CMultSec     *ms)
{
	Collections::IEnumerator^ ie= treeV_Seq->Nodes->GetEnumerator();
	while(ie->MoveNext())
	{
		TreeNode^ tn = safe_cast<TreeNode^>(ie);
		if (ms == safe_cast<CMSref^> (tn->Tag)->ms)
			return tn;
	}
	return nullptr;
}
private: System::Void treeV_Seq_AfterLabelEdit				(System::Object^  sender, System::Windows::Forms::NodeLabelEditEventArgs^  e) 
		{	  try{  
			        if(! e->Label) 
                        return;
					CMultSec     *ms= safe_cast<CMSref^> (e->Node->Tag)->ms;
					ms->_name=CreateStdFromManString(e->Label);
		        }
                catch(InvalidCastException^ e) 
			    {   
				    MessageBox::Show (  gcnew String( "Caught expected exception. \n After LAbel Edit: Tag is not CMref. \n") + e->Message ) ;
                }
				catch ( std::exception& e)
		        { MessageBox::Show ( gcnew String(e.what())  ) ;
		        }
		}
private: System::Void toolStripButAddSeqGr_Click			(System::Object^  sender, System::EventArgs^  e) 
		{	try{    
					System::Windows::Forms::TreeNode^ tn= treeV_Seq->SelectedNode;
                    if (!tn)
                        return;
					CMultSec     *ms= safe_cast<CMSref^> (tn->Tag)->ms;

					ms=_Pr._cp.AddSeqGroup(ms,"New group");
					tn=tn->Nodes->Add(gcnew String( ms->_name.c_str())  );
					tn->Tag= gcnew CMSref(ms);
					tn->EnsureVisible ();
					tn->BeginEdit() ;
		        }
                catch(InvalidCastException^ e) 
			    {   
				    MessageBox::Show (  gcnew String( "Caught expected exception. \n Add seq gr: Tag is not CMref. \n") + e->Message ) ;
                }
				catch ( std::exception& e)
		        { 
					MessageBox::Show ( gcnew String(e.what())  ) ;
		        }		
		}
private: System::Void toolStripButtonAddSeq_Click			(System::Object^  sender, System::EventArgs^  e) 
		{	try{ 
					System::Windows::Forms::TreeNode^ tn= treeV_Seq->SelectedNode;
                    if (!tn)
                        return;
					this->openFileDialog_targets->ShowDialog();
					CMultSec     *ms= safe_cast<CMSref^> (tn->Tag)->ms;
					CMultSec     *nms=_Pr._cp.AddSeqFromFile	
														(ms, CreateStdFromManString
																(this->openFileDialog_targets->FileName)	);
					nms->_name=CreateStdFromManString( this->openFileDialog_targets->SafeFileName );	// mejor qu elo hagfa CMS??		
			
					tn=tn->Nodes->Add( gcnew String( nms->_name.c_str()  ));
					tn->Tag= gcnew CMSref(nms);
					tn->EnsureVisible ();
					RefreshSeqList();           //					treeV_Seq->SelectedNode=tn;
			}
            catch(InvalidCastException^ e) 
			{   
				MessageBox::Show (  gcnew String( "Caught expected exception. \n After Add Seq from file bott click: Tag is not CMref. \n") + e->Message ) ;
            }
			catch ( std::exception& e)
			{ MessageBox::Show ( gcnew String(e.what())  ) ;
			}		 
		}
private: System::Void     addTargetToolStripMenuItem_Click	(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->openFileDialog_targets->ShowDialog();

			 CMultSec     *nms=_Pr._cp.AddTargetFromFile(  CreateStdFromManString(this->openFileDialog_targets->FileName)  ); 
			 nms->_name=CreateStdFromManString( this->openFileDialog_targets->SafeFileName );	// mejor qu elo hagfa CMS??		

			 FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
		 }
private: System::Void  addNontargetToolStripMenuItem_Click	(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->openFileDialog_non_targets->ShowDialog();

			 CMultSec     *nms=_Pr._cp.AddNoTargetFromFile(  CreateStdFromManString(this->openFileDialog_non_targets->FileName)  ); 
			 nms->_name=CreateStdFromManString( this->openFileDialog_non_targets->SafeFileName );	// mejor qu elo hagfa CMS??		
			 FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
		 }
private: System::Void setUArrProbesToolStripMenuItem_Click	(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->openSondesFileDialog->ShowDialog();

			 CMultSec     *nms=_Pr._uArr.AdduArrFromFile(  CreateStdFromManString(this->openSondesFileDialog->FileName)  ); 
			 nms->_name=CreateStdFromManString( this->openSondesFileDialog->SafeFileName );	// mejor qu elo hagfa CMS??		
			 FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
		 }
private: System::Void setMulriplexPCRPrimersToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->openSondesFileDialog->ShowDialog();

			 CMultSec     *nms=_Pr._mPCR.AdduArrFromFile(  CreateStdFromManString(this->openSondesFileDialog->FileName)  ); 
			 nms->_name=CreateStdFromManString( this->openSondesFileDialog->SafeFileName );	// mejor qu elo hagfa CMS??		
			 FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
		 }
private: System::Void setPCRFiltreToolStripMenuItem_Click	(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->openFileDialog_targets->ShowDialog();

			 CMultSec     *nms=_Pr._cp.AddPCRfiltreFromFile(  CreateStdFromManString(this->openFileDialog_targets->FileName)  ); 
			 nms->_name=CreateStdFromManString( this->openFileDialog_targets->SafeFileName );	// mejor qu elo hagfa CMS??		
			 FillOneLevelDown(treeV_Seq->Nodes, _Pr._cp._pSeqTree.get());
		 }
private: System::Void getPCRFiltreFrommTmCalcToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

		 }
private: System::Void listV_Seq_ItemDrag        (System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) 
		 {
              DoDragDrop( e->Item, DragDropEffects::Move | DragDropEffects::Scroll );
		 }
private: System::Void listV_Seq_ItemChecked(System::Object^  sender, System::Windows::Forms::ItemCheckedEventArgs^  e) 
	{try
	    {  
			if(!e->Item->Tag) return;
			CSec *s = safe_cast<CSref^> (e->Item->Tag)->s;
			s->Selected(e->Item->Checked);

		}
        catch(InvalidCastException^ e) 
	    {   
		    MessageBox::Show (  gcnew String( "Caught expected exception. \n After LAbel Edit: Tag is not CMref. \n") + e->Message ) ;
        }
	    catch ( std::exception& e)
	    { MessageBox::Show ( gcnew String(e.what())  ) ;
	    }
	} 
private: System::Void treeV_Seq_ItemDrag        (System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) 
		 {
              DoDragDrop( e->Item, DragDropEffects::Move | DragDropEffects::Scroll );
		 }
private: System::Void treeV_Seq_DragEnter       (System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
		 {
             e->Effect = e->AllowedEffect;
		 }
private: System::Void treeV_Seq_GiveFeedback    (System::Object^  sender, System::Windows::Forms::GiveFeedbackEventArgs^  e) 
		 {
		 }
private: System::Void treeV_Seq_DragLeave       (System::Object^  sender, System::EventArgs^  e) 
		 {
            if (_DropTargetNode)
                _DropTargetNode->BackColor = treeV_Seq->BackColor;
		 }
private: System::Void treeV_Seq_DragOver        (System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
		 {
            Point targetPoint = treeV_Seq->PointToClient( Point(e->X,e->Y) );      // Retrieve the client coordinates of the drop location.
             TreeNode^ targetNode = treeV_Seq->GetNodeAt( targetPoint );            // Retrieve the node at the drop location.
            SetDropTargetNode(targetNode);
		 }
private: System::Void treeV_Seq_DragDrop        (System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
	{try
	    {  
             Point targetPoint = treeV_Seq->PointToClient( Point(e->X,e->Y) );      // Retrieve the client coordinates of the drop location.
             TreeNode^ targetNode = treeV_Seq->GetNodeAt( targetPoint );            // Retrieve the node at the drop location.

             if (e->Data->GetData( TreeNode::typeid ))
		     {  TreeNode^ draggedNode = dynamic_cast<TreeNode^>(e->Data->GetData( TreeNode::typeid )); // Retrieve the node that was dragged.
			 if (  draggedNode->Equals( targetNode ) ||  draggedNode->Parent->Equals( targetNode ))                  // Confirm that the node at the drop location is not  the dragged node 
                    return;
                TreeNode^ parent=targetNode ;                    // or a descendant of the dragged node. 
                while (parent= parent->Parent)
                    if (parent->Equals(draggedNode))
                        return;

			    CMultSec     *d_ms= safe_cast<CMSref^> (draggedNode->Tag)->ms;
			    CMultSec     *t_ms= safe_cast<CMSref^> (targetNode->Tag)->ms;
                                    
 			    t_ms->AddMultiSec(d_ms);		// A move operation, remove the node from its current  location and add it to the node at the drop location. 
                    
			    draggedNode->Remove();
                targetNode->Nodes->Add( draggedNode );
		        targetNode->BackColor=treeV_Seq->BackColor;
 
                targetNode->Expand();
				RefreshSeqList();           //					treeV_Seq->SelectedNode=tn;

              }
			 else

             if (e->Data->GetData( ListViewItem::typeid ))
		     {  
				 if (targetNode->Equals(safe_cast<TreeNode ^>(listV_Seq->Tag))  )
					 return;
			    CMultSec     *t_ms= safe_cast<CMSref^> (targetNode ->Tag)->ms;

                System::Collections::IEnumerator^ sec_item = listV_Seq->SelectedItems->GetEnumerator();
                while ( sec_item->MoveNext() )
                {
                    ListViewItem^ item = safe_cast<ListViewItem^>(sec_item->Current);
                    //ListViewItem^ draggedItem = dynamic_cast<ListViewItem^>(e->Data->GetData( ListViewItem::typeid )); // Retrieve the Item that was dragged.
                    CSec *sec = safe_cast<CSref ^> (item->Tag)->s ;
					if (t_ms == sec->_parentMS) 
						continue;
                    t_ms->AddSec(sec);		// A move operation, remove the node from its current  location and add it to the node at the drop location. 
                    item->Remove();
				}
                targetNode->BackColor=treeV_Seq->BackColor;
                targetNode->Expand();
              }	    
		 }
        catch(InvalidCastException^ e) 
	    {   
		    MessageBox::Show (  gcnew String( "Caught expected exception. \n After LAbel Edit: Tag is not CMref. \n") + e->Message ) ;
        }
	    catch ( std::exception& e)
	    { MessageBox::Show ( gcnew String(e.what())  ) ;
	    }
	} 

	private: System::Windows::Forms::ToolStripLabel^		toolStripLabel_show_seq;
	private: System::Windows::Forms::ToolStripMenuItem^		getPCRFiltreFrommTmCalcToolStripMenuItem;
	private: System::Windows::Forms::ColumnHeader^			colH_deg;
	private: System::Windows::Forms::OpenFileDialog^		openSondesFileDialog;
	private: System::Windows::Forms::OpenFileDialog^		openFileDialog_non_targets;

	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripLabel^		tStripLab_showFiltered;
	private: System::Windows::Forms::ColumnHeader^			colH_seq;
	private: System::Windows::Forms::ColumnHeader^			colH_Tm;
	private: System::Windows::Forms::ToolStripMenuItem^		addTargetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^		addNontargetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^		setUArrProbesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^		setMulriplexPCRPrimersToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^		setPCRFiltreToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^	toolStripSeparator2;
	private: System::Windows::Forms::ToolStripSeparator^	toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^		newGroupToolStripMenuItem;

	private: System::Windows::Forms::MenuStrip^             menuStripSeqExpl;
	private: System::Windows::Forms::ToolStripMenuItem^     sequencesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^     addFromFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^     viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^     allSeqinclInternalGroupsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^     deletedSeqToolStripMenuItem;
	private: System::Windows::Forms::ToolStripContainer^    toolStripContainer1;
	private: System::Windows::Forms::ToolStrip^             toolStrip1;
	private: System::Windows::Forms::ToolStripButton^       toolStripButtonAddSeq;
	private: System::Windows::Forms::ToolStripSeparator^    toolStripSeparator1;

	private: System::Windows::Forms::StatusStrip^           statusStrip1;
	private: System::Windows::Forms::SplitContainer^        splitContainer1;
	private: System::Windows::Forms::TreeView^              treeV_Seq;
	private: System::Windows::Forms::SplitContainer^        splitContainer2;
	private: System::Windows::Forms::ListView^              listV_Seq;

	private: System::Windows::Forms::TextBox^               textBox_Seq;
	private: System::Windows::Forms::ColumnHeader^          colHead_name;
	private: System::Windows::Forms::ColumnHeader^          columnHeader_len;
	private: System::Windows::Forms::ToolStripButton^       toolStripButAddSeqGr;
	private: System::Windows::Forms::OpenFileDialog^        openFileDialog_targets;
    private: System::Windows::Forms::ColumnHeader^          colH_descript;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(SeqExpl::typeid));
            this->menuStripSeqExpl = (gcnew System::Windows::Forms::MenuStrip());
            this->sequencesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->addFromFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->addTargetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->addNontargetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->setUArrProbesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->setMulriplexPCRPrimersToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->setPCRFiltreToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->getPCRFiltreFrommTmCalcToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->newGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->allSeqinclInternalGroupsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->deletedSeqToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripContainer1 = (gcnew System::Windows::Forms::ToolStripContainer());
            this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripButAddSeqGr = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripButtonAddSeq = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripLabel_show_seq = (gcnew System::Windows::Forms::ToolStripLabel());
            this->tStripLab_showFiltered = (gcnew System::Windows::Forms::ToolStripLabel());
            this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
            this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
            this->treeV_Seq = (gcnew System::Windows::Forms::TreeView());
            this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
            this->listV_Seq = (gcnew System::Windows::Forms::ListView());
            this->colHead_name = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader_len = (gcnew System::Windows::Forms::ColumnHeader());
            this->colH_Tm = (gcnew System::Windows::Forms::ColumnHeader());
            this->colH_deg = (gcnew System::Windows::Forms::ColumnHeader());
            this->colH_descript = (gcnew System::Windows::Forms::ColumnHeader());
            this->colH_seq = (gcnew System::Windows::Forms::ColumnHeader());
            this->textBox_Seq = (gcnew System::Windows::Forms::TextBox());
            this->openFileDialog_targets = (gcnew System::Windows::Forms::OpenFileDialog());
            this->openSondesFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
            this->openFileDialog_non_targets = (gcnew System::Windows::Forms::OpenFileDialog());
            this->menuStripSeqExpl->SuspendLayout();
            this->toolStripContainer1->TopToolStripPanel->SuspendLayout();
            this->toolStripContainer1->SuspendLayout();
            this->toolStrip1->SuspendLayout();
            this->statusStrip1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
            this->splitContainer1->Panel1->SuspendLayout();
            this->splitContainer1->Panel2->SuspendLayout();
            this->splitContainer1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->BeginInit();
            this->splitContainer2->Panel1->SuspendLayout();
            this->splitContainer2->Panel2->SuspendLayout();
            this->splitContainer2->SuspendLayout();
            this->SuspendLayout();
            // 
            // menuStripSeqExpl
            // 
            this->menuStripSeqExpl->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->sequencesToolStripMenuItem, 
                this->viewToolStripMenuItem});
            this->menuStripSeqExpl->Location = System::Drawing::Point(0, 0);
            this->menuStripSeqExpl->Name = L"menuStripSeqExpl";
            this->menuStripSeqExpl->Size = System::Drawing::Size(1076, 24);
            this->menuStripSeqExpl->TabIndex = 0;
            this->menuStripSeqExpl->Text = L"menuStripSeqExpl";
            // 
            // sequencesToolStripMenuItem
            // 
            this->sequencesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->addFromFileToolStripMenuItem, 
                this->toolStripSeparator2, this->addTargetToolStripMenuItem, this->addNontargetToolStripMenuItem, this->setUArrProbesToolStripMenuItem, 
                this->setMulriplexPCRPrimersToolStripMenuItem, this->setPCRFiltreToolStripMenuItem, this->getPCRFiltreFrommTmCalcToolStripMenuItem, 
                this->toolStripSeparator3, this->newGroupToolStripMenuItem});
            this->sequencesToolStripMenuItem->Name = L"sequencesToolStripMenuItem";
            this->sequencesToolStripMenuItem->Size = System::Drawing::Size(75, 20);
            this->sequencesToolStripMenuItem->Text = L"Sequences";
            // 
            // addFromFileToolStripMenuItem
            // 
            this->addFromFileToolStripMenuItem->Name = L"addFromFileToolStripMenuItem";
            this->addFromFileToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->addFromFileToolStripMenuItem->Text = L"Add from file";
            this->addFromFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::toolStripButtonAddSeq_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(217, 6);
            // 
            // addTargetToolStripMenuItem
            // 
            this->addTargetToolStripMenuItem->Name = L"addTargetToolStripMenuItem";
            this->addTargetToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->addTargetToolStripMenuItem->Text = L"Add Target ";
            this->addTargetToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::addTargetToolStripMenuItem_Click);
            // 
            // addNontargetToolStripMenuItem
            // 
            this->addNontargetToolStripMenuItem->Name = L"addNontargetToolStripMenuItem";
            this->addNontargetToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->addNontargetToolStripMenuItem->Text = L"Add Non-target";
            this->addNontargetToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::addNontargetToolStripMenuItem_Click);
            // 
            // setUArrProbesToolStripMenuItem
            // 
            this->setUArrProbesToolStripMenuItem->Name = L"setUArrProbesToolStripMenuItem";
            this->setUArrProbesToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->setUArrProbesToolStripMenuItem->Text = L"Set uArr probes";
            this->setUArrProbesToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::setUArrProbesToolStripMenuItem_Click);
            // 
            // setMulriplexPCRPrimersToolStripMenuItem
            // 
            this->setMulriplexPCRPrimersToolStripMenuItem->Name = L"setMulriplexPCRPrimersToolStripMenuItem";
            this->setMulriplexPCRPrimersToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->setMulriplexPCRPrimersToolStripMenuItem->Text = L"Set (Multiplex-)PCR primers";
            this->setMulriplexPCRPrimersToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::setMulriplexPCRPrimersToolStripMenuItem_Click);
            // 
            // setPCRFiltreToolStripMenuItem
            // 
            this->setPCRFiltreToolStripMenuItem->Name = L"setPCRFiltreToolStripMenuItem";
            this->setPCRFiltreToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->setPCRFiltreToolStripMenuItem->Text = L"Set PCR filtre";
            this->setPCRFiltreToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::setPCRFiltreToolStripMenuItem_Click);
            // 
            // getPCRFiltreFrommTmCalcToolStripMenuItem
            // 
            this->getPCRFiltreFrommTmCalcToolStripMenuItem->Name = L"getPCRFiltreFrommTmCalcToolStripMenuItem";
            this->getPCRFiltreFrommTmCalcToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->getPCRFiltreFrommTmCalcToolStripMenuItem->Text = L"Get PCR filtre from Tm Calc";
            this->getPCRFiltreFrommTmCalcToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::getPCRFiltreFrommTmCalcToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this->toolStripSeparator3->Name = L"toolStripSeparator3";
            this->toolStripSeparator3->Size = System::Drawing::Size(217, 6);
            // 
            // newGroupToolStripMenuItem
            // 
            this->newGroupToolStripMenuItem->Name = L"newGroupToolStripMenuItem";
            this->newGroupToolStripMenuItem->Size = System::Drawing::Size(220, 22);
            this->newGroupToolStripMenuItem->Text = L"New group";
            this->newGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &SeqExpl::toolStripButAddSeqGr_Click);
            // 
            // viewToolStripMenuItem
            // 
            this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->allSeqinclInternalGroupsToolStripMenuItem, 
                this->deletedSeqToolStripMenuItem});
            this->viewToolStripMenuItem->Enabled = false;
            this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
            this->viewToolStripMenuItem->Size = System::Drawing::Size(44, 20);
            this->viewToolStripMenuItem->Text = L"View";
            // 
            // allSeqinclInternalGroupsToolStripMenuItem
            // 
            this->allSeqinclInternalGroupsToolStripMenuItem->Checked = true;
            this->allSeqinclInternalGroupsToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
            this->allSeqinclInternalGroupsToolStripMenuItem->Name = L"allSeqinclInternalGroupsToolStripMenuItem";
            this->allSeqinclInternalGroupsToolStripMenuItem->Size = System::Drawing::Size(222, 22);
            this->allSeqinclInternalGroupsToolStripMenuItem->Text = L"All seq (incl internal groups)";
            this->allSeqinclInternalGroupsToolStripMenuItem->CheckStateChanged += gcnew System::EventHandler(this, &SeqExpl::toolStripLabel_show_seq_Click);
            // 
            // deletedSeqToolStripMenuItem
            // 
            this->deletedSeqToolStripMenuItem->Checked = true;
            this->deletedSeqToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
            this->deletedSeqToolStripMenuItem->Name = L"deletedSeqToolStripMenuItem";
            this->deletedSeqToolStripMenuItem->Size = System::Drawing::Size(222, 22);
            this->deletedSeqToolStripMenuItem->Text = L"Show filtered seq";
            this->deletedSeqToolStripMenuItem->CheckStateChanged += gcnew System::EventHandler(this, &SeqExpl::tStripLab_showFiltered_Click);
            // 
            // toolStripContainer1
            // 
            this->toolStripContainer1->BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this->toolStripContainer1->ContentPanel->Size = System::Drawing::Size(687, 0);
            this->toolStripContainer1->LeftToolStripPanelVisible = false;
            this->toolStripContainer1->Location = System::Drawing::Point(0, 27);
            this->toolStripContainer1->Name = L"toolStripContainer1";
            this->toolStripContainer1->RightToolStripPanelVisible = false;
            this->toolStripContainer1->Size = System::Drawing::Size(687, 23);
            this->toolStripContainer1->TabIndex = 1;
            this->toolStripContainer1->Text = L"toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this->toolStripContainer1->TopToolStripPanel->Controls->Add(this->toolStrip1);
            // 
            // toolStrip1
            // 
            this->toolStrip1->Dock = System::Windows::Forms::DockStyle::None;
            this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->toolStripButAddSeqGr, 
                this->toolStripButtonAddSeq, this->toolStripSeparator1, this->toolStripLabel_show_seq, this->tStripLab_showFiltered});
            this->toolStrip1->Location = System::Drawing::Point(3, 0);
            this->toolStrip1->Name = L"toolStrip1";
            this->toolStrip1->Size = System::Drawing::Size(251, 25);
            this->toolStrip1->TabIndex = 0;
            // 
            // toolStripButAddSeqGr
            // 
            this->toolStripButAddSeqGr->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolStripButAddSeqGr->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButAddSeqGr.Image")));
            this->toolStripButAddSeqGr->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripButAddSeqGr->Name = L"toolStripButAddSeqGr";
            this->toolStripButAddSeqGr->Size = System::Drawing::Size(23, 22);
            this->toolStripButAddSeqGr->Text = L"New Group";
            this->toolStripButAddSeqGr->ToolTipText = L"Add a new Group";
            this->toolStripButAddSeqGr->Click += gcnew System::EventHandler(this, &SeqExpl::toolStripButAddSeqGr_Click);
            // 
            // toolStripButtonAddSeq
            // 
            this->toolStripButtonAddSeq->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolStripButtonAddSeq->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButtonAddSeq.Image")));
            this->toolStripButtonAddSeq->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripButtonAddSeq->Name = L"toolStripButtonAddSeq";
            this->toolStripButtonAddSeq->Size = System::Drawing::Size(23, 22);
            this->toolStripButtonAddSeq->Text = L"Add Seq";
            this->toolStripButtonAddSeq->ToolTipText = L"Add Sequences from a file";
            this->toolStripButtonAddSeq->Click += gcnew System::EventHandler(this, &SeqExpl::toolStripButtonAddSeq_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
            // 
            // toolStripLabel_show_seq
            // 
            this->toolStripLabel_show_seq->BackColor = System::Drawing::SystemColors::ButtonFace;
            this->toolStripLabel_show_seq->ForeColor = System::Drawing::SystemColors::HotTrack;
            this->toolStripLabel_show_seq->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
            this->toolStripLabel_show_seq->Name = L"toolStripLabel_show_seq";
            this->toolStripLabel_show_seq->Size = System::Drawing::Size(111, 22);
            this->toolStripLabel_show_seq->Text = L"Show only local seq";
            this->toolStripLabel_show_seq->Click += gcnew System::EventHandler(this, &SeqExpl::toolStripLabel_show_seq_Click);
            // 
            // tStripLab_showFiltered
            // 
            this->tStripLab_showFiltered->ForeColor = System::Drawing::Color::Gray;
            this->tStripLab_showFiltered->Name = L"tStripLab_showFiltered";
            this->tStripLab_showFiltered->Size = System::Drawing::Size(76, 22);
            this->tStripLab_showFiltered->Text = L"Show filtered";
            this->tStripLab_showFiltered->Click += gcnew System::EventHandler(this, &SeqExpl::tStripLab_showFiltered_Click);
            // 
            // statusStrip1
            // 
            this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
            this->statusStrip1->Location = System::Drawing::Point(0, 534);
            this->statusStrip1->Name = L"statusStrip1";
            this->statusStrip1->Size = System::Drawing::Size(1076, 22);
            this->statusStrip1->TabIndex = 2;
            this->statusStrip1->Text = L"statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
            this->toolStripStatusLabel1->Size = System::Drawing::Size(0, 17);
            // 
            // splitContainer1
            // 
            this->splitContainer1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->splitContainer1->Location = System::Drawing::Point(0, 59);
            this->splitContainer1->Name = L"splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this->splitContainer1->Panel1->Controls->Add(this->treeV_Seq);
            // 
            // splitContainer1.Panel2
            // 
            this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
            this->splitContainer1->Size = System::Drawing::Size(1076, 475);
            this->splitContainer1->SplitterDistance = 171;
            this->splitContainer1->TabIndex = 3;
            // 
            // treeV_Seq
            // 
            this->treeV_Seq->AllowDrop = true;
            this->treeV_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
            this->treeV_Seq->FullRowSelect = true;
            this->treeV_Seq->HideSelection = false;
            this->treeV_Seq->LabelEdit = true;
            this->treeV_Seq->Location = System::Drawing::Point(0, 0);
            this->treeV_Seq->MinimumSize = System::Drawing::Size(120, 4);
            this->treeV_Seq->Name = L"treeV_Seq";
            this->treeV_Seq->ShowNodeToolTips = true;
            this->treeV_Seq->Size = System::Drawing::Size(171, 475);
            this->treeV_Seq->TabIndex = 0;
            this->treeV_Seq->AfterLabelEdit += gcnew System::Windows::Forms::NodeLabelEditEventHandler(this, &SeqExpl::treeV_Seq_AfterLabelEdit);
            this->treeV_Seq->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &SeqExpl::treeV_Seq_BeforeExpand);
            this->treeV_Seq->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &SeqExpl::treeV_Seq_ItemDrag);
            this->treeV_Seq->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SeqExpl::treeV_Seq_AfterSelect);
            this->treeV_Seq->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &SeqExpl::treeV_Seq_DragDrop);
            this->treeV_Seq->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &SeqExpl::treeV_Seq_DragEnter);
            this->treeV_Seq->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &SeqExpl::treeV_Seq_DragOver);
            this->treeV_Seq->DragLeave += gcnew System::EventHandler(this, &SeqExpl::treeV_Seq_DragLeave);
            // 
            // splitContainer2
            // 
            this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer2->Location = System::Drawing::Point(0, 0);
            this->splitContainer2->Name = L"splitContainer2";
            this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this->splitContainer2->Panel1->Controls->Add(this->listV_Seq);
            // 
            // splitContainer2.Panel2
            // 
            this->splitContainer2->Panel2->Controls->Add(this->textBox_Seq);
            this->splitContainer2->Size = System::Drawing::Size(901, 475);
            this->splitContainer2->SplitterDistance = 434;
            this->splitContainer2->TabIndex = 0;
            // 
            // listV_Seq
            // 
            this->listV_Seq->CheckBoxes = true;
            this->listV_Seq->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->colHead_name, this->columnHeader_len, 
                this->colH_Tm, this->colH_deg, this->colH_descript, this->colH_seq});
            this->listV_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listV_Seq->FullRowSelect = true;
            this->listV_Seq->GridLines = true;
            this->listV_Seq->Location = System::Drawing::Point(0, 0);
            this->listV_Seq->Name = L"listV_Seq";
            this->listV_Seq->Size = System::Drawing::Size(901, 434);
            this->listV_Seq->TabIndex = 0;
            this->listV_Seq->UseCompatibleStateImageBehavior = false;
            this->listV_Seq->View = System::Windows::Forms::View::Details;
            this->listV_Seq->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &SeqExpl::listV_Seq_ItemDrag);
            // 
            // colHead_name
            // 
            this->colHead_name->Text = L"Name";
            this->colHead_name->Width = 112;
            // 
            // columnHeader_len
            // 
            this->columnHeader_len->Text = L"Lenght";
            this->columnHeader_len->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader_len->Width = 49;
            // 
            // colH_Tm
            // 
            this->colH_Tm->Text = L"Tm °C";
            this->colH_Tm->Width = 44;
            // 
            // colH_deg
            // 
            this->colH_deg->Text = L"Deg.";
            this->colH_deg->Width = 43;
            // 
            // colH_descript
            // 
            this->colH_descript->Text = L"Description";
            this->colH_descript->Width = 286;
            // 
            // colH_seq
            // 
            this->colH_seq->Text = L"Seq";
            this->colH_seq->Width = 973;
            // 
            // textBox_Seq
            // 
            this->textBox_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
            this->textBox_Seq->Location = System::Drawing::Point(0, 0);
            this->textBox_Seq->Multiline = true;
            this->textBox_Seq->Name = L"textBox_Seq";
            this->textBox_Seq->Size = System::Drawing::Size(901, 37);
            this->textBox_Seq->TabIndex = 0;
            // 
            // openFileDialog_targets
            // 
            this->openFileDialog_targets->Filter = L"fasta|*.fas;*.fasta|NCBI BLAST|*-Alignment.xml|GB|*.gb;*-sequence.xml|Text|*.txt|" 
                L"All file|*.*";
            this->openFileDialog_targets->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\targets";
            this->openFileDialog_targets->Title = L"Open Targets File";
            // 
            // openSondesFileDialog
            // 
            this->openSondesFileDialog->DefaultExt = L"txt";
            this->openSondesFileDialog->Filter = L"fasta|*.fas;*.fasta|Text|*.txt|All file|*.*";
            this->openSondesFileDialog->InitialDirectory = L"C:\\Users\\Rodriguez\\Documents\\ThDySec\\sonden";
            this->openSondesFileDialog->Title = L"Open Sondes File";
            // 
            // openFileDialog_non_targets
            // 
            this->openFileDialog_non_targets->Filter = L"fasta|*.fas|fasta|*.fasta|Text|*.txt|All file|*.*";
            this->openFileDialog_non_targets->Title = L"Open Non-Targets Sec File";
            // 
            // SeqExpl
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1076, 556);
            this->Controls->Add(this->splitContainer1);
            this->Controls->Add(this->statusStrip1);
            this->Controls->Add(this->toolStripContainer1);
            this->Controls->Add(this->menuStripSeqExpl);
            this->MainMenuStrip = this->menuStripSeqExpl;
            this->Name = L"SeqExpl";
            this->Text = L"Sequences Explorer";
            this->menuStripSeqExpl->ResumeLayout(false);
            this->menuStripSeqExpl->PerformLayout();
            this->toolStripContainer1->TopToolStripPanel->ResumeLayout(false);
            this->toolStripContainer1->TopToolStripPanel->PerformLayout();
            this->toolStripContainer1->ResumeLayout(false);
            this->toolStripContainer1->PerformLayout();
            this->toolStrip1->ResumeLayout(false);
            this->toolStrip1->PerformLayout();
            this->statusStrip1->ResumeLayout(false);
            this->statusStrip1->PerformLayout();
            this->splitContainer1->Panel1->ResumeLayout(false);
            this->splitContainer1->Panel2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
            this->splitContainer1->ResumeLayout(false);
            this->splitContainer2->Panel1->ResumeLayout(false);
            this->splitContainer2->Panel2->ResumeLayout(false);
            this->splitContainer2->Panel2->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->EndInit();
            this->splitContainer2->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

private: System::Void openFileDialog_targets_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		 }
private: System::Void treeV_Seq_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		 }
private: System::Void treeV_Seq_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e)
		 {
		 }
private: System::Void toolStripComboBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}
