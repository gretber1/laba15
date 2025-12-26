#include "TariffsForm.h"
#include "PromptHelpers.h"

using namespace System::Drawing;
using namespace System::IO;
using namespace System::Windows::Forms;

TariffsForm::TariffsForm(bool adminMode)
{
    isAdmin = adminMode;
    isSaved = true;
    tariffs = gcnew TariffsFunctions();
    InitializeComponent();
    tariffs->LoadFromDB();   
    RefreshGrid();
    UpdateButtons();
}


TariffsForm::TariffsForm(bool admin, TariffsFunctions^ f)
{
    isAdmin = admin;
    isSaved = true;
    tariffs = f;
    InitializeComponent();
    tariffs->LoadFromDB();
    RefreshGrid();
    UpdateButtons();
}

TariffsForm::~TariffsForm()
{
    if (components) delete components;
}

void TariffsForm::InitializeComponent()
{
    components = gcnew System::ComponentModel::Container();
    this->Text = "Тарифы фирмы";
    this->Size = Drawing::Size(700, 350);
    this->BackColor = Color::FromArgb(255, 182, 193);

    dgv = gcnew DataGridView();
    dgv->Location = Point(20, 20);
    dgv->Size = Drawing::Size(400, 250);
    dgv->ReadOnly = true;
    dgv->AllowUserToAddRows = false;
    dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
    dgv->ColumnCount = 3;
    dgv->Columns[0]->Name = "Название";
    dgv->Columns[1]->Name = "Цена";
    dgv->Columns[2]->Name = "Скидка (%)";
    this->Controls->Add(dgv);

    int x = 450, y = 20, dy = 35;

    btnAdd = gcnew Button();
    btnAdd->Text = "Добавить";
    btnAdd->Location = Point(x, y);
    btnAdd->Click += gcnew EventHandler(this, &TariffsForm::Add);
    this->Controls->Add(btnAdd);

    btnEdit = gcnew Button();
    btnEdit->Text = "Изменить";
    btnEdit->Location = Point(x, y += dy);
    btnEdit->Click += gcnew EventHandler(this, &TariffsForm::Edit);
    this->Controls->Add(btnEdit);

    btnDelete = gcnew Button();
    btnDelete->Text = "Удалить";
    btnDelete->Location = Point(x, y += dy);
    btnDelete->Click += gcnew EventHandler(this, &TariffsForm::Delete);
    this->Controls->Add(btnDelete);

    btnFind = gcnew Button();
    btnFind->Text = "Минимальный тариф";
    btnFind->Location = Point(x, y += dy);
    btnFind->Click += gcnew EventHandler(this, &TariffsForm::Find);
    this->Controls->Add(btnFind);

    btnSave = gcnew Button();
    btnSave->Text = "Сохранить";
    btnSave->Location = Point(x, y += dy);
    btnSave->Click += gcnew EventHandler(this, &TariffsForm::Save);
    this->Controls->Add(btnSave);

    btnLoad = gcnew Button();
    btnLoad->Text = "Загрузить";
    btnLoad->Location = Point(x, y += dy);
    btnLoad->Click += gcnew EventHandler(this, &TariffsForm::Load);
    this->Controls->Add(btnLoad);

    btnSaveDB = gcnew Button();
    btnSaveDB->Text = "Сохр в БД";
    btnSaveDB->Location = Point(x, y += dy);
    btnSaveDB->Click += gcnew EventHandler(this, &TariffsForm::SaveDB);
    this->Controls->Add(btnSaveDB);

    btnClose = gcnew Button();
    btnClose->Text = "Закрыть";
    btnClose->Location = Point(x, y += dy);
    btnClose->Click += gcnew EventHandler(this, &TariffsForm::CloseForm);
    this->Controls->Add(btnClose);

    this->FormClosing += gcnew FormClosingEventHandler(
        this, &TariffsForm::OnFormClosing
    );
}

void TariffsForm::OnFormClosing(Object^ sender, FormClosingEventArgs^ e)
{
    if (!isSaved)
    {
        auto r = MessageBox::Show(
            "Есть несохранённые изменения. Выйти без сохранения?",
            "Подтверждение",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Warning
        );

        if (r != System::Windows::Forms::DialogResult::Yes)
            e->Cancel = true;
    }
}

