# STRATOSAT ORION

This repo will work best with VSCode on PlatformIO. 

If you are not familiar with them, see steps below: 


# Step 1: Install and set up VSCode



https://code.visualstudio.com/download?_exp_download=d53503e735

Installation should be simple, with the application simply working as is, but if there seems to be issues, read through Microsoft’s Advanced Setup guides
https://code.visualstudio.com/docs/setup/windows




## Step 2: Install a compiler.

Although VSCode does come with its own compiler after installing the C++ extension, I always recommend having multiple compilers for good practice. 

For Windows users, I recommend installing the gcc compiler via MinGW.
Follow the instructions here: https://code.visualstudio.com/docs/cpp/config-mingw
For Linux users g++ should come with the system: 
https://code.visualstudio.com/docs/cpp/config-linux
For MacOS users, you’ll have to use clang: 
https://code.visualstudio.com/docs/cpp/config-clang-mac



## Step 3: Install PlatformIO
Link: 
https://platformio.org/platformio-ide

PlatformIO will be an extension on VSCode, so you can just search it up on the extensions tab.




## Step 4: Set Up a Project
To set up a project, press the “New Project” button, or if you or someone you work with has a Github repository created, you can click on the “Clone Github Project” and paste the repository’s HTTPS link.  

If PlatformIO isn’t open to the screen yet, you can click on the PlatformIO tab to the side of the screen, and press “Open” to pull up the menu.

Congratulations, you have successfully installed VSCode and set up a project!



# CORE LOGIC

In void setup(), the sensors will be initialized and ready to begin taking measurements. Our function void loop() will take measurements and blink the LEDs built into the payload every second. 

The function CheckState() will be our finite state machine. 

The stabilization algorithm uses a Bang-Bang Controller and communicates to defined pin numbers at the top of the file, it is built into the loop and only triggered if a state is satisfied. 

Apologies in advance for readability issues, I am exhausted. 

