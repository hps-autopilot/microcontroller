#include "attitude_indicator.h"


//Constructor
attitude_indicator::attitude_indicator()
{
}

attitude_indicator::attitude_indicator(int dim_in)
{
	dim = dim_in;
}

void attitude_indicator::setDim(int num)
{
	dim = num;
	calibrate();
}

void attitude_indicator::calibrate()
{
	int i;
	int j;
	flipped = false;

	//flipped = true;
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			if (i < dim / 2)
			{
				LED_Array[i][j] = flipped;
			}
			else
			{
				LED_Array[i][j] = !flipped;
			}
		}
	}
}

void attitude_indicator::rollLeft()
{
	int i, j, k, l;
	for (i = dim-1; i > 0; i--)
	{
		for (j = dim/2 - 1; j >= 0; j--)
		{
			LED_Array[i][j] = detectRed(i, j);
		}
	}
	for (l = 0; l < dim / 2; l++)
	{
		LED_Array[0][l] = detectRed(0, l);
	}
	for (i = 0; i < dim - 1; i++)
	{
		for (j = dim/2; j < dim; j++)
		{
			LED_Array[i][j] = detectGreen(i, j);
		}
	}
	for (k = dim - 1; k >= dim / 2; k--) {
		LED_Array[dim-1][k] = detectGreen(dim-1, k);
	}
}

void attitude_indicator::rollRight()
{
	int i, j, k, l;
	for (i = 0; i < dim-1; i++)
	{
		for (j = dim / 2 - 1; j >= 0; j--)
		{
			LED_Array[i][j] = detectGreen(i, j);
		}
	}
	for (k = 0; k < dim / 2; k++) {
		LED_Array[dim - 1][k] = detectGreen(dim - 1, k);
	}
	for (i = dim - 1; i > 0; i--)
	{
		for (j = dim / 2; j < dim; j++)
		{
			LED_Array[i][j] = detectRed(i, j);
		}
	}
	for (l = dim-1; l >= dim / 2; l--)
	{
		LED_Array[0][l] = detectRed(0, l);
	}

}

bool attitude_indicator::detectGreen(int x, int y)
{
	if (x != dim - 1) {
		if (y == 0 && getValue(x + 1, y))
		{
			return !flipped;
		}
		else if (getValue(x, y - 1) && getValue(x + 1, y))
		{
			return !flipped;
		}
		else if (y == dim - 1 && getValue(x + 1, y))
		{
			return !flipped;
		}
		else if (getValue(x, y + 1) && getValue(x + 1, y))
		{
			return !flipped;
		}
		else {
			return flipped;
		}
	}
	else if (getValue(x, y - 1) || getValue(x, y + 1))
	{
		return !flipped;
	}
	else
	{
		return flipped;
	}
}

bool attitude_indicator::detectRed(int x, int y)
{
	if (x != 0) 
	{
		if (y == 0 && !getValue(x-1,y))
		{
			return flipped;
		}
		else if (!getValue(x, y - 1) && !getValue(x - 1, y))
		{
			return flipped;
		}
		else if (y == dim - 1 && !getValue(x - 1, y))
		{
			return flipped;
		}
		else if (!getValue(x, y + 1) && !getValue(x - 1, y))
		{
			return flipped;
		}
		else {
			return !flipped;
		}
	}
	else if (y == 0)
	{
		if (!getValue(x, y + 1))
		{
			return flipped;
		}
		else {
			return !flipped;
		}
	}
	else if (y == dim-1)
	{
		if (!getValue(x, y - 1))
		{
			return flipped;
		}
		else {
			return !flipped;
		}
	}
	else if (!getValue(x, y - 1) || !getValue(x, y + 1))
	{
		return flipped;
	}
	else 
	{
		return !flipped;
	}
}

void attitude_indicator::pitchUp()
{
	int i, j;
	bool full = true;

	for (i = dim-1; i > 0; i--)
	{
		for (j = 0; j < dim; j++)
		{
			if (!getValue(i - 1,j) && getValue(i, j))
			{
				LED_Array[i][j] = flipped;
				full = false;
			}

		}
	}

	for (i = dim - 1; i >= 1; i--)
	{
		if (getValue(0, i) && !getValue(0, i - 1))
		{
			LED_Array[0][i] = flipped;
			full = false;
		}
	}

	for (i = 0; i < dim - 1; i++)
	{
		if (getValue(0, i) && !getValue(0, i + 1))
		{
			LED_Array[0][i] = flipped;
			full = false;
		}
	}
	//tumble(full);
}

void attitude_indicator::pitchDown()
{
	int i, j;
	bool full = true;

	for (i = 0; i < dim-1; i++)
	{
		for (j = 0; j < dim; j++)
		{
			if (getValue(i+1, j) == true && getValue(i, j) != true)
			{
				LED_Array[i][j] = !flipped;
				full = false;
			}

		}
	}

	for (i = dim-1; i > 0; i--)
	{
		if (getValue(dim - 1, i) == false && getValue(dim - 1, i - 1))
		{
			LED_Array[dim-1][i] = !flipped;
			full = false;
		}
	}

	for (i = 0; i < dim - 1; i++)
	{
		if (getValue(dim - 1, i) == false && getValue(dim - 1, i + 1))
		{
			LED_Array[dim - 1][i] = !flipped;
			full = false;
		}
	}
	//tumble(full);
}

//void attitude_indicator::tumble(bool panic)
//{
//	int i, j;
//	if (panic)
//	{
//		for (i = 0; i < dim-1; i++)
//		{
//			if (i % 2 == 1)
//			{
//				for (j = 0; j < dim; j++)
//				{
//					LED_Array[i][j] = !flipped;
//				}
//			}
//			else
//			{
//				for (j = 0; j < dim; j++)
//				{
//					LED_Array[i][j] = flipped;
//				}
//			}
//		}
//	}
//}

bool attitude_indicator::getValue(int x, int y)
{
	return LED_Array[x][y];
}

int attitude_indicator::getDim()
{
	return dim;
}
