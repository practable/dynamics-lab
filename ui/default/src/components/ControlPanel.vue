<template>

<div class='container-fluid practable-component'>
	<div class="m-2">
		<!-- <div v-if='getCurrentMode != ""' class='panel-heading'><h3>Current mode: {{getModeName}}</h3></div> -->
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
							{{ getModeName }}
						</button>
						<ul class="dropdown-menu" aria-labelledby="hardware-dropdown-menu">
							<li><a class="dropdown-item" id="enter-stopped-mode-select" aria-label="stopped mode" @click="setHardwareStop">stopped</a></li>
					
						</ul>
					</div>
				</div>

				<div class="mb-lg-2">
					<button id="stop-motor-button" v-if='getCurrentMode != "stopped"' class="button-sm button-danger" aria-label="exit mode" @click="setHardwareStop">Exit mode</button>
				</div>
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
			error:'',					//for sending errors to screen
			
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
			'getPosAbs'
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
				this.setHardwareStop();
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
			'setHardwareStop'
		]),
		
		hotkey(event){
			if(event.key == "s"){
				this.setHardwareStop();
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
			};

			this.dataSocket.onmessage = (event) => {
				try {
					var obj = JSON.parse(event.data);
					
					if(obj.error){
						console.log(obj.error)
					}
					else if(obj.t){

						let msgTime = obj.t;
						let acc = obj.a;
						let pos = obj.p;

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

						if(!isNaN(d_msgTime) && !isNaN(pos) && !isNaN(acc)){
							_store.dispatch('setCurrentTime', msgTime);

							_store.dispatch('setCurrentPosition', pos);
							series_position.append(msgTime + thisDelay, pos);

							_store.dispatch('setCurrentAcceleration', acc);
							series_acceleration.append(msgTime + thisDelay, acc);
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
		window.addEventListener('pagehide', this.setHardwareStop);				//closing window
		window.addEventListener('beforeunload', this.setHardwareStop);			//refreshing page, changing URL
		
		
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