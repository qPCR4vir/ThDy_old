#pragma once
//#include "ThDy_DNAHybrid.VC-WF/StdAfx.h"



#pragma managed	
namespace ThDy_DNAHybridWF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Results
	/// </summary>
	public ref class Results : public System::Windows::Forms::Form
	{
	public:
		Results() 		{	InitializeComponent();  }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Results()
		{
			if (components)
			{
				delete components;
			}
		}

	//protected: 		CTable<float> &_Table ;
	public: System::Windows::Forms::DataGridView^  dg;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
	//private: void UI_Update(){}
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->dg = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dg))->BeginInit();
			this->SuspendLayout();
			// 
			// dg
			// 
			this->dg->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dg->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->dg->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::EnableAlwaysIncludeHeaderText;
			this->dg->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			//this->dg->

			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			dataGridViewCellStyle1->Format = L"N1";
			dataGridViewCellStyle1->NullValue = nullptr;
			dataGridViewCellStyle1->Padding = System::Windows::Forms::Padding(0, 0, 2, 0);
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->dg->DefaultCellStyle = dataGridViewCellStyle1;
			this->dg->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dg->Location = System::Drawing::Point(0, 0);
			this->dg->MinimumSize = System::Drawing::Size(400, 200);
			this->dg->Name = L"dg";
			this->dg->ReadOnly = true;
			this->dg->RowTemplate->ReadOnly = true;
			this->dg->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->dg->Size = System::Drawing::Size(984, 762);
			this->dg->TabIndex = 1;
			// 
			// Results
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(984, 762);
			this->Controls->Add(this->dg);
			this->MinimumSize = System::Drawing::Size(100, 100);
			this->Name = L"Results";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dg))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
