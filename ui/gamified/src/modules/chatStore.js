//Store for chat features


const chatStore = {
    state: () => ({
         isChatOn: false,
         messages: [{sender:'admin', time: Date.now(), text: 'Messages from the hardware, admin team and collaborators will appear here'}]
       }),
       mutations:{
         ADD_MESSAGE(state, message){
            state.messages.push(message);
         }
         

       },
       actions:{
         addMessage(context, message){
            context.commit('ADD_MESSAGE', message);
         }


       },
       getters:{
         getIsChatOn(state){
            return state.isChatOn;
         },
         getMessages(state){
            return state.messages;
         },
         getNumMessages(state){
            return state.messages.length;
         }
         
         
       },  
  
  }

  export default chatStore;
