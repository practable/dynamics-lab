//Vue3 update

<template>
<div class='container-fluid practable-component'>
  <div class="d-flex flex-row">
    <div v-if="getDataSets.length > 0">
      <label class="me-2">Select dataset: </label>
      <div v-for='(dataset, index) in getDataSets' class="form-check form-check-inline">
        <input class="form-check-input" type="radio" name="dataset-select-radio-options" :id="'dataset-select-radio' + index" :value="index" v-model="selectedDataSet">
        <label class="form-check-label" :for="'dataset-select-radio' + index">{{ index }}</label>
      </div>
    </div>
    <div v-else>
      <div class="m-2" style="height: 10px"></div>
    </div>
    
  </div>
 
  <div class="table">
      <div class="row">
          <table>
              <thead class='table-head'>
                <tr>
                      <th scope="col"></th>
                      <!-- <th scope="col">Pos[deg]</th> -->
                      <th scope="col" colspan="3">Acc[g]</th>
                      <th scope="col" colspan="3">Gyro[deg/s]</th>
                  </tr>
                  <tr>
                      <th scope="col"></th>
                      <!-- <th scope="col"></th> -->
                      <th scope="col">x</th>
                      <th scope="col">y</th>
                      <th scope="col">z</th>
                      <th scope="col">x</th>
                      <th scope="col">y</th>
                      <th scope="col">z</th>
                  </tr>
              </thead>
              <tbody>
                <tr v-if="getCurrentPosition[0] != undefined">
                  <td class="ps-2">Current</td>
                  <!-- <td>{{getCurrentPosition.pos.toFixed(3)}}</td> -->
                  <td>{{getCurrentAcceleration.x[0]}}</td>
                  <td>{{getCurrentAcceleration.y[0]}}</td>
                  <td>{{getCurrentAcceleration.z[0]}}</td>
                  <td>{{getCurrentGyro.x[0]}}</td>
                  <td>{{getCurrentGyro.y[0]}}</td>
                  <td>{{getCurrentGyro.z[0]}}</td>
                </tr>

                <tr v-if="getDataSets.length != 0">
                  <td class="ps-2">Abs. Max.</td>
                  <!-- <td>-</td> -->
                  <td>{{ maxAcc.x.toFixed(3) }}</td>
                  <td>{{ maxAcc.y.toFixed(3) }}</td>
                  <td>{{ maxAcc.z.toFixed(3) }}</td>
                  <td>{{ maxGyro.x.toFixed(3) }}</td>
                  <td>{{ maxGyro.y.toFixed(3) }}</td>
                  <td>{{ maxGyro.z.toFixed(3) }}</td>
                </tr>

                <tr v-if="getDataSets.length != 0">
                  <td class="ps-2">RMS Avg.</td>
                  <!-- <td>-</td> -->
                  <td>{{ rmsAcc.x.toFixed(3) }}</td>
                  <td>{{ rmsAcc.y.toFixed(3) }}</td>
                  <td>{{ rmsAcc.z.toFixed(3) }}</td>
                  <td>{{ rmsGyro.x.toFixed(3) }}</td>
                  <td>{{ rmsGyro.y.toFixed(3) }}</td>
                  <td>{{ rmsGyro.z.toFixed(3) }}</td>
                </tr>

              </tbody>        
          </table> 

      </div>
  </div>
</div>
</template>

<script>

import { mapGetters} from 'vuex'

