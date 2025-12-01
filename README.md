# UV-Light-Timer
This project uses the results from a UV and light sensor to determine how long you spent outside, inside, or inside by a window. A summary report will also be displayed at the end. It uses the UV index and time to determine what percentage of your daily vitamin D was produced, along with the risk of sunburn.<br> 

When the button is pressed, the LED will turn on to indicate that the timer has started, and the sensors will start recording values. Once the button is pressed again, the LED will turn off, and averages for light and UV index will be calculated. Information regarding light level and UV index averages, how long you were outside, inside, or inside by a window, and the summary report will be displayed.<br>
 

# Hardware
Arduino board (I have an Arduino UNO)<br>
Button<br>
LED<br>
BH1750 light sensor<br>
CJMCU-GUVA-S12SD UV sensor<br>

# Software
Arduino IDE<br>
Arduino Library:<br>
BH1750 by Christopher Laws

# Reason for this project
I came up with this idea to figure out if sitting by a window could offer similar effects as being outside and eventually calcuate how much vitamin D someone has gained from the sun in a length of time. The calculations for vitamin D production and risk of sunburn in this project are currently rather arbitrary values based on loose ranges from my research. However, this project is a first step/proof of concept for my ultimate goal explained above. I felt this was important to pursue because it can provide insight on how an average person can more easily measure and leverage the health benefits gained from just the sun. 
