#pragma once
//Method and variable definitions for attitude indicator object
class attitude_indicator
{
public:
	attitude_indicator();
	attitude_indicator(int dim_in);
	void setDim(int num);
	void calibrate();
	void pitchUp();
	void pitchDown();
	void rollLeft();
	void rollRight();
	bool getValue(int x, int y);
	int getDim();
private:
	int dim;
	bool LED_Array[1024][1024];
	bool flat;
	bool flipped;
	//void tumble(bool panic);
	bool detectGreen(int x, int y);
	bool detectRed(int x, int y);
	
};

