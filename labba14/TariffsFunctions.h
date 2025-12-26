
#pragma once
#include "Tariff.h"

using namespace System;
using namespace System::Collections::Generic;

ref class TariffsFunctions
{
private:
    List<Tariff^>^ _tariffs;
    int GenerateNextId();

public:
    TariffsFunctions();

    bool Add(String^ name, double price, double discount);
    bool Edit(int index, double price, double discount);
    bool Delete(int index);

    void Clear();
    Tariff^ FindMin();
    List<Tariff^>^ GetTariffs();

    void LoadFromDB();
    void SaveToDB();

    void SaveToFile(String^ fileName);
    void LoadFromFile(String^ fileName);
};

