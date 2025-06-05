
<template>

    <nav :class="getDarkTheme ? 'navbar navbar-dark fixed-top navbar-expand-lg navbar-background' : 'navbar navbar-light fixed-top navbar-expand-lg  navbar-background'" id='navbar'>
    <div class="container-fluid">
      <div class="navbar-brand">
          <img src="/images/practable-icon.png" width="30" height="30" alt="practable.io logo">
          Dynamics Lab
      </div>
      <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
      </button>

      <div class="collapse navbar-collapse" id="navbarSupportedContent">
          <ul class="navbar-nav me-auto">
             
                <li class="nav-item">
                    <clock class='nav-link' />
                </li>

          </ul>

            <ul class="navbar-nav dropstart">

              <li class="nav-item me-1">
                <show-hardware-config-button />
              </li>
  
                <li class="nav-item me-1">
                  <button type='button' class='button-toolbar button-secondary' id='toggle-theme-button' aria-label='toggle dark theme' @click='toggleTheme' :disabled="disableThemeButton">
                      <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-circle-half" viewBox="0 0 16 16">
                        <path d="M8 15A7 7 0 1 0 8 1zm0 1A8 8 0 1 1 8 0a8 8 0 0 1 0 16"/>
                    </svg>
                  </button>
              </li>

            </ul>

      </div>
    </div>
  </nav>

</template>

<script>

import Clock from "./Clock.vue";
import { mapGetters } from 'vuex';
import ShowHardwareConfigButton from "./elements/ShowHardwareConfigButton.vue";

export default {

  name: 'NavigationBar',
  emits:[],
  data () {
    return {
      disableThemeButton: false,
    }
  },
  components: {
    Clock,
    ShowHardwareConfigButton
  },
  computed:{
      ...mapGetters([
        'getDarkTheme',
        'getIsChatOn'
      ])
  },
  methods: {
      addTool(tool){
          this.toggleComponent('workspace');
          setTimeout(() => {this.$emit('add' + tool)}, 100);  //give the workspace time to initialise and then send tool event
          
      },
      toggleComponent(component){
          this.$emit('toggle' + component);
      },
      clearWorkspace(){
          this.$emit('clearworkspace');
      },
      // needs a short delay before can be rerun so as to enable the chart to recolour everything
      toggleTheme(){
          this.disableThemeButton = true;
          setTimeout(() => {
            this.disableThemeButton = false
          }, 1000);
          document.body.classList.toggle("dark-theme");
          this.$store.dispatch('setDarkTheme', document.body.classList.contains("dark-theme"));
      }
  }
}
</script>

<style scoped>


</style>