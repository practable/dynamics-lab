<template>
  <div class="chat d-flex justify-content-center">
        <ul class="navbar-nav">
            <li class="nav-item">
              <button type='button' class='button-toolbar button-secondary' id='chat-button' data-bs-toggle="dropdown" data-bs-auto-close="false" aria-expanded="false" aria-label="show chat button">
                  <svg xmlns="http://www.w3.org/2000/svg" id='chatbuttonimage' width="24" height="24" fill="currentColor" class="bi bi-chat-left-dots" viewBox="0 0 16 16">
                      <path d="M14 1a1 1 0 0 1 1 1v8a1 1 0 0 1-1 1H4.414A2 2 0 0 0 3 11.586l-2 2V2a1 1 0 0 1 1-1h12zM2 0a2 2 0 0 0-2 2v12.793a.5.5 0 0 0 .854.353l2.853-2.853A1 1 0 0 1 4.414 12H14a2 2 0 0 0 2-2V2a2 2 0 0 0-2-2H2z"/>
                      <path d="M5 6a1 1 0 1 1-2 0 1 1 0 0 1 2 0zm4 0a1 1 0 1 1-2 0 1 1 0 0 1 2 0zm4 0a1 1 0 1 1-2 0 1 1 0 0 1 2 0z"/>
                  </svg>
                  <!-- <span v-if='getPromptUpdated' class="badge rounded-pill bg-danger" id='prompt-notification' role="status" aria-hidden="false">{{ getNewPromptCount }}<span class="visually-hidden">unread messages</span></span> -->
              </button>
        
              <ul class="dropdown-menu" id="chat-container" aria-label="chat messages">
                  <chat-widget :message_list="getMessages" :message_count="getNumMessages" @onMessageSent="handleMessageSent" class='m-3' @click.stop/>
              </ul>

            </li>
        </ul>
    </div>
</template>

<script>
import ChatWidget from "./ChatWidget.vue";
import { mapActions, mapGetters } from 'vuex';

export default {
  name: 'Chat',
  components: {
    ChatWidget,
  },
  props: {
    
  },
  data () {
        return {
           
        }
    },
    mounted(){
      
      
    },
  computed:{
        ...mapGetters([
            'getMessages',
            'getNumMessages'
        ]),
    },
    watch:{
    
    },
    methods:{
        ...mapActions([
          'addMessage'
        ]),
        handleMessageSent(message){
          this.addMessage(message);
          console.log(message);
        },
        
      }
  }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
#chat-container{
  background-color: var(--background-color-inverted);
}
</style>
