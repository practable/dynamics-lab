//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,
        currentMode: 'stopped',  //driven, undriven - CONSIDER THE NEED FOR BOTH currentMode and currentState
        currentState: 'STATE_WAIT'      //reported state from hardware
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
        },
        SET_UNDRIVEN(state){
            state.currentMode = 'undriven';
        },
        COMMAND_START(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "start"
                }));
            }
        },
        COMMAND_PING(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "ping"
                }));
            }
        },
        COMMAND_UPDATE_SPEED_HZ(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "hz",
                    to: val
                }));
            }
        },
        COMMAND_UPDATE_SPEED_RPM(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "rpm",
                    to: val
                }));
            }
        },
        COMMAND_UPDATE_SAMPLE_RATE(state, val){
            //state.currentMode = 'undriven';
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "sample",
                    to: val
                }));
            }
        },
        COMMAND_START_STREAMING(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "stream"
                }));
            }
        },
        COMMAND_STOP_STREAMING(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "endst"
                }));
            }
        },
        COMMAND_CALIBRATE(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "cal"
                }));
            }
        }
            

       },
       actions:{
        setDataSocket(context, socket){
            context.commit("SET_DATA_SOCKET", socket);
        },
        setModeStop(context){
            context.commit('SET_STOP');
        },
        setModeDriven(context){
            context.commit('SET_DRIVEN');
        },
        setModeUndriven(context){
            context.commit('SET_UNDRIVEN');
        },
        sendCommandStart(context){
            context.commit('COMMAND_START');
        },
        sendCommandPing(context){
            context.commit('COMMAND_PING');
        },
        sendCommandUpdateSpeedHz(context, val){
            context.commit('COMMAND_UPDATE_SPEED_HZ', val);
        },
        sendCommandUpdateSpeedRPM(context, val){
            context.commit('COMMAND_UPDATE_SPEED_RPM', val);
        },
        sendCommandUpdateSampleRate(context, val){
            context.commit('COMMAND_UPDATE_SAMPLE_RATE', val);
        },
        sendCommandStartStream(context){
            context.commit('COMMAND_START_STREAMING');
        },
        sendCommandStopStream(context){
            context.commit('COMMAND_STOP_STREAMING');
        },
        sendCommandCalibrate(context){
            context.commit('COMMAND_CALIBRATE');
        }
        

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