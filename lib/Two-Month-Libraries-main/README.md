# Two-Month-Libraries
Sensor libraries for SHC Two Month 2025


## Usage
Simply clone this repository to your Arduino libraries folder and you should be able to just use them easily. 
They are set up to use I2C on `Wire`, so to change the pins used you can use the `Wire.setSDA(newPin0)` and the `Wire.setSCL(newPin1)` functions.

## Notes about usage
1. You must pull the SDO pin on the BME280 low so that it switches addresses from 0x77 to 0x76, which the code expects.
2. You must call `Wire.begin()` before `init()` of any of the sensors to ensure I2C is actually running.


# Credits
We only wrote wrappers for some libraries to make them a bit more accessible, we did **not** write the actual libraries themselves.
They are amazing libraries in and of themselves, but we wanted to use them a specific way. All credit for their work goes to them of course.
