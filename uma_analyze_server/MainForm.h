#pragma once

#include <string>
#include <sstream>
#include <iomanip>
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
	/// MainForm �̊T�v
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
    private:
        static constexpr char* dataspec_ = "RACEMING";

	private:
        enum struct FormState {
            E_Nop,
            E_JVLink_Setting,
            E_JVLink_Provisioning,
            E_Data_Gathering,
            E_Simulationg
        };

        FormState form_state_;

	public:
		MainForm(void)
		{
			InitializeComponent();

			//
			//TODO: �����ɃR���X�g���N�^�[ �R�[�h��ǉ����܂�
			//
            form_state_ = FormState::E_Nop;
			this->jv_link->JVInit(gcnew String(jvparameter::sid));
            this->to_picker->Value = System::DateTime::Now;
            this->from_picker->Value = System::DateTime::Now.AddMonths(-1);

            BeginInvoke(
                gcnew ProcessCallback(this, &MainForm::provision_jvlink)
            );
            
		}

	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
		/// </summary>
		~MainForm()
		{
            this->jv_link->JVClose();

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
	private: System::Windows::Forms::ProgressBar^  progressbar;
	private: System::Windows::Forms::Button^  button1;


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
            this->progressbar = (gcnew System::Windows::Forms::ProgressBar());
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
            this->jv_link->Size = System::Drawing::Size(259, 145);
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
            this->text_box->Size = System::Drawing::Size(1900, 1234);
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
            this->from_picker->Value = System::DateTime(2011, 6, 21, 0, 0, 0, 0);
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
            this->to_picker->Value = System::DateTime(2018, 9, 23, 0, 0, 0, 0);
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
            // progressbar
            // 
            this->progressbar->Location = System::Drawing::Point(708, 6);
            this->progressbar->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
            this->progressbar->Name = L"progressbar";
            this->progressbar->Size = System::Drawing::Size(155, 34);
            this->progressbar->TabIndex = 6;
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
            this->ClientSize = System::Drawing::Size(1934, 1297);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->progressbar);
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

    private: void provision_jvlink() {
        System::String^ dataspec = gcnew System::String(dataspec_);
		const int option = 3;
		System::String^ fromtime("00000000000000");
		int read_count;
		int download_count;
		System::String^ latest_file;

        // Open
        {
            int res = jv_link->JVOpen((System::String^)dataspec,
                fromtime,
                option,
                read_count,
                download_count,
                latest_file);

            if (res != 0) {
                std::stringstream ss;
                ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << res << std::endl;
                throw std::runtime_error(ss.str());
            }
        }
		
		// observe donload progress
		int remain = download_count;
        
        invoke_progressbar_set_maximum(download_count);
               
		while (remain > 0) {
			// sleep 100 [ms]
			System::Threading::Thread::Sleep(100);
			
			int status = jv_link->JVStatus();
			if (status < 0) {
				std::stringstream ss;
				ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << status << std::endl;
				throw std::runtime_error(ss.str());
			}

            invoke_progressbar_set_value(status);

			remain = download_count - status;

		}

        invoke_progressbar_set_value(download_count);

        // Close
        {
            int res = jv_link->JVClose();

            if (res != 0) {
                std::stringstream ss;
                ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << res << std::endl;
                throw std::runtime_error(ss.str());
            }

        }

	}

    private: delegate void ProcessCallback();

    private: delegate void ProgressCallback(int);
    
    private: void progressbar_set_maximum_(int maximum) {
        progressbar->Maximum = maximum;
    }

    private: void invoke_progressbar_set_maximum(int maximum) {
        BeginInvoke(
            gcnew ProgressCallback(this, &MainForm::progressbar_set_maximum_),
            maximum 
        );
    }
    
    private: void progressbar_set_value_(int value) {
        progressbar->Value = value;
    }

    private: void invoke_progressbar_set_value(int value) {
        BeginInvoke(
            gcnew ProgressCallback(this, &MainForm::progressbar_set_value_),
            value 
        );
    }
             
    private: delegate void TextboxCallback(System::String^ str);
    private: void print_message_(System::String^ str) {
        uas_print_message(this->text_box, str);
    }

    private: void invoke_print_message(System::String^ str) {
        BeginInvoke(
            gcnew TextboxCallback(this, &MainForm::print_message_),
            str 
        );
    }

    private: void invoke_print_message(std::string str) {
        BeginInvoke(
            gcnew TextboxCallback(this, &MainForm::print_message_),
            to_system_string(str)
        );
    }
         
    private: delegate void DatePickerCallback(cli::array<System::String^>^% date);
    
    void get_fromtime_(cli::array<System::String^>^% date) {
        date[0] = from_picker->Value.ToString("yyyyMMdd");
    }

    void get_totime_(cli::array<System::String^>^% date) {
        date[0] = to_picker->Value.ToString("yyyyMMdd");
    }

    void invoke_get_fromtime(cli::array<System::String^>^% date) {
        Invoke(
            gcnew DatePickerCallback(this, &MainForm::get_fromtime_),
            (Object^)date
        );
    }

    void invoke_get_totime(cli::array<System::String^>^% date) {
        Invoke(
            gcnew DatePickerCallback(this, &MainForm::get_totime_),
            (Object^)date
        );
    }

    private: delegate int JVReadCallback(cli::array<System::String^>^% buf, int bufsize, cli::array<System::String^>^% filename);

    int jvread_(cli::array<System::String^>^% buf, int bufsize, cli::array<System::String^>^% filename) {
        System::String^ b;
        System::String^ f;
        int res = jv_link->JVRead(b, bufsize, f);
        
        buf[0] = b;
        filename[0] = f;

        return res;
    }

    int invoke_jvread(cli::array<System::String^>^% buf, int bufsize, cli::array<System::String^>^% filename) {

        return (int)Invoke(
            gcnew JVReadCallback(this, &MainForm::jvread_),
            buf, bufsize, filename
        );

    }

    private: delegate void JVSkipCallback();

    void jvskip_() {
        jv_link->JVSkip();
    }

    void invoke_jvskip() {
        Invoke(
            gcnew JVSkipCallback(this, &MainForm::jvskip_)
        );

    }

    private: delegate void OpenDataSetCallback(System::String^ fromtime);
    void open_dataset_(System::String^ fromtime) {
        open_dataset(fromtime);
    }

    void invoke_open_dataset(System::String^ fromtime) {
        Invoke(
            gcnew OpenDataSetCallback(this, &MainForm::open_dataset_),
            fromtime
        );
    }

    private: delegate void CloseDataSetCallback();
    void close_dataset_() {
        close_dataset();
    }
            
    void invoke_close_dataset() {
        Invoke(
            gcnew CloseDataSetCallback(this, &MainForm::close_dataset_)
        );
    }

    private: void fill_datapool() {
        // Read data from jvlink and pour it to datapool
        cli::array<System::String^>^ from_time = gcnew cli::array<System::String^>(1);
        cli::array<System::String^>^ to_time = gcnew cli::array<System::String^>(1);

        from_time[0] = gcnew System::String(""); 
        to_time[0] = gcnew System::String(""); 

        invoke_get_fromtime(from_time);
        invoke_get_totime(to_time);

        int from = Convert::ToInt32(from_time[0]);
        int to = Convert::ToInt32(to_time[0]);

        int read_count;
        int buf_size = 150000 ;

        cli::array<System::String^>^ buf = gcnew cli::array<System::String^>(1);
        cli::array<System::String^>^ file_name = gcnew cli::array<System::String^>(1);

        buf[0] = gcnew System::String("");
        file_name[0] = gcnew System::String("");

        bool filechanged = true;
        int readed_file = 0;

        for (int i = 0; i < 2; ++i) {
            while ((read_count = invoke_jvread(buf, buf_size, file_name)) != 0) {

                jvdata::datapool::race rp_test;
                jvdata::datapool::ming mp_test;

                if (read_count < -1) {
                    std::stringstream ss;
                    ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << read_count << std::endl;
                    throw std::runtime_error(ss.str());
                }

                else if (read_count == -1) {
                    filechanged = true;
                }

                else {

                    if (filechanged) {
                        ++readed_file;
                        filechanged = false;

                        invoke_print_message("\n");
                        invoke_print_message(file_name[0]);

                        std::stringstream ss;
                        ss << " " << readed_file << std::endl;
                        invoke_print_message(ss.str().c_str());

                    }

                    if (auto res = rp_test(to_string(buf[0]))) {
                        //invoke_print_message(uas_generate_message(res.get()));
                        int date
                            = std::stoi(std::string(res.get().Year, 4)) * 10000
                            + std::stoi(std::string(res.get().MonthDay, 4));

                        assert(date >= 19900101);
                        assert(date <= 20300101);

                        if (date < from || date > to) {
                            invoke_jvskip();
                            invoke_print_message(date.ToString());
                            invoke_print_message(" skipped\n");
                            filechanged = true;
                        }

                        else {
                            heap::race_pool(to_string(buf[0]));
                            invoke_print_message("o[");
                            invoke_print_message(date.ToString());
                            invoke_print_message("]");
                            invoke_print_message(to_string(buf[0]).substr(0, 64));
                            invoke_print_message("\n");
                        }

                    }

                    else if (auto res = mp_test(to_string(buf[0]))) {
                        //invoke_print_message(uas_generate_message(res.get()));
                        int date
                            = std::stoi(std::string(res.get().Year, 4)) * 10000
                            + std::stoi(std::string(res.get().MonthDay, 4));

                        assert(date >= 19900101);
                        assert(date <= 20300101);

                        if (date < from || date > to) {
                            invoke_jvskip();
                            invoke_print_message(date.ToString());
                            invoke_print_message(" skipped\n");
                            filechanged = true;
                        }

                        else {
                            heap::ming_pool(to_string(buf[0]));
                            invoke_print_message("o[");
                            invoke_print_message(date.ToString());
                            invoke_print_message("]");
                            invoke_print_message(to_string(buf[0]).substr(0, 64));
                            invoke_print_message("\n");
                        }

                    }

                    else {
                        invoke_print_message("x");
                        invoke_print_message(to_string(buf[0]).substr(0, 64));
                        invoke_print_message("\n");
                        invoke_jvskip();
                        invoke_print_message("skipped\n");
                        filechanged = true;
                    }

                }
            }
            invoke_print_message("_________________________________________________\n");
            invoke_print_message("_________________________________________________\n");
            invoke_print_message("_________________________________________________\n");
        }

        // Extruct race info from datapool 
        //wpestimator::mingpoint::extruct_race_info(heap::race_info_list, heap::race_pool, heap::ming_pool);
        std::list<wpestimator::mingpoint::RaceInfo> tmp = wpestimator::mingpoint::extruct_race_info(heap::race_pool, heap::ming_pool);
        heap::race_info_list = std::move(tmp);

        // Make distribution
        heap::wp_distribution_ptr = std::make_unique<wpestimator::mingpoint::WinProbabilityDistribution>(heap::race_info_list);

        invoke_print_message("datapoolfilled");

        form_state_ = FormState::E_Nop;
    }

    private: void open_dataset(System::String^ fromtime) {
        System::String^ dataspec = gcnew System::String(dataspec_);
        const int option = 4;
        int read_count;
        int download_count;
        System::String^ latest_file;

        int res = jv_link->JVOpen((System::String^)dataspec,
            fromtime,
            option,
            read_count,
            download_count,
            latest_file);

        if (res != 0) {
            std::stringstream ss;
            ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << res << std::endl;
            throw std::runtime_error(ss.str());
        }

    }

    private: void close_dataset() {
        // Close
        int res = jv_link->JVClose();

        if (res != 0) {
            std::stringstream ss;
            ss << "[" << __FUNCTION__ << __LINE__ << "]" << "error code: " << res << std::endl;
            throw std::runtime_error(ss.str());
        }

    }

    private: 
    void start_button_process(){
        invoke_print_message("Start jvlink provisioning\n");

        try {
            invoke_open_dataset(from_picker->Value.ToString("yyyyMMddhhmmss"));

            invoke_print_message("Filling datapool\n");
            fill_datapool();

            invoke_print_message("datapool filled\n");
            invoke_close_dataset();
        }

        catch (const std::runtime_error& error) {
            std::cerr << error.what() << std::endl;
            exit(EXIT_FAILURE);
        }

    }

	private: System::Void setting_button_click(System::Object^ sender, System::EventArgs^  e) {
        if (form_state_ != FormState::E_Nop) return;

		// Call jvlink property method
		if (long r = this->jv_link->JVSetUIProperties() != 0) {
			uas_print_error(this->text_box, std::to_string(r));
		}
	}

	private: System::Void start_button_click(System::Object^  sender, System::EventArgs^  e) {
        if (form_state_ != FormState::E_Nop) return;

        form_state_ = FormState::E_JVLink_Provisioning;

        System::Threading::Thread^ thread = gcnew System::Threading::Thread(
            gcnew System::Threading::ThreadStart(this, &MainForm::start_button_process)
        );

        thread->IsBackground = true;
        thread->Start();

	}

    private: System::Void simulation_button_click(System::Object^  sender, System::EventArgs^  e) {
        if (form_state_ != FormState::E_Nop) return;

        assert(heap::wp_distribution_ptr);

        form_state_ = FormState::E_Simulationg;

        constexpr unsigned int horse_num = 17;

        wpestimator::mingpoint::Simulator simulator(horse_num);
        std::vector<unsigned int> ming_point_vec{ 10, 70, 130, 190, 250, 310, 370, 330, 390, 450, 510, 570, 630, 700, 760, 820, 880 };

        auto result = simulator(*heap::wp_distribution_ptr, ming_point_vec);

        {
            std::stringstream ss;
            ss << std::fixed;
            ss << "ming_point_vec: " << std::setprecision(1) << ming_point_vec << std::endl;
            invoke_print_message(ss.str());
        }
        
        {
            std::stringstream ss;
            ss << std::fixed;
            ss << "result        : " << std::endl;
            invoke_print_message(ss.str());
            ss.str("");

            for (const auto& r : result) {
                ss << std::setprecision(2) << r << std::endl;
                invoke_print_message(ss.str());
                ss.str("");
            }

        }

        {
            std::stringstream ss;
            ss << std::fixed;
            constexpr unsigned int step = 50;
            constexpr unsigned int point_num = 20;

            for (unsigned int i = 0; i < point_num; ++i) {
                for (unsigned int j = 0; j < point_num; ++j) {
                    ss << std::setprecision(4) << heap::wp_distribution_ptr->operator()({ i * step, j * step }) << ' ';
                }

                ss << std::endl;
                invoke_print_message(ss.str());
                ss.str("");
            }

        }

        form_state_ = FormState::E_Nop;
	}

};
}
