<template>
    <div class="d-flex flex-column">
        <div class="d-flex" id="gamepadInfo" ref="gamepadInfo">

        </div>

        <div id="circularMenu" class="circular-menu">
            <a class="floating-btn" onclick="document.getElementById('circularMenu').classList.toggle('active');">
                <img src="/images/Xbox_RT_trigger.svg" alt="RT trigger button" width="35" height="35">
            </a>

            <menu class="items-wrapper">
                <a href="#" class="menu-item AButton" id="RT_A_Button"><img src="/images/Xbox_button_A.svg" alt="A button" width="35" height="35"></a>
                <a href="#" class="menu-item XButton" id="RT_X_Button"><img src="/images/Xbox_button_X.svg" alt="X button" width="35" height="35"></a>
                <a href="#" class="menu-item YButton" id="RT_Y_Button"><img src="/images/Xbox_button_Y.svg" alt="Y button" width="35" height="35"></a>
                <a href="#" class="menu-item BButton" id="RT_B_Button"><img src="/images/Xbox_button_B.svg" alt="B button" width="35" height="35"></a>
                <a class="menu-text" id="RT_A_Button_text">{{ getIsRecording ? 'Stop': 'Record'}}</a>
                <a class="menu-text" id="RT_X_Button_text">{{ getIsRecording ? '': 'Reset'}}</a>
                <a class="menu-text" id="RT_Y_Button_text">{{ getIsRecording ? '': 'Download'}}</a>
                <a class="menu-text" id="RT_B_Button_text">Close</a>
            </menu>

        </div>


        <div id="circularMenu1" class="circular-menu circular-menu-left">

            <a class="floating-btn" onclick="document.getElementById('circularMenu1').classList.toggle('active');">
                <img src="/images/Xbox_LT_trigger.svg" alt="LT trigger button" width="35" height="35">
            </a>

            <menu class="items-wrapper">
                 <a href="#" class="menu-item AButton" id="LT_A_Button"><img src="/images/Xbox_button_A.svg" alt="A button" width="35" height="35"></a>
                <a href="#" class="menu-item XButton" id="LT_X_Button"><img src="/images/Xbox_button_X.svg" alt="X button" width="35" height="35"></a>
                <a href="#" class="menu-item YButton" id="LT_Y_Button"><img src="/images/Xbox_button_Y.svg" alt="Y button" width="35" height="35"></a>
                <a href="#" class="menu-item BButton" id="LT_B_Button"><img src="/images/Xbox_button_B.svg" alt="B button" width="35" height="35"></a>
                <a class="menu-text" id="LT_A_Button_text">Home</a>
                <a class="menu-text" id="LT_X_Button_text">Ping</a>
                <a class="menu-text" id="LT_Y_Button_text">Drive</a>
                <a class="menu-text" id="LT_B_Button_text">Close</a>
            </menu>

        </div>

        <div id="driveModePopup" class="drive-mode-popup">
            <a class="floating-btn">
                <!-- <img src="/images/Xbox_LT_trigger.svg" alt="LT trigger button" width="35" height="35"> -->
            </a>

            <menu class="items-wrapper">
                <a href="#" v-show="getCurrentMode == 'driven'" class="menu-item" id="set_frequency_text_gamepad"><label id="driving-frequency-slider-label-gamepad" for="driving-frequency-slider-gampad">Set to: {{ driving_frequency.toFixed(2) }}Hz</label></a>
                <a href="#" v-show="getCurrentMode == 'driven'" class="menu-item" id="read_frequency_text_gamepad"><label id="driving-frequency-slider-label-gamepad" for="driving-frequency-slider-gamepad">Current: {{ getReportedDrivingFrequencyHz.toFixed(2) }}Hz</label></a>
                <a href="#" v-show="getCurrentMode == 'driven'" class="menu-item" id="frequency_slider_input_gamepad"><input class="" type="range" :min="getDrivingFrequencyMin" :max="getDrivingFrequencyMax" :step="getDrivingFrequencyStep" v-model="driving_frequency" id="driving-frequency-slider-gamepad"></a>
            </menu>

        </div>

    </div>
    

</template>

<script>
import { mapGetters } from 'vuex';
import { mapActions } from 'vuex';
import config from '../config/logging-config';

let loopStarted = false;
let selectedMode = 0;       //LT = -1, RT = 1, unselected = 0
let buttonsCache = {}       //stores previous state of each button for each gamepad {0:[buttonsCache]}
let axesCache = {}       //stores previous state of each axis for each gamepad {0:[axisCache]}

function getGamepadButtonMapping(gamepad_id, button_string){
  /**
   * Returns the mapped button int depending on the type of gamepad. Only gamepads defined below will work and defined on Google Chrome browser only.
   * gamepad_id (string): the id of the gamepad connected, this is the same for multiples of the same gamepad. 
   *      Standard Xbox 360 control on Chrome - "©Microsoft Corporation Controller (STANDARD GAMEPAD Vendor: 045e Product: 028e)"
   *      Power A Xbox one controller on Chrome - "PowerA Xbox Series X Wired Controller Black (Vendor: 20d6 Product: 2062)"
   * button_string (string): Xbox style button string - A, B, X, Y, RT, LT, RB, LB, D-Left, D-Right, D-Up, D-Down, Start, Back, Menu
   * 
   * RETURNS the button id or 'axis' if an axis rather than a button
   */
  //Xbox 360 controller
  if(gamepad_id.includes('045e') && gamepad_id.includes('028e')){
    switch(button_string) {
      case 'A':
        return 0
      case 'X':
        return 2
      case 'Y':
        return 3
      case 'B':
        return 1
      case 'LT':
        return 6
      case 'RT':
        return 7
      case 'LB':
        return 4
      case 'RB':
        return 5
      case 'D-Left':
        return 14
      case 'D-Right':
        return 15
      case 'D-Up':
        return 12
      case 'D-Down':
        return 13
      case 'Start':
        return 9
      case 'Back':
        return 8
      case 'Menu':
        return 16
      default:
        return ''
    }
  } 
  //Xbox One controller
  else if(gamepad_id.includes('20d6') && gamepad_id.includes('2062')){
    console.log('Xbox One controller')
    console.log(button_string)
    switch(button_string) {
      case 'A':
        return 0
      case 'X':
        return 2
      case 'Y':
        return 3
      case 'B':
        return 1
      case 'LT':
        return 'axis'
      case 'RT':
        return 'axis'
      case 'LB':
        return 4
      case 'RB':
        return 5
      case 'D-Left':
        return 'axis'
      case 'D-Right':
        return 'axis'
      case 'D-Up':
        return 'axis'
      case 'D-Down':
        return 'axis'
      case 'Start':
        return 7
      case 'Back':
        return 6
      case 'Menu':
        return 8
      default:
        return ''
    }
  }
}

