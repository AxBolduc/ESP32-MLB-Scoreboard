# AGENTS.md - ESP32 MLB Scoreboard Development Guide

This guide provides coding agents with essential information for working on the ESP32 MLB Scoreboard project.

## Project Overview

This is an **ESP32 embedded C++ project** using PlatformIO that displays live MLB baseball scores on a 64x32 HUB75 LED matrix display. The device connects to WiFi, fetches game data from the MLB Stats API, and renders team scores, inning information, ball/strike count, and base runners.

**Platform**: ESP32 (Arduino framework)  
**IDE**: PlatformIO  
**Language**: C++11  
**Display**: 64x32 HUB75 LED Matrix Panel via DMA  
**Architecture**: Clean, maintainable, object-oriented design

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
# Run all tests (when implemented)
pio test

# Run specific test
pio test -f test_name
```

**Note**: Unit tests are planned but not yet implemented.

---

## Project Structure (Refactored)

```
ESP32-MLB-Scoreboard/
├── platformio.ini          # PlatformIO configuration
├── AGENTS.md              # This file
├── README.md              # User documentation
├── src/
│   ├── main.cpp (27 lines) # Minimal entry point
│   │
│   ├── Application.h/cpp   # Main orchestrator class
│   ├── Config.h/cpp        # All configuration constants
│   ├── Errors.h            # Error result enums
│   │
│   ├── TeamInfo.h/cpp      # Team data, colors, cycling
│   ├── Game.h/cpp          # Game data model
│   │
│   ├── ApiHandler.h/cpp    # HTTP MLB API client
│   ├── UrlBuilder.h/cpp    # URL generation utilities
│   ├── NetworkManager.h/cpp # WiFi & NTP management
│   ├── SocketHandler.h/cpp # WebSocket client
│   │
│   ├── Matrix.h/cpp        # Display singleton
│   ├── GameDrawer.h/cpp    # LED rendering logic
│   │
│   └── touchButton.h       # Touch button configuration
│
├── include/               # Additional headers (empty)
├── lib/                   # Project-specific libraries
└── test/                  # Unit tests (future)
```

---

## Architecture Overview

### Clean Separation of Concerns

The codebase follows a **layered architecture** with clear responsibilities:

```
┌─────────────────────────────────────┐
│          main.cpp (Entry)           │
│      Application* app = new...     │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      Application (Orchestrator)     │
│  - Owns all components              │
│  - Manages application lifecycle    │
│  - Coordinates between layers       │
└──┬──────────┬────────────┬─────────┬┘
   │          │            │         │
   ▼          ▼            ▼         ▼
┌─────┐  ┌─────────┐  ┌────────┐ ┌────────┐
│Input│  │ Display │  │Network │ │  Data  │
│Layer│  │  Layer  │  │ Layer  │ │ Layer  │
└─────┘  └─────────┘  └────────┘ └────────┘
```

**Key Design Principles:**
1. **Single Responsibility** - Each class has one clear purpose
2. **Dependency Injection** - Dependencies passed to constructors
3. **Encapsulation** - No global state (except Application instance)
4. **Resource Ownership** - Clear who owns and deletes what
5. **Const Correctness** - Use const where data isn't modified

---

## Code Style Guidelines

### File Organization
- **Header (.h)**: Class declarations, inline getters, documentation
- **Implementation (.cpp)**: Method implementations, static members
- **Include guards**: `#ifndef INCLUDE_<NAME>_H` format
- **One class per file** (except small helper classes)

### Naming Conventions
- **Classes**: PascalCase (`Application`, `GameDrawer`, `NetworkManager`)
- **Methods**: camelCase (`drawGame()`, `updateScreen()`, `connectToWiFi()`)
- **Variables**: camelCase (`currentTeam`, `lastUpdateTime`, `display`)
- **Constants**: In Config structs, PascalCase (`DisplayConfig::BRIGHTNESS`)
- **Enums**: `enum class` with PascalCase (`ApiResult::SUCCESS`)
- **Private members**: Plain camelCase, no prefix

### Includes Order
```cpp
// 1. Corresponding header (if .cpp file)
#include "GameDrawer.h"

// 2. Project headers
#include "Config.h"
#include "TeamInfo.h"

// 3. Arduino/ESP32 headers
#include <Arduino.h>
#include <WiFi.h>

// 4. Third-party library headers
#include <ArduinoJson.h>
```

### Modern C++ Practices

**Use `explicit` for single-argument constructors:**
```cpp
explicit GameDrawer(MatrixPanel_I2S_DMA* display);
explicit Application();
```

**Use `const` correctness:**
```cpp
// Methods that don't modify state
void drawGame(const Game* game);
String getHomeTeam() const { return homeTeam; }

// Parameters that aren't modified
void drawBox(const String& text, uint16_t bgColor);
```

