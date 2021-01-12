//Chase Hunter
//University of Washington
//EE 497/8 2020
//The purpose of this class is to handle all the logic of the simulated
//attitude indicator. The attitude indicator is handled as a 2d boolean array.
//Each location in the array corresponds to a pixel in the display.
//Green = true and Red = false

//Includes
#include "attitude_indicator.h"

//Default Constructor
attitude_indicator::attitude_indicator()
{
}

//Constructor
attitude_indicator::attitude_indicator(int dim_in)
{
	dim = dim_in;
}

//Sets the dimensions of the array
//number of LED columns and rows are equal
void attitude_indicator::setDim(int num)
{
	dim = num;
	calibrate();
}

//Resets/calibrates the attitude indicator
//Sets top half red and bottom half green
void attitude_indicator::calibrate()
{
	int i;
	int j;
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			//If upper-half set red
			if (i < dim / 2)
			{
				LED_Array[i][j] = false;
			}
			//Else set green
			else
			{
				LED_Array[i][j] = true;
			}
		}
	}
}

//Rolls the attitude indicator to the left
void attitude_indicator::rollLeft()
{
	int i, j, k, l;
	
	//Scan pixels in left half columns for every row but first row
	//Scan rows from bottom to top
	//Scan columns from right to left
	for (i = dim-1; i > 0; i--)
	{
		for (j = dim/2 - 1; j >= 0; j--)
		{
			LED_Array[i][j] = detectRedLeft(i, j);
		}
	}
	//Scan pixels in left half columns for first row
	//Scan columns from left to right
	for (l = 0; l < dim / 2; l++)
	{
		LED_Array[0][l] = detectRedLeft(0, l);
	}
	//Scan pixels in right half columns for every row but last row
	//Scan rows from top to bottom
	//Scan columns from left to right
	for (i = 0; i < dim - 1; i++)
	{
		for (j = dim/2; j < dim; j++)
		{
			LED_Array[i][j] = detectGreenLeft(i, j);
		}
	}
	//Scan pixels in right half columns for last row
	//Scan columns from right to left
	for (k = dim - 1; k >= dim / 2; k--) {
		LED_Array[dim - 1][k] = detectGreenLeft(dim - 1, k);
	}
}

//Rolls the attitude indicator to the right
void attitude_indicator::rollRight()
{
	int i, j, k, l;
	//Scan pixels in left half columns for every row but last row
	//Scan rows from top to bottom
	//Scan columns from right to left
	for (i = 0; i < dim-1; i++)
	{
		for (j = dim / 2 - 1; j >= 0; j--)
		{
			LED_Array[i][j] = detectGreenRight(i, j);
		}
	}
	//Scan pixels in left half columns for last row
	//Scan columns from left to right
	for (k = 0; k < dim / 2; k++) {
		LED_Array[dim - 1][k] = detectGreenRight(dim - 1, k);
	}
	//Scan pixels in right half columns for every row but first row
	//Scan rows from bottom to top
	//Scan columns from left to right
	for (i = dim - 1; i > 0; i--)
	{
		for (j = dim / 2; j < dim; j++)
		{
			LED_Array[i][j] = detectRedRight(i, j);
		}
	}
	//Scan pixels in right half columns for first row
	//Scan columns from right to left
	for (l = dim-1; l >= dim / 2; l--)
	{
		LED_Array[0][l] = detectRedRight(0, l);
	}

}

//Determines if the pixel at the passed location should be green for left roll
bool attitude_indicator::detectGreenLeft(int x, int y)
{
	//If not last row
	if (x != dim - 1) {
		//If first column or last column and above pixel is green
		if ((y == 0 || y == dim - 1) && getValue(x + 1, y))
		{
			return true;
		}
		//Else if left pixel or right pixel is green and below pixel is green
		else if ((getValue(x, y - 1) || getValue(x, y + 1)) && getValue(x + 1, y))
		{
			return true;
		}
		//Else if left pixel and right pixel is green
		//and not last column and below pixel is red
		else if (getValue(x, y - 1) && getValue(x, y + 1)
			     && y != dim - 1 && !getValue(x + 1, y))
		{
			return true;
		}
		//Else
		else {
			return false;
		}
	}
	//Else if first right side column
	else if (y == dim/2)
	{
		//If pixel above and right are same and pixel
		//in first row, last left side column is red
		if ((getValue(x - 1, y) == getValue(x, y + 1))
			&& !getValue(0, dim / 2 - 1))
		{
			return true;
		}
		//Else
		else
		{
			return false;
		}
	}
	//Else if left pixel is green
	else if (getValue(x, y - 1))
	{
		return true;
	}
	//Else
	else
	{
		return false;
	}
}

