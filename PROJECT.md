# project development

This document is `under development`. Any project conventions are not finalised and require discussion.

This document describes the basic setup of a new project, provides details of any templates that exist for development of remote lab components (e.g. firmware or user interface) and describes common formats or structures that should be adhered to during project development. This template repo does not contain any code used in the production of practable remote labs, but refers to existing code templates where available.

# Starting a new project

To start a new project repository you need to copy this project template using the GUI button on GitHub. You will be asked to provide a new name for your project. You should also ensure that you copy all branches, so that your new project retains the `main` and `develop` branch structure - including further details of project development in the `develop` branch.

# Managing the repo

## Long-lived (permanent) branches

There are two permanent branches: `main` and `develop`.

The `main` branch should only ever include production ready code/documentation that corresponds with the latest available production version of the project. Each component of the project (e.g. hw, fw, ui etc) in the main branch must be compatible with each other.

The `develop` branch contains the latest developments on the project that are either not yet fully compatible or have not been tested for production. Any single new feature or update is developed off of the develop branch and merged back with the develop branch once it is complete. After all new features are completed or the develop branch is sufficiently different to main to warrant a new version, then the `develop` branch code is tested, merged with `main` and tagged as a new release.

## Branch naming conventions for short-lived branches

If a new feature or update is being developed then a new, short-lived branch is created from the `develop` branch. For example, if a new UI graphing component is being developed, then a new branch called `feature/ui/new-graphing-tool` is created and all work performed in that branch. Once development is complete, it is merged back into `develop`.

See [here](https://medium.com/@abhay.pixolo/naming-conventions-for-git-branches-a-cheatsheet-8549feca2534)

Use prefixes in branches according to the following rules:

- feature: a whole new feature is being developed e.g. `feature/analytics`
- bugfix: fixing bugs e.g. `bugfix/responsive-styling`
- hotfix: critical bug fixes that are branched directly from main e.g. `hotfix/security-issue`
- docs: branch for documentation changes only e.g. `docs/command-examples`

Other possibilities:

- update: a currently existing feature is being updated e.g. `update/graph-output`

Could also add in the component that is being worked on e.g. `feature/fw/new-mode` to define a new branch where a new mode is being added to the firmware or `bugfix/ui/error-message` for a bugfix in the UI concerning the output of error messages.

# JSON Communication

The firmware and user interface communicate via JSON messages. The suggested structure for JSON messages is as follows:

```
{
  "timestamp": 999,	# when the JSON message is sent, may differ from time values in the payload
  "level": DEBUG || INFO || WARN || ERROR || FATAL,	# see description of levels below
  "context": {},		# may be empty, e.g. state of sensors or other conditions prior to error 
  "payload": {			# the necessary data for UI functionality
  		t: 5000,
  		theta: 20,
  		omega: 40
  	}
}
```

## Message levels described

The definition of each logging level is derived from [here](https://betterstack.com/community/guides/logging/log-levels-explained/).

- DEBUG: additional information made available only during development for debugging or testing purposes. Should not exist in production.
- INFO: 
  - the default message level
  - all standard command and data flow messages should be of this level
  - not logged by default
- WARN: 
  - all functionality remains for the time being, but (predicted) conditions likely to cause loss of functionality in the near future 
  - firmware has automatically updated settings to stop potential damage to equipment.
  - should be logged 
- ERROR: 
  - loss in functionality of specific sensor or actuator
  - known problem with measured data
  - should be logged
  - generates email to remote lab team
- FATAL: 
  - critical failure, no further functionality of equipment
  - should be logged
  - generates email to remote lab team


# Firmware

Do we want to define a payload structure for sending data and receiving commands?

If firmware is written for Arduino microcontrollers, then a template is available...(TODO)

See (./fw/README.md)[./fw/README.md] for a template of the firmware readme, describing the necessary components of the firmware readme.

# User Interface

User interfaces are created using the [Vue.js](https://vuejs.org/) framework and built for development and deployment using [Vite](https://vitejs.dev/). A template user interface is available...(TODO).

See (./ui/README.md)[./ui/README.md] for details.

# Hardware

See (./hw/README.md)[./hw/README.md] for details.

# Single board computer

See (./sbc/README.md)[./sbc/README.md] for details.

# Analytics

```
{
  "timestamp": 999,	# when the JSON message is sent, may differ from time values in the payload
  "level": DEBUG || INFO || WARN || ERROR || FATAL,	# see description of levels below
  "actor": {
  		"id": USER_ID || EXPERIMENT_ID || ADMIN_ID,
  		"name": null,	#possible if connected with student uni ID
  		"mail": null	# as above
  	},
  "verb": {
  		"id": "start",
  		"params": {x:0, y:0}
  	},
  "object": {
  		"id": UI_DIV_ID || EXPERIMENT_ID || SENSOR_ID,
  		"definition": {"type": "ui"}
  	}
  "context": {},		# may be empty, e.g. state of sensors or other conditions prior to error 
  
}
```


