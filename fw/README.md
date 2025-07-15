#Dynamics Lab Firmware

## Current Version 
-> [dynamics-lab-firmware-V1.0.0](https://github.com/practable/dynamics-lab/tree/fw-dev/fw/dynamics_lab_firmware_V1_x_x)

## Calibration Procedure: Setting home position (V1.0.0 - Beta Firmware)
- Set mode to "free" to remove the brake
- `{"free":1}`
- Spin blades to home position.
- Note down "pos (raw)" value from Serial JSON message
- enter offset into `  stepper.encoder.setHomeActual(30700);` function in setup()
- Upload program and run, motor should home, if not offset value may need some adjustment
- try increasing or decreasing offset value +-100 until desired home position is reached after sending `{"home":1}` command

_Verbose Command Structure_
```
   {"set":"start"}                   -> Start/Update Motor Speed
   {"set":"stop"}                    -> Stop Motor              
   {"set":"hz","to": -20 to 20}      -> Set Motor Speed in Hz   
   {"set":"rpm","to" -200 to 200}    -> Set Motor Speed in RPM  
   {"set":"home"}                    -> Move Motor to home pos (test) 
   {"set":"cal"}                     -> Run Calibration to home motor 
   {"set":"free"}                    -> Set freewheel brake mode (test)
   {"set":"brake"}                   -> Set coolbrake brake mode (test)
   {"set":"goto","to": -360 to 360}  -> Goto Angle (test)              
   {"set":"sample","to": 1 to 40}    -> Set Samplerate in Hz         
   {"set":"stream"}                  -> Start Data Streaming    
   {"set":"endst"}                   -> End Data Streaming      
   {"set":"snap"}                    -> Take Data Snapshot       
   {"set":"time","to": 1 - 250000 }  -> Set Time for Data Snapshot (mS)  
   {"set":"ping"}                    -> Ping Servo               
   {"set":"help"}                    -> Print Commands to Serial Monitor 


```


_Succinct Command Structure_
```
   {"hz": -20 to 20}                            -> Set Motor Speed in Hz   
   {"rpm": -200 to 200}                         -> Set Motor Speed in RPM  
   {"home":0}                                   -> Move Motor to home pos (test) 
   {"cal":0-32768}                              -> Set the home position offset calibration 
   {"free":0}                                   -> Set freewheel brake mode (test)
   {"brake":0}                                  -> Set coolbrake brake mode (test)
   {"goto": -360 to 360}                        -> Goto Angle (test)              
   {"sample": 1 to 200}                         -> Set Samplerate in Hz (dflt: 200)
   {"print": 1 to 50}                           -> Set Print Rate in Hz (dflt: 50)
   {"stream":0}                                 -> Start Data Streaming    
   {"endst":0}                                  -> End Data Streaming      
   {"snap":0}                                   -> Take Data Snapshot       
   {"time": 1 - 250000 }                        -> Set Time for Data Snapshot (mS)  
   {"ping":0}                                   -> Ping Servo               
   {"offset":-32k to 32k}                       -> DEPRECIATED
   {"secret":"XXXXXXXX"}                        -> Set 8 character secret     
   {"setcal":"0 - 32k", "auth":"XXXXXXXX"}      -> Set calibration offset to memory
   {"getcal":0}                                 -> Load calibration from memory 
   {"help":0}                                   -> Print Commands to Serial Monitor    

```

