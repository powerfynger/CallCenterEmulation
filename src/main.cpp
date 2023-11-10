#include <iostream>
#include "../include/json.hpp"
#include <fstream>

using json = nlohmann::json;


int main(int, char**){
    std::cout << "START\n";
    std::ifstream f("../config/test.json");
    // json data = json::parse(f);   
    auto j = nlohmann::json::parse(f);
    // Вывод содержимого объекта
    for (auto& element : j["name"]) {
        std::cout << element << "\n";
    } 
}

