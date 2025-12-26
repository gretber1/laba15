#include "TariffsFunctions.h"
#include <fstream>
#include "sqlite3.h"
#include <vector>
#include <iostream>

using namespace System::IO;
using namespace System::Text;
using namespace System::Runtime::InteropServices;

std::string StringToUtf8(String^ str)
{
    array<Byte>^ bytes = Encoding::UTF8->GetBytes(str);
    pin_ptr<Byte> pinnedBytes = &bytes[0];
    return std::string(reinterpret_cast<char*>(pinnedBytes), bytes->Length);
}

String^ Utf8ToString(const unsigned char* text, int len)
{
    if (!text) return "";
    return gcnew String(reinterpret_cast<const char*>(text), 0, len, Encoding::UTF8);
}

TariffsFunctions::TariffsFunctions()
{
    _tariffs = gcnew List<Tariff^>();
}

int TariffsFunctions::GenerateNextId()
{
    int id = 1;
    while (true)
    {
        bool exists = false;
        for each (Tariff ^ t in _tariffs)
        {
            if (t->Id == id) { exists = true; break; }
        }
        if (!exists) return id;
        id++;
    }
}


List<Tariff^>^ TariffsFunctions::GetTariffs()
{
    return _tariffs;
}

bool TariffsFunctions::Add(String^ name, double price, double discount)
{
    int id = GenerateNextId();
    _tariffs->Add(gcnew Tariff(id, name, price, discount));
    return true;
}

bool TariffsFunctions::Edit(int index, double price, double discount)
{
    if (index < 0 || index >= _tariffs->Count) return false;
    _tariffs[index]->Price = price;
    _tariffs[index]->Discount = discount;
    return true;
}

bool TariffsFunctions::Delete(int index)
{
    if (index < 0 || index >= _tariffs->Count) return false;
    _tariffs->RemoveAt(index);
    return true;
}
void TariffsFunctions::Clear()
{
    _tariffs->Clear();
}

Tariff^ TariffsFunctions::FindMin()
{
    if (_tariffs->Count == 0) return nullptr;

    Tariff^ min = _tariffs[0];
    for each (Tariff ^ t in _tariffs)
    {
        if (t->FinalPrice() < min->FinalPrice())
            min = t;
    }
    return min;
}

void TariffsFunctions::SaveToDB()
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_open("tariffs.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createSql =
        "CREATE TABLE IF NOT EXISTS tariffs ("
        "Id INTEGER PRIMARY KEY, "
        "Name TEXT, "
        "Price REAL, "
        "Discount REAL);";

    rc = sqlite3_exec(db, createSql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка CREATE TABLE\n";
    }

    sqlite3_exec(db, "DELETE FROM tariffs;", nullptr, nullptr, nullptr);

    const char* sql = "INSERT INTO tariffs (Id, Name, Price, Discount) VALUES (?, ?, ?, ?);";

    for each (Tariff ^ t in _tariffs)
    {
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) continue;

        sqlite3_bind_int(stmt, 1, t->Id);

        std::string nameUtf8 = StringToUtf8(t->Name);
        sqlite3_bind_text(stmt, 2, nameUtf8.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, t->Price);
        sqlite3_bind_double(stmt, 4, t->Discount);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}

void TariffsFunctions::SaveToFile(String^ fileName)
{
    StreamWriter^ sw = gcnew StreamWriter(fileName);

    for each (Tariff ^ t in _tariffs)
    {
        sw->WriteLine(
            t->Id + ";" +
            t->Name + ";" +
            t->Price + ";" +
            t->Discount
        );
    }

    sw->Close();
}

void TariffsFunctions::LoadFromDB()
{
    _tariffs->Clear();

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;



    int rc = sqlite3_open("tariffs.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createSql =
        "CREATE TABLE IF NOT EXISTS tariffs ("
        "Id INTEGER PRIMARY KEY, "
        "Name TEXT, "
        "Price REAL, "
        "Discount REAL);";

    rc = sqlite3_exec(db, createSql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка CREATE TABLE\n";
    }

    const char* sql = "SELECT Id, Name, Price, Discount FROM tariffs;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            String^ name = Utf8ToString(sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));
            double price = sqlite3_column_double(stmt, 2);
            double discount = sqlite3_column_double(stmt, 3);

            _tariffs->Add(gcnew Tariff(id, name, price, discount));
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void TariffsFunctions::LoadFromFile(String^ fileName)
{
    _tariffs->Clear();

    array<String^>^ lines = File::ReadAllLines(fileName);

    for each (String ^ line in lines)
    {
        array<String^>^ parts = line->Split(';');
        if (parts->Length < 4) continue;

        int id = Int32::Parse(parts[0]);
        String^ name = parts[1];
        double price = Double::Parse(parts[2]);
        double discount = Double::Parse(parts[3]);

        _tariffs->Add(gcnew Tariff(id, name, price, discount));
    }

    
    SaveToDB();
}
