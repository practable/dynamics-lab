//import Vue from 'vue';

const dataStore = {
   state: () => ({
     data: [],
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
     getDataSets(state){
         let datasets = helpers.GetDataSets(state.data);
         return datasets;

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