void TariffsForm::UpdateButtons()
{
    btnAdd->Visible = isAdmin;
    btnEdit->Visible = isAdmin;
    btnDelete->Visible = isAdmin;
    btnSave->Visible = isAdmin;
    btnLoad->Visible = isAdmin;
    btnSaveDB->Visible = isAdmin;
}

void TariffsForm::RefreshGrid()
{
    dgv->Rows->Clear();
    for each (Tariff ^ t in tariffs->GetTariffs())
        dgv->Rows->Add(t->Name, t->Price, t->Discount);
}

void TariffsForm::Add(Object^, EventArgs^)
{
    String^ name = Prompt("Название тарифа:", "Добавить");
    if (String::IsNullOrWhiteSpace(name)) return;

    double price;
    if (!PromptDouble("Цена:", "Добавить", price, 0.01, 1e9)) return;

    double discount;
    if (!PromptDouble("Скидка (%):", "Добавить", discount, 0, 100)) return;

    tariffs->Add(name, price, discount);
    isSaved = false;
    RefreshGrid();
}

void TariffsForm::Edit(Object^, EventArgs^)
{
    if (dgv->SelectedRows->Count == 0) { 
        MessageBox::Show(
            "Нет достпуных тарифов для изменения.",
            "Ошибка при изменении",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error
        );
        return; 
    }
    int i = dgv->SelectedRows[0]->Index;

    double price, discount;
    if (!PromptDouble("Новая цена:", "Изменить", price, 0.01, 1e9)) return;
    if (!PromptDouble("Новая скидка:", "Изменить", discount, 0, 100)) return;

    tariffs->Edit(i, price, discount);
    isSaved = false;
    RefreshGrid();
}

void TariffsForm::Delete(Object^, EventArgs^)
{
    if (dgv->SelectedRows->Count == 0) return;
    if (MessageBox::Show(
        "Удалить выбранный тариф?",
        "Подтверждение",
        MessageBoxButtons::YesNo,
        MessageBoxIcon::Question
    ) != System::Windows::Forms::DialogResult::Yes)
        return;

    tariffs->Delete(dgv->SelectedRows[0]->Index);
    RefreshGrid();
    isSaved = false;
    RefreshGrid();
}

void TariffsForm::Find(Object^, EventArgs^)
{
    if (dgv->SelectedRows->Count == 0) {
        MessageBox::Show(
            "Нет достпуных тарифов для поиска.",
            "Ошибка при поиске",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error
        );
        return;
    }
    Tariff^ t = tariffs->FindMin();
    if (t)
        MessageBox::Show("Минимальный тариф:\n" + t->Name + " = " + t->FinalPrice());
}

void TariffsForm::Save(Object^, EventArgs^)
{
    SaveFileDialog^ sfd = gcnew SaveFileDialog();
    sfd->Filter = "Текстовые файлы (*.txt)|*.txt";
    sfd->Title = "Сохранить тарифы";

    if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        tariffs->SaveToFile(sfd->FileName);
        MessageBox::Show("Данные сохранены в файл");
        isSaved = true;
    }
}

void TariffsForm::Load(Object^, EventArgs^)
{
        tariffs->LoadFromDB();
        RefreshGrid();
        MessageBox::Show("Данные загружены");
        isSaved = true;

}

void TariffsForm::SaveDB(Object^, EventArgs^)
{
    tariffs->SaveToDB();
    MessageBox::Show(
        "Изменения сохранены в базе данных",
        "Сохранение",
        MessageBoxButtons::OK,
        MessageBoxIcon::Information
    );
    isSaved = true;
}

void TariffsForm::CloseForm(Object^, EventArgs^)
{
    if (!isSaved)
    {
        auto r = MessageBox::Show(
            "Есть несохранённые изменения. Выйти без сохранения?",
            "Подтверждение",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Warning
        );

        if (r != System::Windows::Forms::DialogResult::Yes)
            return;
    }

    this->Close();
}
