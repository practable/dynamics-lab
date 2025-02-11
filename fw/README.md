#Dynamics Lab Firmware

## Current Version 
-> [dynamics-lab-firmware-V0.1.1](https://github.com/practable/dynamics-lab/tree/fw-dev/fw/dynamics_lab_firmware_V0_1_x)


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
   {"start":}          -> Start/Update Motor Speed
   {"stop":}           -> Stop Motor              
   {"hz": -20 to 20}    -> Set Motor Speed in Hz   
   {"rpm": -200 to 200} -> Set Motor Speed in RPM  
   {"home":}        -> Move Motor to home pos (test) 
   {"cal":}         -> Run Calibration to home motor 
   {"free":}        -> Set freewheel brake mode (test)
   {"brake":}       -> Set coolbrake brake mode (test)
   {"goto": -360 to 360}-> Goto Angle (test)              
   {"sample": 1 to 40}  -> Set Samplerate in Hz         
   {"stream":}      -> Start Data Streaming    
   {"endst":}       -> End Data Streaming      
   {"snap":}        -> Take Data Snapshot       
   {"time": 1 - 250000 }-> Set Time for Data Snapshot (mS)  
   {"ping":}        -> Ping Servo               
   {"help":}        -> Print Commands to Serial Monitor 

```

