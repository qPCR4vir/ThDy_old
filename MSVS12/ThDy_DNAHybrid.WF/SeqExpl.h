#pragma once

namespace ThDy_DNAHybridWF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SeqExpl
	/// </summary>
	public ref class SeqExpl : public System::Windows::Forms::Form
	{
	public:
		SeqExpl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
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
	private: System::Windows::Forms::MenuStrip^  menuStripSeqExpl;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  sequencesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addFromFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  allSeqinclInternalGroupsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deletedSeqToolStripMenuItem;
	private: System::Windows::Forms::ToolStripContainer^  toolStripContainer1;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButtonAddSeq;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  showAllSeqToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  onlyCurrentGropSeqToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TreeView^  treeV_Seq;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ListView^  listV_Seq;
	private: System::Windows::Forms::ColumnHeader^  Name;
	private: System::Windows::Forms::ColumnHeader^  Lenght;
	private: System::Windows::Forms::TextBox^  textBox_Seq;

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
			this->toolStripContainer1 = (gcnew System::Windows::Forms::ToolStripContainer());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButtonAddSeq = (gcnew System::Windows::Forms::ToolStripButton());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->treeV_Seq = (gcnew System::Windows::Forms::TreeView());
			this->listV_Seq = (gcnew System::Windows::Forms::ListView());
			this->textBox_Seq = (gcnew System::Windows::Forms::TextBox());
			this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->showAllSeqToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->onlyCurrentGropSeqToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->Name = (gcnew System::Windows::Forms::ColumnHeader());
			this->Lenght = (gcnew System::Windows::Forms::ColumnHeader());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->allSeqinclInternalGroupsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deletedSeqToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStripSeqExpl->SuspendLayout();
			this->toolStripContainer1->TopToolStripPanel->SuspendLayout();
			this->toolStripContainer1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
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
			this->menuStripSeqExpl->Size = System::Drawing::Size(687, 24);
			this->menuStripSeqExpl->TabIndex = 0;
			this->menuStripSeqExpl->Text = L"menuStripSeqExpl";
			// 
			// sequencesToolStripMenuItem
			// 
			this->sequencesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->addFromFileToolStripMenuItem});
			this->sequencesToolStripMenuItem->Name = L"sequencesToolStripMenuItem";
			this->sequencesToolStripMenuItem->Size = System::Drawing::Size(75, 20);
			this->sequencesToolStripMenuItem->Text = L"Sequences";
			// 
			// addFromFileToolStripMenuItem
			// 
			this->addFromFileToolStripMenuItem->Name = L"addFromFileToolStripMenuItem";
			this->addFromFileToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->addFromFileToolStripMenuItem->Text = L"Add from file";
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
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripButtonAddSeq, 
				this->toolStripSeparator1, this->toolStripDropDownButton1, this->toolStripSeparator2});
			this->toolStrip1->Location = System::Drawing::Point(3, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(76, 25);
			this->toolStrip1->TabIndex = 0;
			// 
			// toolStripButtonAddSeq
			// 
			this->toolStripButtonAddSeq->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButtonAddSeq->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButtonAddSeq.Image")));
			this->toolStripButtonAddSeq->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonAddSeq->Name = L"toolStripButtonAddSeq";
			this->toolStripButtonAddSeq->Size = System::Drawing::Size(23, 22);
			this->toolStripButtonAddSeq->Text = L"Add Seq";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Location = System::Drawing::Point(0, 658);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(687, 22);
			this->statusStrip1->TabIndex = 2;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Bottom;
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
			this->splitContainer1->Size = System::Drawing::Size(687, 599);
			this->splitContainer1->SplitterDistance = 229;
			this->splitContainer1->TabIndex = 3;
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
			this->splitContainer2->Size = System::Drawing::Size(454, 599);
			this->splitContainer2->SplitterDistance = 149;
			this->splitContainer2->TabIndex = 0;
			// 
			// treeV_Seq
			// 
			this->treeV_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeV_Seq->Location = System::Drawing::Point(0, 0);
			this->treeV_Seq->Name = L"treeV_Seq";
			this->treeV_Seq->Size = System::Drawing::Size(229, 599);
			this->treeV_Seq->TabIndex = 0;
			// 
			// listV_Seq
			// 
			this->listV_Seq->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->Name, this->Lenght});
			this->listV_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listV_Seq->Location = System::Drawing::Point(0, 0);
			this->listV_Seq->Name = L"listV_Seq";
			this->listV_Seq->Size = System::Drawing::Size(454, 149);
			this->listV_Seq->TabIndex = 0;
			this->listV_Seq->UseCompatibleStateImageBehavior = false;
			// 
			// textBox_Seq
			// 
			this->textBox_Seq->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_Seq->Location = System::Drawing::Point(0, 0);
			this->textBox_Seq->Multiline = true;
			this->textBox_Seq->Name = L"textBox_Seq";
			this->textBox_Seq->Size = System::Drawing::Size(454, 446);
			this->textBox_Seq->TabIndex = 0;
			// 
			// toolStripDropDownButton1
			// 
			this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripDropDownButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->showAllSeqToolStripMenuItem, 
				this->onlyCurrentGropSeqToolStripMenuItem});
			this->toolStripDropDownButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripDropDownButton1.Image")));
			this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->Size = System::Drawing::Size(29, 22);
			this->toolStripDropDownButton1->Text = L"toolStripDropDownButton1";
			// 
			// showAllSeqToolStripMenuItem
			// 
			this->showAllSeqToolStripMenuItem->Name = L"showAllSeqToolStripMenuItem";
			this->showAllSeqToolStripMenuItem->Size = System::Drawing::Size(199, 22);
			this->showAllSeqToolStripMenuItem->Text = L"Show all seq";
			// 
			// onlyCurrentGropSeqToolStripMenuItem
			// 
			this->onlyCurrentGropSeqToolStripMenuItem->Name = L"onlyCurrentGropSeqToolStripMenuItem";
			this->onlyCurrentGropSeqToolStripMenuItem->Size = System::Drawing::Size(199, 22);
			this->onlyCurrentGropSeqToolStripMenuItem->Text = L"Only current group seq.";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->allSeqinclInternalGroupsToolStripMenuItem, 
				this->deletedSeqToolStripMenuItem});
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
			// 
			// deletedSeqToolStripMenuItem
			// 
			this->deletedSeqToolStripMenuItem->Checked = true;
			this->deletedSeqToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->deletedSeqToolStripMenuItem->Name = L"deletedSeqToolStripMenuItem";
			this->deletedSeqToolStripMenuItem->Size = System::Drawing::Size(222, 22);
			this->deletedSeqToolStripMenuItem->Text = L"Deleted seq";
			// 
			// SeqExpl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(687, 680);
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
	};
}
