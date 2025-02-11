import { createStore } from 'vuex'
import streamStore from './modules/streamStore.js'
import dataStore from './modules/dataStore.js'
import uiStore from './modules/uiStore.js'
import commandStore from './modules/commandStore.js'
import chatStore from './modules/chatStore.js'
import loggingStore from './modules/logging.js'
import smoothieStore from './modules/smoothie.js'

const store = createStore({
    modules:{
        stream: streamStore,
        data: dataStore,
        ui: uiStore,
        command: commandStore,
        chat: chatStore,
        logging: loggingStore,
        smoothie: smoothieStore
    }
})

export default store;