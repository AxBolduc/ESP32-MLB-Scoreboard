# AGENTS.md - ESP32 MLB Scoreboard Development Guide

This guide provides coding agents with essential information for working on the ESP32 MLB Scoreboard project.

## Project Overview

This is an **ESP32 embedded C++ project** using PlatformIO that displays live MLB baseball scores on a 64x32 HUB75 LED matrix display. The device connects to WiFi, fetches game data from the MLB Stats API, and renders team scores, inning information, ball/strike count, and base runners.

**Platform**: ESP32 (Arduino framework)
**IDE**: PlatformIO
**Language**: C++ (Arduino-flavored)
**Display**: 64x32 HUB75 LED Matrix Panel via DMA

---

## Build, Upload & Test Commands

### Build Commands
```bash
# Build the project
pio run

# Build with verbose output
pio run -v

# Clean build artifacts
pio run --target clean
```

### Upload/Flash Commands
```bash
# Upload to ESP32 device
pio run --target upload

# Upload and open serial monitor
pio run --target upload && pio device monitor
```

### Serial Monitor
```bash
# Open serial monitor (115200 baud)
pio device monitor

# Monitor with specific baud rate
pio device monitor -b 115200
```

### Test Commands
```bash
# Run all tests
pio test

# Run specific test
pio test -f test_name

# Run tests with verbose output
pio test -v
```

**Note**: The `test/` directory currently contains only a README. No unit tests are currently implemented. Tests would use PlatformIO's testing framework when added.

---

## Project Structure

```
ESP32-MLB-Scoreboard/
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp           # Main entry point (setup/loop)
│   ├── ApiHandler.h       # MLB API HTTP client
│   ├── SocketHandler.h    # WebSocket connection handler
│   ├── Game.h             # Game data model
│   ├── GameDrawer.h       # LED matrix rendering logic
│   ├── Matrix.h           # Singleton matrix display wrapper
│   ├── constants.h        # Global constants, colors, team IDs
│   ├── touchButton.h      # Touch button configuration
│   └── util.hpp           # Utility functions (URL generation, date)
├── include/               # Additional headers (empty)
├── lib/                   # Project-specific libraries
└── test/                  # Unit tests (not yet implemented)
```

---

## Code Style Guidelines

### File Organization
- **Header-only pattern**: Most classes are implemented in `.h` files with inline implementations
- **Include guards**: Use `#ifndef INCLUDE_<NAME>_H` / `#define INCLUDE_<NAME>_H` / `#endif`
- **Single responsibility**: Each header handles one component (API, drawing, sockets, etc.)

### Naming Conventions
- **Classes**: PascalCase (`GameDrawer`, `ApiHandler`, `SocketHandler`)
- **Functions/Methods**: camelCase (`drawGame()`, `getHomeTeam()`, `updateScreen()`)
- **Variables**: camelCase (`teamId`, `currentScore`, `time_counter`)
- **Constants/Defines**: SCREAMING_SNAKE_CASE (`PANEL_WIDTH`, `JSON_BUFFER_SIZE`, `BASE_URL`)
- **Enums**: SCREAMING_SNAKE_CASE for both type and values (`TEAM_ID::BOSTON_REDSOX`, `COLORS::RED`)
- **Private members**: Use `this->` prefix when accessing in methods

### Imports/Includes
```cpp
// Order: System/Arduino libraries first, then project headers
#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h>
#include "constants.h"
#include "ApiHandler.h"
#include "Matrix.h"
```
- Use angle brackets `<>` for library includes
- Use quotes `""` for project headers
- Include `constants.h` early when needed for macros

### Types & Memory Management
- **Prefer stack allocation** for simple objects: `Game game(data);`
- **Use pointers for large objects** and when passing to rendering: `Game *game = new Game(data);`
- **ArduinoJson**: Use `DynamicJsonDocument` with `JSON_BUFFER_SIZE` constant (4096 bytes)
- **Explicit cleanup**: Delete heap objects in destructors, but **avoid** `delete(this)` anti-pattern (see ApiHandler line 71)
- **Singletons**: Matrix uses static singleton pattern (`Matrix::get()`)

### Functions & Methods
- **Inline getters**: Define simple getters inline in header: `int getHomeTeamScore() { return this->homeTeamScore; }`
- **Doxygen comments**: Use for public APIs:
  ```cpp
  /**
   * @brief Get the schedule for a specific date
   */
  ```
