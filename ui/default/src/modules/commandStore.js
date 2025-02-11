//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,
        currentMode: 'stopped'  //driven, undriven
       }),
       mutations:{
        SET_DATA_SOCKET(state, socket){
            state.dataSocket = socket;
        },
        SET_STOP(state){
            state.currentMode = 'stopped';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "stop"
                }));
            }
        },
        SET_DRIVEN(state){
            state.currentMode = 'driven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "start"
                }));
            }
        },
        SET_UNDRIVEN(state){
            state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "ping"
                }));
            }
        },
        UPDATE_SPEED_HZ(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "hz",
                    to: val
                }));
            }
        },
        UPDATE_SPEED_RPM(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "rpm",
                    to: val
                }));
            }
        },
        UPDATE_SAMPLE_RATE(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "sample",
                    to: val
                }));
            }
        },
            

       },
       actions:{
        setDataSocket(context, socket){
            context.commit("SET_DATA_SOCKET", socket);
        },
        setHardwareStop(context){
            context.commit('STOP');
        },
        

       },
       getters:{
        getDataSocket(state){
            return state.dataSocket;
        },
        getCurrentMode(state){
            return state.currentMode;
        },
          
       },  
  
  }

  export default commandStore;