//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,
        currentMode: 'stopped',
        currentState: 'STATE_WAIT',      //reported state from hardware, do I need this in the UI?
        samplingRate: {'value': 20, 'max':40, 'min':1, 'step':1},
        drivingFrequency: {
            'hz': 1, 
            'rpm': 60, 
            'from_hardware': {'hz': 1, 'rpm': 60,}, 
            'max':20, 
            'min':1, 
            'step':1},

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
        UPDATE_DRIVING_FREQUENCY_HZ(state, val){
            state.drivingFrequency.hz = Number(val);
        },
        UPDATE_DRIVING_FREQUENCY_RPM(state, val){
            state.drivingFrequency.rpm = Number(val);
        },
        COMMAND_UPDATE_DRIVING_FREQUENCY_HZ(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "hz",
                    to: state.drivingFrequency.hz
                }));
            }
        },
        COMMAND_UPDATE_DRIVING_FREQUENCY_RPM(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "rpm",
                    to: state.drivingFrequency.rpm
                }));
            }
        },
        SET_REPORTED_DRIVING_FREQUENCY(state, val_object){
            state.drivingFrequency['from_hardware'].hz = val_object.hz;
            state.drivingFrequency['from_hardware'].rpm = val_object.rpm;
        },
        COMMAND_UPDATE_SAMPLE_RATE(state, val){
            state.samplingRate.value = val;
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
        updateDrivingFrequencyHz(context, val){
            let to_set = val;
            if(val < context.getters.getDrivingFrequencyMin){
                to_set = context.getters.getDrivingFrequencyMin;
            } else if(val > context.getters.getDrivingFrequencyMax){
                to_set = context.getters.getDrivingFrequencyMax;
            }
            context.commit('UPDATE_DRIVING_FREQUENCY_HZ', to_set);
            let rpm = helpers.convertHzToRPM(to_set);
            context.commit('UPDATE_DRIVING_FREQUENCY_RPM', rpm);
        },
        updateDrivingFrequencyRPM(context, val){
            context.commit('UPDATE_DRIVING_FREQUENCY_RPM', val);
            let hz = helpers.convertRPMToHz(val);
            context.commit('UPDATE_DRIVING_FREQUENCY_HZ', hz);
        },
        sendCommandUpdateDrivingFrequencyHz(context){
            context.commit('COMMAND_UPDATE_DRIVING_FREQUENCY_HZ');
        },
        sendCommandUpdateDrivingFrequencyRPM(context){
            context.commit('COMMAND_UPDATE_DRIVING_FREQUENCY_RPM');
        },
        setReportedDrivingFrequency(context, val_object){
            context.commit('SET_REPORTED_DRIVING_FREQUENCY', val_object)
        },
        sendCommandUpdateSampleRate(context, val){
            let to_set = val;
            if(val < context.getters.getSamplingRateMin){
                to_set = context.getters.getSamplingRateMin;
            } else if(val > context.getters.getSamplingRateMax){
                to_set = context.getters.getSamplingRateMax;
            }
            context.commit('COMMAND_UPDATE_SAMPLE_RATE', to_set);
        },
        sendCommandStartStream(context){
            context.commit('COMMAND_START_STREAMING');
        },
        sendCommandStopStream(context){
            context.commit('COMMAND_STOP_STREAMING');
        },
        sendCommandCalibrate(context){
            context.commit('COMMAND_CALIBRATE');
        },
        

       },
       getters:{
        getDataSocket(state){
            return state.dataSocket;
        },
        getCurrentMode(state){
            return state.currentMode;
        },
        getSamplingRate(state){
            return state.samplingRate.value;
        },
        getSamplingRateMax(state){
            return state.samplingRate.max;
        },
        getSamplingRateMin(state){
            return state.samplingRate.min;
        },
        getSamplingRateStep(state){
            return state.samplingRate.step;
        },
        getDrivingFrequencyMin(state){
            return state.drivingFrequency.min;
        },
        getDrivingFrequencyMax(state){
            return state.drivingFrequency.max;
        },
        getDrivingFrequencyStep(state){
            return state.drivingFrequency.step;
        },
        getDrivingFrequencyHz(state){
            return state.drivingFrequency.hz;
        },
        getDrivingFrequencyRPM(state){
            return state.drivingFrequency.rpm;
        },
        getReportedDrivingFrequencyHz(state){
            return state.drivingFrequency['from_hardware'].hz;
        },
        getReportedDrivingFrequencyRPM(state){
            return state.drivingFrequency['from_hardware'].rpm;
        },
          
       },  
  
  }

  export const helpers = {
    convertHzToRPM(hz) {
       return Math.round(hz*60)
    },
    convertRPMToHz(rpm) {
        return Math.round(rpm/60)
    }
 }

  export default commandStore;