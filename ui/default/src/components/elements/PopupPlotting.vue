//Vue3 update

<template>
<div :id='id'>
    <button type='button' class='button-toolbar button-secondary' :id='id + "-button"' @click='openHelp' aria-label="popup help" data-bs-toggle="tooltip" title="Help">
        <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-graph-up" viewBox="0 0 16 16">
                <path fill-rule="evenodd" d="M0 0h1v15h15v1H0zm14.817 3.113a.5.5 0 0 1 .07.704l-4.5 5.5a.5.5 0 0 1-.74.037L7.06 6.767l-3.656 5.027a.5.5 0 0 1-.808-.588l4-5.5a.5.5 0 0 1 .758-.06l2.609 2.61 4.15-5.073a.5.5 0 0 1 .704-.07"/>
            </svg>
    </button>


    <!-- <transition name='fade'> -->
        <div v-if='popup_showing' class="modal" id='modal-popup-help' tabindex="-1">
            <div class="modal-dialog model-dialog-centered modal-dialog-scrollable">
            <div class="modal-content">
                <div class="modal-header" @mousedown="(event) => {selectModal(event)}" @mousemove="(event) => {moveModal(event)}" @mouseup="(event) => {deselectModal(event)}" @mouseleave="(event) => {deselectModal(event)}">
                    <slot name="header"></slot>
                    <button type='button' :class="getDarkTheme ? 'btn-close btn-close-white' : 'btn-close'" id='close-button' aria-label="Close" @click='closeHelp'></button>
                  </div>
                <div class="modal-body">
                    <slot name="body"></slot>

                </div>

                <div class="modal-footer">
                    <slot name="footer"></slot>
                    <button type="button" class="button-sm button-danger" id="close-modal" @click="closeHelp">Close</button>
                </div>
            </div>
            </div>
        </div>
    <!-- </transition> -->

    
    
</div>
</template>

<script>
import { mapGetters } from 'vuex';

export default {

  name: 'PopupPlotting',
  props:{
      'id': String
  },
  data () {
    return {
        popup_showing: false,
        popup_clicked: false,
        popup_ready: false,
        mouse_intial_X: 0,
        mouse_initial_Y: 0,
    }
  },
  components: {
    
  },
  created(){
      
  },
  mounted(){
    

  },
  computed:{
    ...mapGetters([
        'getDarkTheme'
      ])
  },
  watch:{
      
  },
  
  methods: {
      openHelp(){
        this.popup_showing = true;

        this.$nextTick(() => {
          let modal = document.getElementById('modal-popup-help');
          document.body.appendChild(modal);
      });
        
      },
      closeHelp(){
        if(this.popup_showing){
          let modal = document.getElementById('modal-popup-help');
          //document.body.removeChild(modal);
          let here = document.getElementById(this.id);
          try{
            here.appendChild(modal);
          } catch(e){
            console.log(e)
          }

          this.popup_showing = false;
          this.popup_clicked = false;
          
        }
        
      },
      selectModal(event){
        event.preventDefault();
        const target = document.querySelector('.modal-content');
        this.popup_clicked = true;
        this.mouse_intial_X = event.clientX;
        this.mouse_initial_Y = event.clientY;
        target.style.cursor = 'move';

        this.popup_ready = false;
      },
      moveModal(event){
        if(this.popup_clicked){
          event.preventDefault();
          const target = document.querySelector('.modal-content');
          const x = this.mouse_intial_X - event.clientX;
          const y = this.mouse_intial_Y - event.clientY;
          this.mouse_intial_X = event.clientX;
          this.mouse_intial_Y = event.clientY;

          //requires one loop of setting parameters before you should actually set position
          if(this.popup_ready){
            target.style.top = (target.offsetTop - y) + "px";
            target.style.left = (target.offsetLeft - x) + "px";
          } else{
            this.popup_ready = true;
          }
        }
      },
      deselectModal(event){
        const target = document.querySelector('.modal-content');
        this.popup_clicked = false;
        target.style.cursor = 'default';
        this.popup_ready = false;
      },
  }
}
</script>

<style>
#modal-popup-help{
  display: block;
}


#close-button{
    position:absolute;
    right:10px;
}

.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.5s ease;
}

.fade-enter-from,
.fade-leave-to {
  opacity: 0;
}

</style>