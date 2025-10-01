<template>
    <div class="d-flex" id="gamepadInfo" ref="gamepadInfo">

    </div>

    <div class="d-flex" id="gampadDisplay" ref="gamepadDisplay">
        <button class="button-xlg button-primary" id="leftTriggerPressedAlert" hidden="true">Left trigger</button>
        <button class="button-xlg button-primary" id="rightTriggerPressedAlert" hidden="true">Right trigger</button>
    </div>

</template>

<script>
import { mapGetters } from 'vuex';
import { mapActions } from 'vuex';
import config from '../config/logging-config';

let loopStarted = false;



function addGamepad(gamepad, infoElement) {
  const d = document.createElement("div");
  d.setAttribute("id", `controller${gamepad.index}`);

  const t = document.createElement("h1");
  t.textContent = `gamepad: ${gamepad.id}`;
  d.append(t);

  const b = document.createElement("ul");
  b.className = "buttons";
  gamepad.buttons.forEach((button, i) => {
    const e = document.createElement("li");
    e.className = "button-gamepad button-secondary align-content-center me-2";
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

function removeGamepad(gamepad) {
  document.querySelector(`#controller${gamepad.index}`).remove();
}

function updateStatus() {
  for (const gamepad of navigator.getGamepads()) {
    if (!gamepad) continue;

    const d = document.getElementById(`controller${gamepad.index}`);
    const buttonElements = d.getElementsByClassName("button-gamepad");

    for (const [i, button] of gamepad.buttons.entries()) {
      const el = buttonElements[i];

      const pct = `${Math.round(button.value * 100)}%`;
    //   el.style.backgroundSize = `${pct} ${pct}`;
      if (button.pressed) {
        
        checkForHapticResponse(gamepad, i);
        checkForTriggerGUIUpdate(gamepad, i);
        
        el.textContent = `${i}`;
        // el.style.color = "#42f593";
        el.className = "button-gamepad button-pressed align-content-center me-2";
      } else {
        el.textContent = `${i}`;
        // el.style.color = "#2e2d33";
        el.className = "button-gamepad button-secondary align-content-center me-2";
      }
    }

    const axisElements = d.getElementsByClassName("axis");
    for (const [i, axis] of gamepad.axes.entries()) {
      const el = axisElements[i];
      el.textContent = `${i}: ${axis.toFixed(4)}`;
      el.setAttribute("value", axis + 1);
    }
  }

  requestAnimationFrame(updateStatus);
}

function checkForHapticResponse(gamepad, buttonIndex){
    if(buttonIndex != 6 && buttonIndex != 7){
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
}

function checkForTriggerGUIUpdate(gamepad, buttonIndex){
    if(buttonIndex == 6){
        document.getElementById("leftTriggerPressedAlert").hidden = false;
        document.getElementById("rightTriggerPressedAlert").hidden = true;
        // this.leftTriggerPressed = true;
        // this.rightTriggerPressed = false;
    } else if(buttonIndex == 7){
        // this.leftTriggerPressed = false;
        // this.rightTriggerPressed = true;
        document.getElementById("leftTriggerPressedAlert").hidden = true;
        document.getElementById("rightTriggerPressedAlert").hidden = false;
    } else{
        // this.leftTriggerPressed = false;
        // this.rightTriggerPressed = false;
        document.getElementById("leftTriggerPressedAlert").hidden = true;
        document.getElementById("rightTriggerPressedAlert").hidden = true;
    }
}

export default {

  name: 'Gamepad',
  data () {
    return {
        // leftTriggerPressed: false,
        // rightTriggerPressed: false
    }
  },
  components: {
    
  },
created(){
      
  },
  mounted(){
    window.addEventListener("DOMContentLoaded", (event) => {
        window.addEventListener("gamepadconnected", (evt) => {
            // console.log(this.$refs.gamepadInfo);
            addGamepad(evt.gamepad, this.$refs.gamepadInfo);
        });

        window.addEventListener("gamepaddisconnected", (evt) => {
            removeGamepad(evt.gamepad);
        });
    });


    
  },
  computed:{
    ...mapGetters([

    ])
  },
  watch:{
      
  },
  methods: {
      ...mapActions([

      ])
     
    
  }
}
</script>

<style scoped>


</style>