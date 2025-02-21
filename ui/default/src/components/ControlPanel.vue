<template>

<div class='container-fluid practable-component'>
	<div class="m-2">
		<div class='panel-body'>{{message}}</div>
		<div :class='getErrorClass'><h3>{{ error }}</h3></div>
	</div>

	<div class="d-flex flex-column-reverse flex-lg-row">
		<div class="col-lg-3">
			<div class="d-flex flex-column">
				<div class="mb-lg-2">
					<label for="hardware-select-dropdown">Select Hardware Mode</label>
					<div class="dropdown">
						<button class="button-sm button-dropdown dropdown-toggle" type="button" id="hardware-select-dropdown" data-bs-toggle="dropdown" aria-expanded="false">
							{{ getCurrentMode }}
						</button>
						<ul class="dropdown-menu" aria-labelledby="hardware-dropdown-menu">
							<li><a class="dropdown-item" id="stopped-mode-select" aria-label="stopped mode" @click="setModeStop">stopped</a></li>
							<li><a class="dropdown-item" id="undriven-mode-select" aria-label="undriven mode" @click="setModeUndriven">undriven</a></li>
							<li><a class="dropdown-item" id="driven-mode-select" aria-label="driven mode" @click="setModeDriven">driven</a></li>
					
						</ul>
					</div>
				</div>

				<div class="mb-lg-2">
					<button id="stop-motor-button" v-if='getCurrentMode != "stopped"' class="button-sm button-danger" aria-label="exit mode" @click="setModeStop">Exit mode</button>
				</div>
			</div>
		</div>

		<div class="col-lg-3">
			<!-- STOPPED mode command options -->
			<button id="stop-stream-button" v-if='getCurrentMode == "stopped"' class="button-sm button-primary" aria-label="stop data streaming" @click="sendCommandStopStream">Stop Stream</button>
			<button id="start-stream-button" v-if='getCurrentMode == "stopped"' class="button-sm button-primary" aria-label="start data streaming" @click="sendCommandStartStream">Start Stream</button>
			<button id="calibrate-button" v-if='getCurrentMode == "stopped"' class="button-sm button-primary" aria-label="calibrate zero position" @click="sendCommandCalibrate">Zero</button>
			
			<!-- UNDRIVEN mode command options -->
			<button id="undriven-ping-button" v-if='getCurrentMode == "undriven"' class="button-sm button-primary" aria-label="start an undriven oscillation" @click="sendCommandPing">Ping</button>
			
			<!-- DRIVEN mode command options -->
			<button id="driven-start-button" v-if='getCurrentMode == "driven"' class="button-sm button-primary" aria-label="start a driven oscillation" @click="sendCommandStart">Start Driving</button>
			<button id="driven-stop-button" v-if='getCurrentMode == "driven"' class="button-sm button-primary" aria-label="stop a driven oscillation" @click="setModeStop">Stop Driving</button>
		</div>

		<div class="col-lg-6 align-content-top">
			<!-- DRIVEN mode settings -->
			<div v-if='getCurrentMode == "driven"' class="d-flex flex-row align-items-center justify-content-end">
				<div class="d-flex flex-column">
					<label id="driving-frequency-slider-label" for="driving-frequency-slider">Set to: ({{ driving_frequency }}Hz)</label>
					<label id="driving-frequency-slider-label" for="driving-frequency-slider">Current ({{ getReportedDrivingFrequencyHz }}Hz)</label>
				</div>
				<input class="ms-2" type="range" :min="getDrivingFrequencyMin" :max="getDrivingFrequencyMax" :step="getDrivingFrequencyStep" v-model="driving_frequency" id="driving-frequency-slider" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
				<button id="update-driving-frequency-button" class="button-sm button-primary" aria-label="update driving frequency" @click="updateDrivingFrequency">Run</button>
			</div>

			<!-- STOPPED mode settings -->
			<div class="d-flex flex-row align-items-center justify-content-end">
				<label id="sampling-rate-slider-label" for="sampling-rate-slider">Sampling rate ({{ sampling_rate }}Hz)</label>
				<input class="ms-2" type="range" :min="getSamplingRateMin" :max="getSamplingRateMax" :step="getSamplingRateStep" v-model="sampling_rate" id="sampling-rate-slider" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
			</div>
		
		</div>

	</div>

	<div class="d-flex flex-row">
		<popup-help class="me-2" id="popup-help-control-panel">
            <template v-slot:header>
                <h5> Control Panel Help </h5>
            </template>
            <template v-slot:body>
                

                //to fill in

            </template>
        </popup-help>
	</div>
	

</div>

</template>

<script>

