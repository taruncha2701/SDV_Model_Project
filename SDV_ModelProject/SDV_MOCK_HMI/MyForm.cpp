#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(0);
    SDVMOCKHMI::MyForm form;
    Application::Run(% form);
}