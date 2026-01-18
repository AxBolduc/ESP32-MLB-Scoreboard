#ifndef INCLUDE_MATRIX_H
#define INCLUDE_MATRIX_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Config.h"

/**
 * @brief Singleton wrapper for LED matrix display
 */
class Matrix
{
private:
    static MatrixPanel_I2S_DMA* instance;
    
    // Prevent instantiation
    Matrix() = delete;
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

public:
    /**
     * @brief Get the singleton matrix display instance
     * @return Pointer to the matrix display
     */
    static MatrixPanel_I2S_DMA* getInstance();
    
    /**
     * @brief Clean up the matrix display (call before restart if needed)
     */
    static void cleanup();
};

#endif // INCLUDE_MATRIX_H