#include "MainForm.h"
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	// gcnewプロジェクト名:: Form名()
	Application::Run(gcnew umaanalyzeserver::MainForm());
	return 0;
}