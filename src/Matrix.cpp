#include "Matrix.h"

// Static member initialization
MatrixPanel_I2S_DMA* Matrix::instance = nullptr;

MatrixPanel_I2S_DMA* Matrix::getInstance()
{
    if (instance == nullptr)
    {
        HUB75_I2S_CFG mxconfig;
        mxconfig.mx_height = DisplayConfig::PANEL_HEIGHT;
        mxconfig.chain_length = DisplayConfig::PANELS_NUMBER;
        mxconfig.clkphase = false;
        
        instance = new MatrixPanel_I2S_DMA(mxconfig);
    }
    
    return instance;
}

void Matrix::cleanup()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}
