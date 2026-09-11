//Store for variables that are common amongst multiple UI components. 


const uiStore = {
    state: () => ({
       isDraggable: true,
       usesLocalStorage: false,        //can only use localStorage if the browser allows it.
       config_json: '',  
       darkTheme: document.body.classList.contains('dark-theme') ? true : false,
       windowWidth: window.innerWidth
       }),
       mutations:{
         SET_DRAGGABLE(state, draggable){
            state.isDraggable = draggable;
         },
         SET_USES_LOCAL_STORAGE(state, set){
            state.usesLocalStorage = set;
         },
         SET_DARK_THEME(state, set){
            state.darkTheme = set;
         },
         SET_CONFIG_JSON(state, json){
            state.config_json = json;
         },
         SET_WINDOW_WIDTH(state, val){
            state.windowWidth = val;
         }
         

       },
       actions:{
         setDraggable(context, draggable){
             context.commit('SET_DRAGGABLE', draggable);
         },
         setUsesLocalStorage(context, set){
            context.commit('SET_USES_LOCAL_STORAGE', set);
         },
         setDarkTheme(context, set){
            context.commit('SET_DARK_THEME', set);
         },
         setConfigJSON(context, json){
            context.commit('SET_CONFIG_JSON', json);
         },
         setWindowWidth(context, val){
            context.commit('SET_WINDOW_WIDTH', val);
         }


       },
       getters:{
         getDraggable(state){
             return state.isDraggable;
         },
         getUsesLocalStorage(state){
            return state.usesLocalStorage;
         },
         getDarkTheme(state){
            return state.darkTheme;
         },
         getConfigJSON(state){
            return state.config_json;
         },
           getWindowWidth(state){
            return state.windowWidth;
         },
         isMobile(state){
            if(state.windowWidth < 992){
               return true;
            } else{
               return false;
            }
         },
         
         
       },  
  
  }

  export default uiStore;
