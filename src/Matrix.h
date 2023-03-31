#ifndef INCLUDE_MATRIX_H
#define INCLUDE_MATRIX_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "constants.h"

class Matrix
{
protected:
    static MatrixPanel_I2S_DMA* dma_display;
    Matrix();
public:
    ~Matrix();
    static MatrixPanel_I2S_DMA *get()
    {
        HUB75_I2S_CFG mxconfig;
        mxconfig.mx_height = PANEL_HEIGHT;
        mxconfig.chain_length = PANELS_NUMBER;

        if (dma_display == nullptr)
            dma_display = new MatrixPanel_I2S_DMA(mxconfig);

        
        return dma_display;
    }
};

Matrix::Matrix(/* args */)
{
}

Matrix::~Matrix()
{
    delete(this->dma_display);
}

#endif // INCLUDE_MATRIX_H