//Determines if the pixel at the passed location should be red for left roll
bool attitude_indicator::detectRedLeft(int x, int y)
{
	//If not first row
	if (x != 0)
	{
		//If first column or last column and above pixel is red
		if ((y == 0 || y == dim - 1) && !getValue(x - 1, y))
		{
			return false;
		}
		//Else if right pixel or left pixel is red and above pixel is red
		else if ((!getValue(x, y - 1) || !getValue(x, y + 1)) && !getValue(x - 1, y))
		{
			return false;
		}
		//Else if right pixel is red and above pixel is green
		//and not first column and left pixel is red
		else if (!getValue(x, y + 1) && getValue(x - 1, y)
			     && y != 0 && !getValue(x, y - 1))
		{
			return false;
		}
		//Else
		else {
			return true;
		}
	}
	//Else if right pixel is red
	else if (!getValue(x, y + 1))
	{
		return false;
	}
	//Else
	else
	{
		return true;
	}
}

//Determines if the pixel at the passed location should be green for right roll
bool attitude_indicator::detectGreenRight(int x, int y)
{
	//If not last row
	if (x != dim - 1) {
		//If first column or last column and below pixel is green
		if ((y == 0 || y == dim - 1) && getValue(x + 1, y))
		{
			return true;
		}
		//Else if left pixel or right pixel is green and below pixel is green
		else if ((getValue(x, y - 1) || getValue(x, y + 1)) && getValue(x + 1, y))
		{
			return true;
		}
		//Else if left pixel and above pixel is green
		//and below pixel is red and not last column
		else if (getValue(x, y - 1) && getValue(x, y + 1)
			&& y != dim - 1 && !getValue(x + 1, y))
		{
			return true;
		}
		//Else
		else {
			return false;
		}
	}
	//Else if first column
	else if (y == 0)
	{
		//If pixel above and right are green
		if (getValue(x, y + 1) && getValue(x - 1, y))
		{
			return true;
		}
		else if (getValue(x, y + 1) && !getValue(x - 1, y))
		{
			return true;
		}
		//Else
		else
		{
			return false;
		}
	}
	//Else if right pixel is green
	else if (getValue(x, y + 1))
	{
		return true;
	}
	//Else
	else
	{
		return false;
	}
}

//Determines if the pixel at the passed location should be red for right roll
bool attitude_indicator::detectRedRight(int x, int y)
{
	//If not first row
	if (x != 0)
	{
		//If first column or last column and above pixel is red
		if ((y == 0 || y == dim - 1) && !getValue(x - 1, y))
		{
			return false;
		}
		//Else if right pixel or left pixel is red and above pixel is red
		else if ((!getValue(x, y - 1) || !getValue(x, y + 1)) && !getValue(x - 1, y))
		{
			return false;
		}
		//Else if left pixel is red and above pixel is green
		//and not last column and right pixel is red
		else if (!getValue(x, y - 1) && getValue(x - 1, y)
			&& y != dim - 1 && !getValue(x, y + 1))
		{
			return false;
		}
		//Else
		else {
			return true;
		}
	}
	//Else if first column of right half
	else if (y == dim / 2)
	{
		//If pixel below and right are same and pixel
		//in last row, last left side column is green
		if ((getValue(x + 1, y) == getValue(x, y + 1))
			&& getValue(dim - 1, dim / 2 - 1))
		{
			return false;
		}
		//Else
		else
		{
			return true;
		}
	}
	//Else if left pixel is red
	else if (!getValue(x, y - 1))
	{
		return false;
	}
	//Else
	else
	{
		return true;
	}
}

