/*
    Author: John Pierce
    Date: 10/23/2025
    Description: this is the file that is for the "products"
*/

#pragma once
#include <string>

struct Product {
    std::string uniqID;
    std::string productName;
    std::string categories;

    Product() : uniqID(), productName(), categories() {}

    Product(const std::string& id, const std::string& name, const std::string& cats) : uniqID(id), productName(name), categories(cats) {}
};
