<template>
<div id='hardware-config-button'>
    <button type='button' class='button-toolbar button-secondary' id='achievements-button' @click='openAchievements' aria-label="hardware configuration popup button" data-bs-toggle="tooltip" title="Hardware Config">
        <svg xmlns="http://www.w3.org/2000/svg" id='achievementsmenubutton' width="24" height="24" :fill="getFillColour" class="bi bi-award" viewBox="0 0 16 16">
            <path d="M9.669.864 8 0 6.331.864l-1.858.282-.842 1.68-1.337 1.32L2.6 6l-.306 1.854 1.337 1.32.842 1.68 1.858.282L8 12l1.669-.864 1.858-.282.842-1.68 1.337-1.32L13.4 6l.306-1.854-1.337-1.32-.842-1.68L9.669.864zm1.196 1.193.684 1.365 1.086 1.072L12.387 6l.248 1.506-1.086 1.072-.684 1.365-1.51.229L8 10.874l-1.355-.702-1.51-.229-.684-1.365-1.086-1.072L3.614 6l-.25-1.506 1.087-1.072.684-1.365 1.51-.229L8 1.126l1.356.702 1.509.229z"/>
            <path d="M4 11.794V16l4-1 4 1v-4.206l-2.018.306L8 13.126 6.018 12.1 4 11.794z"/>
        </svg>
        <span v-if='getAchievementUpdated' class='badge rounded-pill bg-danger' id='achievement-notification' role="status" aria-hidden="true">{{ getNewAchievementCount }}<span class="visually-hidden">unread messages</span></span> 
    </button>


    <transition name='fade'>
        <div v-if='popup_showing' class="modal" id='modal-show' tabindex="-1">
            <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title">Achievements</h5>
                    <button type='button' :class="getDarkTheme ? 'btn-close btn-close-white' : 'btn-close'" id='close-button' aria-label="Close" @click='closeAchievements'></button>
                </div>
                <div class="modal-body">
                    
                    <div class='d-flex row m-2 justify-content-center'>
                            <div class='col-4 text-center'>
                                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="chocolate" class="bi bi-award" viewBox="0 0 16 16">
                                    <path d="M9.669.864 8 0 6.331.864l-1.858.282-.842 1.68-1.337 1.32L2.6 6l-.306 1.854 1.337 1.32.842 1.68 1.858.282L8 12l1.669-.864 1.858-.282.842-1.68 1.337-1.32L13.4 6l.306-1.854-1.337-1.32-.842-1.68L9.669.864zm1.196 1.193.684 1.365 1.086 1.072L12.387 6l.248 1.506-1.086 1.072-.684 1.365-1.51.229L8 10.874l-1.355-.702-1.51-.229-.684-1.365-1.086-1.072L3.614 6l-.25-1.506 1.087-1.072.684-1.365 1.51-.229L8 1.126l1.356.702 1.509.229z"/>
                                    <path d="M4 11.794V16l4-1 4 1v-4.206l-2.018.306L8 13.126 6.018 12.1 4 11.794z"/>
                                </svg>
                                <p>50%+</p>
                            </div>
                            
                            <div class='col-4 text-center'>
                                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="silver" class="bi bi-award" viewBox="0 0 16 16">
                                    <path d="M9.669.864 8 0 6.331.864l-1.858.282-.842 1.68-1.337 1.32L2.6 6l-.306 1.854 1.337 1.32.842 1.68 1.858.282L8 12l1.669-.864 1.858-.282.842-1.68 1.337-1.32L13.4 6l.306-1.854-1.337-1.32-.842-1.68L9.669.864zm1.196 1.193.684 1.365 1.086 1.072L12.387 6l.248 1.506-1.086 1.072-.684 1.365-1.51.229L8 10.874l-1.355-.702-1.51-.229-.684-1.365-1.086-1.072L3.614 6l-.25-1.506 1.087-1.072.684-1.365 1.51-.229L8 1.126l1.356.702 1.509.229z"/>
                                    <path d="M4 11.794V16l4-1 4 1v-4.206l-2.018.306L8 13.126 6.018 12.1 4 11.794z"/>
                                </svg>
                                <p>75%+</p>
                            </div>

                            <div class='col-4 text-center'>
                                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="gold" class="bi bi-award" viewBox="0 0 16 16">
                                    <path d="M9.669.864 8 0 6.331.864l-1.858.282-.842 1.68-1.337 1.32L2.6 6l-.306 1.854 1.337 1.32.842 1.68 1.858.282L8 12l1.669-.864 1.858-.282.842-1.68 1.337-1.32L13.4 6l.306-1.854-1.337-1.32-.842-1.68L9.669.864zm1.196 1.193.684 1.365 1.086 1.072L12.387 6l.248 1.506-1.086 1.072-.684 1.365-1.51.229L8 10.874l-1.355-.702-1.51-.229-.684-1.365-1.086-1.072L3.614 6l-.25-1.506 1.087-1.072.684-1.365 1.51-.229L8 1.126l1.356.702 1.509.229z"/>
                                    <path d="M4 11.794V16l4-1 4 1v-4.206l-2.018.306L8 13.126 6.018 12.1 4 11.794z"/>
                                </svg>
                                <p>90%+</p>
                            </div>
                    </div>

                     <div v-for='item in getAchievements' :key='item.name' class='text-start'>
                        <div v-if='item.completed' class='form-check'>
                            <label :for="item.name" class="form-label">{{item.verbose}}</label>
                            <input class="form-check-input" type="checkbox" value="" :id="item.name" :checked='item.completed' disabled>
                        </div>
                        <div v-else-if="!item.hidden" class='form-check'>
                            <label :for="item.name" class="form-label">{{item.verbose}}</label>
                            <input class="form-check-input" type="checkbox" value="" :id="item.name" :checked='item.completed' disabled>
                            <label v-if='"n" in item' :for="item.name" class="form-label"> {{ item.n}}/{{item.required}}</label>
                        </div>
                        <div v-else class='form-check'>
                            <label :for="item.name" class="form-label">Achievement Hidden &nbsp;</label>
                            <input class="form-check-input" type="checkbox" value="" :id="item.name" :checked='item.completed' disabled>
                            <label v-if='"n" in item' :for="item.name" class="form-label"> {{ item.n }}/{{ item.required }} </label>
                        </div>

                    </div>

                  
                </div>

                <div class="modal-footer">
                    <button type="button" class="button-sm button-danger" id="close-modal" @click="closeAchievements">Close</button>
                </div>
            </div>
            </div>
        </div>
    </transition>
