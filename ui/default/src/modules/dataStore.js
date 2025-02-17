//import Vue from 'vue';

const dataStore = {
   state: () => ({
      //current values
      start_time: 0,
      currentTime: 0,
      currentPos: {},
      currentAcc: {},
      currentGyro: {},
      //recorded data
      data: [],
      max_data_points: 5000,
      isRecording: false,
   }),
   mutations:{
      SET_IS_RECORDING(state, set){
         state.isRecording = set;
      },
      CLEAR_ALL_DATA(state){
         state.data = [];
      },
      ADD_DATA(state, object){
         state.data.push(object);
      },
      DELETE_DATA(state, id){
         state.data.splice(id, 1);
      },
      SET_START_TIME(state, time){
         state.start_time = time;
      },
      SET_CURRENT_TIME(state, time){
         state.currentTime = time;
      },
      SET_CURRENT_POS(state, pos){
         state.currentPos = pos;
      },
      SET_CURRENT_ACC(state, acc){
         state.currentAcc = acc;
      },
      SET_CURRENT_GYRO(state, gyro){
         state.currentGyro = gyro;
      }

   },
   actions:{
      setIsRecording(context, value){
         context.commit('SET_IS_RECORDING', value);
      },
      clearAllData(context){
         context.commit('CLEAR_ALL_DATA');
      },
      addData(context,object){
         context.commit('ADD_DATA', object);
      },
      deleteData (context, dataId) {
         context.commit('DELETE_DATA', dataId);
      },
      setStartTime(context, time){
         context.commit("SET_START_TIME", time);
      },
      setCurrentTime(context, time){
         context.commit('SET_CURRENT_TIME', time);
      },
      setCurrentPosition(context, pos){
         context.commit('SET_CURRENT_POS', pos);
      },
      setCurrentAcceleration(context, acc){
         context.commit('SET_CURRENT_ACC', acc);
      },
      setCurrentGyro(context, gyro){
         context.commit('SET_CURRENT_GYRO', gyro);
      }
   },
   getters:{
      getData(state){
         return state.data;
      },
      getIsRecording(state){
         return state.isRecording;
      },
      getNumData(state){
         return state.data.length;
     },
     getMaxReached(state){
      if(state.data.length >= state.max_data_points){
         return true;
      } else {
         return false;
      }
   },
     getDataSets(state){
         let datasets = helpers.GetDataSets(state.data);
         return datasets;
     },
     getStartTime(state){
      return state.start_time;
     },
     getCurrentTime(state){
      return state.currentTime;
     },
     getCurrentPosition(state){
      return state.currentPos;
     },
     getCurrentAcceleration(state){
      return state.currentAcc;
     },
     getCurrentGyro(state){
      return state.currentGyro;
     }
   }
      
}

export const helpers = {
   GetDataSets(data) {
      let datasets = [];   //array of arrays storing each separate dataset
      let next_dataset = [];
      let current_index = 0;
      data.forEach(d => {

         if(d.set == current_index + 1){
            datasets.push(next_dataset);
            current_index += 1;

            next_dataset = [];
         }

         next_dataset.push(d);
      })

      if(next_dataset.length > 0){
         datasets.push(next_dataset);
      }
      
      return datasets;
   }
}

export default dataStore