function getGamepadAxisMapping(gamepad_id, button_string){
  /**
   * Different gamepads and browser combinations have different axes on different indices
   * gamepad_id (string): the id of the gamepad connected, this is the same for multiples of the same gamepad
   * axis_string (string): Xbox style axis string
   * RETURNS the axis id or 'button' if a button rather than an axis
   */
  //Xbox 360 controller
  if(gamepad_id.includes('045e') && gamepad_id.includes('028e')){
    switch(button_string) {
      case 'LT':
        return 'button'
      case 'RT':
        return 'button'
      case 'D-Horizontal':
        return 'button'
      case 'D-Vertical':
        return 'button'
      case 'Left-Stick-Horizontal':
        return 0
      case 'Left-Stick-Vertical':
        return 1
      case 'Right-Stick-Horizontal':
        return 2
      case 'Right-Stick-Vertical':
        return 3
      default:
        return ''
    }
  } 
  //Xbox One controller
  else if(gamepad_id.includes('20d6') && gamepad_id.includes('2062')){
    switch(button_string) {
      case 'LT':
        return 2
      case 'RT':
        return 5
      case 'D-Horizontal':
        return 6
      case 'D-Vertical':
        return 7
      case 'Left-Stick-Horizontal':
        return 0
      case 'Left-Stick-Vertical':
        return 1
      case 'Right-Stick-Horizontal':
        return 3
      case 'Right-Stick-Vertical':
        return 4
      default:
        return ''
    }
  }
}

function addGamePadWithAllButtons(gamepad, infoElement) {
  console.log(gamepad);
  const d = document.createElement("div");
  d.setAttribute("id", `controller${gamepad.index}`);

  const t = document.createElement("h4");
  t.textContent = `gamepad: ${gamepad.id}`;
  d.append(t);

  const b = document.createElement("ul");
  b.className = "buttons";
  gamepad.buttons.forEach((button, i) => {
    const e = document.createElement("li");
    e.className = "button-gamepad-temp button-secondary align-content-center me-2";
    e.textContent = `${i}`;
    b.append(e);
  });

  d.append(b);

  const a = document.createElement("div");
  a.className = "axes";

  gamepad.axes.forEach((axis, i) => {
    const p = document.createElement("progress");
    p.className = "axis me-2";
    p.setAttribute("max", "2");
    p.setAttribute("value", "1");
    p.textContent = i;
    a.append(p);
  });

  d.appendChild(a);

  // See https://github.com/luser/gamepadtest/blob/master/index.html
  const start = document.querySelector("#start");
  if (start) {
    start.style.display = "none";
  }

  infoElement.appendChild(d);

  if (!loopStarted) {
    requestAnimationFrame(updateStatus);
    loopStarted = true;
  }
}

function addGamepad(gamepad, infoElement) {
  

    if (!loopStarted) {
        requestAnimationFrame(updateStatus);
        loopStarted = true;
    }
  
}

function removeGamepad(gamepad) {
  document.querySelector(`#controller${gamepad.index}`).remove();
}

function updateStatus() {

    doUpdateOfTempButtons();
    doUpdateOfGamePadButtons();

    doHapticUpdateBasedOnHardwareState();

    requestAnimationFrame(updateStatus);

}

/**
 * Temporary functionality to help identify gamepad buttons
 * 
 */
function doUpdateOfTempButtons(){
    for (const gamepad of navigator.getGamepads()) {
    if (!gamepad) continue;

    const d = document.getElementById(`controller${gamepad.index}`);
    const buttonElements = d.getElementsByClassName("button-gamepad-temp");

    for (const [i, button] of gamepad.buttons.entries()) {
      const el = buttonElements[i];

      const pct = `${Math.round(button.value * 100)}%`;
    //   el.style.backgroundSize = `${pct} ${pct}`;
      if (button.pressed) {
        el.textContent = `${i}`;
        // el.style.color = "#42f593";
        el.className = "button-gamepad-temp button-pressed align-content-center me-2";
      } else {
        el.textContent = `${i}`;
        // el.style.color = "#2e2d33";
        el.className = "button-gamepad-temp button-secondary align-content-center me-2";
      }
    }

    const axisElements = d.getElementsByClassName("axis");
    for (const [i, axis] of gamepad.axes.entries()) {
      const el = axisElements[i];
      el.textContent = `${i}: ${axis.toFixed(4)}`;
      el.setAttribute("value", axis + 1);
    }
  }
}

