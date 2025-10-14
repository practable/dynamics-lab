# Dynamics Lab Firmware

Please Note: This firmware relies on a MODIFIED version of the uStepperS32 library, please uninstall any version installed from the Arduino library manager and install [this](https://github.com/ImogenWren/uStepperS32) version available at [https://github.com/ImogenWren/uStepperS32](https://github.com/ImogenWren/uStepperS32)

## Current Version 
-> [dynamics-lab-firmware-V1.0.0](https://github.com/practable/dynamics-lab/tree/fw-dev/fw/dynamics_lab_firmware_V1_x_x)

## Calibration Procedure: Setting home position (V1.0.0 - Beta Firmware)
#### Setting Secret
_Secret must be set before calibration data can be entered. Once set, a finite number (20) of calibrations are permitted. To calibrate after this point, secret must be reset using `Resetting Secret` procedure_
- Enter new 8-character secret using command `{"set":"secret","to":"XXXXXXXX"}`
- Once set this is persistant and will require re-programming firmware to reset
<br><br>

_All following steps should be carried out during runtime without powering off in between. If steps are missed and status is unknown, or system has run out of calibration memory, please see section `Resetting Secret`_

#### Finding Home Position & Calibration Offset figure
- `{"set":"cal","to":"0"} ` to remove any existing offset
- Use `{"set":goto","to":"X"}` (int -> goto angle) and `{"set":"move","to":"X"}` (float -> move angle) to position weighted blade in the 12 o'clock or 0 degree position
- Note down `"pos(raw)"` value from Serial JSON stream. Retain this value as it will be required later.
- `{"set":"cal","to":"{pos(raw)}"} ` to apply offset.
<br>

#### Testing Calibration
- Use `{"set":goto","to":"180"}` to position weighted blade away from home position.
- Test homing function with `{"set":"home"}`. Weighted blade should return to the home position
- If nessissary run complete experiment to check for any deviation from nominal data set.
- Repeat untill happy with system calibration
- When happy with calibration, move on to `Saving Calibration Data to Persistant Memory`
- <br>

#### Saving Calibration Data to Persistant Memory
- use command ` {"set":"setcal","to":"0 - 32k", "auth":"XXXXXXXX"}` using the integer value previously noted down, and the secret entered into persistant memory in the first step. This will error if authorisation secret is incorrect, or the system has run out of available calibrations
- Use command ` {"set":"getcal"} ` to check contents of persistant memory
<br>

##### Possible Errors
- If error `signature did not match` secret may not have been entered
<br>

#### Resetting Secret
- To reset secret firmware file must be modified.
- In file `secretObject.h` find line `#define PROGRAM_SIGNATURE 0xXXXXXXXX`
- This value is arbitary and simply validates the data stored in persistant memory with the currently installed firmware.
- Change this value and re-Upload firmware.
- On boot-up, system will not recognise previous signature and will allow entry of a new secret (though calibration data may still be visible having been recalled from persistant memory -> this will be overwritten when new secret is added)
<br><br>

_Verbose Command Structure_
```
   {"set":"hz","to": -20 to 20}                       -> Set Motor Speed in Hz   
   {"set":"rpm","to": -200 to 200}                    -> Set Motor Speed in RPM  
   {"set":"home"}                                     -> Move Motor to home pos  
   {"set":"cal","to":0-32768}                         -> Set the home position offset calibration 
   {"set":"free"}                                     -> Set freewheel brake mode
   {"set":"brake"}                                    -> Set coolbrake brake mode 
   {"set":"goto","to": -360 to 360}                   -> Goto Angle (int val)
   {"set":"move","to": -360 to 360}                   -> Move Angle  (float val)       
   {"set":"sample","to": 1 to 200}                    -> Set Samplerate in Hz (dflt: 200)
   {"set":"print","to": 1 to 50}                      -> Set Print Rate in Hz (dflt: 50)
   {"set":"stream"}                                   -> Start Data Streaming    
   {"set":"endst"}                                    -> End Data Streaming      
   {"set":"snap"}                                     -> Take Data Snapshot       
   {"set":"time","to": 1 to 250000 }                  -> Set Time for Data Snapshot (mS)  
   {"set":"ping"}                                     -> Ping Servo               
   {"set":"offset","to":-32k to 32k}                  -> DEPRECIATED
   {"set":"secret","to":"XXXXXXXX"}                   -> Set 8 character secret     
   {"set":"setcal","to":"0 - 32k", "auth":"XXXXXXXX"} -> Set calibration offset to memory
   {"set":"getcal"}                                   -> Load calibration from memory
   {"set":"demo"}                                     -> Run Demo Mode (interrupted by any other command)
   {"set":"help"}                                     -> Print Commands to Serial Monitor    


```


_Succinct Command Structure_
```
   {"hz": -20 to 20}                            -> Set Motor Speed in Hz   
   {"rpm": -200 to 200}                         -> Set Motor Speed in RPM  
   {"home":0}*                                  -> Move Motor to home pos 
   {"cal":0-32768}                              -> Set the home position offset calibration 
   {"free":0}*                                  -> Set freewheel brake mode 
   {"brake":0}*                                 -> Set coolbrake brake mode 
   {"goto": -360 to 360}                        -> Goto Angle (int val)
   {"move", -360 to 360}                        -> Move Angle  (float val)             
   {"sample": 1 to 200}                         -> Set Samplerate in Hz (dflt: 200)
   {"print": 1 to 50}                           -> Set Print Rate in Hz (dflt: 50)
   {"stream":0}*                                -> Start Data Streaming    
   {"endst":0}*                                 -> End Data Streaming      
   {"snap":0}*                                  -> Take Data Snapshot       
   {"time": 1 - 250000 }                        -> Set Time for Data Snapshot (mS)  
   {"ping":0}*                                  -> Ping Servo               
   {"offset":-32k to 32k}                       -> DEPRECIATED
   {"secret":"XXXXXXXX"}                        -> Set 8 character secret     
   {"setcal":"0 - 32k", "auth":"XXXXXXXX"}      -> Set calibration offset to memory
   {"getcal":0}*                                -> Load calibration from memory
   {"demo":0}*                                  -> Run Demo Mode (interrupted by any other command)
   {"help":0}*                                  -> Print Commands to Serial Monitor    

* For single commands data value is ignored but required for valid JSON string
```

