//Store for variables that are common amongst multiple UI components. 


const smoothieStore = {
    state: () => ({
        chart_acceleration: null,
        canvas_acceleration: null,
        chart_position: null,
        canvas_position: null,
        y_min_acc: -2,
        y_max_acc: 2,
        y_min_pos: 0,
        y_max_pos: 360,
        y_acc_abs: 2,
        y_pos_abs: 360,
        millis_per_pixel: 10
       }),
       mutations:{
        SET_CHART_ACCELERATION(state, chart){
            state.chart_acceleration = chart;
         },
         SET_CHART_POSITION(state, chart){
            state.chart_position = chart;
         },
         SET_CANVAS_ACCELERATION(state, canvas){
            state.canvas_acceleration = canvas;
         },
         SET_CANVAS_POSITION(state, canvas){
            state.canvas_position = canvas;
         },
         SET_Y_MIN_ACC(state, value){
            state.y_min_acc = value;
            state.chart_acceleration.options.minValue = value;
         },
         SET_Y_MAX_ACC(state, value){
            state.y_max_acc = value;
            state.chart_acceleration.options.maxValue = value;
         },
         SET_Y_MIN_POS(state, value){
            state.y_min_pos = value;
            state.chart_position.options.minValue = value;
         },
         SET_Y_MAX_POS(state, value){
            state.y_max_pos = value;
            state.chart_position.options.maxValue = value;
         },
         SET_MILLIS_PER_PIXEL(state, value){
            state.millis_per_pixel = value;
            state.chart_acceleration.options.millisPerPixel = value;
            state.chart_position.options.millisPerPixel = value;
         },
         SET_ACC_ABS(state, value){
            state.y_acc_abs = value;
         },
         SET_POS_ABS(state, value){
            state.y_pos_abs = value;
         },
       },
       actions:{
         setChartAcceleration(context, chart){
            context.commit('SET_CHART_ACCELERATION', chart);
         },
         setChartPosition(context, chart){
            context.commit('SET_CHART_POSITION', chart);
         },
         setCanvasAcceleration(context, canvas){
            context.commit('SET_CANVAS_ACCELERATION', canvas);
         },
         setCanvasPosition(context, canvas){
            context.commit('SET_CANVAS_POSITION', canvas);
         },
         setYMinAcc(context, value){
            context.commit('SET_Y_MIN_ACC', value)
         },
         setYMaxAcc(context, value){
            context.commit('SET_Y_MAX_ACC', value)
         },
         setYMinPos(context, value){
            context.commit('SET_Y_MIN_POS', value)
         },
         setYMaxPos(context, value){
            context.commit('SET_Y_MAX_POS', value)
         },
         setMillisPerPixel(context, value){
            context.commit('SET_MILLIS_PER_PIXEL', value)
         },
         setAccAbs(context, value){
            context.commit('SET_ACC_ABS', value)
         },
         setPosAbs(context, value){
            context.commit('SET_POS_ABS', value)
         },
         resetSmoothieSettings(context){
            context.dispatch('setYMinAcc', default_settings.y_min_acc)
            context.dispatch('setYMaxAcc', default_settings.y_max_acc)
            context.dispatch('setYMinPos', default_settings.y_min_pos)
            context.dispatch('setYMaxPos', default_settings.y_max_pos)
            context.dispatch('setMillisPerPixel', default_settings.millis_per_pixel)
            context.dispatch('setAccAbs', default_settings.y_acc_abs)
            context.dispatch('setPosAbs', default_settings.y_pos_abs)
         }
       },
       getters:{
        getChartAcceleration(state){
            return state.chart_acceleration;
         },
         getChartPosition(state){
            return state.chart_position;
         },
         getCanvasAcceleration(state){
            return state.canvas_acceleration;
         },
         getCanvasPosition(state){
            return state.canvas_position;
         },
         getYMinAcc(state){
            return state.y_min_acc;
         },
         getYMaxAcc(state){
            return state.y_max_acc;
         },
         getYMinPos(state){
            return state.y_min_pos;
         },
         getYMaxPos(state){
            return state.y_max_pos;
         },
         getMillisPerPixel(state){
            return state.millis_per_pixel;
         },
         getAccAbs(state){
            return state.y_acc_abs;
         },
         getPosAbs(state){
            return state.y_pos_abs;
         }
         
       },  
  }

  export const default_settings = {
      y_min_acc: -2,
      y_max_acc: 2,
      y_min_pos: 0,
      y_max_pos: 360,
      y_acc_abs: 2,
      y_pos_abs: 360,
      millis_per_pixel: 10
  }

  export default smoothieStore;