function doUpdateOfGamePadButtons(){
    for (const gamepad of navigator.getGamepads()) {
    if (!gamepad) continue;

    // const d = document.getElementById(`controller${gamepad.index}`);
    // const buttonElements = d.getElementsByClassName("button-gamepad-temp");

    for (const [i, button] of gamepad.buttons.entries()) {
    //   const el = buttonElements[i];
        
    //   const pct = `${Math.round(button.value * 100)}%`;
    //   el.style.backgroundSize = `${pct} ${pct}`;
      if (button.pressed) {
        //console.log(gamepad);
        triggerHapticResponse(gamepad, buttonsCache[gamepad.index], i);
        triggerGUIUpdate(gamepad, buttonsCache[gamepad.index], i);
        triggerCommand(gamepad, buttonsCache[gamepad.index], i);     //may merge these functions later
        // el.textContent = `${i}`;
        // el.className = "button-gamepad-temp button-pressed align-content-center me-2";
      } 
      else {
        cancelHapticResponse(gamepad, buttonsCache[gamepad.index], i);
        cancelGUIUpdate(gamepad, buttonsCache[gamepad.index], i);
        // el.textContent = `${i}`;
        // el.className = "button-gamepad-temp button-secondary align-content-center me-2";
      }

      
    }
    //update axes
    for (const [i, axis] of gamepad.axes.entries()) {
      
        TriggerAxisUpdate(gamepad, axis, i, axesCache[gamepad.index]);
      
    }

    buttonsCache[gamepad.index] = gamepad.buttons;       //store the state of the buttons
    axesCache[gamepad.index] = gamepad.axes;       //store the state of the buttons
  }
}

function triggerHapticResponse(gamepad, buttonsCache, buttonIndex){
    if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'Menu')){
        if(gamepad.vibrationActuator){
            //widely supported but not firefox and perhaps not safari
                gamepad.vibrationActuator.playEffect("dual-rumble", {
                startDelay: 0,
                duration: 200,
                weakMagnitude: 1.0,
                strongMagnitude: 1.0,
            });
        } else if(gamepad.hapticActuators){
            //firefox, but no hapticActuators are registered with the control I am using
            for(const actuator of gamepad.hapticActuators){
                if (!actuator) continue;
                actuator.hapticActuators.pulse(1.0, 200);
            }
            
        }
    }
     else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'LT')){
        if(gamepad.vibrationActuator){
            //widely supported but not firefox and perhaps not safari
                gamepad.vibrationActuator.playEffect("trigger-rumble", {
                startDelay: 0,
                duration: 200,
                weakMagnitude: 0.1,
                strongMagnitude: 0.1,
                leftTrigger: 1.0,
                rightTrigger: 0.0
            });
        } else if(gamepad.hapticActuators){
            //firefox, but no hapticActuators are registered with the control I am using
            for(const actuator of gamepad.hapticActuators){
                if (!actuator) continue;
                actuator.hapticActuators.pulse(1.0, 200);
            }
            
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'RT')){
        if(gamepad.vibrationActuator){
            //widely supported but not firefox and perhaps not safari
                gamepad.vibrationActuator.playEffect("trigger-rumble", {
                startDelay: 0,
                duration: 200,
                weakMagnitude: 0.1,
                strongMagnitude: 0.1,
                leftTrigger: 0.0,
                rightTrigger: 1.0
            });
        } else if(gamepad.hapticActuators){
            //firefox, but no hapticActuators are registered with the control I am using
            for(const actuator of gamepad.hapticActuators){
                if (!actuator) continue;
                actuator.hapticActuators.pulse(1.0, 200);
            }
            
        }
    }
}

function cancelHapticResponse(gamepad, buttonsCache, buttonIndex){
    return
}

//from https://thecodersblog.com/simulating-tab-enter-key-press-events-browser-console
//reverse = true will cycle backwards through tabs
function simulateTab(reverse = false) {
  const event = new KeyboardEvent("keydown", {
    key: "Tab",
    code: "Tab",
    keyCode: 9,
    shiftKey: reverse, // Shift+Tab for reverse navigation
    bubbles: true,
    cancelable: true
  });

  document.activeElement.dispatchEvent(event);

  // If default behavior wasn't prevented, manually move focus
  if (!event.defaultPrevented) {
    const focusableElements = Array.from(
      document.querySelectorAll(
        'button, input, select, [tabindex]:not([tabindex="-1"])'
      )
    ).filter((el) => !el.disabled && el.offsetParent !== null);

    const currentIndex = focusableElements.indexOf(document.activeElement);
    const nextIndex = reverse
      ? (currentIndex - 1 + focusableElements.length) % focusableElements.length
      : (currentIndex + 1) % focusableElements.length;

    focusableElements[nextIndex].focus();
  }
}

//from https://thecodersblog.com/simulating-tab-enter-key-press-events-browser-console
// Intelligent Enter simulation that considers context
function simulateEnter(targetElement = document.activeElement) {
  const event = new KeyboardEvent("keydown", {
    key: "Enter",
    code: "Enter",
    keyCode: 13,
    bubbles: true,
    cancelable: true
  });

  targetElement.dispatchEvent(event);

  // Handle specific element types if default behavior wasn't triggered
  if (!event.defaultPrevented) {
    const tagName = targetElement.tagName.toLowerCase();
    const type = targetElement.type?.toLowerCase();

    switch (tagName) {
      case "button":
        targetElement.click();
        break;
      case "input":
        if (type === "submit") {
          targetElement.click();
        } else if (type === "text" || type === "email" || type === "password") {
          // Find and submit parent form
          const form = targetElement.closest("form");
          if (form) {
            const submitButton = form.querySelector(
              'input[type="submit"], button[type="submit"]'
            );
            if (submitButton) {
              submitButton.click();
            } else {
              form.submit();
            }
          }
        }
        break;
      case "a":
        if (targetElement.href) {
          targetElement.click();
        }
        break;
    }
  }
}

