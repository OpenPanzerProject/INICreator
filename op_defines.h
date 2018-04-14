#ifndef OP_DEFINES
#define OP_DEFINES

#include <arduino_compat.h>

#define DEFAULTBAUDRATE        115200


// Function IDs *below* 1000 are user sounds.
// User Sound Function IDs are constructed by (user sound number * 10) + (1,2,3) to represent play (1), repeat (2), or stop (3)
// Example: sound  2 repeat = Function ID is 22
// Example: sound 14 play   = Function ID is 141
// Example: sound 22 stop   = Function ID is 223
#define MAX_NUM_USER_SOUNDS 22
#define function_id_usersound_multiplier        10
#define function_id_usersound_min_range         10          // (sound 1  * multiplier)
#define function_id_usersound_max_range         999         // (sound 99 *  multiplier)


// Function IDs *above* 1000 are specific actions.
// They are constructed by adding 1000 to the switch_function enum
#define function_id_other_function_start_range  1000        // start of regular (non-user sound) function IDs
enum switch_function : uint8_t {
    SF_NULL = 0,
    SF_ENGINE_START,
    SF_ENGINE_STOP,
    SF_ENGINE_TOGGLE,
    SF_CANNON_FIRE,
    SF_MG_FIRE,
    SF_MG_STOP,
    SF_MG2_FIRE,
    SF_MG2_STOP,
    SF_USER                 // Special case that will get converted to a user sound function
};


// RC Settings
const byte CHANNELS 			= 5;
const byte MIN_SWITCH_POSITIONS = 2;
const byte MAX_SWITCH_POSITIONS = 6;

// Aux channels
enum switch_positions : byte {
    SP_NULLPOS = 0,
    SP_POS1,
    SP_POS2,
    SP_POS3,
    SP_POS4,
    SP_POS5,
    SP_POS6
};

// Channel types
enum channel_function : uint8_t {
    CF_SWITCH = 0,
    CF_ENGINE_SPEED,
    CF_VOLUME
};

enum sound_action : uint8_t {
    SOUND_PLAY = 1,
    SOUND_REPEAT = 2,
    SOUND_STOP = 3
};

struct channel_settings{
    boolean reversed;                                           // Is the channel reversed
    boolean Digital;                                            // Is this a digital channel (switch input or discrete function) or an analog input (for engine speed/volume, etc)
    channel_function chFunction;                                // Channel function - either switch or some analog control
    uint8_t numPositions;                                       // If digital, how many positions does the switch have
    uint16_t swFunctionID[MAX_SWITCH_POSITIONS];                // Function ID for digital functions (one for each switch position) - raw number
    switch_function swFunction[MAX_SWITCH_POSITIONS];           // Actual function code (not necessarily same as ID)
    uint16_t userSoundNum[MAX_SWITCH_POSITIONS];                // If swFunction = SF_USER, let's save the user sound number...
    sound_action userSoundAction[MAX_SWITCH_POSITIONS];         // ...and the play action (play, repeat, or stop)
};


// Squeak Settings
const byte NUM_SQUEAKS = 6;
struct squeak_settings{
    uint16_t MinInterval_mS;			 // Minimum length of time between Squeak 1 intervals
    uint16_t MaxInterval_mS;			 // Maximum length of time between Squeak 1 intervals
    boolean  Enabled;					 // Is Squeak1 enabled or not
};


#endif // OP_DEFINES


