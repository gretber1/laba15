#include "Tariff.h"

Tariff::Tariff(int index,String^ name, double price, double discount)
{   
    Id = index;
    Name = name;
    Price = price;
    Discount = discount;
}

double Tariff::FinalPrice()
{
    return Price * (1.0 - Discount / 100.0);
}
