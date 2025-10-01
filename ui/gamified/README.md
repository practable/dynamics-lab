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

### Achievement type

Achievements can be `singular`, `multiple` or `fractional`. Singular achievements will activate the first time a single condition is met. Multiple achievements require the same interaction to occur a given number of times before activating. Fractional achievements require a series of different interactions in order to be met. Fractional achievements can also support achievements that have a number of options, each of which triggers the achievement. This is accomplished by having a series of sub-achievements in the `fractional` key, but setting `required` to 1 rather than the length of the sub-achievements array.

### Interaction type

A `type` key must be associated with each achievement with existing options: `click` or `hotkey`

### Interaction id or hotkey

For any `click` achievement an `id` must be associated with it that corresponds to the div id of the button that needs to be clicked. If the achievement is fractional then the id is associated with the sub-achievement.

For `hotkey` achievements, rather than an id, a `hotkey` is defined. This must correspond with the appropriate `event.key` triggered.

# Example config file

Below is an example configuration file for setting the achievements. It contains singular, multiple and fractional achievements both where all sub-achievements must be met and where a single sub-achievement will trigger the parent achievement.

```
{
    "name": "dyna00",
    "version": "1.0",
    "date": 1739449599,
    "aud": "https://app.practable.io/ed0/dynamics-default-1.0",
    "images": [],
    "parameters": {
        "ui": {
                "hardware": "Dynamics Lab",
                "short": "dyna00",
                "m": "452g",
                "offset": "50mm",
                "l": "400mm",
                "w": "40mm",
                "t": "3mm",
                "density": "7850kg/m3"
             },
        "achievements": [
        	{"name":"driven-command", "type": "click", "id": "update-driving-frequency-button", "verbose":"Set your first driven frequency", "completed": false, "hidden": true},
        	{"name":"undriven-command", "type": "click", "id": "undriven-ping-button", "verbose":"Ping the oscillator for the first time", "completed": false, "hidden": true},
        	{"name":"home-command", "type": "click", "id": "home-button", "verbose":"Return to home", "completed": false, "hidden": true},
        	{"name":"command-all", "type": "click", "verbose":"Send all hardware commands by clicking", "completed": false, "hidden": true, "fractional": [
				{"name":"driven-command-all", "id": "update-driving-frequency-button", "completed": false},
				{"name":"undriven-command-all", "id": "undriven-ping-button", "completed": false},
				{"name":"home-command-all", "id": "home-button", "completed": false},
				{"name":"stop-command-all", "id": "driven-stop-button", "completed": false}
            		], "required": 4, "n": 0},
            	{"name":"update-frequency", "type": "click", "verbose":"Update the driven frequency", "completed": false, "hidden": true, "fractional": [
				{"name":"freq-step-positive-big", "id": "frequency-step-positive-01", "completed": false},
				{"name":"freq-step-negative-big", "id": "frequency-step-negative-01", "completed": false},
				{"name":"freq-step-positive-small", "id": "frequency-step-positive-001", "completed": false},
				{"name":"freq-step-negative-small", "id": "frequency-step-negative-001", "completed": false}
            		], "required": 1, "n": 0},
            {"name":"multiple-runs", "type": "click", "id": "update-driving-frequency-button", "verbose":"Run a step or ramp 10 times", "completed": false, "hidden": true, "required": 10, "n": 0}
        ]
    }
}
```

# Development

During development use:

http://localhost:5173?config=./dev-config/dyna00-1.0.json 

