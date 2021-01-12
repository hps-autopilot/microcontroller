//Chase Hunter
//University of Washington
//EE 497/8 2020
//The purpose of this class is to handle all the logic of the hardware
//LED matrix. The hardware LED matrix is an 8x8 bicolor pixel matrix.

//Each pixel contains two LEDs that are controlled seperatly. 
//Every row of the display is stored as two 8 bit binary numbers.
//One binary number controls the green LEDs and the other the red LEDS.
//Each digit in the binary numbers corresponds to the location in the row.
//A 1 corresponds to a on LED and 0 corresponds to an off LED.

//For Example:
//RED:   00010010
//GREEN: 11101101
//Would correspond to a row where the left three pixels are green, the next is red,
//next two green, next red, and final pixel in the row is green.

//All commands for the LED matrix hardware must be sent using 8 bit binary values.
//Differnt values correspond to different commands and settings. 


//Includes
#include "LED_Matrix.h"

//Default constructor that calls the initialization method
LED_Matrix::LED_Matrix()
{
    this->FT_Init();
}

//Initializes the FT232H interface board
void LED_Matrix::FT_Init()
{
    //If using visual studio, additional library functions must be used
#ifdef _MSC_VER
    Init_libMPSSE();
#endif
    //Declare varibles
    ChannelConfig channelConf; //variable used by FT232H board
    uint32 channels;
    uint8 buffer[1];
    uint32 bytesTransfered;

    //Sets the clock rate and latency timer for the FT232H board
    channelConf.ClockRate = I2C_CLOCK_STANDARD_MODE;
    channelConf.LatencyTimer = 255;

    //Optional configurations of the FT232H board
    //channelConf.Options = I2C_DISABLE_3PHASE_CLOCKING;
    //channelConf.Options = I2C_ENABLE_DRIVE_ONLY_ZERO;

    //Return number of connected I2C devices
    status = I2C_GetNumChannels(&channels);

    if (channels > 0)
    {
        //Open the first channel and intialzie the connection
        //status OK if 0
        status = I2C_OpenChannel(0, &ftHandle);
        status = I2C_InitChannel(ftHandle, &channelConf);
        if (status == FT_OK)
        {

            //==========INITIALISATION================================================================================================================================
                        //Start Clock
            buffer[0] = 0b00100001;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Set to LED hardware matrix logic to ROW output
            buffer[0] = 0b10101110;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Set LED hardware matrix LED brightness level
            buffer[0] = 0b11100011;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Turn LED hardware matrix display on and set LEDs to not blink
            buffer[0] = 0b10001001;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //=========================================================================================================================================================
        }
    }
}

//Logic for shutting down the LED hardware matrix
void LED_Matrix::FT_Close()
{
    //Declare variables
    uint8 buffer[1];
    uint32 bytesTransfered;

    //Turn off all green and red LEDs
    clearMatrix();

    //Turn LED hardware matrix display off
    buffer[0] = 0b10001000;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

    //Set LED hardware matrix to standby
    buffer[0] = 0b00100000;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    std::cout << "\nDone..." << status << "\n";

    //Shutdown additional libraries on visual studio
#ifdef _MSC_VER
    Cleanup_libMPSSE();
#endif
}

//Checks to see if the pixel at the passed location is the same as the passed boolean value
//If it is not, the pixel color is flipped to match
//Green = true, red = false
void LED_Matrix::checkPixel(bool value, int x, int y)
{
    //Declare variables
    int greenRow = (7 - x) * 2;
    int redRow = greenRow + 1;
    uint8 num = (uint8)pow(2, (7 - y));
    uint8 greenValue = matrix[greenRow];
    uint8 redValue = matrix[redRow];
    bool writeFlag = false;

    //If green off and value true,
    if (value && (greenValue / num) % 2 == 0)
    {
        writeFlag = flipPixel(true, &greenValue, &redValue, num);
    }
    //If red off and value false
    else if (!value && (redValue / num) % 2 == 0)
    {
        writeFlag = flipPixel(false, &greenValue, &redValue, num);
    }

    //Writes the new row values to the storage array if a change is made
    if (writeFlag) {
        matrix[greenRow] = greenValue;
        matrix[redRow] = redValue;
        writeRow(greenRow, greenValue, redValue);
    }
}

//Flips the color of the pixel by adding or subtracting the digit from 
//the storage value
bool LED_Matrix::flipPixel(bool flag, uint8* green, uint8* red, uint8 num)
{
    //If green on, red off
    if (flag)
    {
        *green = *green + num;
        *red = *red - num;
    }
    //Else green off, red on
    else
    {
        *green = *green - num;
        *red = *red + num;
    }

    return true;
}

//Writes new Row values to the LED hardware matrix
void LED_Matrix::writeRow(uint8 writeAddress, uint8 numToWriteGreen, uint8 numToWriteRed)
{
    uint32 bytesTransfered;
    uint8 buffer[3];

    //First the write address corresponding to the desired row is sent
    //Then the green and red led values are sent
    buffer[0] = writeAddress;
    buffer[1] = numToWriteGreen;
    buffer[2] = numToWriteRed;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 3, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
}

//Resets/calibrates the storage matrix
//Sets top half of display to red and bottom half to green.
void LED_Matrix::calibrateMatrix()
{
    int i;
    for (i = 0; i < 8; i = i + 2)
    {
        matrix[i] = 255;
        matrix[i + 1] = 0;
    }
    for (i = 8; i < 16; i = i + 2)
    {
        matrix[i] = 0;
        matrix[i + 1] = 255;
    }
    drawMatrix();
}

//Turns off all LEDS in the matrix
void LED_Matrix::clearMatrix()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        matrix[i] = 0;
        writeRow(i, 0, 0);
    }
}

//Resets/calibrates the hardware LED matrix by setting the top half to red and the bottom half to green
void LED_Matrix::drawMatrix()
{
    writeRow(0, 255, 0);
    writeRow(2, 255, 0);
    writeRow(4, 255, 0);
    writeRow(6, 255, 0);
    writeRow(8, 0, 255);
    writeRow(10, 0, 255);
    writeRow(12, 0, 255);
    writeRow(14, 0, 255);
}