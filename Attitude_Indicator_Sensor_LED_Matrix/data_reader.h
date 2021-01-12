#pragma once
//Method and variable definitions for data_reader object

//Includes
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class data_reader
{
public:
	data_reader();
	void setFile(string fileName);
	void read(float* roll, float* pitch, float* yaw);

private:
	ifstream File;
	ios::streampos filePosition;
	string stringCharReplace(string str, char find, char replace);
};

