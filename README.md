# OpenGL Graphics Injector

## Demo

### Injecting into SuperTuxKart

![Text ontop of the SuperTuxKart game.](demo-supertuxkart.png)

### Injecting onto glxgears

![Text ontop of the GLXGears application.](demo-glxgears.png)

## What is this about?

This project serves as a sample on how to inject custom graphics ontop of
programs using OpenGL to render their imagery. A full instance of the cairo 2d rendering
system (which may be used in the new C++ graphics standard extension) is loaded
into the context of the started program and all functions of the rendering toolkit
can be used inside of a Lua environment, running next to the normal program.

## Features

  - Full color rendering of text, images and geometric shapes.
  - Live-updates.
  - Capturing keyboard inputs and prohibiting them from being entered into the game.
  - Possible application in the future: OCR of OpenGL based applications to be read aloud to visually impaired people.

## Running the injector

    # Download the repository.
    git clone https://github.com/maximaximal/opengl-injector.git
	cd opengl-injector
	
	# Create directory and build project.
	mkdir build
	cd build
	cmake ..
	make -j4
	
	# Set environment variable on where to find scripts and run with preloaded library.
	export INJECTOR_PATH=../injector
	LD_PRELOAD="./injector/libinjector.so" glxgears

Controls inside the application can be read in [injector/lua/options.lua](./injector/lua/options.lua).
To sum up: Tab for showing "settings" screen, F1 for the "virtual keyboard". No actual functionality has
been implemented for this demo.

## Origin of this project

This project originated in the quest to build a completely DIY gaming console. 
Because such a task turned out to be bigger than expected (and not enough
time could be found), the most interesting projects out of this endeavour 
can still be harvested for good content. This is why the injector part of the 
graphics stack is now online and may be of interest to other people.

## License

All code written by me is under MIT License, other parts are highlighted as such.
