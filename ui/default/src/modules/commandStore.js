//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,

       }),
       mutations:{
        SET_DATA_SOCKET(state, socket){
            state.dataSocket = socket;
        },
            

       },
       actions:{
        setDataSocket(context, socket){
            context.commit("SET_DATA_SOCKET", socket);
        },
        

       },
       getters:{
        getDataSocket(state){
            return state.dataSocket;
        },
          
       },  
  
  }

  export default commandStore;