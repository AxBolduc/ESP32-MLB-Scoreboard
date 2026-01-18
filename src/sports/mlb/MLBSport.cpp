#include "MLBSport.h"

MLBSport::MLBSport(MatrixPanel_I2S_DMA* display)
    : Sport(display)
{
    // Initialize MLB-specific components
    drawer = createDrawer();
    apiHandler = createApiHandler();
}

MLBSport::~MLBSport()
{
    // Base class handles cleanup of drawer and apiHandler
}

GameDrawer* MLBSport::createDrawer()
{
    return new MLBGameDrawer(display);
}

ApiHandler* MLBSport::createApiHandler()
{
    return new MLBApiHandler();
}

TeamInfo* MLBSport::getTeamInfo()
{
    return &teamInfo;
}

Game* MLBSport::createGame(const JsonObject& data)
{
    return new MLBGame(data);
}

String MLBSport::getSportName() const
{
    return "MLB";
}

SportType MLBSport::getSportType() const
{
    return SportType::MLB;
}
