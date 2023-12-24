#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>



class Pipes
{
    std::string name = "";
    float length = 0;
    float diameter = 0;
    bool inRepare = false;
public:
    void addPipe(int d);
    void printPipe(std::ofstream& fout);
    void loadPipe(std::ifstream& in);
    bool filter(int status, std::string name_str, int d = -1, bool use = false);
    void View(int id);
    int getDiameter();
    int getLength();
    float getProductivity();
    void changeStatus(bool status);
    bool inUse;
};

