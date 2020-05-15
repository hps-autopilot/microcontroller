#pragma once
//Method and variable definitions for LED_matrix object

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include<stdlib.h>
#ifdef _WIN32
#include<windows.h>
#endif
#include <string.h>
#include <math.h>
#include <ftd2xx.h>
#include <libMPSSE_i2c.h>

class LED_Matrix
{
public:
	LED_Matrix();
	void FT_Init();
	void FT_Close();
	void checkPixel(bool value, int x, int y);
	void calibrateMatrix();
private:
	FT_STATUS status;
	uint8 SLAVE_ADDRESS = 0x70;
	//The value of every LED for each pixel is tracked using a storage array
	//Each value in the array corresponds to either the red or green LEDs for a row
	//Matrix[0] = green leds for Row 1
	//Matrix[1] = red leds for Row 1
	//Matrix[2] = green leds for Row 2
	//Matrix[3] = red leds for Row 2
	//and so on and so forth
	uint8 matrix[16];
	FT_HANDLE ftHandle;
	bool flipPixel(bool flag, uint8* green, uint8* red, uint8 num);
	void writeRow(uint8 writeAddress, uint8 numToWriteGreen, uint8 numToWriteRed);
	void clearMatrix();
	void drawMatrix();
};

