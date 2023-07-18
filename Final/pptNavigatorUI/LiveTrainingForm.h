#pragma once
#include <msclr\marshal_cppstd.h>


namespace pptNavigatorUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LiveTrainingForm
	/// </summary>
	public ref class LiveTrainingForm : public System::Windows::Forms::Form
	{
	public:
		Form ^obj;
		
		LiveTrainingForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	//	LiveTrainingForm(Form ^obj1)
	//	{
		//	obj=obj1;
		//	InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
	//	}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LiveTrainingForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;

	private: System::Windows::Forms::Button^  button2;


	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(LiveTrainingForm::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::HighlightText;
			this->label1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 16, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::MenuText;
			this->label1->Location = System::Drawing::Point(272, 103);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(348, 44);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Enter word to train";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Click += gcnew System::EventHandler(this, &LiveTrainingForm::label1_Click);
			// 
			// listBox1
			// 
			this->listBox1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->listBox1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 30;
			this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"goto", L"open", L"next", L"back", L"close", L"two", 
				L"three", L"four", L"five", L"six", L"present"});
			this->listBox1->Location = System::Drawing::Point(276, 197);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(344, 184);
			this->listBox1->TabIndex = 1;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &LiveTrainingForm::listBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::SystemColors::Window;
			this->label2->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::Black;
			this->label2->Location = System::Drawing::Point(283, 147);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(173, 33);
			this->label2->TabIndex = 2;
			this->label2->Text = L"List of words";
			this->label2->Click += gcnew System::EventHandler(this, &LiveTrainingForm::label2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(369, 410);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(207, 37);
			this->textBox1->TabIndex = 3;
			// 
			// button2
			// 
			this->button2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button2->AutoSize = true;
			this->button2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button2.BackgroundImage")));
			this->button2->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button2->ForeColor = System::Drawing::Color::Black;
			this->button2->Location = System::Drawing::Point(369, 482);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(198, 52);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Start";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &LiveTrainingForm::button2_Click);
			// 
			// LiveTrainingForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Info;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(930, 764);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->label1);
			this->ForeColor = System::Drawing::Color::DodgerBlue;
			this->Name = L"LiveTrainingForm";
			this->Text = L"LiveTrainingForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: String^ GetData() {
				return textBox1->Text;
			}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void listBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
			// obj->ShowDialog();
			// obj->Show();
		 }
};
}
