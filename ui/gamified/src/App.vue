<template>
  <div id="app" class='container-fluid-sm m-0'>
       <navigation-bar />

        <streams id='streams' />
        <data-stream id="data-stream" />
        <logging v-if="getIsLoggingOn" id='logging' />

        <div v-if='!isMobile' class='row' id='component-grid'>

            <div class='d-flex' id='first-row'>
                <div class='drop-area drop-area-two-fifths' id='drop_0_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><webcam-stream id='webcam-stream' /></div>
                <div class='drop-area drop-area-three-fifths' id='drop_1_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><graph v-if='isGraphOn' id='graph' /></div>
            </div>

            <div class='d-flex' id='second-row'>
                <div v-show="!getControlPadConnected" class='drop-area drop-area-two-fifths' id='drop_0_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><control-panel id='control-panel' :url="getDataURL"/></div>
                <div v-show="!getControlPadConnected" class='drop-area drop-area-one-fifth' id='drop_1_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><data-recorder v-if='isDataRecorderOn' id='data-recorder' /></div>
                <div :class="getControlPadConnected ? 'drop-area drop-area-full': 'drop-area drop-area-two-fifths'" id='drop_2_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><data-panel id='data-panel' /></div>
              </div>

            <div class='d-flex' id='third-row'>
                <div class='drop-area drop-area-full' id='drop_0_2' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><gamepad id='gamepad' /></div>
             </div>

        </div>

        <div v-else class='d-flex flex-column' id='component-grid'>

            <div class='drop-area drop-area-mobile' id='drop_0_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><webcam-stream id='webcam-stream' /></div>
            <div class='drop-area drop-area-mobile' id='drop_1_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><control-panel id='control-panel' :url="getDataURL"/></div>
            <div class='drop-area drop-area-mobile' id='drop_2_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><data-recorder v-if='isDataRecorderOn' id='data-recorder' /></div>
            <div class='drop-area drop-area-mobile' id='drop_3_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><data-panel id='data-panel' /></div>
            <div class='drop-area drop-area-mobile' id='drop_4_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><graph v-if='isGraphOn' id='graph' /></div>
           
        </div>
     
   
  </div>
</template>

<script>

import WebcamStream from "./components/WebcamStream.vue";
import DataStream from "./components/DataStream.vue";
import NavigationBar from "./components/NavigationBar.vue";
import Streams from "./components/Streams.vue";
import Logging from './components/Logging.vue'
import ControlPanel from "./components/ControlPanel.vue";
import DataRecorder from "./components/DataRecorder.vue"
import Graph from "./components/Graph.vue"
import DataPanel from "./components/DataPanel.vue";
import Gamepad from "./components/Gamepad.vue";

import { mapGetters } from 'vuex'

export default {
  name: 'App',
  components: {
    Streams,
    WebcamStream,
    DataStream,
    NavigationBar,
    Logging,
    ControlPanel,
    DataRecorder,
    Graph,
    DataPanel,
    Gamepad

  },
  mounted(){
    
  },
  data() {
    return {
      isDataRecorderOn: true,
      isGraphOn: true,
    }
  },
  created(){
    this.$store.dispatch('setUsesLocalStorage', this.hasStorage());
  },
  mounted(){
    this.updateUUID();
  },
  computed:{
    ...mapGetters([
      'getDraggable',
      'getUsesLocalStorage',
      'getIsLoggingOn',
      'getDataURL',
      'getControlPadConnected'
    ]),
    isMobile(){
      if(window.screen.width < 992){
        return true;
      } else{
        return false;
      }
    }
  },
  methods:{
    dragComponent(event){
        event.dataTransfer.effectAllowed = 'move';
         let element = event.target;
         if(element.classList.contains('drop-area')){
           if(element.childNodes[0] != null){
              event.dataTransfer.setData("text/html", element.id + "|" + element.childNodes[0].id);
           } else{
              event.dataTransfer.setData("text/html", element.id + "|" + 'empty');
           }
            
         } else{
           while(element.parentNode){
              element = element.parentNode;
              if(element.classList.contains('drop-area')){
                if(element.childNodes[0] != null){
                  event.dataTransfer.setData("text/html", element.id + "|" + element.childNodes[0].id);
              } else{
                  event.dataTransfer.setData("text/html", element.id + "|" + 'empty');
              }
                break;
              }
            }
         }
    },
    dropComponent(event){
      event.preventDefault();
      event.stopPropagation();
      let dropData = event.dataTransfer.getData('text/html');
      let dropItems = dropData.split("|");
      let draggedZone = document.getElementById(dropItems[0]);
      let droppedElement = document.getElementById(event.target.id);
      let draggedID = dropItems[1];
      
      // only try if the dragged element is not empty
      if(draggedID != 'empty' && document.getElementById(draggedID) != null){
          if(droppedElement != null && droppedElement.classList.contains('drop-area')){
            if(event.target.childNodes.length > 0){
              draggedZone.appendChild(event.target.childNodes[0]);
            }
            droppedElement.appendChild(document.getElementById(draggedID));
            droppedElement.classList.remove('drop-area-highlighted');
        } 
        else if(droppedElement){
          let element = droppedElement;
          while(element.parentNode){
            element = element.parentNode;
            if(element.classList.contains('drop-area')){
              draggedZone.appendChild(element.childNodes[0]);
              element.appendChild(document.getElementById(draggedID));
              element.classList.remove('drop-area-highlighted');
              break;
            }
          }
        }
      }
      
      return false;
    },
    dragEnter(event){
      let dropData = event.dataTransfer.getData('text/html');
      let dropItems = dropData.split("|");
      let draggedID = dropItems[1];
      if(draggedID != 'empty' && document.getElementById(draggedID) != null){
        let element = document.getElementById(event.target.id);
        if(element != null && element.classList.contains('drop-area')){
          element.classList.add('drop-area-highlighted');
        }
        
      }
      
    },
    dragLeave(event){
      let element = document.getElementById(event.target.id);
      if(element != null){
        element.classList.remove('drop-area-highlighted');
      }
      
    },
  
    // loading and saving data for logging consent and uuid etc.
    hasStorage(){
        try {
            window.localStorage.setItem('test', 'storage');
            window.localStorage.removeItem('test');
            return true;
        } catch (exception) {
            return false;
          }
      },
      // UUID is generated by the practable booking system and stored in localStorage (as userName)
      // This function should just find that userName and set it in vuex, else set uuid to null
    updateUUID(){
        let stored_uuid;

        if(this.getUsesLocalStorage){
          stored_uuid = window.localStorage.getItem('userName');
        } else {
          stored_uuid = null;
        }
        
        if(stored_uuid){
            this.$store.dispatch('setUUID', stored_uuid);
        } else{
          this.$store.dispatch('setUUID', 'NA');
        }
      },
      
  },
}
</script>

<style>

</style>
