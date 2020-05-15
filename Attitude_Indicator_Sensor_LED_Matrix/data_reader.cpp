//Chase Hunter
//University of Washington
//EE 497/8 2020
//The purpose of this class is to handle all the logic of reading in a new 
//data set from the arduino sensors.

//This class must be run alongside the modifed Bluetooth LE Explorer program.
//The Bluetooth LE Explorer program connects to the arduino device using 
//Bluetooth Low Energy and writes the Gyroscope data to a text file. This 
//class reads the gyroscope data from that text file as it is being written.

//Includes
#include "data_reader.h"

//Default constructor
data_reader::data_reader()
{

}

//Opens the passed file and saves the current position in the file.
void data_reader::setFile(string fileName)
{
	File.open(fileName);
	filePosition = File.tellg();
}

//Replaces all the find characters in the passed string with the replace character
//Used to replace commas with spaces in passed Gyroscope data.
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

//Reads a set of data from the data file.
void data_reader::read(float* roll, float* pitch, float* yaw)
{
	string data;
	int dataEnd;
	std::string::size_type first, second;

	//If there is no data to read, return to the preivously saved file postition.
	if (!getline(File, data) || File.eof())
	{
		File.clear();
		File.seekg(filePosition);
	}

	//Save current file postition
	filePosition = File.tellg();

	//Processes the data string and returns the roll, pitch, and yaw
	//If the string is not gyroscope data, roll, pitch, and yaw are
	//set to zero
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