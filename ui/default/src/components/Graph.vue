//Vue3
//removed unit choices, since data has been streamlined to not include alternative units

<template>
<div class='practable-component'>
    <div class="d-flex flex-row mb-4 align-items-center toolbar-top">
        <download-image-button class="me-2" id="download-graph-image" parentCanvasID="graph-canvas" parentComponentName="graph"></download-image-button>
        
        <popup-help class="me-2" id="popup-help-graph">
            <template v-slot:header>
                <h5> Graph Help </h5>
            </template>
            <template v-slot:body>
                <div class='row mb-2'>
                    <h5>Recording more than 5000 data points</h5>
                    <p>For smooth running of the web app there is a limit of 5000 recorded data points and 2000 data points on the Graph tool. Please download this dataset or perform the necessary analysis
                        and then reset the data and run again in order to collect more data.
                    </p>
                </div>
                
                <div class='row mb-2'>
                    <h5> Gradient tool </h5>
                    <p> Click and drag on the graph in order to draw a straight line segment. The gradient of this line is displayed above the graph.</p>
                </div>

                <div class='row mb-2'>
                    <h5> Interactive data points </h5>
                    <p> Hover over a graph point to display the corresponding data. Click on a coloured key item to toggle the display of that dataset on the graph.
                    </p>
                </div>

                <div class='row mb-2'>
                    <h5> Function Plotting </h5>
                    <p> Function plotting is available by clicking the toolbar button above the graph. Select the function type from the dropdown menu. Input the function parameters. Angular parameters are in radians. Click plot to display the function.
                        The function is plotted up to the maximum time value currently displayed on the x-axis.
                    </p>
                </div>

                <div class='row mb-2'>
                    <h5> Download </h5>
                    <p> An image of the plotted datasets can be downloaded by clicking the download toolbar button. The datasets themselves can be downloaded as separate CSV files by
                        clicking 'Download CSV' which is available after datasets have been plotted.
                    </p>
                </div>
            </template>
        </popup-help>

        <popup-plotting class="me-2" id="popup-plotting">
            <template v-slot:header>
                <h5>Plot Function</h5>
            </template>
            <template v-slot:body>
               <div id="chart-functions" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
					<div class="dropdown">
						<button class="button-sm button-dropdown dropdown-toggle" type="button" id="function-select-dropdown" data-bs-toggle="dropdown" aria-expanded="false">
							{{ currentFunction }}
						</button>
						<ul class="dropdown-menu" aria-labelledby="function-dropdown-menu">
							<li><a class="dropdown-item" id='select-linear-option' aria-label="select linear function" @click="currentFunction = 'linear'">Linear</a></li>
							<li><a class="dropdown-item" id='select-quadratic-option' aria-label="select quadratic function" @click="currentFunction = 'quadratic'">Quadratic</a></li>
                            <li><a class="dropdown-item" id='select-trigonometric-option' aria-label="select trigonometric function" @click="currentFunction = 'trigonometric'">Trigonometric</a></li>
                            <li><a class="dropdown-item" id='select-exponential-option' aria-label="select exponential function" @click="currentFunction = 'exponential'">Exponential</a></li>
                        </ul>
					</div>
      
   
                    <div v-if="currentFunction === 'linear'">
                            <img id='linear_function' src='/images/LinearFunction.png' alt="linear function equation">

                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_a">a = </label>
                                <input type="number" class='form-control' id="func_a" v-model="func_a">
                            </div>
                        
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_b">b = </label>
                                <input type="number" class='form-control' id="func_b" v-model="func_b">
                            </div>
                    </div>

                    <div v-else-if="currentFunction === 'quadratic'">
                            <img id='linear_function' src='/images/QuadraticFunction.png' alt="quadratic function equation">
                        
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_a">a = </label>
                                <input type="number" class='form-control' id="func_a" v-model="func_a">
                            </div>
                     
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_b">b = </label>
                                <input type="number" class='form-control' id="func_b" v-model="func_b">
                            </div>
                    </div>

                    <div v-else-if="currentFunction === 'trigonometric'">
                        <img id='trig_function' src='/images/TrigFunction.png' alt="trigonometric function equation">
                       
                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_a">A = </label>
                            <input type="number" class='form-control' id="func_a" v-model="func_a">
                        </div>

                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_b">&omega; = </label>
                            <input type="number" class='form-control' id="func_b" v-model="func_b">
                        </div>
                          
                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_c">&phi; = </label>
                            <input type="number" class='form-control' id="func_c" v-model="func_c">
                        </div>
                    </div>

                    <div v-else-if="currentFunction === 'exponential'">
                            <img id='linear_function' src='/images/ExpFunction.png' alt="exponential function equation">

                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_a">A = </label>
                                <input type="number" class='form-control' id="func_a" v-model="func_a">
                            </div>
                    
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_b">b = </label>
                                <input type="number" class='form-control' id="func_b" v-model="func_b">
                            </div>
                    </div>


                </div>
            </template>

            <template v-slot:footer>
                 <button v-if="currentFunction === 'linear'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(linear)">Plot</button>
                <button v-else-if="currentFunction === 'quadratic'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(quadratic)">Plot</button>
                <button v-else-if="currentFunction === 'trigonometric'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(trigonometric)">Plot</button>
                <button v-else-if="currentFunction === 'exponential'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(exponential)">Plot</button>
                    
                <button class="button-sm button-warning m-1" id="clearFunctionButton" @click="deleteFunctionDataset">Clear</button>
            </template>
        </popup-plotting>

        <!-- <button type='button' class="button-toolbar button-secondary me-2" id="show-plotting-button" aria-label="show plotting functions" @click="showPlotting = true" data-bs-toggle="tooltip" title="Function Plotting">
            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-graph-up" viewBox="0 0 16 16">
                <path fill-rule="evenodd" d="M0 0h1v15h15v1H0zm14.817 3.113a.5.5 0 0 1 .07.704l-4.5 5.5a.5.5 0 0 1-.74.037L7.06 6.767l-3.656 5.027a.5.5 0 0 1-.808-.588l4-5.5a.5.5 0 0 1 .758-.06l2.609 2.61 4.15-5.073a.5.5 0 0 1 .704-.07"/>
            </svg>
        </button> -->
        
        <div>
            <span v-if="getNumData < maxDataPoints" class="align-middle">Plotted: {{ getNumData }} / {{ maxDataPoints }}</span>
            <span v-else class="align-middle">Plotted: {{ maxDataPoints }} / {{ maxDataPoints }} MAX REACHED</span>
            <span class="align-middle ms-2" for="gradient">Gradient: {{ gradient.toFixed(2) }}</span>
        </div>
    </div>

    
    <canvas id='graph-canvas' ref="graphCanvas" @mousedown="startLine" @mouseup="endDrag" @mousemove="endLine"></canvas>
    
    
    
    
    <!-- <transition name='fade'>
      <div v-if="showPlotting" class="modal" id='modal-show' tabindex="-1">
        <div class="modal-dialog modal-dialog-scrollable">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Plot Function</h5>
                <button type='button' :class="getDarkTheme ? 'btn-close btn-close-white' : 'btn-close'" id='close-button' aria-label="Close" @click='showPlotting = false'>

                </button>
            </div>
            <div class="modal-body">
                <div id="chart-functions" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)" @mouseleave="setDraggable(true)">
					<div class="dropdown">
						<button class="button-sm button-dropdown dropdown-toggle" type="button" id="function-select-dropdown" data-bs-toggle="dropdown" aria-expanded="false">
							{{ currentFunction }}
						</button>
						<ul class="dropdown-menu" aria-labelledby="function-dropdown-menu">
							<li><a class="dropdown-item" id='select-linear-option' aria-label="select linear function" @click="currentFunction = 'linear'">Linear</a></li>
							<li><a class="dropdown-item" id='select-quadratic-option' aria-label="select quadratic function" @click="currentFunction = 'quadratic'">Quadratic</a></li>
                            <li><a class="dropdown-item" id='select-trigonometric-option' aria-label="select trigonometric function" @click="currentFunction = 'trigonometric'">Trigonometric</a></li>
                            <li><a class="dropdown-item" id='select-exponential-option' aria-label="select exponential function" @click="currentFunction = 'exponential'">Exponential</a></li>
                        </ul>
					</div>
      
   
                    <div v-if="currentFunction === 'linear'">
                            <img id='linear_function' src='/images/LinearFunction.png' alt="linear function equation">

                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_a">a = </label>
                                <input type="number" class='form-control' id="func_a" v-model="func_a">
                            </div>
                        
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_b">b = </label>
                                <input type="number" class='form-control' id="func_b" v-model="func_b">
                            </div>
                    </div>

                    <div v-else-if="currentFunction === 'quadratic'">
                            <img id='linear_function' src='/images/QuadraticFunction.png' alt="quadratic function equation">
                        
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_a">a = </label>
                                <input type="number" class='form-control' id="func_a" v-model="func_a">
                            </div>
                     
                            <div class='input-group mt-2 mb-2'>
                                <label class='input-group-text' for="func_b">b = </label>
                                <input type="number" class='form-control' id="func_b" v-model="func_b">
                            </div>
                    </div>

                    <div v-else-if="currentFunction === 'trigonometric'">
                        <img id='trig_function' src='/images/TrigFunction.png' alt="trigonometric function equation">
                       
                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_a">A = </label>
                            <input type="number" class='form-control' id="func_a" v-model="func_a">
                        </div>

                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_b">&omega; = </label>
                            <input type="number" class='form-control' id="func_b" v-model="func_b">
                        </div>
                          
                        <div class='input-group mt-2 mb-2'>
                            <label class='input-group-text' for="func_c">&phi; = </label>
                            <input type="number" class='form-control' id="func_c" v-model="func_c">
                        </div>
                    </div>

       <div v-else-if="currentFunction === 'exponential'">
            <img id='linear_function' src='/images/ExpFunction.png' alt="exponential function equation">

            <div class='input-group mt-2 mb-2'>
                <label class='input-group-text' for="func_a">A = </label>
                <input type="number" class='form-control' id="func_a" v-model="func_a">
            </div>
    
            <div class='input-group mt-2 mb-2'>
                <label class='input-group-text' for="func_b">b = </label>
                <input type="number" class='form-control' id="func_b" v-model="func_b">
            </div>
       </div>


   </div>
            </div>

            <div class="modal-footer d-flex flex-row">
                <button v-if="currentFunction === 'linear'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(linear)">Plot</button>
                <button v-else-if="currentFunction === 'quadratic'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(quadratic)">Plot</button>
                <button v-else-if="currentFunction === 'trigonometric'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(trigonometric)">Plot</button>
                <button v-else-if="currentFunction === 'exponential'" class="button-sm button-primary m-1" id="plotFunctionButton" @click="plotFunc(exponential)">Plot</button>
                    
                <button class="button-sm button-warning m-1" id="clearFunctionButton" @click="deleteFunctionDataset">Clear</button>
                <button type="button" class="button-sm button-danger" id="close-plotting-modal" aria-label="close plotting modal" @click='showPlotting = false'>Close</button>
            
            </div>
          </div>
        </div>
      </div>
      </transition> -->
    
    
   