function triggerGUIUpdate(gamepad, buttonsCache, buttonIndex){
    //left trigger pressed - this can be a button on some controller and an axis on others so also deal with this in the triggerAxisUpdate function
    if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'LT')){
        document.getElementById('circularMenu1').classList.add('active');
        //if in driven mode then also make the drive mode popup active
        if(window.gamepadComponent.getCurrentMode == 'driven' && !buttonsCache[buttonIndex].pressed){
            document.getElementById('driveModePopup').classList.add('active');
        }
        document.getElementById('circularMenu').classList.remove('active');
        selectedMode = -1;
    //right trigger pressed
    } else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'RT')){
        document.getElementById('circularMenu1').classList.remove('active');
        document.getElementById('driveModePopup').classList.remove('active');
        document.getElementById('circularMenu').classList.add('active');
        selectedMode = 1;
    } 
    //left bumper should act as a forward tab for navigation
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'LB') && !buttonsCache[buttonIndex].pressed){
      console.log('LB')
      simulateTab(true);
    }
    //right bumper should act as a backward tab for navigation
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'RB') && !buttonsCache[buttonIndex].pressed){
      simulateTab();
    }
    //menu button closes all
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'Menu')){
        document.getElementById('circularMenu1').classList.remove('active');
        document.getElementById('driveModePopup').classList.remove('active');
        document.getElementById('circularMenu').classList.remove('active');
        selectedMode = 0;
    } 
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'A')){
        //A button pressed
        if(selectedMode == -1){
            document.getElementById('LT_A_Button').classList.add('active');
        } else if(selectedMode == 1){
            document.getElementById('RT_A_Button').classList.add('active');
        } 
        //else simulate pressing enter
        else if(!buttonsCache[buttonIndex].pressed){
          simulateEnter();
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'X')){
        //X button pressed
        if(selectedMode == -1){
            document.getElementById('LT_X_Button').classList.add('active');
            document.getElementById('LT_B_Button_text').innerText = 'Back';
        } else if(selectedMode == 1){
            document.getElementById('RT_X_Button').classList.add('active');
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'Y')){
        //Y button pressed
        if(selectedMode == -1){
            document.getElementById('LT_Y_Button').classList.add('active');
            document.getElementById('driveModePopup').classList.add('active');  //additional popup menu for drive mode
            document.getElementById('LT_A_Button_text').innerText = 'Run';
            document.getElementById('LT_X_Button_text').innerText = 'Stop';
            document.getElementById('LT_Y_Button_text').innerText = '';
            document.getElementById('LT_B_Button_text').innerText = 'Back';
        } else if(selectedMode == 1){
            document.getElementById('RT_Y_Button').classList.add('active');
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'B')){
        //B button pressed
        if(selectedMode == -1){
            document.getElementById('LT_B_Button').classList.add('active');
            document.getElementById('LT_A_Button_text').innerText = 'Home';
            document.getElementById('LT_X_Button_text').innerText = 'Ping';
            document.getElementById('LT_Y_Button_text').innerText = 'Drive';
            document.getElementById('LT_B_Button_text').innerText = 'Close';
            
            //if in the stopped mode then B should close the popup
            if(window.gamepadComponent.getCurrentMode == 'stopped' && !buttonsCache[buttonIndex].pressed){
                document.getElementById('circularMenu1').classList.remove('active');
                document.getElementById('circularMenu').classList.remove('active');
                selectedMode = 0;   //no trigger
            }

            //if in drive mode then the additional drive popup should be deactivated
            if(window.gamepadComponent.getCurrentMode == 'driven' && !buttonsCache[buttonIndex].pressed){
                document.getElementById('driveModePopup').classList.remove('active');
            }
            
        } else if(selectedMode == 1){
            document.getElementById('RT_B_Button').classList.add('active');
            document.getElementById('circularMenu1').classList.remove('active');
            document.getElementById('circularMenu').classList.remove('active');
            selectedMode = 0;   //no trigger
        }
    }
    //D-pad up
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'D-Up') && !buttonsCache[buttonIndex].pressed){
      let activeElement = document.activeElement;
      if(activeElement.tagName.toLowerCase() == 'input'){
        if(activeElement.type == 'number'){
          activeElement.valueAsNumber += 1; 
          activeElement.dispatchEvent(new Event('input')); 
        }
      }
    }
    //d-pad down
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'D-Down') && !buttonsCache[buttonIndex].pressed){
      let activeElement = document.activeElement;
      if(activeElement.tagName.toLowerCase() == 'input'){
        if(activeElement.type == 'number'){
          activeElement.valueAsNumber -= 1; 
          activeElement.dispatchEvent(new Event('input'));
        }
      }
    }
   
    // else{
    //     // this.leftTriggerPressed = false;
    //     // this.rightTriggerPressed = false;
    //     document.getElementById("leftTriggerPressedAlert").hidden = true;
    //     document.getElementById("rightTriggerPressedAlert").hidden = true;
    // }
}