</div>
</template>

<script>
import { mapGetters } from 'vuex';
import { mapActions } from 'vuex';

export default {
    name: 'Achievements',
    data () {
        return {
            popup_showing: false,
        }
    },
    mounted(){
        // window.addEventListener('pagehide', () => {this.saveAchievements()});				//closing window
        // window.addEventListener('beforeunload', () => {this.saveAchievements()});			//refreshing page, changing URL

        window.addEventListener("click", (event) => {
              this.handleButtonClick(event);
          });
        


    },
    computed:{
        ...mapGetters([
            'getAchievements',
            'getAchievementUpdated',
            'getNewAchievementCount',
            'getAchievementByName',
            'getDarkTheme',
            'getConfigJSON'
        ]),
        getFillColour(){
            let count = 0;
            let total = this.getAchievements.length;
            this.getAchievements.forEach(achievement => {
                if(achievement.completed){
                    count += 1;
                }
            });
            let frac = count / total;
            if(frac >= 0.9){
                return 'gold';
            } else if(frac >= 0.75){
                return 'silver';
            } else if(frac >= 0.5){
                return 'chocolate';
            } else {
                return 'dark';
            }
            
        }
    },
    watch:{
        getConfigJSON(config){
            this.getAchievementsFromConfig(config);   //get the achievements from config file
            //this.loadAchievements();                    //load any saved achievements from localStorage

            
        },
        getSessionExpired(expired){
          if(expired){
            //create a session expired achievement
          }
      },
    },
    methods:{
        ...mapActions([
            'setAchievementUpdate',
            'addAchievements'
        ]),
        openAchievements(){
          this.popup_showing = true;
          this.setAchievementUpdate(false);
        },
        closeAchievements(){
            this.popup_showing = false;
        },
        getAchievementsFromConfig(config){
            try{
                let achievement_list = config['parameters']['achievements'];
                this.addAchievements(achievement_list);
            } catch (e){
                this.addAchievements([]);
            }
        },
        saveAchievements(){
            let data_json = JSON.stringify(this.$store.getters.getAchievements);
            window.localStorage.setItem('achievementsSpinningDisk', data_json);
        },
        loadAchievements(){
            if(this.getUsesLocalStorage && window.localStorage.getItem('achievementsSpinningDisk')){
                let data = window.localStorage.getItem('achievementsSpinningDisk');
                data = JSON.parse(data);
                this.$store.dispatch('loadAchievements', data);
            }
        },
        AddDragDropListeners(){
            const drag_divs = document.querySelectorAll('[id^="drop_"]');
            drag_divs.forEach(div => {
                div.addEventListener("dragstart", (event) => {
                    console.log('drag event');
                });

                div.addEventListener("drop", (event) => {
                    console.log('drop event');
                });
            })
        },
        AddHotkeyListener(){
            window.addEventListener("keydown", (event) => {
                //update to check achievements for hotkey type achievements and send corresponding achievement that matches hotkey
                if(config.hotkeys.includes(event.key)){
                    console.log('hotkey pressed');
                }
            });
        },
        handleButtonClick(event){
            //check all achievements to see if an appropriate button has been clicked, if so send an achievement with the corresponding name
            console.log(`button clicked: ${event.target.id}`)
        }

    }
}
</script>

<style scoped>

/* #modal-show{
    position: fixed;
} */

.modal-dialog {
  margin: 2.5vh auto;
}

.modal-content {
  position: fixed;
  right: 10px;
  top: 50px;
  height: 95vh;
  max-height: 95vh;
  max-width: 20vw;
  overflow: scroll;
}

@media (max-width: 992px) {
  .modal-content {
        max-width: 95vw;
    }
}

/*Sets the maximum height of the modal body to 90% of the screen height*/
.modal-body {
  height: 90vh;
  max-height: 90vh;
}

#achievement-notification{
  position: absolute;
  top: 100;
  right: 0;
}

#close-button{
    position:absolute;
    right:10px;
}

#popup-text{
    z-index: 999;
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