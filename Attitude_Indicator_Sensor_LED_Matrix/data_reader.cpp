#include "data_reader.h"

data_reader::data_reader()
{

}

void data_reader::setFile(string fileName)
{
	File.open(fileName);
	filePosition = File.tellg();
}

string data_reader::stringCharReplace(string str, char find, char replace)
{
	int i;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == find)
		{
			str[i] = replace;
		}
	}
	return str;
}

void data_reader::read(float* roll, float* pitch, float* yaw)
{
	string data;
	int dataEnd;
	std::string::size_type first, second;

	if (!getline(File, data) || File.eof())
	{
		File.clear();
		File.seekg(filePosition);
	}

	filePosition = File.tellg();

	if (data.compare(0, 4, "Gyro") == 0)
	{
		dataEnd = data.find(")", 24);
		data = data.substr(12, dataEnd - 12);
		data = stringCharReplace(data, ',', ' ');
		*roll = stof(data, &first);
		*pitch = stof(data.substr(first), &second);
		*yaw = stof(data.substr(first+second));
	}
	else
	{
		*roll = 0;
		*pitch = 0;
		*yaw = 0;
	}
}