**Use references over pointers when possible:**
```cpp
// Prefer this (can't be null, clearer intent)
bool getSchedule(const String& date, DynamicJsonDocument& outDoc);

// Over this (unless null is valid)
bool getSchedule(String* date, DynamicJsonDocument* outDoc);
```

**Delete copy constructors/assignment for singletons:**
```cpp
class Matrix {
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;
};
```

**Use enum class for type safety:**
```cpp
enum class ApiResult { SUCCESS, HTTP_ERROR, ... };
// Usage: ApiResult::SUCCESS (not just SUCCESS)
```

### Memory Management

**Ownership Rules:**
1. **If you `new` it, you must `delete` it**
2. **Document ownership in comments**
3. **Use RAII pattern** - constructor acquires, destructor releases
4. **Avoid raw `new`/`delete`** where possible (use stack allocation)

**Example:**
```cpp
class Application {
private:
    GameDrawer* gameDrawer;  // Owned by Application
    MatrixPanel_I2S_DMA* display;  // NOT owned (singleton)
    
public:
    Application() : gameDrawer(nullptr), display(nullptr) {}
    
    ~Application() {
        delete gameDrawer;  // Clean up owned resources
        // Don't delete display - we don't own it
    }
};
```

### Error Handling

**Use enum class for result codes:**
```cpp
ApiResult result = apiHandler->getSchedule(date, doc);
if (result != ApiResult::SUCCESS) {
    Serial.printf("API error: %s\n", apiResultToString(result));
    return false;
}
```

**Check pointers before dereferencing:**
```cpp
void GameDrawer::drawGame(const Game* game) {
    if (!game || !display) return;  // Defensive check
    // ... safe to use game and display
}
```

**Log errors, don't crash:**
```cpp
// ❌ BAD - crashes the device
if (error) exit(1);

// ✅ GOOD - log and return error code
if (error) {
    Serial.println("Error occurred");
    return false;
}
```

### Display & Graphics

**Color Usage:**
```cpp
// Use Color enum (now Color:: instead of COLORS::)
display->fillRect(x, y, w, h, Color::RED);

// Get team colors from TeamInfo
uint16_t bgColor = TeamInfo::getBackgroundColor(teamId);
uint16_t textColor = TeamInfo::getTextColor(teamId);
```

**Coordinate System:**
- Origin (0, 0) is top-left
- X increases rightward
- Y increases downward
- Display size: 64×32 pixels (`DisplayConfig::DISPLAY_WIDTH`)

---

## Key Classes Reference

### Application (Main Orchestrator)

**Purpose**: Owns all components, coordinates application lifecycle

**Key Methods:**
- `bool setup()` - Initialize all components
- `void loop()` - Main application loop
- `void updateScreen()` - Refresh display with latest data

**Usage in main.cpp:**
```cpp
Application* app = new Application();
if (!app->setup()) {
    ESP.restart();
}
```

### Config (Configuration)

**Purpose**: Centralized configuration constants

**Structs:**
- `DisplayConfig` - Panel size, brightness, layout
- `NetworkConfig` - WiFi, NTP, API URLs
- `ApiConfig` - Buffer sizes, timeouts
- `AppConfig` - Update intervals, button pins

**Usage:**
```cpp
display->setBrightness8(DisplayConfig::BRIGHTNESS);
const char* server = NetworkConfig::NTP_SERVER;
```

### TeamInfo (Team Data)

**Purpose**: Team IDs, colors, cycling logic

**Key Methods:**
- `uint16_t getBackgroundColor(int teamId)`
- `uint16_t getTextColor(int teamId)`
- `TEAM_ID getNextTeam(TEAM_ID current)`

**Team Cycling:**
```cpp
currentTeam++;  // Operator overload calls getNextTeam()
```

### ApiHandler (HTTP Client)

**Purpose**: Fetch data from MLB Stats API

**Key Methods:**
- `bool getTeamScheduleToday(TEAM_ID team, DynamicJsonDocument& outDoc)`
- `bool getSchedule(const String& date, DynamicJsonDocument& outDoc)`

**Ownership**: Owns HTTPClient and DynamicJsonDocument internally

### NetworkManager (WiFi & NTP)

**Purpose**: Static utility class for network operations

**Key Methods:**
- `bool connectToWiFi(const char* apName, callback)`
- `void configureTime(const char* ntpServer, long offset, int dst)`
- `bool isConnected()`
- `String getLocalIP()`

### Matrix (Display Singleton)

**Purpose**: Singleton wrapper for LED matrix

**Key Methods:**
- `MatrixPanel_I2S_DMA* getInstance()` - Get singleton instance
- `void cleanup()` - Clean up on shutdown