function cancelGUIUpdate(gamepad, buttonsCache, buttonIndex){
    //behaviour of trigger buttons - do I want the last one triggered to remain active or to have to hold a button for the mode to be active?
    // if(buttonIndex == 6){
    //     document.getElementById('circularMenu1').classList.remove('active');
    //     // document.getElementById('circularMenu').classList.remove('active');
    //     //selectedMode = 0;   //LT
    // } else if(buttonIndex == 7){
    //     // document.getElementById('circularMenu1').classList.remove('active');
    //     document.getElementById('circularMenu').classList.remove('active');
    //     //selectedMode = 0;   //RT
    // } 
    if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'A')){
        //A button pressed
        if(selectedMode == -1){
            document.getElementById('LT_A_Button').classList.remove('active');
        } else if(selectedMode == 1){
            document.getElementById('RT_A_Button').classList.remove('active');
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'X')){
        //X button pressed
        if(selectedMode == -1){
            document.getElementById('LT_X_Button').classList.remove('active');
        } else if(selectedMode == 1){
            document.getElementById('RT_X_Button').classList.remove('active');
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'Y')){
        //Y button pressed
        if(selectedMode == -1){
            document.getElementById('LT_Y_Button').classList.remove('active');
        } else if(selectedMode == 1){
            document.getElementById('RT_Y_Button').classList.remove('active');
        }
    }
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'B')){
        //B button pressed
        if(selectedMode == -1){
            document.getElementById('LT_B_Button').classList.remove('active');
        } else if(selectedMode == 1){
            document.getElementById('RT_B_Button').classList.remove('active');
        }
    }
}

function triggerCommand(gamepad, buttonsCache, buttonIndex){

    //A button pressed
    if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'A') && !buttonsCache[buttonIndex].pressed){
        //hardware trigger mode
        if(selectedMode == -1){
            if(window.gamepadComponent.getCurrentMode == 'driven'){
                console.log('run @ ' + window.gamepadComponent.getDrivingFrequencyHz);
                window.gamepadComponent.updateDrivingFrequency();
            } else{
                console.log('home');
                window.gamepadComponent.sendCommandHome();
            }
            
            
        } 
        else if(selectedMode == 1){
            //data trigger mode
            if(window.gamepadComponent.getIsRecording == true){
                window.gamepadComponent.setIsRecording(false);
            } else{
                window.gamepadComponent.setIsRecording(true);
            }
        }
    }

    //X button pressed
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'X') && !buttonsCache[buttonIndex].pressed){
        //hardware trigger mode
        if(selectedMode == -1){
            if(window.gamepadComponent.getCurrentMode == 'driven'){
                console.log('stop');
                window.gamepadComponent.sendCommandStop();
            } else{
                console.log('ping');
                window.gamepadComponent.setModeUndriven();
                window.gamepadComponent.sendCommandPing();
            }

            
        } 
        //data trigger mode
        else if(selectedMode == 1){
            if(window.gamepadComponent.getIsRecording == false){
                console.log('clear all data');
                window.gamepadComponent.clearData();
            }
        }
    }

    //Y button pressed
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'Y') && !buttonsCache[buttonIndex].pressed){
        if(selectedMode == -1){
            //hardware trigger mode
            console.log('driven mode');
            window.gamepadComponent.setModeDriven();
        } 
        //data trigger mode
        else if(selectedMode == 1){
            if(window.gamepadComponent.getIsRecording == false){
                console.log('download data');
                window.gamepadComponent.outputToCSV();
            }
            
        }
    }

    //B button pressed
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'B') && !buttonsCache[buttonIndex].pressed){
        //hardware trigger mode
        if(selectedMode == -1){
            if(window.gamepadComponent.getCurrentMode == 'driven'){
                console.log('stop and exit driven mode');
                window.gamepadComponent.sendCommandStop();
                window.gamepadComponent.setModeStop();
            } else if(window.gamepadComponent.getCurrentMode == 'undriven'){
                console.log('exit undriven mode');
                window.gamepadComponent.setModeStop();
            }
            
        } 
        else if(selectedMode == 1){
            //data trigger mode
            // if(window.gamepadComponent.getIsRecording == true){
            //     window.gamepadComponent.setIsRecording(false);
            // } else{
            //     window.gamepadComponent.setIsRecording(true);
            // }
            console.log('data function B');
            
        }
    }

    //D-Pad RIGHT pressed
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'D-Right') && !buttonsCache[buttonIndex].pressed){
        //D-Pad should only function when hardware functions selected and in driven mode
        if(selectedMode == -1 && window.gamepadComponent.getCurrentMode == 'driven'){
            //hardware trigger mode
            window.gamepadComponent.driving_frequency += 0.01;
        } 
        else if(selectedMode == 1){
            return
            
        }
    }
    //D-Pad LEFT pressed
    else if(buttonIndex == getGamepadButtonMapping(gamepad.id, 'D-Left') && !buttonsCache[buttonIndex].pressed){
        //D-Pad should only function when hardware functions selected and in driven mode
        if(selectedMode == -1 && window.gamepadComponent.getCurrentMode == 'driven'){
            //hardware trigger mode
            window.gamepadComponent.driving_frequency -= 0.01;
        } 
        else if(selectedMode == 1){
            return
            
        }
    }
}

