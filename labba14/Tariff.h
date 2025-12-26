#pragma once
using namespace System;

public ref class Tariff
{
public:
    int Id;
    String^ Name;
    double Price;
    double Discount;

    Tariff(int i, String^ n, double p, double d);

    double FinalPrice();
};