import { SmoothieChart } from 'smoothie';
import { TimeSeries } from 'smoothie';
import { mapActions, mapGetters } from 'vuex';
import PopupHelp from './elements/PopupHelp.vue';

export default {
	name: "ControlPanel",
	props:{
		url: String,
	},
	emits:['toggledraggable'],
	components:{
		PopupHelp
	},
    data(){
        return{
			dataSocket: null,
			message: '',				//for sending user messages to screen
			error:''					//for sending errors to screen
        }
    },
	created(){
		
	},
	mounted(){
		
	},
	computed: {
		...mapGetters([
			'getDataURLObtained',
			'getSessionExpired',
			'getMaxReached',
			'getChartAcceleration',
			'getChartPosition',
			'getCanvasAcceleration',
			'getCanvasPosition',
			'getAccAbs',
			'getPosAbs',
			'getCurrentTime',
			'getCurrentPosition',
			'getCurrentAcceleration',
			'getCurrentGyro',
			'getCurrentMode',
			'getSamplingRateMax',
			'getSamplingRateMin',
			'getSamplingRateStep',
			'getSamplingRate',
			'getDrivingFrequencyMin',
			'getDrivingFrequencyMax',
			'getDrivingFrequencyStep',
			'getDrivingFrequencyHz',
			'getReportedDrivingFrequencyHz'
		]),
		getDataSocket(){
			return this.dataSocket;
		},
		getErrorClass(){
			if(this.error == ''){
				return ""
			} else {
				return "error-message panel-body border border-danger";
			}
		},
		driving_frequency: {
			get(){
				return this.getDrivingFrequencyHz;
			},
			set(val){
				this.updateDrivingFrequencyHz(val);
			}
		},
		sampling_rate:{
			get(){
				return this.getSamplingRate;
			},
			set(val){
				this.sendCommandUpdateSampleRate(val);
			}
		},
		smoothie_y_max_acc: {
			get(){
				return this.$store.getters.getYMaxAcc;
			},
			set(val){
				this.$store.dispatch('setYMaxAcc', val);
			}
		},
		smoothie_y_max_pos: {
			get(){
				return this.$store.getters.getYMaxPos;
			},
			set(val){
				this.$store.dispatch('setYMaxPos', val);
			}
		},
		smoothie_y_min_acc: {
			get(){
				return this.$store.getters.getYMinAcc;
			},
			set(val){
				this.$store.dispatch('setYMinAcc', val);
			}
		},
		smoothie_y_min_pos: {
			get(){
				return this.$store.getters.getYMinPos;
			},
			set(val){
				this.$store.dispatch('setYMinPos', val);
			}
		},
		smoothie_millis_per_pixel: {
			get(){
				return this.$store.getters.getMillisPerPixel;
			},
			set(val){
				this.$store.dispatch('setMillisPerPixel', val);
			}
		}
		
	},
	watch:{
        url(){
			try{
				if(this.url != '' && this.getDataURLObtained){
					console.log('connected to websocket');
					this.connect();								
				} else{
					console.log('disconnecting');
				}
				
			} catch(e){
				console.log(e);
			}
			
			
		},
		getSessionExpired(exp){
			if(exp){
				this.setModeStop();
			}
		},
		getMaxReached(reached){
			if(reached){
				this.error = 'Max data points reached, recording automatically stopped'
			} else{
				this.error = '';
			}
		}
    },
	methods:{
		...mapActions([
			'setCurrentMode',
			'setDraggable',
            'setIsRecording',
			'setChartAcceleration',
			'setChartPosition',
			'setCanvasAcceleration',
			'setCanvasPosition',
			'setModeStop',
			'setModeDriven',
			'setModeUndriven',
			'sendCommandStart',
			'sendCommandPing',
			'sendCommandStartStream',
			'sendCommandStopStream',
			'sendCommandCalibrate',
			'sendCommandUpdateSampleRate',
			'updateDrivingFrequencyHz',
			'setReportedDrivingFrequency',
			'sendCommandUpdateDrivingFrequencyHz',
			'setCurrentTime',
			'setCurrentPosition',
			'setCurrentAcceleration',
			'setCurrentGyro'
		]),
		updateDrivingFrequency(){
			this.sendCommandUpdateDrivingFrequencyHz();
			setTimeout(() => {
				this.sendCommandStart();
			}, 100);
		},
		hotkey(event){
			if(event.key == "s"){
				this.setModeStop();
			} 
		},
		clearMessages(){
			this.message = '';
			this.error = '';
		},
		connect(){

			let _store = this.$store;
			let _this = this;

			this.dataSocket = new WebSocket(this.url);
			_store.dispatch('setDataSocket', this.dataSocket);
			var delay = 0
			let delay_sum = 0;
			var messageCount = 0
			let a;
			let b;
			let debug = false;
			var initialSamplingCount = 1200 // 2 mins at 10Hz, 1200
			var delayWeightingFactor = 30  // 
			let responsiveSmoothie = true;
			//let thisTime;
			
			var chart_acceleration = new SmoothieChart({responsive: responsiveSmoothie, millisPerPixel:_this.smoothie_millis_per_pixel,grid:{fillStyle:'#eeeeee'},maxValue:_this.smoothie_y_max_acc,minValue:_this.smoothie_y_min_acc, interpolation:"linear",labels:{fillStyle:'#000000',precision:2}});
			let canvas_acceleration = document.getElementById("smoothie-chart-acceleration");
			let series_acceleration = new TimeSeries();
			chart_acceleration.addTimeSeries(series_acceleration, {lineWidth:2,strokeStyle:'#000000'});
			chart_acceleration.streamTo(canvas_acceleration, 0);
			_this.setChartAcceleration(chart_acceleration);
			_this.setCanvasAcceleration(canvas_acceleration);

			var chart_position = new SmoothieChart({responsive: responsiveSmoothie, millisPerPixel:_this.smoothie_millis_per_pixel,grid:{fillStyle:'#eeeeee'}, maxValue:_this.smoothie_y_max_pos,minValue:_this.smoothie_y_min_pos, interpolation:"linear",labels:{fillStyle:'#000000',precision:2}});
			let canvas_position = document.getElementById("smoothie-chart-position");
			let series_position = new TimeSeries();
			chart_position.addTimeSeries(series_position, {lineWidth:2,strokeStyle:'#000000'});
			chart_position.streamTo(canvas_position, 0);
			_this.setChartPosition(chart_position);
			_this.setCanvasPosition(canvas_position);

			this.dataSocket.onopen = () => {
				console.log('data connection opened');
				this.sendCommandUpdateSampleRate(this.getSamplingRate);
			};

			this.dataSocket.onmessage = (event) => {
				try {
					var obj = JSON.parse(event.data);
					if(obj.error){
						console.log(obj.error)
					}
					else if(obj.payload){
						let msgTime = obj.timestamp;		//int
						//let state = obj.payload.state		//string
						let pos = obj.payload.encode;		//object
						let acc = obj.payload.mpu.acc;		//object
						let gyro = obj.payload.mpu.gyro;	//object

						_this.setReportedDrivingFrequency(obj.payload.step);

						let d_msgTime = parseFloat(msgTime);
						var thisDelay = new Date().getTime() - d_msgTime;

						if(messageCount == 0){
							delay = thisDelay
							delay_sum += thisDelay;
						} else{
							if(!isNaN(thisDelay)){
								delay_sum += thisDelay;
								delay = delay_sum / (messageCount + 1);
							} else{
								delay_sum += delay;
								delay = delay_sum / (messageCount + 1);
								
							}
							
						}

						a = 1 / delayWeightingFactor
						b = 1 - a

						if (messageCount < initialSamplingCount) {
							thisDelay = ((delay * messageCount) + thisDelay) / (messageCount + 1)
						} else {
							thisDelay = (delay * b) + (thisDelay * a)
						}
			
						messageCount += 1

						if(!isNaN(d_msgTime)){
							_this.setCurrentTime(msgTime);

							_this.setCurrentPosition(pos);
							series_position.append(msgTime + thisDelay, pos.pos);

							_this.setCurrentAcceleration(acc);
							series_acceleration.append(msgTime + thisDelay, acc.x);

							_this.setCurrentGyro(gyro);
							}
					}
				} catch (e) {
					if(debug){
						console.log(e)
					}
					
				}
			}

		_store.dispatch('setStartTime', new Date().getTime());
		window.addEventListener('keydown', this.hotkey, false);
		//window.addEventListener('pagehide', this.setModeStop);				//closing window
		//window.addEventListener('beforeunload', this.setModeStop);			//refreshing page, changing URL
		
		
		},

	},
}




</script>

<style scoped>

.error-message{
	color: red;
	text-decoration: bold;
	border: thin;
	box-shadow: 0px 0px;
}

.error{
    /* border:thick solid red */
	border: auto;
}

.error:focus{
    /* border:thick solid red */
	border: auto;
}

/* #smoothie-chart_omega{
	width:100%;
	height: 120px;
}

#smoothie-chart_theta{
	width:100%;
	height: 120px;
} */

.sliderlabel{ text-align: left;}

select{
    color: white;
    padding-top: 5px;
    padding-bottom: 5px;
    background-color: #4490d8;
}

</style>