<template>

<div v-if='getSessionExpired' class='session-end-root'>
  <div class="media-frame">
    <div class='media-inner'>
        <img id='session-end-image' src='https://app.practable.io/ed0/static/images/common/thank-you-screen.svg' alt='session ended'>
    </div>
  </div>
  
</div>
<div v-else class="video-element-root">
  <div class='media-frame'>
      <div class='media-inner'>
        <canvas id="video-canvas"></canvas>
      </div>
    </div>
    
</div>  

</template>

<script>
import JSMpeg from "@cycjimmy/jsmpeg-player";
import { mapGetters } from 'vuex';


export default {
  name: "VideoElement",
  props: ["url"],
  data(){
        return{
          player: null,
        }
    },
    components:{
    
    },
    computed:{
      ...mapGetters([
        'getSessionExpired'
      ]),
    },
    watch:{
        url:{
            // let canvas = document.getElementById("video-canvas");
            // this.player = new JSMpeg.Player(this.url, {canvas: canvas, preserveDrawingBuffer: true});
           immediate: true,
            handler(newUrl, oldUrl) {
              if (newUrl === oldUrl) return;
              this.destroyPlayer();
              if (!newUrl) return;                  
              this.createPlayer(newUrl);
            }
         }
    },
    created(){
  
    },
    beforeUnmount() { 
      this.destroyPlayer();
    },
    mounted() {
    
    },
    methods:{
      destroyPlayer() {
        if (!this.player) return;
        try {
          this.player.destroy();
        } catch (e) {
          console.warn('player destroy failed', e);
        }
        this.player = null;
      },
       createPlayer(url) {
        this.destroyPlayer();
        let canvas = document.getElementById("video-canvas");
        if (!canvas || !url) return;

        this.player = new JSMpeg.Player(this.url, {
          canvas: canvas, 
          preserveDrawingBuffer: true
        });
      }
  }
};


</script>

<style scoped>
.video-element-root,
.session-end-root {
  height: 100%;
  min-height: 0;
  display: flex;
  flex-direction: column;
}

.media-frame {
  position: relative;   /* containing block for .media-inner */
  flex: 1 1 auto;
  min-height: 0;
  overflow: hidden;
}

.media-inner {
  position: absolute;
  inset: 0;             /* definite height, no percentage involved */
}

#session-end-image {
  display: block;
  width: 100%;
  height: auto;
  max-width: 100%;
  max-height: 100%;     /* now resolves — .media-inner is definite */
}

#video-canvas {
  position: absolute;
  inset: 0;
  width: 100%;
  height: 100%;
  object-fit: contain;
  object-position: center;
}
 </style>