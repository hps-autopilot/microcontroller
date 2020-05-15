#include "LED_Matrix.h"

LED_Matrix::LED_Matrix()
{
    this->FT_Init();
}

void LED_Matrix::FT_Init()
{
#ifdef _MSC_VER
    Init_libMPSSE();
#endif

    ChannelConfig channelConf;
    uint32 channels;
    uint8 buffer[1];
    uint32 bytesTransfered;

    channelConf.ClockRate = I2C_CLOCK_STANDARD_MODE;
    channelConf.LatencyTimer = 255;
    //channelConf.Options = I2C_DISABLE_3PHASE_CLOCKING;
    //channelConf.Options = I2C_ENABLE_DRIVE_ONLY_ZERO;

    status = I2C_GetNumChannels(&channels);

    if (channels > 0)
    {
        //status OK if 0
        status = I2C_OpenChannel(0, &ftHandle);
        status = I2C_InitChannel(ftHandle, &channelConf);
        if (status == FT_OK)
        {

            //==========INITIALISATION================================================================================================================================
                        //Start Clock
            buffer[0] = 0b00100001;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Set to ROW output
            buffer[0] = 0b10101110;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Set dimming
            buffer[0] = 0b11100011;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //Display on, no blinking
            buffer[0] = 0b10001001;
            status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

            //=========================================================================================================================================================
        }
    }
}

void LED_Matrix::FT_Close()
{

    uint8 buffer[1];
    uint32 bytesTransfered;

    clearMatrix();

    //Display off, no blinking
    buffer[0] = 0b10001000;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);

    //Set to standby
    buffer[0] = 0b00100000;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 1, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    std::cout << "\nDone..." << status << "\n";


#ifdef _MSC_VER
    Cleanup_libMPSSE();
#endif
}

void LED_Matrix::checkPixel(bool value, int x, int y)
{
    int greenRow = (7 - x) * 2;
    int redRow = greenRow + 1;
    uint8 num = (uint8)pow(2, (7 - y));

    uint8 greenValue = matrix[greenRow];
    uint8 redValue = matrix[redRow];
    bool writeFlag = false;

    //If green off and value true
    if (value && (greenValue / num) % 2 == 0)
    {
        writeFlag = flipPixel(true, &greenValue, &redValue, num);
    }
    //If red off and value false
    else if (!value && (redValue / num) % 2 == 0)
    {
        writeFlag = flipPixel(false, &greenValue, &redValue, num);
    }

    if (writeFlag) {
        matrix[greenRow] = greenValue;
        matrix[redRow] = redValue;
        writeRow(greenRow, greenValue, redValue);
    }
}

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

void LED_Matrix::writeRow(uint8 writeAddress, uint8 numToWriteGreen, uint8 numToWriteRed)
{
    uint32 bytesTransfered;
    uint8 buffer[3];

    buffer[0] = writeAddress;
    buffer[1] = numToWriteGreen;
    buffer[2] = numToWriteRed;
    status = I2C_DeviceWrite(ftHandle, SLAVE_ADDRESS, 3, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
}

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

void LED_Matrix::clearMatrix()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        matrix[i] = 0;
        writeRow(i, 0, 0);
    }
}

void LED_Matrix::drawMatrix()
{
    writeRow(0,  255, 0);
    writeRow(2,  255, 0);
    writeRow(4,  255, 0);
    writeRow(6,  255, 0);
    writeRow(8,  0, 255);
    writeRow(10, 0, 255);
    writeRow(12, 0, 255);
    writeRow(14, 0, 255);
}