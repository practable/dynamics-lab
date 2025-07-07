//Vue3 updated
//Streamlined data to not include alternative units

<template>
<div class='m-2 p-2 practable-component'>
    <div class="d-grid gap-2 d-sm-block">
        <button type='button' class="button-xsm button-primary" aria-label="record" v-if="!getIsRecording" id="record-data-button" @click='$store.dispatch("setIsRecording", true)'>Rec<u>o</u>rd</button>
        <button type='button' class="button-xsm button-danger" aria-label="stop" v-if="getIsRecording" id="stop-data-button" @click='$store.dispatch("setIsRecording", false)'>Sto<u>p</u></button>
        <button type='button' class="button-xsm button-warning" aria-label="reset" id="reset-data-button" @click="toggleResetModal">Reset</button>
        <button type='button' class="button-xsm button-primary" aria-label="download csv" v-if="hasData" id="download-data-button" @click="outputToCSV">Download CSV</button>
    </div>
    <div class='row m-2 justify-content-center'>
      <div v-if='getIsRecording' class='col-2'>
        <img id='red-light' src='/images/red-light.png' width='20' height='20' :hidden='!showRedLight'>
      </div>
      <div class='col-10'>
        <p class='m-1'>Recorded: {{getNumData}}/{{getMaxDataPoints}} data points</p>
      </div>
    </div>

  <transition name='fade'>
    <div v-if='showResetConfirmModal' class="modal modal-show" tabindex="-1" role="dialog">
        <div class="modal-dialog modal-dialog-centered modal-dialog-scrollable" role="document">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Reset Data</h5>
              <button type="button" class="btn btn-close" data-bs-dismiss="modal" aria-label="Close reset modal" @click='toggleResetModal'>
                
              </button>
            </div>
            <div class="modal-body">
              <p>Are you sure you want to Reset? This will clear all stored data.</p>
            </div>
            <div class="modal-footer">
              <button type="button" class="button-xsm button-danger" id='resetmodalclear' aria-label="reset" @click="clearData(); toggleResetModal();">Reset</button>
              <button type="button" class="button-xsm button-secondary" data-dismiss="modal" data-bs-dismiss="modal" aria-label="cancel reset" @click="toggleResetModal">Cancel</button>
            </div>
          </div>
        </div>
      </div>
    </transition>

    <div class="d-flex flex-row">
		<popup-help class="me-2" id="popup-help-data-recorder">
            <template v-slot:header>
                <h5> Data Recorder Help </h5>
            </template>
            <template v-slot:body>  
                <p>Click record or press <b>o</b> on the keyboard to start recording.</p>
                <p>Click stop or press <b>p</b> on the keyboard to stop recording.</p>
                <p>Clicking <b>Reset</b> will delete all recorded data, including clearing the graph component.</p>
                <p>If you want to save data prior to resetting then click <b>Download CSV</b>
                to save the data to your local computer.</p>  
                <p>A maximum of 5000 data points can be recorded for download and 2000 for display in the graph component. It is recommended to perform
                  runs in batches and download data in between.
                </p>         

            </template>
        </popup-help>
	</div>
</div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';
import PopupHelp from './elements/PopupHelp.vue';

