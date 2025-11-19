# UV-Light-Timer
This project uses the results from a UV and light sensor to determine how long you spent outside, inside, or inside by a window. A summary report will also be displayed at the end. It uses the UV index and time to determine what percentage of your daily vitamin D was produced along with the risk of sunburn.<br> 

When the button is pressed, the LED will turn on to indicate that the timer has started, and the sensors will start recording values. After each reading, a new average will be calculated for both the light and UV values. Once the button is pressed again, the LED will turn off. Information regarding the final averages for the light level and UV values, how long you were outside, inside, or inside by a window, and the summary report will be displayed.<br>
 

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
I came up with this idea to figure out if sitting by a window could offer simular effects as being outside and eventually calcuate how much vitamin D someone is getting in a length of time. This project is a first step/proof of concept for determining if the person using this was outside, inside, or inside by a window, and how much UV they were exposed to. I felt this was important because the sun provides many necessary health benefits. 
