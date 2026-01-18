#include "SportFactory.h"
#include "../sports/mlb/MLBSport.h"
#include "../sports/nhl/NHLSport.h"

Sport* SportFactory::createSport(SportType type, MatrixPanel_I2S_DMA* display)
{
    switch (type)
    {
    case SportType::MLB:
        return new MLBSport(display);
    
    case SportType::NHL:
        return new NHLSport(display);
    
    default:
        Serial.println("ERROR: Unknown sport type");
        return nullptr;
    }
}

SportType SportFactory::sportTypeFromString(const String& name)
{
    String upperName = name;
    upperName.toUpperCase();
    
    if (upperName == "MLB" || upperName == "BASEBALL")
    {
        return SportType::MLB;
    }
    else if (upperName == "NHL" || upperName == "HOCKEY")
    {
        return SportType::NHL;
    }
    
    Serial.println("WARNING: Unknown sport name '" + name + "', defaulting to MLB");
    return SportType::MLB;
}

String SportFactory::sportTypeToString(SportType type)
{
    switch (type)
    {
    case SportType::MLB:
        return "MLB";
    
    case SportType::NHL:
        return "NHL";
    
    default:
        return "UNKNOWN";
    }
}
