#ifndef OP_DEFINES
#define OP_DEFINES

#include <arduino_compat.h>


// Firmware Location
// -----------------------------------------------------------------------------------------------------
// Not very sophisticated. Here we store the URLs to the latest release hex and version files, and we
// assume they will never change in a million years.
#define LATEST_RELEASE_VERSION_URL_TEENSYSOUND    "http://openpanzer.org/downloads/soundcard/firmware/version.txt"
#define LATEST_RELEASE_HEX_URL_TEENSYSOUND        "http://openpanzer.org/downloads/soundcard/firmware/opsound.hex"

// General
// -----------------------------------------------------------------------------------------------------
#define DEFAULTBAUDRATE         115200      // Baud rate
#define NUM_LIGHTS              3           // Number of light outputs
#define MAX_NUM_USER_SOUNDS     20          // How many user sounds can the sound card handle

// Channel types
// -----------------------------------------------------------------------------------------------------
const byte CHANNELS 			= 5;        // Number of channels
enum channel_type : uint8_t {
    CT_SWITCH = 0,                          // "Digital" - signal is extracted to switch positions
    CT_ENGINE_SPEED,                        //  Analog   - input represents engine speed
    CT_VOLUME                               //  Analog   - input represents volume
};


// Switch positions
// -----------------------------------------------------------------------------------------------------
// In the case where channel type = CT_SWITCH
const byte MIN_SWITCH_POSITIONS = 2;
const byte MAX_SWITCH_POSITIONS = 6;
enum switch_positions : byte {
    SP_NULLPOS = 0,
    SP_POS1,
    SP_POS2,
    SP_POS3,
    SP_POS4,
    SP_POS5,
    SP_POS6
};


// Switch functions
// -----------------------------------------------------------------------------------------------------
// In the case where channel type = CT_SWITCH
enum switch_function : uint8_t {
    SF_NULL = 0,
    SF_ENGINE_START,
    SF_ENGINE_STOP,
    SF_ENGINE_TOGGLE,
    SF_CANNON_FIRE,
    SF_MG,
    SF_LIGHT,
    SF_USER,
    SF_SOUNDBANK
};


// Switch actions
// -----------------------------------------------------------------------------------------------------
// Some (but not all) switch functions are further defined by actions as well as action numbers
// For example, the SF_USER function will control a user sound, but the action number defines which sound,
// and the action defines whether to play it, stop it, or repeat it.
enum switch_action : uint8_t {
    ACTION_NULL = 0,
    ACTION_ONSTART = 1,             // Turn on, or start
    ACTION_OFFSTOP = 2,             // Turn off, or stop
    ACTION_REPEATTOGGLE = 3,        // Repeat, or toggle
    ACTION_STARTBLINK = 4,          // Start blinking
    ACTION_TOGGLEBLINK = 5,         // Toggle blinking
    ACTION_FLASH = 6,               // Flash
    ACTION_PLAYNEXT = 7,            // Sound bank - play next
    ACTION_PLAYPREV = 8,            // Sound bank - play previous
    ACTION_PLAYRANDOM = 9           // Sound bank - play random
};


// Function ID
// -----------------------------------------------------------------------------------------------------
// The ID is a single number that combines the switch function, action, and action number
// It is defined as (switch function * 10,000) + (switch action * 100) + (action number)
#define multiplier_switchfunction   10000
#define multiplier_switchaction     100


// Complete Channel Settings
// -----------------------------------------------------------------------------------------------------
// A collection of all settings related to each channel
struct channel_settings{
    boolean reversed;                                           // Is the channel reversed
    boolean Digital;                                            // Is this a digital channel (switch input or discrete function) or an analog input (for engine speed/volume, etc)
    channel_type chType;                                        // Channel type - either switch or some analog control
    uint8_t numPositions;                                       // If digital, how many positions does the switch have
    uint32_t swFunctionID[MAX_SWITCH_POSITIONS];                // Function ID for digital functions (one for each switch position) - raw number
    switch_function swFunction[MAX_SWITCH_POSITIONS];           // Actual switch function
    uint8_t actionNum[MAX_SWITCH_POSITIONS];                    // Certain functions require a number, such as user sounds, MG, cannon, lights
    switch_action switchAction[MAX_SWITCH_POSITIONS];           // And also a modifier to describe the action (play, repeat, stop, on, off, blink, flash, toggle, etc...)
};


// Squeak Settings
// -----------------------------------------------------------------------------------------------------
const byte NUM_SQUEAKS = 6;
struct squeak_settings{
    uint16_t MinInterval_mS;                // Minimum length of time between Squeak intervals
    uint16_t MaxInterval_mS;                // Maximum length of time between Squeak intervals
    boolean  Enabled;                       // Is Squeak enabled or not
};


#endif // OP_DEFINES