//Pitches the attitude indicator up
void attitude_indicator::pitchUp()
{
	int i, j;
	bool color;

	//If the orientation is straight up or straight down
	if (checkPitch(&color))
	{
		//If orientation is straight down
		if (color)
		{
			for (i = 0; i < dim; i++)
			{
				LED_Array[0][i] = false;
			}
		}
		//else if orientation is straight up
		else
		{
			for (i = 0; i < dim; i++)
			{
				LED_Array[0][i] = true;
			}
		}
	}
	//Else
	else {
		//Pitch up every row except top row
		for (i = dim - 1; i > 0; i--)
		{
			for (j = 0; j < dim; j++)
			{
				//If above pixel does not equal this pixel
				//switch this pixel
				if (getValue(i - 1, j) != getValue(i, j))
				{
					LED_Array[i][j] = !getValue(i, j);
				}
			}
		}
		
		j = -1;
		//Find position where top row changes if exists
		for (i = 0; i < dim - 1; i++)
		{
			//If this pixel does not equal right pixel save
			//location of this pixel
			if (getValue(0, i) != getValue(0, i + 1))
			{
				j = i;
			}
		}
		
		//Pitch up top row
		if (j != -1)
		{
		    if (getValue(0, dim - 1) && !getValue(dim - 1, 0)
				&& (!getValue(dim - 1, dim - 1)
					|| (!getValue(0,0) && getValue(dim - 1, dim - 1)
					    && getValue(0, dim / 2))))
			{
				LED_Array[0][j] = true;
			}
			else if (getValue(0, 0) && !getValue(dim - 1, dim - 1)
				     && (!getValue(dim - 1, 0)
						 || (!getValue(0, dim - 1) && getValue(dim - 1, 0)
							 && getValue(0, dim / 2 - 1))))
			{
				LED_Array[0][j + 1] = true;
			}
			else if (!getValue(0, 0))
			{
				LED_Array[0][j + 1] = false;
			}
			else
			{
				LED_Array[0][j] = false;
			}
		}
	}
}

//Pitches the attitude indicator down
void attitude_indicator::pitchDown()
{
	int i, j;
	bool color;

	//If the orientation is straight up or straight down
	if (checkPitch(&color))
	{
		//If orientation is straight down
		if (color)
		{
			for (i = 0; i < dim; i++)
			{
				LED_Array[dim-1][i] = false;
			}
		}
		//else if orientation is straight up
		else
		{
			for (i = 0; i < dim; i++)
			{
				LED_Array[dim-1][i] = true;
			}
		}
	}
	else
	{
		//Pitch up every row except bottom row
		for (i = 0; i < dim - 1; i++)
		{
			for (j = 0; j < dim; j++)
			{
				//If below pixel does not this pixel switch this pixel
				if (getValue(i + 1, j) != getValue(i, j))
				{
					LED_Array[i][j] = !getValue(i, j);
				}

			}
		}

		j = -1;
		//Find horizontal position where bottom row changes if exists
		for (i = 0; i < dim - 1; i++)
		{
			//If left pixel does not equal this pixel set j to location
			//of this pixel
			if (getValue(dim - 1, i) != getValue(dim - 1, i + 1))
			{
				j = i;
			}
		}

		//Pitch down bottom row
		if (j != -1)
		{
			if (getValue(0, 0) && !getValue(dim - 1, dim - 1)
				&& (getValue(0, dim - 1)
					|| (!getValue(0, dim - 1) && getValue(dim - 1, 0)
						&& !getValue(dim - 1, dim / 2))))
			{
				LED_Array[dim - 1][j] = false;
			}
			else if (!getValue(dim - 1, 0) && getValue(0, dim - 1)
				&& (getValue(0, 0)
					|| (!getValue(0, 0) && getValue(dim - 1, dim - 1)
						&& !getValue(dim - 1, dim / 2 - 1))))
			{
				LED_Array[dim - 1][j + 1] = false;
			}
			else if (getValue(dim - 1, 0))
			{
				LED_Array[dim - 1][j + 1] = true;
			}
			else
			{
				LED_Array[dim - 1][j] = true;
			}
		}
	}
}

//Checks the display to see if the orientation is straight up
//or straight down and if true writes the corresponding color using
//a boolean.
//color = false means the orientation is straight up
//color = true means the orientation is straight down
bool attitude_indicator::checkPitch(bool *color)
{
	bool test;
	//Use color of first pixel as control test
	test = getValue(0, 0);
	int i, j;
	//See if every pixel in display is same as control
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			//If this pixel does not equal test value return false
			if (test != getValue(i, j))
			{
				return false;
			}

		}
	}
	*color = test;
	return true;
}

//Returns the color of the pixel at the passed location
//Green = true, Red = false
bool attitude_indicator::getValue(int x, int y)
{
	return LED_Array[x][y];
}

//Returns the current set dimenstion of the pixel array
int attitude_indicator::getDim()
{
	return dim;
}
