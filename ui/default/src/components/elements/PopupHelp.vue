//Vue3 update

<template>
<div :id='id'>
    <button type='button' class='button-toolbar button-secondary' :id='id + "-button"' @blur="closeHelp" @click='openHelp' aria-label="popup help" data-bs-toggle="tooltip" title="Help">
        <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-info-square" viewBox="0 0 16 16">
            <path d="M14 1a1 1 0 0 1 1 1v12a1 1 0 0 1-1 1H2a1 1 0 0 1-1-1V2a1 1 0 0 1 1-1h12zM2 0a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V2a2 2 0 0 0-2-2H2z"/>
            <path d="m8.93 6.588-2.29.287-.082.38.45.083c.294.07.352.176.288.469l-.738 3.468c-.194.897.105 1.319.808 1.319.545 0 1.178-.252 1.465-.598l.088-.416c-.2.176-.492.246-.686.246-.275 0-.375-.193-.304-.533L8.93 6.588zM9 4.5a1 1 0 1 1-2 0 1 1 0 0 1 2 0z"/>
        </svg>
    </button>


    <!-- <transition name='fade'> -->
        <div v-if='popup_showing' class="modal" id='modal-popup-help' tabindex="-1">
            <div class="modal-dialog modal-dialog-centered modal-dialog-scrollable">
            <div class="modal-content">
                <div class="modal-header">
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

  name: 'PopupHelp',
  props:{
      'id': String
  },
  data () {
    return {
        popup_showing: false,
        // popupWidth: 50,
        // popupHeight: 50,
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
        let modal = document.getElementById('modal-popup-help');
        //document.body.removeChild(modal);
        let here = document.getElementById(this.id);
        here.appendChild(modal);

        this.$nextTick(() => {
          this.popup_showing = false;
        });
       
        
        
      }
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