- **Serial logging**: Use `Serial.println()` and `Serial.printf()` for debugging
- **Constructors/Destructors**: Implement in header after class definition

### Error Handling
- **HTTP errors**: Check `httpCode == 200` before processing
- **Null checks**: Check for `nullptr` before dereferencing:
  ```cpp
  if (schedule == nullptr) {
      Serial.println("Failed to get schedule for some reason");
      return;
  }
  ```
- **Time failures**: Handle `getLocalTime()` failures gracefully
- **Memory allocation**: Check if DMA display begins successfully
- **WiFi failures**: Restart ESP32 after timeout: `ESP.restart();`

### Display & Graphics
- **Color format**: RGB565 using `COLORS` enum (defined in constants.h)
- **Team colors**: Use `TEAM_BG_COLORS` and `TEAMS_TEXT_COLORS` maps
- **Drawing order**: Clear screen, draw layout, draw dynamic content
- **Text rendering**: Use `display->setCursor()` then `display->print()`
- **Coordinate system**: Origin (0,0) is top-left corner

### Constants & Configuration
- All hardware/API constants in `constants.h`
- Panel dimensions: 64x32 pixels (`PANEL_WIDTH`, `PANEL_HEIGHT`)
- Base URL: MLB Stats API (`BASE_URL`)
- WebSocket URL for live updates (`WEBSOCKET_URL`)
- Team IDs: 30 MLB teams in `TEAM_ID` enum and `TEAM_IDS[]` array

### Arduino/ESP32 Patterns
- **setup()**: Initialize hardware, WiFi, display in `setup()`
- **loop()**: Poll WebSocket, check button, update display every 30s
- **WiFiManager**: Auto-connect with AP fallback (`MLBScoreboard` SSID)
- **Touch buttons**: Use AceButton library with touch threshold
- **Time/NTP**: Configure timezone with `configTime()` for EST

---

## Dependencies (platformio.ini)

```ini
lib_deps = 
    adafruit/Adafruit GFX Library@^1.11.5
    mrfaptastic/ESP32 HUB75 LED MATRIX PANEL DMA Display@^3.0.5
    fastled/FastLED@^3.5.0
    bblanchon/ArduinoJson@^6.20.0
    https://github.com/tzapu/WiFiManager
    https://github.com/bxparks/AceButton
    links2004/WebSockets@^2.6.1
```

---

## Common Tasks

### Adding a New Display Feature
1. Add drawing method to `GameDrawer` class in `GameDrawer.h`
2. Call from `drawGame()` or create public method
3. Use existing primitives: `fillRect`, `drawCircle`, `fillTriangle`, etc.
4. Follow color scheme from `COLORS` enum

### Adding a New API Endpoint
1. Add method to `ApiHandler` class in `ApiHandler.h`
2. Use `http->begin(url)` and `http->GET()`
3. Deserialize JSON with `deserializeJson(*doc, http->getStream())`
4. Return `JsonObject` or `DynamicJsonDocument*`
5. Always call `http->end()` to close connection

### Adding a New Team
1. Add enum value to `TEAM_ID` in `constants.h`
2. Add team ID to `TEAM_IDS[]` array
3. Add team colors to `TEAM_BG_COLORS` and `TEAMS_TEXT_COLORS` maps
4. Update `NUM_TEAM_IDS` count

### Debugging
- Use `Serial.println()` liberally (monitor at 115200 baud)
- Check WiFi connection status before API calls
- Verify JSON structure matches MLB API response
- Test display rendering incrementally

---

## Important Notes

- **No Cursor/Copilot rules found** in this repository
- **Memory constraints**: ESP32 has limited heap; avoid large allocations
- **JSON buffer size**: 4096 bytes may need adjustment for complex API responses
- **Display updates**: Minimize full redraws; update only changed regions when possible
- **WiFi stability**: Device restarts on connection failure; add reconnection logic if needed
- **Operator overloading**: Custom `++` operators for cycling through teams (`TEAM_ID` enum)

---

## Getting Started (for Agents)

1. Ensure PlatformIO is installed and available in PATH
2. Run `pio run` to build and verify dependencies
3. For hardware testing, connect ESP32 and run `pio run --target upload`
4. Open serial monitor to see debug output: `pio device monitor`
5. Read existing code in `main.cpp` and key headers before making changes
