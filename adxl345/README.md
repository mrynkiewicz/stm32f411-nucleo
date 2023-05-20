
# STM32F411 NUCLEO accelerometer reading with Low Pass FIR

Simple example showing how low pass FIR (**Finite Response Filter**) filter can help smoothen value changes read from accelerometer.
Code was deployed on STM32F411RE Nucleo board. I bought cheap ADXSL345 accelerometer installed on breakout board. The adxsl345 chip
provides I2C and SPI interfaces, I decided to try I2C. Accelerometer was configured to take measures with 100[Hz] frequency, and
to trigger DATA_READY interrupt on pin INT1. All drivers are written in bare metal mode, with assumption that SYSCLK is 16[Mhz].
This is the default clock setup when you power up stm32f411 mcu. I wasn't sure if such speed will be sufficient enough to handle filtering
with sixteen coefficients, but turned out it's enough. Increasing clock speed would require adjustment in configuration of I2C interface
(stm needs to know what is peripheral bus speed), as well adjustment in UART driver - peripheral clock is used to define proper baud rate.

Whole project was compiled, and uploaded to the dev board with STM32CubeIDE version 1.7.0.

Screenshot below shows output from the x-axis readout of the accelerometer. Red trace shows raw data from adxsl345, blue trace is filtered output.
I used serial ploter from Arduino Studio to present data in graphical form. It's pretty useful tool when you want to plot graph based on input send
via serial port.

![screenshot of the accelerometer traces with and without filtering](https://github.com/mrynkiewicz/stm32f411-nucleo/blob/main/adxl345/LP_FIR_Filter_2.jpg)