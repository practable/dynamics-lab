<template>
<div class='container-fluid practable-component'>
	<div class='d-flex flex-column' id="video">
		
		<div class="d-flex flex-row text-center">
			<canvas class="" id="smoothie-chart-acceleration"></canvas>
			<p class="rotate-90">acc [g]</p>
		</div>
		
		<div class="d-flex flex-row text-center">
			<canvas class="" id="smoothie-chart-position"></canvas>
			<p class="rotate-90">pos [<sup>o</sup>]</p>
		</div>
		

		<video-element :url="url" />

		<div class="d-flex flex-row toolbar-bottom">
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

export default {
	name: "WebcamStream",
	components:{
		VideoElement,
		DownloadImageButton,
		OptionsTool
	},
    data(){
        return{
			// player: null,
			stream: Object,
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
	mounted(){
		var _this = this;
		var reconnect = function () {
			_this.accessVideo();
		};
		//make second and subsequent connections
		document.addEventListener("streams:dropped", reconnect);
	},
	methods:{
		...mapActions([
			'setDraggable',
			'resetSmoothieSettings'
		]),
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
#video-canvas{
	width:80%;
	height: 100%;
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