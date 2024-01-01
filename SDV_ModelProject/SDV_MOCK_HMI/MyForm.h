#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define LOG_FILE_PATH "C:\\temp\\CentralComputeOutput.txt"

#pragma once
namespace SDVMOCKHMI {



	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ lblBattery;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ lblHotSpots;
	private: System::Windows::Forms::Label^ lblWeather;
	private: System::Windows::Forms::Timer^ tmTimer;
	private: System::Windows::Forms::RichTextBox^ txtLogs;
	private: System::Windows::Forms::CheckBox^ chkPowerOn;
	private: System::Windows::Forms::Label^ lblSensor;
	private: System::Windows::Forms::Label^ label5;
	private: System::ComponentModel::IContainer^ components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->lblBattery = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lblHotSpots = (gcnew System::Windows::Forms::Label());
			this->lblWeather = (gcnew System::Windows::Forms::Label());
			this->tmTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->txtLogs = (gcnew System::Windows::Forms::RichTextBox());
			this->chkPowerOn = (gcnew System::Windows::Forms::CheckBox());
			this->lblSensor = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(1, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(761, 463);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// lblBattery
			// 
			this->lblBattery->AutoSize = true;
			this->lblBattery->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblBattery->ForeColor = System::Drawing::Color::LawnGreen;
			this->lblBattery->Location = System::Drawing::Point(112, 476);
			this->lblBattery->Name = L"lblBattery";
			this->lblBattery->Size = System::Drawing::Size(26, 25);
			this->lblBattery->TabIndex = 2;
			this->lblBattery->Text = L"--";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(12, 512);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(93, 25);
			this->label1->TabIndex = 3;
			this->label1->Text = L"HotSpots";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(12, 551);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(87, 25);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Weather";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(12, 476);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(73, 25);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Battery";
			// 
			// lblHotSpots
			// 
			this->lblHotSpots->AutoSize = true;
			this->lblHotSpots->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblHotSpots->ForeColor = System::Drawing::Color::LawnGreen;
			this->lblHotSpots->Location = System::Drawing::Point(112, 512);
			this->lblHotSpots->Name = L"lblHotSpots";
			this->lblHotSpots->Size = System::Drawing::Size(26, 25);
			this->lblHotSpots->TabIndex = 2;
			this->lblHotSpots->Text = L"--";
			// 
			// lblWeather
			// 
			this->lblWeather->AutoSize = true;
			this->lblWeather->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblWeather->ForeColor = System::Drawing::Color::LawnGreen;
			this->lblWeather->Location = System::Drawing::Point(112, 551);
			this->lblWeather->Name = L"lblWeather";
			this->lblWeather->Size = System::Drawing::Size(26, 25);
			this->lblWeather->TabIndex = 6;
			this->lblWeather->Text = L"--";
			// 
			// tmTimer
			// 
			this->tmTimer->Enabled = true;
			this->tmTimer->Interval = 5000;
			this->tmTimer->Tick += gcnew System::EventHandler(this, &MyForm::tmTimer_Tick);
			// 
			// txtLogs
			// 
			this->txtLogs->BackColor = System::Drawing::Color::DarkSeaGreen;
			this->txtLogs->ForeColor = System::Drawing::Color::Brown;
			this->txtLogs->Location = System::Drawing::Point(330, 473);
			this->txtLogs->Name = L"txtLogs";
			this->txtLogs->Size = System::Drawing::Size(421, 137);
			this->txtLogs->TabIndex = 8;
			this->txtLogs->Text = L"Logs";
			// 
			// chkPowerOn
			// 
			this->chkPowerOn->Appearance = System::Windows::Forms::Appearance::Button;
			this->chkPowerOn->AutoSize = true;
			this->chkPowerOn->BackColor = System::Drawing::Color::Black;
			this->chkPowerOn->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->chkPowerOn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->chkPowerOn->ForeColor = System::Drawing::Color::LawnGreen;
			this->chkPowerOn->Location = System::Drawing::Point(333, 9);
			this->chkPowerOn->Name = L"chkPowerOn";
			this->chkPowerOn->Size = System::Drawing::Size(120, 35);
			this->chkPowerOn->TabIndex = 9;
			this->chkPowerOn->TabStop = false;
			this->chkPowerOn->Text = L"Power ON";
			this->chkPowerOn->UseVisualStyleBackColor = false;
			this->chkPowerOn->CheckedChanged += gcnew System::EventHandler(this, &MyForm::chkPowerOn_CheckedChanged);
			// 
			// lblSensor
			// 
			this->lblSensor->AutoSize = true;
			this->lblSensor->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSensor->ForeColor = System::Drawing::Color::LawnGreen;
			this->lblSensor->Location = System::Drawing::Point(112, 585);
			this->lblSensor->Name = L"lblSensor";
			this->lblSensor->Size = System::Drawing::Size(26, 25);
			this->lblSensor->TabIndex = 11;
			this->lblSensor->Text = L"--";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(12, 585);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(75, 25);
			this->label5->TabIndex = 10;
			this->label5->Text = L"Sensor";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::SaddleBrown;
			this->ClientSize = System::Drawing::Size(759, 618);
			this->Controls->Add(this->lblSensor);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->chkPowerOn);
			this->Controls->Add(this->txtLogs);
			this->Controls->Add(this->lblWeather);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lblHotSpots);
			this->Controls->Add(this->lblBattery);
			this->Controls->Add(this->pictureBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"SANTA CAR Sleightainment HMI";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion




		/// <summary>
		/// Auto refreshing the HMI
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns></returns>
	private: System::Void tmTimer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		//Just sample representation iun txt boxes
		if (chkPowerOn->Checked)
		{
			std::string line;
			std::ifstream rfile;
			rfile.open(LOG_FILE_PATH, ios::in);

			if (rfile.is_open()) {
				while (std::getline(rfile, line)) {

					if (line.find("System:") != std::string::npos)
					{
						lblBattery->Text = "Charging";
					}
					else if (line.find("Connectivity:") != std::string::npos)
					{
						lblHotSpots->Text = "Available";
					}
					else if (line.find("Cloud:") != std::string::npos)
					{
						lblWeather->Text = "Forecast available";
					}
					else if (line.find("Sensor:") != std::string::npos)
					{
						lblSensor->Text = "Sensor activated";
					}

					txtLogs->Text = gcnew String(line.c_str());
				}
			}
			rfile.close();
		}

	}

	private: System::Void chkPowerOn_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

		if (chkPowerOn->Checked)
			chkPowerOn->Text = "Power Off";
		else
			chkPowerOn->Text = "Power On";
	}
};
}
