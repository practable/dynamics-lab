# dynamics lab gamified interface

This interface has the same core functionality as the analytics UI, but includes a number of additional components to support a 'gamified learning' experience.

## Additional components

### Completed

### ToDo

- Achievements
- Interaction modes - game controller, touchscreen
- AR/Simulation - 3D CAD model of setup, support for AR or simply a model to interact with in the interface.
- Circuit puzzle - need to correctly connect the pins on a simulated version of the control electronics before interaction can take place.
- Group role UIs - different user interfaces that provide a single role to each user (e.g. controller, visualiser/validator (check when recording should take place), data gatherer)


# Gamified Components

## Achievements

Hidden achievements added to the user experience in much the same way as achievements in Steam or XBox.

Like Logging, this component should be added to the UI without the need to make any changes to other components - except adding the HTML component to the appropriate location (e.g. to the navigation bar for achievements).

The remote lab config file should have an achievements key added to it. This is where the specific achievements are configured. 
The Achievements.vue component should access that config file once it has been loaded and populate the achievementStore.js vuex store. 
Saved achievements from previous sessions should be loaded and achievements accomplished during a session should be saved to localStorage.

To enable tracking of achievements without having to add code to other components, like Logging, the achievements component must use the config file to add events to the appropriate components.

### Achievements list

- send driven command
- send undriven command
- send home command
- send all commands
- update frequency
- record a dataset
- download a dataset
- download an image of the dynamics lab
- plot a curve over your data
- toggle dark mode
- re-configure the UI
- send 50 commands
- download 10 datasets
- complete a whole session