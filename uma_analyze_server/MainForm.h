#pragma once

#include <string>
#include "jv_controller/jv_parameter.hpp"

namespace umaanalyzeserver {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainForm �̊T�v
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();

			//
			//TODO: �����ɃR���X�g���N�^�[ �R�[�h��ǉ����܂�
			//
			this->jv_link->JVInit(gcnew String(jvparameter::sid));
		}

	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: AxJVDTLabLib::AxJVLink^  jv_link;
	private: System::Windows::Forms::Button^  setting_button;
	private: System::Windows::Forms::TextBox^  text_box;
	private: System::Windows::Forms::DateTimePicker^  from_picker;
	private: System::Windows::Forms::DateTimePicker^  to_picker;


	private: System::Windows::Forms::Button^  start_button;
	private: System::Windows::Forms::ProgressBar^  progress_bar;



	protected:


	protected:

	private:
		/// <summary>
		/// �K�v�ȃf�U�C�i�[�ϐ��ł��B
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i�[ �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�[�ŕύX���Ȃ��ł��������B
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->jv_link = (gcnew AxJVDTLabLib::AxJVLink());
			this->setting_button = (gcnew System::Windows::Forms::Button());
			this->text_box = (gcnew System::Windows::Forms::TextBox());
			this->from_picker = (gcnew System::Windows::Forms::DateTimePicker());
			this->to_picker = (gcnew System::Windows::Forms::DateTimePicker());
			this->start_button = (gcnew System::Windows::Forms::Button());
			this->progress_bar = (gcnew System::Windows::Forms::ProgressBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->jv_link))->BeginInit();
			this->SuspendLayout();
			// 
			// jv_link
			// 
			this->jv_link->Enabled = true;
			this->jv_link->Location = System::Drawing::Point(188, 109);
			this->jv_link->Name = L"jv_link";
			this->jv_link->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"jv_link.OcxState")));
			this->jv_link->Size = System::Drawing::Size(173, 97);
			this->jv_link->TabIndex = 0;
			// 
			// setting_button
			// 
			this->setting_button->Location = System::Drawing::Point(524, 2);
			this->setting_button->Name = L"setting_button";
			this->setting_button->Size = System::Drawing::Size(76, 28);
			this->setting_button->TabIndex = 1;
			this->setting_button->Text = L"settings";
			this->setting_button->UseVisualStyleBackColor = true;
			this->setting_button->Click += gcnew System::EventHandler(this, &MainForm::setting_button_click);
			// 
			// text_box
			// 
			this->text_box->Location = System::Drawing::Point(12, 33);
			this->text_box->Multiline = true;
			this->text_box->Name = L"text_box";
			this->text_box->ReadOnly = true;
			this->text_box->Size = System::Drawing::Size(588, 245);
			this->text_box->TabIndex = 2;
			// 
			// from_picker
			// 
			this->from_picker->Location = System::Drawing::Point(12, 11);
			this->from_picker->MaxDate = System::DateTime(2020, 12, 31, 0, 0, 0, 0);
			this->from_picker->MinDate = System::DateTime(1990, 1, 1, 0, 0, 0, 0);
			this->from_picker->Name = L"from_picker";
			this->from_picker->Size = System::Drawing::Size(136, 19);
			this->from_picker->TabIndex = 3;
			// 
			// to_picker
			// 
			this->to_picker->Location = System::Drawing::Point(154, 11);
			this->to_picker->MaxDate = System::DateTime(2020, 12, 31, 0, 0, 0, 0);
			this->to_picker->MinDate = System::DateTime(1990, 1, 1, 0, 0, 0, 0);
			this->to_picker->Name = L"to_picker";
			this->to_picker->Size = System::Drawing::Size(136, 19);
			this->to_picker->TabIndex = 4;
			// 
			// start_button
			// 
			this->start_button->BackColor = System::Drawing::Color::Maroon;
			this->start_button->Cursor = System::Windows::Forms::Cursors::Cross;
			this->start_button->Location = System::Drawing::Point(296, 9);
			this->start_button->Name = L"start_button";
			this->start_button->Size = System::Drawing::Size(23, 21);
			this->start_button->TabIndex = 5;
			this->start_button->UseVisualStyleBackColor = false;
			this->start_button->Click += gcnew System::EventHandler(this, &MainForm::start_button_click);
			// 
			// progress_bar
			// 
			this->progress_bar->Location = System::Drawing::Point(325, 7);
			this->progress_bar->Name = L"progress_bar";
			this->progress_bar->Size = System::Drawing::Size(110, 23);
			this->progress_bar->TabIndex = 6;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(612, 290);
			this->Controls->Add(this->progress_bar);
			this->Controls->Add(this->start_button);
			this->Controls->Add(this->to_picker);
			this->Controls->Add(this->from_picker);
			this->Controls->Add(this->text_box);
			this->Controls->Add(this->setting_button);
			this->Controls->Add(this->jv_link);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->jv_link))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:

		template<int N>
		std::string to_string(const char(&str)[N]) {
			return std::string(str);
		}

		void print_text(String^ str) {
			this->text_box->AppendText(str);
		}

		void print_text(std::string str) {
			this->text_box->AppendText(gcnew String(str.c_str()));
		}

		template<int N>
		void print_text(const char(&str)[N]) {
			print_text(std::string(str));
		}

		void print_log(const std::string str) {
			this->text_box->AppendText(gcnew String(str.c_str()));
		}
		
		template<int N>
		void print_log(const char(&str)[N]) {
			print_log(std::string(str));
		}

		void print_date(System::DateTime^ date) {
			print_text(date->ToString("yyyymmdd"));
		}

		void print_term(System::DateTime^ from, System::DateTime^ to) {
			print_text("From: ");
			print_date(from);
			print_text(", To: ");
			print_date(to);
			print_text("\n");
		}

	private: System::Void download_file(System::DateTime^ from_date) {
		const int option = 3;
		int readcount;
		int downloadcount;
		System::String^ latest_file;

		int res = jv_link->JVOpen("RACE",
			from_date->ToString("yyyymmdd"),
			option,
			readcount,
			downloadcount,
			latest_file);

		if (res != 0) {
			print_log(__FUNCTION__);
			print_log(std::string(" - error code: "));
			print_log(std::to_string(res));
		}

	}

	private: System::Void setting_button_click(System::Object^  sender, System::EventArgs^  e) {
		if (long r = this->jv_link->JVSetUIProperties() != 0) {
			print_log(std::to_string(r));
		}
	}

	private: System::Void start_button_click(System::Object^  sender, System::EventArgs^  e) {

		print_term(from_picker->Value, to_picker->Value);

		download_file(from_picker->Value);

	}

	};
}