export default {

  name: 'DataPanel',
  data () {
    return {
      selectedDataSet: 0,
      avgPos: 0,
      avgAcc: {x:0,y:0,z:0},
      avgGyro: {x:0,y:0,z:0},
      maxAcc: {x:0,y:0,z:0},
      maxGyro: {x:0,y:0,z:0},
      rmsAcc: {x:0,y:0,z:0},
      rmsGyro: {x:0,y:0,z:0},
    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
        'getIsRecording',
        'getTimeFromStart',
        'getCurrentPosition',
        'getCurrentAcceleration',
        'getCurrentGyro',
        'getData',
        'getDataSets',
        'getNumData'
    ]),
    
  },
  watch:{
      // getNumData(){
      //   let data = this.getData;
      //   let n = data.length;
      //   if(n == 0){
      //     return
      //   } 
      //   else if(n == 1){
      //     this.avgPos = data[0].pos;
      //     this.avgAcc = data[0].acc;
      //     this.maxAcc = data[0].acc;
      //     this.avgGyro = data[0].gyro;
      //     this.maxGyro = data[0].gyro;
      //   } 
      //   else{
      //     this.avgPos = this.cumulativeAverage(data[n - 1].pos, this.avgPos, n);
      //     this.avgAcc = this.calcNewAverageAccelerations(data[n - 1].acc, this.avgAcc, n);
      //     this.avgGyro = this.calcNewAverageGyros(data[n - 1].gyro, this.avgGyro, n);

      //     this.maxAcc.x = this.getBiggerValue(data[n - 1].acc.x, this.maxAcc.x)
      //     this.maxAcc.y = this.getBiggerValue(data[n - 1].acc.y, this.maxAcc.y)
      //     this.maxAcc.z = this.getBiggerValue(data[n - 1].acc.z, this.maxAcc.z)

      //     this.maxGyro.x = this.getBiggerValue(data[n - 1].gyro.x, this.maxGyro.x)
      //     this.maxGyro.y = this.getBiggerValue(data[n - 1].gyro.y, this.maxGyro.y)
      //     this.maxGyro.z = this.getBiggerValue(data[n - 1].gyro.z, this.maxGyro.z)
      //   }
      // }
      getIsRecording(now, then){
        if(then && !now){
          this.updateAllCalculations();
          this.selectedDataSet = this.getDataSets.length - 1;
        } 
      },
      selectedDataSet(now,then){
        this.updateAllCalculations();
      },
      getNumData(n){
        //if data has been reset, then reset the selected dataset to 0 and the average values
        if(n == 0){
          this.selectedDataSet = 0;
          this.avgPos = 0;
          this.avgAcc = {x:0,y:0,z:0};
          this.avgGyro = {x:0,y:0,z:0};
          this.maxAcc = {x:0,y:0,z:0};
          this.maxGyro = {x:0,y:0,z:0};
          this.rmsAcc = {x:0,y:0,z:0};
          this.rmsGyro = {x:0,y:0,z:0};
        }
      }
  },
  created(){
      
  },
  mounted(){

  },
  methods: {
    updateAllCalculations(){
      let data = this.getDataSets[this.selectedDataSet];
      if(data != undefined && data.length > 0){
        this.maxAcc.x = this.getAbsMaxValueFromDataSetWithAxis(data, 'acc', 'x');
        this.maxAcc.y = this.getAbsMaxValueFromDataSetWithAxis(data, 'acc', 'y');
        this.maxAcc.z = this.getAbsMaxValueFromDataSetWithAxis(data, 'acc', 'z');
        this.maxGyro.x = this.getAbsMaxValueFromDataSetWithAxis(data, 'gyro', 'x');
        this.maxGyro.y = this.getAbsMaxValueFromDataSetWithAxis(data, 'gyro', 'y');
        this.maxGyro.z = this.getAbsMaxValueFromDataSetWithAxis(data, 'gyro', 'z');

        this.rmsAcc.x = this.rootMeanSquare(data, 'acc', 'x');
        this.rmsAcc.y = this.rootMeanSquare(data, 'acc', 'y');
        this.rmsAcc.z = this.rootMeanSquare(data, 'acc', 'z');
        this.rmsGyro.x = this.rootMeanSquare(data, 'gyro', 'x');
        this.rmsGyro.y = this.rootMeanSquare(data, 'gyro', 'y');
        this.rmsGyro.z = this.rootMeanSquare(data, 'gyro', 'z');
      }
      
    },
    // calcNewAverageAccelerations(new_vals, old_vals, old_count){
    //   let average = {x:0,y:0,z:0}
    //   for (let key in old_vals){
    //       average[key] = this.cumulativeAverage(new_vals[key], old_vals[key], old_count)
    //   }

    //   return average;
    // },
    // calcNewAverageGyros(new_vals, old_vals, old_count){
    //   let average = {x:0,y:0,z:0}
    //   for (let key in old_vals){
    //     average[key] = this.cumulativeAverage(new_vals[key], old_vals[key], old_count)
    //   }
    //   return average;
    // },
    // cumulativeAverage(new_val, old_val, old_count){
    //     return (old_val*old_count + new_val)/(old_count+1)
    // },
    // getBiggerValue(new_value, current_value){
    //   if(Math.abs(new_value) >= current_value){
    //     return Math.abs(new_value)
    //   } else{
    //     return current_value
    //   }
    // },
    rootMeanSquare(dataset, quantity, axis){
      let square_sum = 0;
      for (let i=0; i<dataset.length;i++){
        let value = dataset[i][quantity][axis];
        square_sum += value*value;
      }

      return Math.sqrt(square_sum/dataset.length);

    },
    getAbsMaxValueFromDataSetWithAxis(dataset, quantity, axis){
      let abs_max_value = -Infinity
      for (let i=0; i<dataset.length;i++){
        if(Math.abs(dataset[i][quantity][axis]) > abs_max_value){
          abs_max_value = Math.abs(dataset[i][quantity][axis]);
        }
      }

      return abs_max_value;

    },
    getAbsMaxValueFromDataSet(dataset, quantity){
      let abs_max_value = -Infinity
      for (let i=0; i<dataset.length;i++){
        if(Math.abs(dataset[i][quantity]) > abs_max_value){
          abs_max_value = Math.abs(dataset[i][quantity]);
        }
      }

      return abs_max_value;

    }
      
  }
}
</script>

<style scoped>
table
{
    table-layout: fixed;
    width: 100%;
}

</style>