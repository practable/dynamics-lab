<template>
<div id='hardware-config-button'>
    <!-- <button type='button' class='button-toolbar button-secondary' id='achievements-button' @click='openAchievements' aria-label="achievements popup button" data-bs-toggle="tooltip" title="Achievements">
        <svg xmlns="http://www.w3.org/2000/svg" id='achievementsmenubutton' width="24" height="24" :fill="getFillColour" class="bi bi-award" viewBox="0 0 16 16">
            <path d="M9.669.864 8 0 6.331.864l-1.858.282-.842 1.68-1.337 1.32L2.6 6l-.306 1.854 1.337 1.32.842 1.68 1.858.282L8 12l1.669-.864 1.858-.282.842-1.68 1.337-1.32L13.4 6l.306-1.854-1.337-1.32-.842-1.68L9.669.864zm1.196 1.193.684 1.365 1.086 1.072L12.387 6l.248 1.506-1.086 1.072-.684 1.365-1.51.229L8 10.874l-1.355-.702-1.51-.229-.684-1.365-1.086-1.072L3.614 6l-.25-1.506 1.087-1.072.684-1.365 1.51-.229L8 1.126l1.356.702 1.509.229z"/>
            <path d="M4 11.794V16l4-1 4 1v-4.206l-2.018.306L8 13.126 6.018 12.1 4 11.794z"/>
        </svg>
        <span v-if='getAchievementUpdated' class='badge rounded-pill bg-danger' id='achievement-notification' role="status" aria-hidden="true">{{ getNewAchievementCount }}<span class="visually-hidden">unread messages</span></span> 
    </button> -->

    <button type='button' class='button-secondary button-achievements' id='achievements-button' @click='openAchievements' aria-label="achievements popup button" data-bs-toggle="tooltip" title="Achievements">
        
        <template v-for="frac in [0.1,0.3,0.5,0.7,0.9]">
            <svg v-if="getFillFraction >= frac+0.1" xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="yellow" class="bi bi-star-fill" viewBox="0 0 16 16">
                <path d="M3.612 15.443c-.386.198-.824-.149-.746-.592l.83-4.73L.173 6.765c-.329-.314-.158-.888.283-.95l4.898-.696L7.538.792c.197-.39.73-.39.927 0l2.184 4.327 4.898.696c.441.062.612.636.282.95l-3.522 3.356.83 4.73c.078.443-.36.79-.746.592L8 13.187l-4.389 2.256z"/>
            </svg>
            <svg v-else-if="getFillFraction >= frac" xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="yellow" class="bi bi-star-half" viewBox="0 0 16 16">
                <path d="M5.354 5.119 7.538.792A.52.52 0 0 1 8 .5c.183 0 .366.097.465.292l2.184 4.327 4.898.696A.54.54 0 0 1 16 6.32a.55.55 0 0 1-.17.445l-3.523 3.356.83 4.73c.078.443-.36.79-.746.592L8 13.187l-4.389 2.256a.5.5 0 0 1-.146.05c-.342.06-.668-.254-.6-.642l.83-4.73L.173 6.765a.55.55 0 0 1-.172-.403.6.6 0 0 1 .085-.302.51.51 0 0 1 .37-.245zM8 12.027a.5.5 0 0 1 .232.056l3.686 1.894-.694-3.957a.56.56 0 0 1 .162-.505l2.907-2.77-4.052-.576a.53.53 0 0 1-.393-.288L8.001 2.223 8 2.226z"/>
            </svg>
            <svg v-else xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-star" viewBox="0 0 16 16">
                <path d="M2.866 14.85c-.078.444.36.791.746.593l4.39-2.256 4.389 2.256c.386.198.824-.149.746-.592l-.83-4.73 3.522-3.356c.33-.314.16-.888-.282-.95l-4.898-.696L8.465.792a.513.513 0 0 0-.927 0L5.354 5.12l-4.898.696c-.441.062-.612.636-.283.95l3.523 3.356-.83 4.73zm4.905-2.767-3.686 1.894.694-3.957a.56.56 0 0 0-.163-.505L1.71 6.745l4.052-.576a.53.53 0 0 0 .393-.288L8 2.223l1.847 3.658a.53.53 0 0 0 .393.288l4.052.575-2.906 2.77a.56.56 0 0 0-.163.506l.694 3.957-3.686-1.894a.5.5 0 0 0-.461 0z"/>
            </svg>
        </template>
        
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
                    
                    <div class='d-flex flex-row m-2 justify-content-center'>
                        <template v-for="frac in [0.1,0.3,0.5,0.7,0.9]">
                            <svg v-if="getFillFraction >= frac+0.1" xmlns="http://www.w3.org/2000/svg" width="32" height="32" fill="yellow" class="bi bi-star-fill me-1" viewBox="0 0 16 16">
                                <path d="M3.612 15.443c-.386.198-.824-.149-.746-.592l.83-4.73L.173 6.765c-.329-.314-.158-.888.283-.95l4.898-.696L7.538.792c.197-.39.73-.39.927 0l2.184 4.327 4.898.696c.441.062.612.636.282.95l-3.522 3.356.83 4.73c.078.443-.36.79-.746.592L8 13.187l-4.389 2.256z"/>
                            </svg>
                            <svg v-else-if="getFillFraction >= frac" xmlns="http://www.w3.org/2000/svg" width="32" height="32" fill="yellow" class="bi bi-star-half me-1" viewBox="0 0 16 16">
                                <path d="M5.354 5.119 7.538.792A.52.52 0 0 1 8 .5c.183 0 .366.097.465.292l2.184 4.327 4.898.696A.54.54 0 0 1 16 6.32a.55.55 0 0 1-.17.445l-3.523 3.356.83 4.73c.078.443-.36.79-.746.592L8 13.187l-4.389 2.256a.5.5 0 0 1-.146.05c-.342.06-.668-.254-.6-.642l.83-4.73L.173 6.765a.55.55 0 0 1-.172-.403.6.6 0 0 1 .085-.302.51.51 0 0 1 .37-.245zM8 12.027a.5.5 0 0 1 .232.056l3.686 1.894-.694-3.957a.56.56 0 0 1 .162-.505l2.907-2.77-4.052-.576a.53.53 0 0 1-.393-.288L8.001 2.223 8 2.226z"/>
                            </svg>
                            <svg v-else xmlns="http://www.w3.org/2000/svg" width="32" height="32" fill="currentColor" class="bi bi-star me-1" viewBox="0 0 16 16">
                                <path d="M2.866 14.85c-.078.444.36.791.746.593l4.39-2.256 4.389 2.256c.386.198.824-.149.746-.592l-.83-4.73 3.522-3.356c.33-.314.16-.888-.282-.95l-4.898-.696L8.465.792a.513.513 0 0 0-.927 0L5.354 5.12l-4.898.696c-.441.062-.612.636-.283.95l3.523 3.356-.83 4.73zm4.905-2.767-3.686 1.894.694-3.957a.56.56 0 0 0-.163-.505L1.71 6.745l4.052-.576a.53.53 0 0 0 .393-.288L8 2.223l1.847 3.658a.53.53 0 0 0 .393.288l4.052.575-2.906 2.77a.56.56 0 0 0-.163.506l.694 3.957-3.686-1.894a.5.5 0 0 0-.461 0z"/>
                            </svg>
                        </template>
                    </div>

                    <div class='d-flex flex-row m-2 justify-content-center'>
                        <h5>Completed {{ getAchievementsCompleted.length }}/{{ getAchievements.length }}</h5>
                    </div>

                     <div v-for='item in getAchievements' :key='item.name' class='text-start'>
                        <div v-if='item.completed' class='form-check'>
                            <label :for="item.name" class="form-label">{{item.title}}</label>
                            <input class="form-check-input" type="checkbox" value="" :id="item.name" :checked='item.completed' disabled>
                            
                            <button class="button-expand" type="button" data-bs-toggle="collapse" :data-bs-target="'#expand'+item.name" aria-expanded="false" aria-controls="collapseExample">
                                <svg xmlns="http://www.w3.org/2000/svg" width="8" height="8" fill="currentColor" class="bi bi-caret-down-fill" viewBox="0 0 16 16">
                                    <path d="M7.247 11.14 2.451 5.658C1.885 5.013 2.345 4 3.204 4h9.592a1 1 0 0 1 .753 1.659l-4.796 5.48a1 1 0 0 1-1.506 0z"/>
                                </svg>
                            </button>
                            
                            <div class="collapse" :id="'expand'+item.name">
                                <div :class="getDarkTheme ? 'card card-body text-dark' : 'card card-body text-dark'">
                                    {{ item.verbose }}
                                </div>
                            </div>
                        </div>
                        <div v-else-if="!item.hidden" class='form-check'>
                            <label :for="item.name" class="form-label">{{item.title}}</label>
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
import config from '../config/logging-config'

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

          window.addEventListener("keydown", (event) => {
                this.handleHotkeyPress(event);
            });
        


    },
    computed:{
        ...mapGetters([
            'getAchievements',
            'getAchievementUpdated',
            'getNewAchievementCount',
            'getAchievementByName',
            'getAchievementsUncompleted',
            'getAchievementsCompleted',
            'getDarkTheme',
            'getConfigJSON'
        ]),
        // getFillColour(){
        //     let count = 0;
        //     let total = this.getAchievements.length;
        //     this.getAchievements.forEach(achievement => {
        //         if(achievement.completed){
        //             count += 1;
        //         }
        //     });
        //     let frac = count / total;
        //     if(frac >= 0.9){
        //         return 'gold';
        //     } else if(frac >= 0.75){
        //         return 'silver';
        //     } else if(frac >= 0.5){
        //         return 'chocolate';
        //     } else {
        //         return 'dark';
        //     }
            
        // },
        getFillFraction(){
            let count = 0;
            let total = this.getAchievements.length;
            this.getAchievements.forEach(achievement => {
                if(achievement.completed){
                    count += 1;
                }
            });
            return count / total;
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
            'addAchievements',
            'addMultipleAchievement',
            'setFractionalAchievementCompleted',
            'setAchievementCompleted'
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
        /**
        * For loading of achievements that are saved in localStorage
        * whilst achievements are locally stored - may upgrade this to server-based
        */
        loadAchievements(){
            if(this.getUsesLocalStorage && window.localStorage.getItem('achievementsSpinningDisk')){
                let data = window.localStorage.getItem('achievementsSpinningDisk');
                data = JSON.parse(data);
                this.$store.dispatch('loadAchievements', data);
            }
        },
        /** 
         * Checks whether a button click corresponds with an achievement, checks if the
         * achievement is completed and completes it if not.
         * 
         */
        handleButtonClick(event){
            for(let i=0; i<this.getAchievements.length; i++){
                if(this.getAchievements[i]['type'] == 'click'){
                    if('fractional' in this.getAchievements[i]){
                        for(let j=0;j<this.getAchievements[i]['fractional'].length;j++){
                            if(this.getAchievements[i]['fractional'][j]['id'] == event.target.id){
                                //different sub-achievements required to complete the full achievement
                                console.log(this.getAchievements[i]['fractional'][j]['name']);
                                this.setFractionalAchievementCompleted({"name": this.getAchievements[i]['name'], "fractional": this.getAchievements[i]['fractional'][j]['name']});
                            }
                        }
                    }
                    else if(this.getAchievements[i]['id'] == event.target.id){
                        if('required' in this.getAchievements[i]){
                            //multiple stages required for this achievement
                            this.addMultipleAchievement(this.getAchievements[i]['name']);
                        }  
                        else
                        {
                            //achievement complete
                            this.setAchievementCompleted(this.getAchievements[i]['name']);
                        }
                    }
                }
                
            }
        },
        /**
         * Checks whether a recognised hotkey has been pressed, checks hotkey achievements and sends achievement if corresponding hotkey was pressed.
         */
        handleHotkeyPress(event){
            if(config.hotkeys.includes(event.key)){
                for(let i=0; i<this.getAchievements.length; i++){
                    if(this.getAchievements[i]['type'] == 'hotkey'){
                        if('fractional' in this.getAchievements[i]){
                            for(let j=0;j<this.getAchievements[i]['fractional'].length;j++){
                                if(this.getAchievements[i]['fractional'][j]['hotkey'] == event.key){
                                    //different sub-achievements required to complete the full achievement
                                    console.log(this.getAchievements[i]['fractional'][j]['name']);
                                    this.setFractionalAchievementCompleted({"name": this.getAchievements[i]['name'], "fractional": this.getAchievements[i]['fractional'][j]['name']});
                                }
                            }
                        }
                        else if(this.getAchievements[i]['hotkey'] == event.key){
                            if('required' in this.getAchievements[i]){
                                //multiple stages required for this achievement
                                this.addMultipleAchievement(this.getAchievements[i]['name']);
                            }  
                            else
                            {
                                //achievement complete
                                this.setAchievementCompleted(this.getAchievements[i]['name']);
                            }
                        }
                    }
                }
            }
        },
        // AddDragDropListeners(){
        //     const drag_divs = document.querySelectorAll('[id^="drop_"]');
        //     drag_divs.forEach(div => {
        //         div.addEventListener("dragstart", (event) => {
        //             console.log('drag event');
        //         });

        //         div.addEventListener("drop", (event) => {
        //             console.log('drop event');
        //         });
        //     })
        // }
        
        

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

.button-achievements{
    border-radius: 4px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1rem;
    margin: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 100px;
    height: 40px;
    min-width: 40px;
    min-height: 40px;
    max-height: 40px;
    max-width: 100px;
    padding: 0px 0px;
}

.button-expand{
    background-color: var(--background-color-primary);
    color: var(--text-color);
    border-style: none;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1rem;
    margin: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 20px;
    height: 20px;
    max-height: 20px;
    max-width: 20px;
    padding: 0px 0px;
}
</style>