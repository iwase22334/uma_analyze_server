#include "MainForm.h"
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	// gcnew�v���W�F�N�g��:: Form��()
	Application::Run(gcnew umaanalyzeserver::MainForm());
	return 0;
}