</div>

</template>

<script>

import Chart from 'chart.js/auto';
import { mapGetters, mapActions } from 'vuex';
import DownloadImageButton from './elements/DownloadImageButton.vue';
import PopupHelp from './elements/PopupHelp.vue';
import PopupPlotting from './elements/PopupPlotting.vue';

var scatterChart = null;        //if part of the responsive Vue data then causes a recursion error on dynamically adding datasets.

export default {
    
    name: 'Graph',
    emits: [],
    components:{
        DownloadImageButton,
        PopupHelp,
        PopupPlotting
    },
    data(){
        return{
            //chart: null,
            gradient_start_point: {x:0, y:0},
            gradient_end_point: {x:0, y:0},
            gradient: 0,
            mouseDragging: false,
            currentFunction: "linear",
            func_a: 0,
            func_b: 0,
            func_c: 0,
            func_d:0,
            funcTimeStep: 0.01,
            YAxisMax: 0,
            YAxisMin: 0,
            XAxisMax: 0,
            XAxisMin: 0,
            maxDataPoints: 2000,
            current_data_index: 0,
            data_index_interval: 10,
            latest_index: 0,
            light_colours: ['rgba(0, 0, 0, 1)', 'rgba(255, 0, 0, 1)', 'rgba(0, 0, 255, 1)', '#A3A3A3', '#F5A300', '#5B5F97'],
            dark_colours: ['rgba(255, 255, 255, 1)', 'rgba(255, 0, 255, 1)', 'rgba(0, 255, 0, 1)', 'rgba(0, 255, 255, 1)', 'rgba(255, 255, 0, 1)', 'rgba(255, 0, 0, 1)'],
            showPlotting: false,
        }
    },
    mounted() {
        this.createChart();
        this.getAllData();
        this.updateChart();
      },
    computed:{
        ...mapGetters([
            'getData',
            //'getGraphDataParameter',
            'getNumData',
            'getIsRecording',
            'getDarkTheme',
            'getCurrentMode'
        ]),
      },
    watch:{
        getData(){
            this.clearData(); //only runs if data array gets reset to [];
        },
        getDarkTheme(){
            this.clearData();
        },
        // getGraphDataParameter(new_value){
        //     scatterChart.options.scales['y'].title.text = new_value;
        //     scatterChart.update();
        // }
    },
    methods: {
        ...mapActions([
            'setDraggable'
        ]),
        updateChart(){
            let num_data = this.getNumData;
            let max_index = this.maxDataPoints;
            if(num_data < this.maxDataPoints){
                max_index = num_data
            }
            
            if(this.latest_index < max_index){
                for(let i=this.latest_index; i < max_index; i++){
                    this.getDataAtIndex(i);
                }
                this.latest_index = max_index;
                //scatterChart.options.scales['y'].title.text = this.getGraphDataParameter;
                scatterChart.update();                
                
            } 
         

            setTimeout(this.updateChart, 100);
        },
        createChart() {
            var _this = this;
            //const canvas = document.getElementById('graph-canvas');
            const canvas = this.$refs.graphCanvas;
            const ctx = canvas.getContext('2d');
            scatterChart = new Chart(ctx, {
            type: 'scatter',
            data: {
                datasets: []
            },
            options: {
                responsive: true,
                animation: false,
                normalized: true,
                parsing: true,
                maintainAspectRatio: false,
                legend:{
                    display: true
                },
                scales: {
                    x: {
                        title:{
                            display: true,
                            text: 'time[s]',
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        type: 'linear',
                        position: 'bottom',
                        ticks: {
                            callback : (value,index,values) => {
                                _this.updateXAxisMax(value, index, values);
                                _this.updateXAxisMin(value, index);
                                return value;
                            },
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        grid: {
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 0.5)' : 'rgba(0, 0, 0, 0.5)'
                        },
                        minRotation: 20,
                        maxRotation: 20,
                        sampleSize: 2,
                    },
                    y: {
                        title:{
                            display: true,
                            text: 'acceleration [x 9.81m/s/s]',
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        type: 'linear',
                        position: 'left',
                        ticks: {
                            precision: 2,
                            callback : (value,index,values) => {
                                _this.updateYAxisMax(value, index);
                                _this.updateYAxisMin(value, index, values);
                                return value;
                            },
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        grid: {
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 0.5)' : 'rgba(0, 0, 0, 0.5)',
                        },
                        sampleSize: 2,
                    },
                    y2: {
                        display: 'auto',    //only displays when a dataset is added and not hidden
                        title:{
                            display: true,
                            text: 'normalised driver position',
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        type: 'linear',
                        position: 'right',
                        ticks: {
                            precision: 2,
                            // callback : (value,index,values) => {
                            //     _this.updateYAxisMax(value, index);
                            //     _this.updateYAxisMin(value, index, values);
                            //     return value;
                            // },
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)'
                        },
                        grid: {
                            color: _this.getDarkTheme ? 'rgba(255, 255, 255, 0.5)' : 'rgba(0, 0, 0, 0.5)',
                            drawOnChartArea: false,
                        },
                        sampleSize: 2,
                    },
                },
                plugins: {
                    tooltip:{
                        callbacks:{
                            label: function(context){
                                try{
                                    canvas.dispatchEvent(
                                    new CustomEvent('chartdatahover', {
                                        bubbles: true,
                                        detail: {
                                            dataset: context.dataset.label,
                                            x: context.parsed.x,
                                            y: context.parsed.y
                                            }
                                        })
                                    )
                                } catch (e){
                                    console.log('chartdatahover event error');
                                }
                                
                            }
                        }
                    },
                    legend:{
                        labels:{
                            usePointStyle: true,
                        }
                    }
                }
                
            }
        });
        },
        updateYAxisMax(value, index){
            if(index == 0){
                this.YAxisMax = value;
            }
            
        },
        updateYAxisMin(value,index,values){
            if(index == values.length - 1){
                this.YAxisMin = value;
            }
        },
        updateXAxisMin(value, index){
            if(index == 0){
                this.XAxisMin = value;
            }
            
        },
        updateXAxisMax(value,index,values){
            if(index == values.length - 1){
                this.XAxisMax = value;
            }
        },
        addDataToChart(data, dataset_index) {
            //check if the next dataset doesn't exist and create it
            if(dataset_index == this.countDataSets()){
                this.deleteFunctionDataset();   //remove any plotted functions to not conflict with dataset index
                //this.addEmptyDataSet(dataset_index);
                if(this.getCurrentMode == 'driven'){
                    this.addTwoEmptyDataSets(dataset_index);
                } else{
                    //this.addEmptyDataSet(dataset_index);
                    this.addOneEmptyOneHiddenDataSets(dataset_index);   //position data plotted but not shown by default
                }
                
            }
            //add data to the existing dataset
            try{
                scatterChart.data.datasets[dataset_index].data.push(data);
            } catch(e){
                console.log(e);
            }
        },
        clearData(){
            this.latest_index = 0;          //NEW
            
            scatterChart.destroy();
            this.createChart();
        },
        getAllData(){
            if(this.current_data_index == 0){
                this.clearData();
                
            }
            let _current_index = this.current_data_index;
            for(let i=_current_index; i<this.getNumData;i++){
                let data = this.getData[i];
                let x_data = data.t;
                let y_data = data.acc.x;
                let y_data_two = Math.sin(Math.PI*data.pos/180);

                if(this.getCurrentMode == 'driven'){
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                } else{
                    //this.addDataToChart({x: x_data, y: y_data}, data.set);
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                }
                

                if(i >= this.current_data_index + this.data_index_interval || i == this.getNumData - 1){
                    this.current_data_index = i + 1;
                    break;
                }
                
            }

                if(this.current_data_index < this.getNumData && this.current_data_index <= this.maxDataPoints){
                    setTimeout(this.getAllData, 20);
                    scatterChart.update(0);
                } else{
                    scatterChart.update(0);
                    this.count = 0;
                    this.current_data_index = 0;
                }
                
            
        },
        getLatestData(){
            let index = this.getNumData - 1;
            if(index >= 0){
                let data = this.getData[index];
                let x_data = data.t;
                let y_data = data.acc.x;
                let y_data_two = Math.sin(Math.PI*data.pos/180);

                if(this.getCurrentMode == 'driven'){
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                } else{
                    //this.addDataToChart({x: x_data, y: y_data}, data.set);
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                }
                
            } 
            
        },
        getDataAtIndex(index){
            if(index >= 0){
                let data = this.getData[index];
                let x_data = data.t;
                let y_data = data.acc.x;
                let y_data_two = Math.sin(Math.PI*data.pos/180);
                
                if(this.getCurrentMode == 'driven'){
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                } else{
                    //this.addDataToChart({x: x_data, y: y_data}, data.set);
                    this.addDataToChart({x: x_data, y: y_data}, 2*parseInt(data.set));
                    this.addDataToChart({x: x_data, y: y_data_two}, 2*parseInt(data.set) + 1);
                }

                
            } 
        },
        removeChart(){
            scatterChart.destroy();
        },
        startLine(event){
            event.preventDefault();
            this.gradient_start_point.x = event.offsetX;
            this.gradient_start_point.y = event.offsetY;

            this.mouseDragging = true;
            
        },
        endDrag(){
            this.mouseDragging = false;
        },
        endLine(event){
            if(this.mouseDragging){
                //get ratio of y axis to x axis scales, get ratio of y difference in mouse positions and x difference in mouse positions.
                this.gradient_end_point.x = event.offsetX;
                this.gradient_end_point.y = event.offsetY;

                let pointer_ratio = (this.gradient_start_point.y - this.gradient_end_point.y) / (this.gradient_end_point.x - this.gradient_start_point.x);  //pointer ratio
        
                let canvas_width_offset = 61;
                let canvas_height_offset = 83;
                let canvas = document.getElementById('graph-canvas');
                let canvas_height = canvas.clientHeight - canvas_height_offset;
                let canvas_width = canvas.clientWidth - canvas_width_offset;
                let canvas_ratio = canvas_height/canvas_width;      //canvas ratio
                
                let y_diff = this.YAxisMax - this.YAxisMin;
                let x_diff = this.XAxisMax - this.XAxisMin;
                let axis_ratio = y_diff/x_diff;         //axis ratio

                
                this.gradient = -axis_ratio*pointer_ratio/canvas_ratio; //negative comes from difference in direction of Y increase between axis and screen
                this.drawLine(this.gradient_start_point, this.gradient_end_point);
                
            }
            
        },
        drawLine(from, to){
            
            //draw the gradient line
            //only draw anything if at least 2 data points have been plotted
            let canvas = document.getElementById('graph-canvas');
            const context = canvas.getContext('2d');
            context.clearRect(0, 0, canvas.width, canvas.height);
            scatterChart.update(0);       //instantly update with no animation

            context.beginPath(); 
            // Staring point 
            context.moveTo(from.x, from.y);
            // End point 
            context.lineTo(to.x, to.y);
            context.lineWidth = 5;
            // set line color
            context.strokeStyle = 'red';
            // Make the line visible
            context.stroke();
            
        },
        plotFunc(func){
            // let min = store.getMinTime();
            // let max = store.getMaxTime();
            let min = this.XAxisMin;
            let max = this.XAxisMax;
            let t_delta = max-min;
            let num_plots = t_delta/this.funcTimeStep;
            let new_data = [];
            let new_t;
            let new_y;
            for(let i=0; i<num_plots;i++){
                new_t = min + i*this.funcTimeStep;
                new_y = func(new_t);
                let data = {
                    x: new_t,
                    y: new_y
                };
                new_data.push(data);
            }
            
            this.addFunctionPlot('rgba(255, 0, 0, 0.5)', new_data);
        },
        linear(t){
            return (parseFloat(this.func_a)*t + parseFloat(this.func_b));
        },
        quadratic(t){
            return parseFloat(this.func_a)*t*t + parseFloat(this.func_b);
        },
        trigonometric(t){
            return parseFloat(this.func_a)*Math.sin(parseFloat(this.func_b)*t + parseFloat(this.func_c));
        },
        exponential(t){
            return parseFloat(this.func_a)*Math.exp(parseFloat(this.func_b)*t);
        },
        addFunctionPlot(colour, data){
            scatterChart.data.datasets.push({
                id: "function",
                label:"function",
                pointBackgroundColor: colour,
                borderColor: colour,
                data: data,
                showLine: true
                });
                scatterChart.update(0);
        },
        addEmptyDataSet(new_index){
            scatterChart.data.datasets.push({
                id: `dataset${new_index}`,
                label:`acceleration${new_index}`,
                pointBackgroundColor: this.getDarkTheme ? this.dark_colours[new_index % this.dark_colours.length] : this.light_colours[new_index % this.light_colours.length],
                borderColor: this.getDarkTheme ? this.dark_colours[new_index % this.dark_colours.length] : this.light_colours[new_index % this.light_colours.length],
                data: [],
                showLine: true
                });
            scatterChart.update(0);
        },
        addTwoEmptyDataSets(new_index){
            scatterChart.data.datasets.push({
                yAxisID: 'y',
                id: `dataset${new_index}`,
                label:`acceleration${parseInt(new_index)/2}`,
                pointBackgroundColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                borderColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                data: [],
                showLine: true
                });

            scatterChart.data.datasets.push({
                yAxisID: 'y2',
                id: `dataset${new_index+1}`,
                label:`driverPosition${parseInt(new_index)/2}`,
                pointBackgroundColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                borderColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                //borderColor: this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)',
                pointStyle: 'triangle',
                pointRadius: 5,
                data: [],
                showLine: true
                });

            scatterChart.update(0);
        },
        addOneEmptyOneHiddenDataSets(new_index){
            scatterChart.data.datasets.push({
                yAxisID: 'y',
                id: `dataset${new_index}`,
                label:`acceleration${parseInt(new_index)/2}`,
                pointBackgroundColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                borderColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                data: [],
                showLine: true
                });

            scatterChart.data.datasets.push({
                yAxisID: 'y2',
                id: `dataset${new_index+1}`,
                label:`driverPosition${parseInt(new_index)/2}`,
                pointBackgroundColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                borderColor: this.getDarkTheme ? this.dark_colours[(new_index/2) % this.dark_colours.length] : this.light_colours[(new_index/2) % this.light_colours.length],
                //borderColor: this.getDarkTheme ? 'rgba(255, 255, 255, 1)' : 'rgba(0, 0, 0, 1)',
                pointStyle: 'triangle',
                pointRadius: 5,
                data: [],
                showLine: true,
                hidden: true
                });

            scatterChart.update(0);
        },
        deleteFunctionDataset(){
            scatterChart.data.datasets = scatterChart.data.datasets.filter(set => set.label !== "function");
            scatterChart.update(0);
        },
        countDataSets(){
                let datasets = scatterChart.data.datasets.filter(set => set.id.includes("dataset"));
                return datasets.length;
        },
      },
      
}
</script>



<style scoped>

#graph-canvas{
    cursor: crosshair;
    background-color: var(--background-color-highlight);
    max-height: 90%;
    max-width: 100%;
}

#transfer_function{
    width: 120px;
    height: 50px;
}

#second_order_transfer_function{
    width: 160px;
    height: 50px;
}

#linear_function{
    width: 120px;
    height: 30px;
}

#trig_function{
    width: 150px;
    height: 30px;
}

#chart-functions{
    min-height: 40dvh;
}

/* .modal-content{
   position: absolute;
  top: 100px;
  right: 100px;
  bottom: 0;
  left: -25dvw;
} */


</style>
