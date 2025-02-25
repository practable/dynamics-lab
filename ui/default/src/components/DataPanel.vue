//Vue3 update

<template>
  <div class="d-flex flex-column">
    <!-- <div>Average angular position = {{ avgPos }} <sup>o</sup></div>
    <div>Average Acceleration = {{ avgAcc }} g</div>
    <div>Average Rotational Speed = {{ avgGyro }} rads<sup>-1</sup></div> -->

    <div>Max (abs) Acc. X = {{ maxAcc.x.toFixed(2) }} g</div>
    <div>Max (abs) Acc. Y = {{ maxAcc.y.toFixed(2) }} g</div>
    <div>Max (abs) Acc. Z = {{ maxAcc.z.toFixed(2) }} g</div>
  </div>
    
</template>

<script>

import { mapGetters} from 'vuex'

export default {

  name: 'DataPanel',
  data () {
    return {
      avgPos: 0,
      avgAcc: {x:0,y:0,z:0},
      avgGyro: {x:0,y:0,z:0},
      maxAcc: {x:0,y:0,z:0},
      maxGyro: {x:0,y:0,z:0},
    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
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
      getNumData(){
        let data = this.getData;
        let n = data.length;
        if(n == 0){
          return
        } 
        else if(n == 1){
          this.avgPos = data[0].pos;
          this.avgAcc = data[0].acc;
          this.maxAcc = data[0].acc;
          this.avgGyro = data[0].gyro;
          this.maxGyro = data[0].gyro;
        } 
        else{
          this.avgPos = this.cumulativeAverage(data[n - 1].pos, this.avgPos, n);
          this.avgAcc = this.calcNewAverageAccelerations(data[n - 1].acc, this.avgAcc, n);
          this.avgGyro = this.calcNewAverageGyros(data[n - 1].gyro, this.avgGyro, n);

          this.maxAcc.x = this.getBiggerValue(data[n - 1].acc.x, this.maxAcc.x)
          this.maxAcc.y = this.getBiggerValue(data[n - 1].acc.y, this.maxAcc.y)
          this.maxAcc.z = this.getBiggerValue(data[n - 1].acc.z, this.maxAcc.z)

          this.maxGyro.x = this.getBiggerValue(data[n - 1].gyro.x, this.maxGyro.x)
          this.maxGyro.y = this.getBiggerValue(data[n - 1].gyro.y, this.maxGyro.y)
          this.maxGyro.z = this.getBiggerValue(data[n - 1].gyro.z, this.maxGyro.z)
        }
      }
  },
  created(){
      
  },
  mounted(){

  },
  methods: {
    calcNewAverageAccelerations(new_vals, old_vals, old_count){
      let average = {x:0,y:0,z:0}
      for (let key in old_vals){
          average[key] = this.cumulativeAverage(new_vals[key], old_vals[key], old_count)
      }

      return average;
    },
    calcNewAverageGyros(new_vals, old_vals, old_count){
      let average = {x:0,y:0,z:0}
      for (let key in old_vals){
        average[key] = this.cumulativeAverage(new_vals[key], old_vals[key], old_count)
      }
      return average;
    },
    cumulativeAverage(new_val, old_val, old_count){
        return (old_val*old_count + new_val)/(old_count+1)
    },
    getBiggerValue(new_value, current_value){
      if(Math.abs(new_value) >= current_value){
        return Math.abs(new_value)
      } else{
        return current_value
      }
    },
    rootMeanSquare(peak){
      return (1/Math.sqrt(2))*peak;
    }
      
  }
}
</script>

<style scoped>


</style>