function TriggerAxisUpdate(gamepad, axis, axesIndex, axisCache){
    if(axesIndex == getGamepadAxisMapping(gamepad.id, 'Left-Stick-Horizontal')){
        if(selectedMode == -1 && window.gamepadComponent.getCurrentMode == 'driven'){
            //hardware trigger mode
            if(axis > 0.9){
                window.gamepadComponent.driving_frequency += 0.01;
            } else if(axis < -0.9){
                window.gamepadComponent.driving_frequency -= 0.01;
            } else{
                return
            }
            
        }
    }
    //some gamepads consider LT as a trigger so active menu on LT if appropriate
    else if(axesIndex == getGamepadAxisMapping(gamepad.id, 'LT')){
      if(axis > 0.5){
        document.getElementById('circularMenu1').classList.add('active');
        //if in driven mode then also make the drive mode popup active
        if(window.gamepadComponent.getCurrentMode == 'driven'){
            document.getElementById('driveModePopup').classList.add('active');
        }
        document.getElementById('circularMenu').classList.remove('active');
        selectedMode = -1;
      }
        
    }
    //some gamepads consider RT as a trigger so active menu on LT if appropriate
    else if(axesIndex == getGamepadAxisMapping(gamepad.id, 'RT')){
      if(axis > 0.5){
        document.getElementById('circularMenu1').classList.remove('active');
        document.getElementById('driveModePopup').classList.remove('active');
        document.getElementById('circularMenu').classList.add('active');
        selectedMode = 1;
      }
    }
    //some gamepads consider D-Pad as a trigger
    else if(axesIndex == getGamepadAxisMapping(gamepad.id, 'D-Horizontal')){
      if(axis > 0.9 && axisCache[axesIndex] == 0){  //gives behaviour of a single button press
         if(selectedMode == -1 && window.gamepadComponent.getCurrentMode == 'driven'){
            //hardware trigger mode
            window.gamepadComponent.driving_frequency += 0.01;
        } 
        else {
            return
            
        }
      } else if(axis < -0.9 && axisCache[axesIndex] == 0){  //gives behaviour of a single button press
         if(selectedMode == -1 && window.gamepadComponent.getCurrentMode == 'driven'){
            //hardware trigger mode
            window.gamepadComponent.driving_frequency -= 0.01;
        } 
        else {
            return
            
        }
      }
    }
    //some gamepads consider D-Pad as a trigger
    else if(axesIndex == getGamepadAxisMapping(gamepad.id, 'D-Vertical')){
      if(axis > 0.9 && axisCache[axesIndex] == 0){  //gives behaviour of a single button press
          let activeElement = document.activeElement;
          if(activeElement.tagName.toLowerCase() == 'input'){
            if(activeElement.type == 'number'){
              activeElement.valueAsNumber -= 1; 
              activeElement.dispatchEvent(new Event('input')); 
            }
          }
      } else if(axis < -0.9 && axisCache[axesIndex] == 0){  //gives behaviour of a single button press
         let activeElement = document.activeElement;
          if(activeElement.tagName.toLowerCase() == 'input'){
            if(activeElement.type == 'number'){
              activeElement.valueAsNumber += 1; 
              activeElement.dispatchEvent(new Event('input')); 
            }
          }
      }
    }
}

function doHapticUpdateBasedOnHardwareState(){
    if(window.gamepadComponent.getCurrentMode != 'stopped'){
        for (const gamepad of navigator.getGamepads()) {
            if (!gamepad) continue;

            //only do this if an acceleration value actually exists
            if(gamepad.vibrationActuator && 'x' in window.gamepadComponent.getCurrentAcceleration){
              if(window.gamepadComponent.getCurrentMode != 'driven'){
                  //widely supported but not firefox and perhaps not safari
                    gamepad.vibrationActuator.playEffect("dual-rumble", {
                    startDelay: 0,
                    duration: 100,
                    weakMagnitude: Math.min(window.gamepadComponent.getCurrentAcceleration.x[0]/window.gamepadComponent.maxVibrationAcceleration_driven, 1.0),
                    strongMagnitude: Math.min(window.gamepadComponent.getCurrentAcceleration.x[0]/window.gamepadComponent.maxVibrationAcceleration_driven, 1.0),
                });
              } else if(window.gamepadComponent.getCurrentMode != 'undriven'){
                  //widely supported but not firefox and perhaps not safari
                    gamepad.vibrationActuator.playEffect("dual-rumble", {
                    startDelay: 0,
                    duration: 200,
                    weakMagnitude: Math.min(window.gamepadComponent.getCurrentAcceleration.x[0]/window.gamepadComponent.maxVibrationAcceleration_undriven, 1.0),
                    strongMagnitude: Math.min(window.gamepadComponent.getCurrentAcceleration.x[0]/window.gamepadComponent.maxVibrationAcceleration_undriven, 1.0),
                });
              }
                
            } else if(gamepad.hapticActuators){
                //firefox, but no hapticActuators are registered with the control I am using
                for(const actuator of gamepad.hapticActuators){
                    if (!actuator) continue;
                    actuator.hapticActuators.pulse(1.0, 200);
                }
            }
        }
    }
    
}

