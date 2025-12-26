#pragma once
#include "TariffsFunctions.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class TariffsForm : public Form
{
public:
    TariffsForm(bool adminMode);
    TariffsForm(bool isAdmin, TariffsFunctions^ funcs);

protected:
    ~TariffsForm();

private:
    bool isAdmin;
    bool isSaved;
    TariffsFunctions^ tariffs;
    System::ComponentModel::Container^ components;

    DataGridView^ dgv;
    Button^ btnAdd;
    Button^ btnEdit;
    Button^ btnDelete;
    Button^ btnFind;
    Button^ btnSave;
    Button^ btnLoad;
    Button^ btnSaveDB;
    Button^ btnClose;

    void InitializeComponent();
    void RefreshGrid();
    void UpdateButtons();

    void OnFormClosing(Object^ sender, FormClosingEventArgs^ e);
    void Add(Object^ sender, EventArgs^ e);
    void Edit(Object^ sender, EventArgs^ e);
    void Delete(Object^ sender, EventArgs^ e);
    void Find(Object^ sender, EventArgs^ e);
    void Save(Object^ sender, EventArgs^ e);
    void Load(Object^ sender, EventArgs^ e);
    void SaveDB(Object^ sender, EventArgs^ e);
    void CloseForm(Object^ sender, EventArgs^ e);
};
