# ESP32 MLB Scoreboard

A real-time MLB baseball scoreboard powered by ESP32 and a 64x32 LED matrix display. Displays live game information including scores, inning, ball/strike/out count, and base runners for your favorite MLB team.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B11-orange)

## Features

- 🏟️ **Live Game Data** - Real-time scores from MLB Stats API
- 🎨 **Team Colors** - Authentic team color schemes
- 📊 **Game Details** - Score, inning, count, and base runners
- 🔄 **Auto-Refresh** - Updates every 30 seconds
- 📱 **Touch Button** - Cycle through teams manually
- 🌐 **WebSocket** - Real-time updates via WebSocket
- 📡 **WiFi Manager** - Easy WiFi configuration via AP mode
- ⏰ **NTP Sync** - Automatic time synchronization

## Hardware Requirements

- **ESP32 Development Board** (e.g., ESP32-DevKitC)
- **64x32 HUB75 LED Matrix Panel** (RGB, 1/16 scan)
- **5V Power Supply** (4-6A recommended)
- **Touch-capable GPIO pin** (T9/GPIO32 for button)

### Wiring

The project uses the ESP32 HUB75 library default pin configuration. Refer to your LED matrix panel pinout and the [library documentation](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA) for wiring details.

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- USB cable for programming
- Serial monitor (115200 baud)

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/ESP32-MLB-Scoreboard.git
cd ESP32-MLB-Scoreboard
```

### 2. Configure Your Settings

Edit `src/Config.cpp` to set your preferences:

```cpp
// WiFi AP name when in configuration mode
const char* AppConfig::WIFI_AP_NAME = "MLBScoreboard";

// NTP time configuration (default: EST)
const long NetworkConfig::GMT_OFFSET_SEC = -5 * 3600;  // -5 hours
```

Edit `src/Application.cpp` line 15 to set your default team:

```cpp
, currentTeam(TEAM_ID::BOSTON_REDSOX)  // Change to your team
```

### 3. Build and Upload

Using PlatformIO:

```bash
# Build the project
pio run

# Upload to ESP32
pio run --target upload

# Open serial monitor
pio device monitor
```

### 4. Connect to WiFi

On first boot:
1. Device creates WiFi AP named "MLBScoreboard"
2. Connect to it with your phone/laptop
3. Configuration page opens automatically
4. Select your WiFi network and enter password
5. Device connects and starts displaying games

## Usage

### Cycling Through Teams

- **Touch Button**: Briefly touch GPIO32 (T9) to cycle to the next team
- **WebSocket**: Send events via WebSocket to cycle teams automatically

### Display Information

The 64x32 display shows:
- **Top Left**: Away team abbreviation and score
- **Top Center**: Current inning (△ = top, ▽ = bottom)
- **Top Right**: Home team abbreviation and score  
- **Middle**: Baseball diamond showing base runners
- **Bottom**: Ball/strike/out count (green dots = balls, red dots = strikes/outs)

## Configuration

### Team Selection

Available teams (defined in `TeamInfo.h`):
- All 30 MLB teams with official color schemes
- Cycle through teams using the touch button
- Default team set in `Application.cpp`

### Display Settings

Edit `Config.h`:

```cpp
struct DisplayConfig {
    static const int PANEL_WIDTH = 64;      // Panel width in pixels
    static const int PANEL_HEIGHT = 32;     // Panel height in pixels
    static const int BRIGHTNESS = 32;       // 0-255
    static const int DISPLAY_WIDTH = 64;    // Total display width
};
```

### Network Settings

Edit `Config.cpp`:

```cpp
struct NetworkConfig {
    static const char* NTP_SERVER = "pool.ntp.org";
    static const long GMT_OFFSET_SEC = -5 * 3600;  // Your timezone
    static const int DAYLIGHT_OFFSET_SEC = 3600;   // DST offset
};
```

### Update Interval

Edit `Config.h`:

```cpp
struct AppConfig {
    static const unsigned long UPDATE_INTERVAL_MS = 30000;  // 30 seconds
};
```

## Architecture

The project follows a clean, layered architecture:

```
Application (Orchestrator)
├── Display Layer
│   ├── Matrix (Singleton)
│   └── GameDrawer (Rendering)
├── Network Layer
│   ├── NetworkManager (WiFi/NTP)
│   ├── ApiHandler (HTTP)
│   └── SocketHandler (WebSocket)
└── Data Layer
    ├── Game (Data Model)
    ├── TeamInfo (Team Data)
    └── Config (Settings)
```

See [AGENTS.md](AGENTS.md) for detailed architecture documentation.

## Development

### Project Structure

```
ESP32-MLB-Scoreboard/
├── src/
│   ├── main.cpp              # Entry point (27 lines)
│   ├── Application.h/cpp     # Main orchestrator
│   ├── Config.h/cpp          # Configuration
│   ├── TeamInfo.h/cpp        # Team data & colors
│   ├── Game.h/cpp            # Game data model
│   ├── ApiHandler.h/cpp      # MLB API client
│   ├── NetworkManager.h/cpp  # WiFi & NTP
│   ├── GameDrawer.h/cpp      # LED rendering
│   └── ...                   # Other components
├── platformio.ini            # Build configuration
├── AGENTS.md                 # Developer guide
└── README.md                 # This file
```

### Building from Source

```bash
# Install PlatformIO
pip install platformio

# Build
pio run

# Upload to device
pio run --target upload

# Clean build
pio run --target clean
```

### Memory Usage

- **RAM**: ~49KB / 320KB (15%)
- **Flash**: ~1.1MB / 3MB (35%)

## Troubleshooting

### Display Not Working

1. Check power supply (5V, sufficient amperage)
2. Verify wiring connections
3. Check serial monitor for errors
4. Ensure `display->begin()` returns true

### WiFi Connection Failed

1. Device creates AP "MLBScoreboard"
2. Connect and configure via captive portal
3. Check SSID and password
4. Verify 2.4GHz network (ESP32 doesn't support 5GHz)

### No Game Data

1. Check WiFi connection: `NetworkManager::isConnected()`
2. Verify API access: Check serial monitor for HTTP errors
3. Ensure team has a game scheduled today
4. Check JSON buffer size (4096 bytes)

### Touch Button Not Responding

1. Verify GPIO32 (T9) connection
2. Check `AppConfig::TOUCH_THRESHOLD` value
3. Monitor serial output for button events
4. Adjust threshold if needed (default: 35)

## API

The project uses the official [MLB Stats API](http://statsapi.mlb.com/api/):
- Endpoint: `http://statsapi.mlb.com/api/v1/schedule`
- No API key required
- Rate limiting: Not enforced for individual use

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on hardware
5. Submit a pull request

See [AGENTS.md](AGENTS.md) for coding guidelines.

## License

This project is open source and available under the MIT License.

## Acknowledgments

- [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA) - LED matrix library
- [ArduinoJson](https://arduinojson.org/) - JSON parsing
- [WiFiManager](https://github.com/tzapu/WiFiManager) - WiFi configuration
- MLB Stats API - Game data source

## Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/ESP32-MLB-Scoreboard/issues)
- **Documentation**: [AGENTS.md](AGENTS.md)
- **Hardware Guide**: See LED matrix library documentation

---

**Made with ⚾ for baseball fans**