export default {

  name: 'DataRecorder',
  props:{
	
	},
  data () {
    return {
        stopped_recording: false,
        showResetConfirmModal: false,
        //data_set_index: 0,      //moved to dataStore vuex
    }
  },
  components: {
    PopupHelp
  },
  created(){
    
	},
  computed:{
    ...mapGetters([
        'getIsRecording',
        'getCurrentTime',
        'getCurrentMode',
        'getNumData',
        'getMaxDataPoints',
        'getDatasetIndex',
        'getTimeFromStart',
        'getCurrentPosition',
        'getCurrentAcceleration',
        'getCurrentGyro',
        'getReportedDrivingFrequencyHz'
    ]),
      hasData(){
          return this.getNumData !== 0;
      },
      showRedLight(){
        return this.getNumData % 100 > 50 ? true : false;
      }
  },
  watch:{
    getIsRecording(now, prev){
        if(prev && !now){
          this.stopRecording();
        } else if(now){
          this.record();
        }
    },
    getCurrentTime(){
      if(this.getIsRecording && this.getNumData < this.getMaxDataPoints){
        this.plot();
      } else if(this.getNumData == this.getMaxDataPoints && !this.stopped_recording){
          this.stopRecording();
          this.stopped_recording = true;
      }
    },
  },
  methods: {
    ...mapActions([
      'addToDatasetIndex',
      'setDatasetIndex'
    ]),
      record(){
          this.$store.dispatch('setStartTime', this.getCurrentTime);
      },
      stopRecording(){
        //console.log('data recording stopped');
        this.addToDatasetIndex();
      },
      plot(){
        let time = this.getTimeFromStart;     
        let pos = this.getCurrentPosition; 
        let acc = this.getCurrentAcceleration; 
        //let gyro = this.getCurrentGyro; 
        let driving_freq = this.getReportedDrivingFrequencyHz;
        let mode = this.getCurrentMode;
        
        pos.forEach((pos, index) => {
          let data_object = {
              id: this.getNumData,  
              set: this.getDatasetIndex, 
              t: parseFloat(time[index]), 
              pos: parseFloat(pos), 
              acc: {x: parseFloat(acc['x'][index]), y: parseFloat(acc['y'][index]), z: parseFloat(acc['z'][index])}, 
              //gyro: {x: parseFloat(gyro['x'][index]), y: parseFloat(gyro['y'][index]), z: parseFloat(gyro['z'][index])},
              freq: mode == 'driven' ? driving_freq : 0,
              mode: mode,
              showDataPoint: true};

          this.$store.dispatch('addData', data_object);
        });
        
      },
      clearData(){
          this.$store.dispatch('clearAllData');
          this.setDatasetIndex(0);
      },
      toggleResetModal(){
          this.showResetConfirmModal = !this.showResetConfirmModal;
      },
      outputToCSV(){
        let data = this.$store.getters.getData;
        let current_dataset = 0;
        //let csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g], Gyro_x[rad/s], Gyro_y[rad/s], Gyro_z[rad/s]\n';
        let csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g]\n';
        let date = new Date();

        data.forEach(function(d){
            if(d.set == current_dataset + 1){
                let hiddenElement = document.createElement('a');
                hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
                hiddenElement.target = '_blank';
                hiddenElement.download = `dynamics-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
                hiddenElement.click();

                //csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g], Gyro_x[rad/s], Gyro_y[rad/s], Gyro_z[rad/s]\n';
                csv = 'Time[s],Mode,Driving_Freq.[Hz],Pos[deg],Acc_x[g], Acc_y[g], Acc_z[g]\n';
                current_dataset += 1;
              }

              csv += d.t.toString();
              csv += ",";
              csv += d.mode.toString();
              csv += ",";
              csv += d.freq.toFixed(2);
              csv += ",";
              csv += d.pos.toString();
              csv += ',';
              csv += d.acc.x.toString();
              csv += ',';
              csv += d.acc.y.toString();
              csv += ',';
              csv += d.acc.z.toString();
              // csv += ',';
              // csv += d.gyro.x.toString();
              // csv += ',';
              // csv += d.gyro.y.toString();
              // csv += ',';
              // csv += d.gyro.z.toString();
              
              csv += "\n";
        });

        //output the final dataset
        let hiddenElement = document.createElement('a');
        hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
        hiddenElement.target = '_blank';
        hiddenElement.download = `dynamics-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
        hiddenElement.click();
    },
      
  }
}
</script>

<style scoped>

.error{
    border:thick solid red
}

.error:focus{
    border:thick solid red
}

.modal-show{
  display: block;
}

</style>