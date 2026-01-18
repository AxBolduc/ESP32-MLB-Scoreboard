#ifndef INCLUDE_CORE_SPORTFACTORY_H
#define INCLUDE_CORE_SPORTFACTORY_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Sport.h"

/**
 * @brief Factory class for creating Sport instances
 * 
 * This factory provides a central location for sport creation and type conversion.
 * When adding a new sport, update:
 * 1. SportType enum in Sport.h
 * 2. createSport() implementation
 * 3. sportTypeFromString() implementation
 * 4. sportTypeToString() implementation
 */
class SportFactory
{
public:
    /**
     * @brief Create a Sport instance based on type
     * @param type Sport type to create
     * @param display Pointer to LED matrix display
     * @return Pointer to newly created Sport (caller owns)
     */
    static Sport* createSport(SportType type, MatrixPanel_I2S_DMA* display);
    
    /**
     * @brief Convert string to SportType enum
     * @param name Sport name (case-insensitive, e.g., "MLB", "NHL", "Baseball", "Hockey")
     * @return SportType enum value (defaults to MLB if unknown)
     */
    static SportType sportTypeFromString(const String& name);
    
    /**
     * @brief Convert SportType enum to string
     * @param type Sport type enum
     * @return Sport name string (e.g., "MLB", "NHL")
     */
    static String sportTypeToString(SportType type);

private:
    // No instantiation - static methods only
    SportFactory() = delete;
    SportFactory(const SportFactory&) = delete;
    SportFactory& operator=(const SportFactory&) = delete;
};

#endif // INCLUDE_CORE_SPORTFACTORY_H
