<template>

 <div class="container-fluid practable-component webcam-stream" id="video">
	<div class="d-flex flex-row text-center">
			<canvas class="" id="smoothie-chart-acceleration"></canvas>
			<p class="rotate-90">acc [g]</p>
		</div>
		
		<div class="d-flex flex-row text-center">
			<canvas class="" id="smoothie-chart-position"></canvas>
			<p class="rotate-90">pos [<sup>o</sup>]</p>
		</div>
		

		<video-element :url="url" />

		<div class="d-flex flex-row sticky-wrap">
			<div class="toolbar-bottom">
			<download-image-button class="me-2" id="download-image-webcam" parentCanvasID="video-canvas" parentComponentName="webcam"></download-image-button>
			
			<options-tool id="options-live-stream" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
				<template v-slot:header>
					<h4>Live graph options</h4>
				</template>
				
				<template v-slot:body>
					<div class='row'>
						<div class='col-lg-12'>
							<div class="d-flex flex-row justify-content-start align-items-center">
								<label class='m-2 flex-fill' for="smoothie_y_max_acc">Acceleration Y<sub>max</sub></label>
								<input type="number" class="flex-grow-1" id="smoothie_y_max_acc" v-model="smoothie_y_max_acc">
							</div>
							<div class="d-flex flex-row justify-content-center align-items-center">
								<label class='m-2 flex-fill' for="smoothie_y_min_acc">Acceleration Y<sub>min</sub></label>
								<input class="flex-grow-1" type="number" id="smoothie_y_min_acc" v-model="smoothie_y_min_acc">
							</div>
							<div class="d-flex flex-row justify-content-start align-items-center">
								<label class='m-2 flex-fill' for="smoothie_y_max_pos">Position Y<sub>max</sub></label>
								<input type="number" class="flex-grow-1" id="smoothie_y_max_pos" v-model="smoothie_y_max_pos">
							</div>
							<div class="d-flex flex-row justify-content-center align-items-center">
								<label class='m-2 flex-fill' for="smoothie_y_min_pos">Position Y<sub>min</sub></label>
								<input class="flex-grow-1" type="number" id="smoothie_y_min_pos" v-model="smoothie_y_min_pos">
							</div>
							<div class="d-flex flex-row justify-content-center align-items-center">
								<label class='m-2 flex-fill' for="smoothie_millis_per_pixel">ms/pixel</label>
								<input type="number" class="flex-grow-1" id="smoothie_millis_per_pixel" v-model="smoothie_millis_per_pixel">
							</div>
						</div>
					</div>
				</template>

				<template v-slot:footer>
					<button class="button-sm button-warning" id="reset-smoothie-options" aria-label="reset smoothie options" @click="resetSmoothieSettings">Reset</button>
				</template>
			</options-tool>

			<popup-help class="ms-2" id="popup-help-streams">
				<template v-slot:header>
					<h5> Streams Help </h5>
				</template>
				<template v-slot:body>
					<p>This component displays live streams of video and data: the acceleration (along x axis, measured in units of g) and angular position 
						of the rotating mass (in degrees) are provided as live data streams, alongside a video feed of the hardware. The live data stream axes can 
						be updated using the options button. The webcam feed can be enlarged or reduced by clicking and dragging (horizontally) on the bottom right corner 
						of this component.
					</p>
				</template>
			</popup-help>
			</div>
		</div>
  </div>

</template>

<script>
import axios from "axios";
import VideoElement from "./VideoElement.vue";
import { mapGetters, mapActions } from 'vuex';
import DownloadImageButton from "./elements/DownloadImageButton.vue";
import OptionsTool from './elements/OptionsTool.vue';
import PopupHelp from "./elements/PopupHelp.vue";

export default {
	name: "WebcamStream",
	components:{
		VideoElement,
		DownloadImageButton,
		OptionsTool,
		PopupHelp
	},
    data(){
        return{
			// player: null,
			stream: {},
        }
    },
    computed:{
		urlOK() {
			return this.$store.getters.getVideoURLObtained;
		},
		streamOK(){			
			return this.$store.getters.getStream("video");
		},
		url(){
			return this.$store.getters.getVideoURL;
			
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
	beforeUnmount() { 
        document.removeEventListener("streams:dropped", this.reconnect);
    },
	mounted(){
		// var _this = this;
		// var reconnect = function () {
		// 	_this.accessVideo();
		// };
		//make second and subsequent connections
		document.addEventListener("streams:dropped", this.reconnect);
	},
	methods:{
		...mapActions([
			'setDraggable',
			'resetSmoothieSettings'
		]),
		reconnect(){
			this.accessVideo();
		},
		accessVideo(){
			this.stream = this.$store.getters.getStream("video");
				var accessURL = this.stream.url;
				var token = this.stream.token;
				var store = this.$store;
				store.dispatch("deleteVideoURL");		////THIS HAS BEEN ADDED
				axios
				.post(accessURL, {}, { headers: { Authorization: token } })
				.then((response) => {
					store.dispatch("setVideoURL", response.data.uri);
				})
				.catch((err) => console.log(err));
		}
	},
	watch:{
		streamOK: function(is) {
			if (is) {
				this.accessVideo();
			} else{
				console.log("no stream");
			}
    },
		// urlOK(is) {
		// 	if (is) {
		// 		console.log("get videoURL", this.urlOK, this.url);
		// 	}
		// },

	}
}


</script>

<style scoped>
.webcam-stream {
  display: flex;
  flex-direction: column;
  min-height: 0;
  padding: 0.5rem;
}

.webcam-stream :deep(.video-element-root),
.webcam-stream :deep(.session-end-root) {
  flex: 1 1 auto;
  min-height: 0;
}

#smoothie-chart-acceleration{
	width:100%;
	height: 60px;
}

#smoothie-chart-position{
	width:100%;
	height: 60px;
}

.rotate-90{
	transform: rotate(-90deg);
}
</style>