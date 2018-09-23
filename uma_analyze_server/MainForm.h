#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <memory>

#include <msclr/marshal_cppstd.h>

#include "jv_controller/jv_parameter.hpp"
#include "jv_reader/JVDataPool.hpp"
#include "wp_estimator/wp_estimator.hpp"
#include "buyer/Buyer.hpp"

#include "uas_util.h"

namespace umaanalyzeserver {

	namespace heap {
		extern jvdata::datapool::race race_pool;
		extern jvdata::datapool::ming ming_pool;
		extern std::list<wpestimator::mingpoint::RaceInfo> race_info_list;
		extern std::unique_ptr<wpestimator::mingpoint::WinProbabilityDistribution> wp_distribution_ptr;
	}

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainForm の概要
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();

			//
			//TODO: ここにコンストラクター コードを追加します
			//
			this->jv_link->JVInit(gcnew String(jvparameter::sid));
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
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
	private: System::Windows::Forms::Button^  button1;

	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->jv_link))->BeginInit();
			this->SuspendLayout();
			// 
			// jv_link
			// 
			this->jv_link->Enabled = true;
			this->jv_link->Location = System::Drawing::Point(188, 109);
			this->jv_link->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->jv_link->Name = L"jv_link";
			this->jv_link->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"jv_link.OcxState")));
			this->jv_link->Size = System::Drawing::Size(173, 97);
			this->jv_link->TabIndex = 0;
			// 
			// setting_button
			// 
			this->setting_button->Location = System::Drawing::Point(873, 3);
			this->setting_button->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->setting_button->Name = L"setting_button";
			this->setting_button->Size = System::Drawing::Size(127, 42);
			this->setting_button->TabIndex = 1;
			this->setting_button->Text = L"settings";
			this->setting_button->UseVisualStyleBackColor = true;
			this->setting_button->Click += gcnew System::EventHandler(this, &MainForm::setting_button_click);
			// 
			// text_box
			// 
			this->text_box->Location = System::Drawing::Point(20, 50);
			this->text_box->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->text_box->Multiline = true;
			this->text_box->Name = L"text_box";
			this->text_box->ReadOnly = true;
			this->text_box->Size = System::Drawing::Size(977, 366);
			this->text_box->TabIndex = 2;
			// 
			// from_picker
			// 
			this->from_picker->Location = System::Drawing::Point(20, 16);
			this->from_picker->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->from_picker->MaxDate = System::DateTime(2020, 12, 31, 0, 0, 0, 0);
			this->from_picker->MinDate = System::DateTime(1990, 1, 1, 0, 0, 0, 0);
			this->from_picker->Name = L"from_picker";
			this->from_picker->Size = System::Drawing::Size(224, 25);
			this->from_picker->TabIndex = 3;
			// 
			// to_picker
			// 
			this->to_picker->Location = System::Drawing::Point(245, 16);
			this->to_picker->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->to_picker->MaxDate = System::DateTime(2020, 12, 31, 0, 0, 0, 0);
			this->to_picker->MinDate = System::DateTime(1990, 1, 1, 0, 0, 0, 0);
			this->to_picker->Name = L"to_picker";
			this->to_picker->Size = System::Drawing::Size(224, 25);
			this->to_picker->TabIndex = 4;
			// 
			// start_button
			// 
			this->start_button->BackColor = System::Drawing::Color::Transparent;
			this->start_button->Cursor = System::Windows::Forms::Cursors::Cross;
			this->start_button->Location = System::Drawing::Point(479, 6);
			this->start_button->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->start_button->Name = L"start_button";
			this->start_button->Size = System::Drawing::Size(103, 34);
			this->start_button->TabIndex = 5;
			this->start_button->Text = L"Setup";
			this->start_button->UseVisualStyleBackColor = false;
			this->start_button->Click += gcnew System::EventHandler(this, &MainForm::start_button_click);
			// 
			// progress_bar
			// 
			this->progress_bar->Location = System::Drawing::Point(708, 6);
			this->progress_bar->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->progress_bar->Name = L"progress_bar";
			this->progress_bar->Size = System::Drawing::Size(155, 34);
			this->progress_bar->TabIndex = 6;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(592, 6);
			this->button1->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(106, 34);
			this->button1->TabIndex = 7;
			this->button1->Text = L"Simulate";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::simulation_button_click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(10, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1020, 435);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->progress_bar);
			this->Controls->Add(this->start_button);
			this->Controls->Add(this->to_picker);
			this->Controls->Add(this->from_picker);
			this->Controls->Add(this->text_box);
			this->Controls->Add(this->setting_button);
			this->Controls->Add(this->jv_link);
			this->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->jv_link))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void step_download(System::String^ dataspec, System::DateTime^ from_date) {
		const int option = 3;
		int read_count;
		int download_count;
		System::String^ latest_file;
		
		// open file
		int res = jv_link->JVOpen(dataspec,
			from_date->ToString("yyyymmdd"),
			option,
			read_count,
			download_count,
			latest_file);

		if (res != 0) {
			std::stringstream ss;
			ss << __FUNCTION__ << __LINE__ << "-error code: " << res << std::endl;
			throw std::runtime_error(ss.str());
		}
		
		// observe donload progress
		int remain = download_count;
		this->progress_bar->Maximum = download_count;

		while (remain > 0) {
			// sleep 100 [ms]
			System::Threading::Thread::Sleep(100);
			
			int status = jv_link->JVStatus();
			if (status < 0) {
				std::stringstream ss;
				ss << __FUNCTION__ << __LINE__ << " - error code: " << status << std::endl;
				throw std::runtime_error(ss.str());
			}

			this->progress_bar->Value = status;
			remain = download_count - status;
		}

		this->progress_bar->Value = 0;

	}

	private: 
		System::Void pour_data_to_pool() {
		{
			int read_count;
			System::String^ buf;
			int size;
			System::String^ file_name;
			
			bool show_name = true;
			while ((read_count = jv_link->JVRead(buf, size, file_name)) != 0) {
				if (show_name) {
					uas_print_message(this->text_box, file_name);
					uas_print_message(this->text_box, "\n");
					show_name = false;
				}
				
				if (auto res = heap::race_pool(to_string(buf))) {
					uas_print_message(this->text_box, res.get());
				}

				else if (auto res = heap::ming_pool(to_string(buf))) {
					uas_print_message(this->text_box, res.get());
				}

				if (read_count == -1) show_name = true;
			}
		}
	}

	private: System::Void setting_button_click(System::Object^ sender, System::EventArgs^  e) {
		// Call jvlink property method
		if (long r = this->jv_link->JVSetUIProperties() != 0) {
			uas_print_error(this->text_box, std::to_string(r));
		}
	}

	private: System::Void start_button_click(System::Object^  sender, System::EventArgs^  e) {
		
		uas_print_message(this->text_box, from_picker->Value, to_picker->Value);
		
		// Make jvlink to download needed data from JRA
		step_download("RACE", from_picker->Value);
		step_download("MING", from_picker->Value);
		
		// Read data from jvlink and pour it to datapool
		pour_data_to_pool();
		
		// Extruct race info from datapool 
		heap::race_info_list = wpestimator::mingpoint::extruct_race_info(heap::race_pool, heap::ming_pool);

		// Make distribution
		heap::wp_distribution_ptr = std::make_unique<wpestimator::mingpoint::WinProbabilityDistribution>(heap::race_info_list);
		
	}

	private: System::Void simulation_button_click(System::Object^  sender, System::EventArgs^  e) {
		assert(heap::wp_distribution_ptr);

	}

};
}
