#include "NHLSport.h"

NHLSport::NHLSport(MatrixPanel_I2S_DMA* display)
    : Sport(display)
{
    // Initialize NHL-specific components
    drawer = createDrawer();
    apiHandler = createApiHandler();
}

NHLSport::~NHLSport()
{
    // Base class handles cleanup of drawer and apiHandler
}

GameDrawer* NHLSport::createDrawer()
{
    return new NHLGameDrawer(display);
}

ApiHandler* NHLSport::createApiHandler()
{
    return new NHLApiHandler();
}

TeamInfo* NHLSport::getTeamInfo()
{
    return &teamInfo;
}

Game* NHLSport::createGame(const JsonObject& data)
{
    return new NHLGame(data);
}

String NHLSport::getSportName() const
{
    return "NHL";
}

SportType NHLSport::getSportType() const
{
    return SportType::NHL;
}
