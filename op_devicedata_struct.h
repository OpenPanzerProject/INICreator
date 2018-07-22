#ifndef OP_DEVICEDATA_STRUCT
#define OP_DEVICEDATA_STRUCT

#include <arduino_compat.h>         // Gives us Arduino-like data type names
#include <op_defines.h>

// This struct is similiar to the _eeprom_data struct in Arduino, but not exactly!
// We include a few more things here than there. These are all settings we might be interested
// in reading from the device, not just things the device stores in EEPROM. (For example, we might want
// to know the current throttle stick position, but that is not a setting the device stores in EEPROM)
//--------------------------------------------------------------------------------------------------------------------------------------->>
struct _device_data {
// First Var
    uint8_t FirstVar;

// Channel settings
    channel_settings RCChannel[CHANNELS];        // Create CHANNELS number of type channel_settings

// Sound settings
    squeak_settings Squeak[NUM_SQUEAKS];
    uint8_t	 MinSqueakSpeedPct;					 // Prevent squeaks from occuring when vehicle is moving slower than this percent of movement
    boolean  HeadlightSound_Enabled;			 // Is the headlight sound enabled or not
    boolean  TurretSound_Enabled;				 // Is turret rotation sound enabled or not
    boolean  BarrelSound_Enabled;                // Is barrel elevation sound enabled or not

    uint8_t  VolumeEngine;                       // Relative, not absolute volumes
    uint8_t  VolumeTrackOverlay;                 // Relative, not absolute volumes
    uint8_t  VolumeEffects;                      // Relative, not absolute volumes

// Lights
    uint16_t Light1FlashTime_mS;				 // Single blink on time in milliseconds.
    uint16_t Light1BlinkOnTime_mS;               // This is the time the blink stays on.
    uint16_t Light1BlinkOffTime_mS;              // This is the time the blink stays off.
    uint16_t Light2FlashTime_mS;				 // Single blink on time in milliseconds.
    uint16_t Light2BlinkOnTime_mS;               // This is the time the blink stays on.
    uint16_t Light2BlinkOffTime_mS;              // This is the time the blink stays off.
    uint16_t Light3FlashTime_mS;				 // Single blink on time in milliseconds.
    uint16_t Light3BlinkOnTime_mS;               // This is the time the blink stays on.
    uint16_t Light3BlinkOffTime_mS;              // This is the time the blink stays off.

// Servo
    uint16_t ServoTimeToRecoil;                  // In mS
    uint16_t ServoTimeToReturn;                  // In mS
    boolean  ServoReversed;
    uint8_t  ServoEndPointRecoiled;
    uint8_t  ServoEndPointBattery;

// Throttle
    boolean centerThrottle;                      // If true, stick centered = idle, if false stick at low position = idle
    uint8_t idleDeadband;                        // Percentage of throttle movement to ignore around idle position

// Engine
    boolean EngineAutoStart;					 // If true, engine will auto-start on first blip of throttle. If false, start engine with user selected trigger.
    int32_t EngineAutoStopTime_mS;				 // If positive, after this amount of time at idle the engine will turn itself off (in milliseconds). 0 to disable.

// Program setting
    boolean PrintDebug;							 // If true, will print debugging messages out the DebugSerial serial port

// Marker
    uint32_t InitStamp;       		 		     //
};


#endif	// Define OP_DEVICEDATA_STRUCT
