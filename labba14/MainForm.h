/*#pragma once
#include "TariffsForm.h"
#include "TariffsFunctions.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class MainForm : public Form
{
public:
    MainForm()
    {
        funcs = gcnew TariffsFunctions();
        InitializeComponent(false);
    }

private:
    bool isAdmin;
    TariffsFunctions^ funcs;
    array<Button^>^ btns;

    void InitializeComponent(bool a)
    {
        isAdmin = a;
        btns = gcnew array<Button^>(3);
        BackColor = System::Drawing::Color::FromArgb(255, 182, 193);

        array<String^>^ n = {
            a ? "Отключить администратора" : "Включить администратора",
            "Тарифы","Выход" };

        for (int i = 0;i < 3;i++) {
            btns[i] = gcnew Button();
            btns[i]->Text = n[i];
            btns[i]->Size = System::Drawing::Size(300, 30);
            btns[i]->Location = System::Drawing::Point(20, 20 + i * 40);
            btns[i]->BackColor = System::Drawing::Color::FromArgb(48, 25, 52);
            btns[i]->ForeColor = System::Drawing::Color::White;
            Controls->Add(btns[i]);
        }

        btns[0]->Click += gcnew EventHandler(this, &MainForm::Toggle);
        btns[1]->Click += gcnew EventHandler(this, &MainForm::Open);
        btns[2]->Click += gcnew EventHandler(this, &MainForm::Exit);

        Text = "Фирма грузоперевозок";
        Size = System::Drawing::Size(360, 200);
    }

    void Toggle(Object^, EventArgs^)
    {
        isAdmin = !isAdmin;
        btns[0]->Text = isAdmin ? "Отключить администратора" : "Включить администратора";
    }
    void Open(Object^, EventArgs^) { gcnew TariffsForm(isAdmin, funcs)->Show(); }
    void Exit(Object^, EventArgs^) { Application::Exit(); }
};
*/
#pragma once
#include "TariffsForm.h"
#include "TariffsFunctions.h"

namespace labba14
{
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MainForm : public Form
    {
    public:
        MainForm()
        {
            isAdmin = false;
            funcs = gcnew TariffsFunctions();
            InitializeComponent();
        }

    private:
        bool isAdmin;
        TariffsFunctions^ funcs;
        array<Button^>^ btns;

        void InitializeComponent()
        {
            btns = gcnew array<Button^>(3);
            BackColor = System::Drawing::Color::FromArgb(255, 182, 193);

            array<String^>^ n = {
                "Включить администратора",
                "Тарифы",
                "Выход"
            };

            for (int i = 0; i < 3; i++)
            {
                btns[i] = gcnew Button();
                btns[i]->Text = n[i];
                btns[i]->Size = System::Drawing::Size(300, 30);
                btns[i]->Location = System::Drawing::Point(20, 20 + i * 40);
                btns[i]->BackColor = System::Drawing::Color::FromArgb(48, 25, 52);
                btns[i]->ForeColor = System::Drawing::Color::White;
                Controls->Add(btns[i]);
            }

            btns[0]->Click += gcnew EventHandler(this, &MainForm::Toggle);
            btns[1]->Click += gcnew EventHandler(this, &MainForm::OpenTariffs);
            btns[2]->Click += gcnew EventHandler(this, &MainForm::ExitApp);

            Text = "Фирма грузоперевозок";
            Size = System::Drawing::Size(360, 200);
        }

        void Toggle(Object^, EventArgs^)
        {
            isAdmin = !isAdmin;
            btns[0]->Text = isAdmin
                ? "Отключить администратора"
                : "Включить администратора";
        }

        void OpenTariffs(Object^, EventArgs^)
        {
            TariffsForm^ f = gcnew TariffsForm(isAdmin, funcs);
            f->ShowDialog();
        }

        void ExitApp(Object^, EventArgs^)
        {
            Application::Exit();
        }
    };
}
