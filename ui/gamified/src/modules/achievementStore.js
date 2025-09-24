//Store for the achievements available in the remote lab.


const achievementStore = {
    state: () => ({
        achievements: [],     //will be added dynamically by the Achievements.vue component using the lab config file.
        new_achievement_update: false,
        new_achievement_count: 0,

       }),
       mutations:{
        ADD_ACHIEVEMENTS(state, list){
            state.achievements = list;
        },
        LOAD_ACHIEVEMENTS(state, achievements_to_load){
            let updated_achievements = [];
            state.achievements.forEach(achievement => {
                let update_achievement = achievements_to_load.find(ach => ach.name == achievement.name);
                if(update_achievement != undefined){
                    updated_achievements.push(update_achievement);
                } else{
                    updated_achievements.push(achievement);
                }
            })

            state.achievements = updated_achievements;
        },
         SET_ACHIEVEMENT_COMPLETED(state, name){
            state.achievements.forEach(item => {
                if(item.name == name){
                    item.completed = true;
                }
            });
         },
         //payload -> achievement = {name: name, fractional: fractional}
         SET_FRACTIONAL_ACHIEVEMENT_COMPLETED(state, achievement){
            state.achievements.forEach(item => {
                if(item.name == achievement.name){
                    if('fractional' in item){
                        item.fractional.forEach(frac => {
                            if(frac.name == achievement.fractional && !frac.completed){
                                frac.completed = true;
                                item.n++;
    
                                if(item.n >= item.required){
                                    item.completed = true;
                                }
                            }
                        })
                    }
                }
            });
         },
         ADD_MULTIPLE_ACHIEVEMENT(state, name){
            state.achievements.forEach(item => {
                if(item.name == name){
                    if('n' in item){
                        if(item.n >= item.required - 1){
                            item.n++;
                            item.completed = true;
                        } else {
                            item.n++;
                        }
                    }
                }
            });
         },
         SET_ACHIEVEMENT_UPDATE(state, set){
             if(set){
                state.new_achievement_count += 1;
             } else{
                state.new_achievement_count = 0;
             }
             state.new_achievement_update = set;
             
         },
         CLEAR_COMPLETED_ACHIEVEMENTS(state){
             state.achievements.forEach(achievement => {
                 achievement.completed = false;
             })
         }
         

       },
       actions:{
        addAchievements(context, list){
            context.commit('ADD_ACHIEVEMENTS', list);
        },
        loadAchievements(context, achievements){
            context.commit('LOAD_ACHIEVEMENTS', achievements);
        },
        setAchievementCompleted(context, name){
             if(context.getters.getAchievementsUncompleted.includes(name)){
                context.commit('SET_ACHIEVEMENT_COMPLETED', name);
                context.commit('SET_ACHIEVEMENT_UPDATE', true);
             }
         },
         setFractionalAchievementCompleted(context, achievement){
            if(context.getters.getAchievementsUncompleted.includes(achievement.name)){
                context.commit('SET_FRACTIONAL_ACHIEVEMENT_COMPLETED', achievement);
                context.commit('SET_ACHIEVEMENT_UPDATE', true);     //although perhaps not completed, should show some kind of update to progress.
            }
         },
         addMultipleAchievement(context, name){
            if(context.getters.getAchievementsUncompleted.includes(name)){
                context.commit('ADD_MULTIPLE_ACHIEVEMENT', name);
                context.commit('SET_ACHIEVEMENT_UPDATE', true);     //although perhaps not completed, should show some kind of update to progress.
            }
         },
         setAchievementUpdate(context, set){
             context.commit('SET_ACHIEVEMENT_UPDATE', set);
         },
        clearCompletedAchievements(context){
            context.commit('CLEAR_COMPLETED_ACHIEVEMENTS');
        }


       },
       getters:{
         getAchievements(state){
            return state.achievements;
         },
         getAchievementsCompleted(state){
             let completed = [];
            state.achievements.forEach(item => {
                if(item.completed){
                    completed.push(item.name);
                }
            });
            return completed;
         },
         getAchievementsUncompleted(state){
            let uncompleted = [];
            state.achievements.forEach(item => {
                if(!item.completed){
                    uncompleted.push(item.name);
                }
            });
            return uncompleted;
         },
         getAchievementUpdated(state){
             return state.new_achievement_update;
         },
         getNewAchievementCount(state){
             return state.new_achievement_count;
         },
         getAchievementByName: (state) => (name) => {
            return state.achievements.find(achievement => achievement.name == name);
         },
         
          
         
       },  
  
  }

  export default achievementStore;