export default {

  name: 'Gamepad',
  data () {
    return {
        maxVibrationAcceleration_undriven: 2,
        maxVibrationAcceleration_driven: 0.5,
    }
  },
  components: {
    
  },
created(){
      
  },
  mounted(){
    window.gamepadComponent = this;     //make this Vue component available in regular javascript functions via the window object

    window.addEventListener("DOMContentLoaded", (event) => {
        window.addEventListener("gamepadconnected", (evt) => {
            // console.log(this.$refs.gamepadInfo);
            addGamePadWithAllButtons(evt.gamepad, this.$refs.gamepadInfo);  //shows all available buttons and responds on button presses
            //addGamepad(evt.gamepad, this.$refs.gamepadInfo);

            buttonsCache[evt.gamepad.index] = evt.gamepad.buttons;     //initialise the buttonCache
            //console.log(buttonsCache)
        });

        window.addEventListener("gamepaddisconnected", (evt) => {
            removeGamepad(evt.gamepad);
        });
    });


    
  },
  computed:{
    ...mapGetters([
			'getCurrentTime',
			'getCurrentPosition',
			'getCurrentAcceleration',
			'getCurrentMode',
			'getDrivingFrequencyMin',
			'getDrivingFrequencyMax',
			'getDrivingFrequencyStep',
			'getDrivingFrequencyHz',
			'getReportedDrivingFrequencyHz',
            'getIsRecording'
    ]),
    driving_frequency: {
			get(){
				return this.getDrivingFrequencyHz;
			},
			set(val){
				this.updateDrivingFrequencyHz(val);
			}
		},
  },
  watch:{
      
  },
  methods: {
      ...mapActions([
            'setCurrentMode',
            'setIsRecording',
			'setModeStop',
			'setModeDriven',
			'setModeUndriven',
			'sendCommandStop',
			'sendCommandStart',
			'sendCommandPing',
			'sendCommandHome',
			'updateDrivingFrequencyHz',
			'sendCommandUpdateDrivingFrequencyHz',
            'clearAllData',
            'setDatasetIndex'
      ]),
      updateDrivingFrequency(){
			this.sendCommandUpdateDrivingFrequencyHz();
			setTimeout(() => {
				this.sendCommandStart();
			}, 100);
		},
        clearData(){
          this.clearAllData();
          this.setDatasetIndex(0);
      },
      outputToCSV(){
        let data = this.$store.getters.getData;
        let current_dataset = 0;
        //let csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g], Gyro_x[rad/s], Gyro_y[rad/s], Gyro_z[rad/s]\n';
        let csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g]\n';
        let date = new Date();

        data.forEach(function(d){
            if(d.set == current_dataset + 1){
                let hiddenElement = document.createElement('a');
                hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
                hiddenElement.target = '_blank';
                hiddenElement.download = `dynamics-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
                hiddenElement.click();

                //csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g], Gyro_x[rad/s], Gyro_y[rad/s], Gyro_z[rad/s]\n';
                csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g]\n';
                current_dataset += 1;
              }

              csv += d.t.toString();
              csv += ",";
              csv += d.mode.toString();
              csv += ",";
              csv += d.freq.toFixed(2);
              csv += ",";
              csv += d.pos.toString();
              csv += ',';
              csv += d.acc.x.toString();
              csv += ',';
              csv += d.acc.y.toString();
              csv += ',';
              csv += d.acc.z.toString();
              // csv += ',';
              // csv += d.gyro.x.toString();
              // csv += ',';
              // csv += d.gyro.y.toString();
              // csv += ',';
              // csv += d.gyro.z.toString();
              
              csv += "\n";
        });

        //output the final dataset
        let hiddenElement = document.createElement('a');
        hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
        hiddenElement.target = '_blank';
        hiddenElement.download = `dynamics-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
        hiddenElement.click();
    },
     
  }
}
</script>

<style scoped>



.circular-menu {
  position: fixed;
  bottom: 1em;
  right: 1em;
}

.circular-menu .floating-btn {
  display: block;
  width: 3.5em;
  height: 3.5em;
  border-radius: 50%;
  background-color: hsl(4, 98%, 60%);
  box-shadow: 0 2px 5px 0 hsla(0, 0%, 0%, .26);  
  color: hsl(0, 0%, 100%);
  text-align: center;
  line-height: 3.9;
  cursor: pointer;
  outline: 0;
}

.circular-menu.active .floating-btn {
  box-shadow: inset 0 0 3px hsla(0, 0%, 0%, .3);
}

.circular-menu .floating-btn:active {
  box-shadow: 0 4px 8px 0 hsla(0, 0%, 0%, .4);
}

.circular-menu .floating-btn i {
  font-size: 1.3em;
  transition: transform .2s;  
}

.circular-menu.active .floating-btn i {
  transform: rotate(-45deg);
}

.circular-menu:after {
  display: block;
  content: ' ';
  width: 3.5em;
  height: 3.5em;
  border-radius: 50%;
  position: absolute;
  top: 0;
  right: 0;
  z-index: -2;
  background-color: hsl(4, 98%, 60%);
  transition: all .3s ease;
}

.circular-menu.active:after {
  transform: scale3d(8, 8, 1);
  transition-timing-function: cubic-bezier(.68, 1.55, .265, 1);
}

.circular-menu .items-wrapper {
  padding: 0;
  margin: 0;
}

.circular-menu .menu-item {
  position: absolute;
  top: .2em;
  right: .2em;
  z-index: -1;
  display: block;
  text-decoration: none;
  color: hsl(0, 0%, 100%);
  font-size: 1em;
  width: 3em;
  height: 3em;
  border-radius: 50%;
  text-align: center;
  line-height: 3;
  background-color: hsla(0,0%,0%,.1);
  transition: transform .3s ease, background .2s ease;
}
.circular-menu .menu-item.active {
  background-color: hsl(120, 100%, 50%);
  transition: background-color 1000ms linear;
}

.circular-menu .menu-item:hover {
  background-color: hsla(0,0%,0%,.3);
}

.circular-menu.active .menu-item {
  transition-timing-function: cubic-bezier(0.175, 0.885, 0.32, 1.275);
}

.circular-menu.active .menu-item:nth-child(1) {
  /* transform: translate3d(1em,-7em,0); */
  transform: translate3d(1em,-6.5em,0);
  
}

.circular-menu.active .menu-item:nth-child(2) {
    /* transform: translate3d(-3.5em,-6.3em,0); */
  transform: translate3d(-3em,-6em,0);
}

.circular-menu.active .menu-item:nth-child(3) {
    /* transform: translate3d(-6.3em,-3.5em,0); */
    transform: translate3d(-6em,-3em,0);
}

.circular-menu.active .menu-item:nth-child(4) {
    /* transform: translate3d(-7em,1em,0); */
  transform: translate3d(-6.5em,1em,0);
}

/**
 * The other theme for this menu
 */

.circular-menu.circular-menu-left {
  right: auto; 
  left: 1em;
}

.circular-menu.circular-menu-left .floating-btn {
  background-color: hsl(217, 89%, 61%);
}

.circular-menu.circular-menu-left:after {
  background-color: hsl(217, 89%, 61%);
}

.circular-menu.circular-menu-left.active .floating-btn i {
  transform: rotate(90deg);
}

.circular-menu.circular-menu-left.active .menu-item:nth-child(1) {
  /* transform: translate3d(-1em,-7em,0); */
  transform: translate3d(-1em,-6.5em,0);
}

.circular-menu.circular-menu-left.active .menu-item:nth-child(2) {
  /* transform: translate3d(3.5em,-6.3em,0); */
  transform: translate3d(3em,-6em,0);
}

.circular-menu.circular-menu-left.active .menu-item:nth-child(3) {
  /* transform: translate3d(6.5em,-3.2em,0); */
  transform: translate3d(6em,-3em,0);
}

.circular-menu.circular-menu-left.active .menu-item:nth-child(4) {
  /* transform: translate3d(7em,1em,0); */
  transform: translate3d(6.5em,1em,0);
}

/**
 * The text boxes for each menu
 */

.circular-menu .menu-text {
  position: absolute;
  top: .2em;
  right: .2em;
  z-index: -1;
  display: block;
  text-decoration: none;
  color: hsl(0, 0%, 100%);
  font-size: 1em;
  width: 3em;
  height: 3em;
  border-radius: 50%;
  text-align: center;
  line-height: 3;
  background-color: hsla(0,0%,0%,.1);
  transition: transform .3s ease, background .2s ease;
}

.circular-menu.active .menu-text {
  transition-timing-function: cubic-bezier(0.175, 0.885, 0.32, 1.275);
}

.circular-menu.active .menu-text:nth-child(5) {
  /* transform: translate3d(1em,-7em,0); */
  transform: translate3d(1em,-10em,0);
}

.circular-menu.active .menu-text:nth-child(6) {
    /* transform: translate3d(-3.5em,-6.3em,0); */
  transform: translate3d(-5em,-9em,0);
}

.circular-menu.active .menu-text:nth-child(7) {
    /* transform: translate3d(-6.3em,-3.5em,0); */
  transform: translate3d(-9em,-5em,0);
}

.circular-menu.active .menu-text:nth-child(8) {
    /* transform: translate3d(-7em,1em,0); */
  transform: translate3d(-10em,1em,0);
}

.circular-menu.circular-menu-left.active .menu-text:nth-child(5) {
  /* transform: translate3d(-1em,-7em,0); */
  transform: translate3d(-1em,-10em,0);
}

.circular-menu.circular-menu-left.active .menu-text:nth-child(6) {
  /* transform: translate3d(3.5em,-6.3em,0); */
  transform: translate3d(5em,-9em,0);
}

.circular-menu.circular-menu-left.active .menu-text:nth-child(7) {
  /* transform: translate3d(6.5em,-3.2em,0); */
  transform: translate3d(9em,-5em,0);
}

.circular-menu.circular-menu-left.active .menu-text:nth-child(8) {
  /* transform: translate3d(7em,1em,0); */
  transform: translate3d(10em,1em,0);
}

/**
 * Driven mode parameters popup
 */

.drive-mode-popup {
  position: fixed;
  bottom: -1em;
  right: auto; 
  left: -1em;
}

/* .drive-mode-popup .floating-btn {
  display: none;
  width: 3.5em;
  height: 3.5em;
  border-radius: 50%;
  background-color: hsl(4, 98%, 60%);
  box-shadow: 0 2px 5px 0 hsla(0, 0%, 0%, .26);  
  color: hsl(0, 0%, 100%);
  text-align: center;
  line-height: 3.9;
  cursor: pointer;
  outline: 0;
}

.drive-mode-popup.active .floating-btn {
  box-shadow: inset 0 0 3px hsla(0, 0%, 0%, .3);
}

.drive-mode-popup .floating-btn:active {
  box-shadow: 0 4px 8px 0 hsla(0, 0%, 0%, .4);
} */

.drive-mode-popup:after {
  display: block;
  content: ' ';
  width: 3.5em;
  height: 3.5em;
  border-radius: 50%;
  position: absolute;
  top: 0;
  right: 0;
  z-index: -2;
  background-color: hsl(4, 98%, 60%);
  transition: all .3s ease;
}

.drive-mode-popup.active:after {
  transform: translate3d(19em,-12.5em,0) scale3d(4, 4, 1);
  transition-timing-function: cubic-bezier(.68, 1.55, .265, 1);
}

.drive-mode-popup .items-wrapper {
  padding: 0;
  margin: 0;
}

.drive-mode-popup .menu-item {
  position: absolute;
  top: .2em;
  right: .2em;
  z-index: -1;
  display: block;
  text-decoration: none;
  color: hsl(0, 0%, 100%);
  font-size: 1em;
  width: 3em;
  height: 3em;
  border-radius: 50%;
  text-align: center;
  line-height: 3;
  background-color: hsla(0, 0%, 0%, 0);
  transition: transform .3s ease, background .2s ease;
}
/* .drive-mode-popup .menu-item.active {
  background-color: hsl(120, 100%, 50%);
  transition: background-color 1000ms linear;
}

.drive-mode-popup .menu-item:hover {
  background-color: hsla(0,0%,0%,.3);
} */

.drive-mode-popup.active .menu-item {
  transition-timing-function: cubic-bezier(0.175, 0.885, 0.32, 1.275);
}

.drive-mode-popup.active .menu-item:nth-child(1) {
  /* transform: translate3d(1em,-7em,0); */
  transform: translate3d(17em,-16em,0);
  
}

.drive-mode-popup.active .menu-item:nth-child(2) {
    transform: translate3d(21em,-16em,0);
}

.drive-mode-popup.active .menu-item:nth-child(3) {
    transform: translate3d(17em,-10em,0);
}

/* .drive-mode-popup.active .menu-item:nth-child(4) {
  transform: translate3d(-6.5em,1em,0);
} */

</style>