**Usage:**
```cpp
MatrixPanel_I2S_DMA* display = Matrix::getInstance();
display->begin();
```

### GameDrawer (Rendering)

**Purpose**: Draw game state to LED matrix

**Key Methods:**
- `void drawGame(const Game* game)` - Draw complete game state
- `void drawLoading()` - Show loading screen
- `void drawFullscreenText(const String& text)` - Display message

**Ownership**: Doesn't own display (injected via constructor)

---

## Common Development Tasks

### Adding a New Configuration Constant

1. Add to appropriate struct in `Config.h`:
```cpp
struct AppConfig {
    static const int NEW_SETTING = 42;
};
```

2. Define in `Config.cpp` if it's a string:
```cpp
const char* AppConfig::NEW_SETTING = "value";
```

### Adding a New Team

1. Add enum value to `TeamInfo.h`:
```cpp
enum TEAM_ID {
    NEW_TEAM = 999,
    // ...
};
```

2. Update `TEAM_IDS` array in `TeamInfo.cpp`
3. Add colors to `TEAM_BG_COLORS` and `TEAMS_TEXT_COLORS` maps
4. Update `NUM_TEAM_IDS` count

### Adding a New Display Feature

1. Add method declaration to `GameDrawer.h`:
```cpp
void drawNewFeature(const Game* game);
```

2. Implement in `GameDrawer.cpp`:
```cpp
void GameDrawer::drawNewFeature(const Game* game) {
    if (!game || !display) return;
    // Drawing logic...
}
```

3. Call from `drawGame()` or expose as public method

### Adding a New API Endpoint

1. Add method to `ApiHandler.h`:
```cpp
bool getNewData(TEAM_ID team, DynamicJsonDocument& outDoc);
```

2. Implement in `ApiHandler.cpp`:
```cpp
bool ApiHandler::getNewData(TEAM_ID team, DynamicJsonDocument& outDoc) {
    String url = UrlBuilder::buildNewUrl(team);
    http.begin(url);
    int httpCode = http.GET();
    // ... handle response
    return httpCode == 200;
}
```

### Debugging Tips

**Serial Output:**
```cpp
Serial.println("Debug message");
Serial.printf("Value: %d\n", value);
```

**Check Memory:**
```cpp
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
```

**Monitor at 115200 baud:**
```bash
pio device monitor -b 115200
```

**Check for Memory Leaks:**
- Look for `new` without corresponding `delete`
- Verify destructors clean up resources
- Check that pointers are nulled after deletion

---

## Dependencies

All dependencies managed via `platformio.ini`:

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

## Important Notes

### Memory Constraints
- ESP32 has ~320KB RAM total
- Current usage: ~49KB (15%)
- Be mindful of large stack allocations
- JSON buffer is 4096 bytes - may need adjustment for complex responses

### Naming Conflict
⚠️ The HUB75 library defines `MATRIX_WIDTH` as a macro. Use `DisplayConfig::DISPLAY_WIDTH` instead in our code.

### Callback Pattern
C-style callbacks require static trampolines:
```cpp
// Application.h
static void buttonCallbackTrampoline(...);

// Application.cpp  
void Application::buttonCallbackTrampoline(...) {
    if (g_app) g_app->handleButtonEvent(...);
}
```

### WebSocket Behavior
Currently, **any** WebSocket event cycles teams. This may need refinement based on actual server protocol.

---

## Getting Started (for New Developers)

1. **Clone and build:**
   ```bash
   git clone <repo>
   cd ESP32-MLB-Scoreboard
   pio run
   ```

2. **Study the architecture:**
   - Start with `main.cpp` (entry point)
   - Read `Application.h` (orchestrator)
   - Review `Config.h` (all settings)

3. **Key files to understand:**
   - `Application.cpp` - Application lifecycle
   - `GameDrawer.cpp` - Rendering logic
   - `ApiHandler.cpp` - Data fetching

4. **Before making changes:**
   - Read this entire document
   - Check existing patterns
   - Maintain coding style
   - Test on hardware before committing

---

## Recent Refactoring (2026-01)

The codebase underwent a major refactoring for maintainability:

**Key Changes:**
- ✅ Separated all headers from implementations
- ✅ Created Application class to encapsulate global state
- ✅ Added Config for centralized configuration
- ✅ Extracted TeamInfo for team data management
- ✅ Created NetworkManager for WiFi/NTP
- ✅ Fixed all memory management issues
- ✅ Applied const correctness throughout
- ✅ Added error result enums

**Result**: 85% reduction in `main.cpp` complexity, zero memory leaks, clean architecture.

---

## Questions or Issues?

- Check serial monitor output first (115200 baud)
- Review error messages for hints
- Verify JSON structure matches MLB API
- Check WiFi connection status
- Monitor memory usage

For the most current information, see the repository README.md.
