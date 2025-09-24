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
							<li><a class="dropdown-item" id="stopped-mode-select" aria-label="stopped mode" @click="setModeAndCommandStop(); hideCurrentFrequency = true">stopped</a></li>
							<li><a class="dropdown-item" id="undriven-mode-select" aria-label="undriven mode" @click="setModeUndriven"><u>u</u>ndriven</a></li>
							<li><a class="dropdown-item" id="driven-mode-select" aria-label="driven mode" @click="setModeDriven"><u>d</u>riven</a></li>
					
						</ul>
					</div>
				</div>

				<div class="mb-lg-2">
					<button id="stop-motor-button" v-if='getCurrentMode != "stopped"' class="button-sm button-danger" aria-label="exit mode" @click="setModeAndCommandStop(); hideCurrentFrequency = true">E<u>x</u>it mode</button>
				</div>
			</div>
		</div>

		<div class="col-lg-3 align-content-top mt-lg-3">
			<!-- STOPPED mode command options -->
			<!-- <button id="stop-stream-button" v-if='getCurrentMode == "stopped"' class="button-sm button-primary" aria-label="stop data streaming" @click="sendCommandStopStream">Stop Stream</button>
			<button id="start-stream-button" v-if='getCurrentMode == "stopped"' class="button-sm button-primary" aria-label="start data streaming" @click="sendCommandStartStream">Start Stream</button> -->
			<button id="home-button" v-if='getCurrentMode == "stopped"' class="button-lg button-primary" aria-label="return to home position" @click="sendCommandHome"><u>H</u>ome</button>
			<!-- <button id="cal-button" v-if='getCurrentMode == "stopped"' class="button-lg button-primary" aria-label="calibrate stepper position" @click="sendCommandCalibrate">Cal</button> -->

			<!-- UNDRIVEN mode command options -->
			<button id="undriven-ping-button" v-if='getCurrentMode == "undriven"' class="button-lg button-primary" aria-label="start an undriven oscillation" @click="sendCommandPing">Pi<u>n</u>g</button>
			
			<!-- DRIVEN mode command options -->
			<!-- <button id="driven-start-button" v-if='getCurrentMode == "driven"' class="button-sm button-primary" aria-label="start a driven oscillation" @click="sendCommandStart">Start Driving</button> -->
			<button id="driven-stop-button" v-if='getCurrentMode == "driven"' class="button-lg button-danger" aria-label="stop a driven oscillation" @click="sendCommandStop(); hideCurrentFrequency = true"><u>S</u>top</button>
		</div>

		<div class="col-lg-6 align-content-top">
			<!-- DRIVEN mode settings -->
			<div v-if='getCurrentMode == "driven"' class="d-flex flex-column align-items-center justify-content-end">
				
				<label id="driving-frequency-slider-label" for="driving-frequency-slider">Set to: {{ driving_frequency.toFixed(2) }}Hz</label>
				<label v-if="hideCurrentFrequency == false" id="driving-frequency-slider-label" for="driving-frequency-slider">Current: {{ getReportedDrivingFrequencyHz.toFixed(2) }}Hz</label>
				<label v-else id="driving-frequency-slider-label" for="driving-frequency-slider">Current: 0.00 Hz</label>

				<div class="d-flex flex-row">
					<input class="ms-2" type="range" :min="getDrivingFrequencyMin" :max="getDrivingFrequencyMax" :step="getDrivingFrequencyStep" v-model="driving_frequency" id="driving-frequency-slider" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
					<button id="update-driving-frequency-button" class="button-lg button-primary" aria-label="update driving frequency" @click="updateDrivingFrequency(); hideCurrentFrequency = false"><u>R</u>un</button>
				</div>
				
				<div class="d-flex flex-row align-items-center">
					<label id="frequency-step-negative-01-label" for="frequency-step-negative-01">-0.1Hz</label>
					<button id="frequency-step-negative-01" class="button-sm button-primary" aria-label="reduce frequency by 0.1 hertz" @click="driving_frequency -= 0.1">&nbsp; - &nbsp;</button>
					<button id="frequency-step-positive-01" class="button-sm button-primary" aria-label="increase frequency by 0.1 hertz" @click="driving_frequency += 0.1">&nbsp; + &nbsp;</button>
					<label id="frequency-step-positive-01-label" for="frequency-step-positive-01">+0.1Hz</label>
				</div>

				<div class="d-flex flex-row align-items-center">
					<label id="frequency-step-negative-001-label" for="frequency-step-negative-001">-0.01Hz</label>
					<button id="frequency-step-negative-001" class="button-sm button-primary" aria-label="reduce frequency by 0.01 hertz" @click="driving_frequency -= 0.01">&nbsp; - &nbsp;</button>
					<button id="frequency-step-positive-001" class="button-sm button-primary" aria-label="increase frequency by 0.01 hertz" @click="driving_frequency += 0.01">&nbsp; + &nbsp;</button>
					<label id="frequency-step-positive-001-label" for="frequency-step-positive-001">+0.01Hz</label>
				</div>
				
			</div>

			<!-- STOPPED mode settings -->
			<!-- <div class="d-flex flex-row align-items-center justify-content-end">
				<label id="sampling-rate-slider-label" for="sampling-rate-slider">Sampling rate ({{ sampling_rate }}Hz)</label>
				<input class="ms-2" type="range" :min="getSamplingRateMin" :max="getSamplingRateMax" :step="getSamplingRateStep" v-model="sampling_rate" id="sampling-rate-slider" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
			</div> -->
		
		</div>

	</div>

	<!-- TEMP FOR DEBUGGING -->
	<!-- <div><button id="force-reconnect-button" class="button-sm button-primary" aria-label="force a reconnect" @click="mockDroppedStreams">RECONNECT</button></div> -->

	<div class="d-flex flex-row">
		<popup-help class="me-2" id="popup-help-control-panel">
            <template v-slot:header>
                <h5> Control Panel Help </h5>
            </template>
            <template v-slot:body>
                
				<p>Dynamics lab has two modes of operation <b>undriven</b> and <b>driven</b>.</p>

				<p>In <b>undriven</b> mode, the <b>ping</b> function will displace and then release the mass which will undergo 
				damped oscillations that will decay until the mass returns to its equilibrium position.</p>
			
				<p>In <b>driven</b> mode, select a drive frequency using the slider and click <b>Run</b> to cause the unbalanced mass 
				to rotate at the set frequency. To update the driving frequency, first select a new frequency on the slider and then click <b>Run</b> 
				again. <b>Stop</b> will stop the rotation.</p>
			
				<p>Driven and undriven modes can also be selected using the <b>d</b> and <b>u</b> keyboard keys respectively. Once in undriven mode <b>n</b> 
				can be used to run the ping function. In driven mode, <b>s</b> and <b>r</b> can be used to stop and run respectively. <b>x</b> will exit any mode. </p>
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
			error:'',					//for sending errors to screen
			hideCurrentFrequency: true,
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
			'sendCommandStop',
			'sendCommandStart',
			'sendCommandPing',
			'sendCommandStartStream',
			'sendCommandStopStream',
			'sendCommandCalibrate',
			'sendCommandHome',
			'sendCommandUpdateSampleRate',
			'updateDrivingFrequencyHz',
			'setReportedDrivingFrequency',
			'sendCommandUpdateDrivingFrequencyHz',
			'setCurrentTime',
			'setCurrentPosition',
			'setCurrentAcceleration',
			'setCurrentGyro'
		]),
		setModeAndCommandStop(){
			this.sendCommandStop();
			this.setModeStop();
		},
		updateDrivingFrequency(){
			this.sendCommandUpdateDrivingFrequencyHz();
			setTimeout(() => {
				this.sendCommandStart();
			}, 100);
		},
		hotkey(event){
			if(event.key == "s"){
				this.sendCommandStop();
				this.hideCurrentFrequency = true;
			} else if(event.key == "r" & this.getCurrentMode == 'driven'){
				this.updateDrivingFrequency();
				this.hideCurrentFrequency = false;
			} else if(event.key == "o"){
				this.setIsRecording(true);
			} else if(event.key == "p"){
				this.setIsRecording(false);
			} else if(event.key == "u"){
				this.setModeUndriven();
			} else if(event.key == "d"){
				this.setModeDriven();
			} else if(event.key == "n" & this.getCurrentMode == 'undriven'){
				this.sendCommandPing();
			} else if(event.key == "h" & this.getCurrentMode == 'stopped'){
				this.sendCommandHome();
			} else if(event.key == "x" & this.getCurrentMode != 'stopped'){
				this.setModeAndCommandStop();
				this.hideCurrentFrequency = true;
			}
		},
		clearMessages(){
			this.message = '';
			this.error = '';
		},
		connect(){

			let _store = this.$store;
			let _this = this;

			if(this.dataSocket != null){
				this.dataSocket.disconnect();		//must disconnect from previous websocket connection or will duplicate data.
			}
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
				//this.sendCommandUpdateSampleRate(this.getSamplingRate);
			};

			this.dataSocket.onmessage = (event) => {
				try {
					var obj = JSON.parse(event.data);
					if(obj.error){
						console.log(obj.error)
					}
					else if(obj.payload){
						let msgTime = obj.timestamp;		//int
						let time = obj.payload.meta.time;			//int array
						//let state = obj.payload.state		//string
						let pos = obj.payload.encode.pos;		//float array pos in degrees
						let acc = obj.payload.mpu.acc;		//object
						//let gyro = obj.payload.mpu.gyro;	//object

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
							_this.setCurrentTime(time);
							_this.setCurrentPosition(pos);
							_this.setCurrentAcceleration(acc);
							//_this.setCurrentGyro(gyro);

							for(let i=0; i<time.length;i++){
								series_position.append(time[i] + thisDelay, pos[i]);
								series_acceleration.append(time[i] + thisDelay, acc.x[i]);	//only streaming x axis data
							}
							
						}
					}
				} catch (e) {
					if(debug){
						console.log(e)
					}
					
				}
			}

		//_store.dispatch('setStartTime', new Date().getTime());
		window.addEventListener('keydown', this.hotkey, false);
		//window.addEventListener('pagehide', this.setModeAndCommandStop());				//closing window
		//window.addEventListener('beforeunload', this.setModeAndCommandStop());			//refreshing page, changing URL
		
		
		},
		// mockDroppedStreams(){
		// 	var reconnectEvent = new Event("streams:dropped");
        // 	document.dispatchEvent(reconnectEvent);
		// }

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