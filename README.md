# ESP32 snake-game using pushbuttons
### Setting up a simple game (snake-game) on ESP32 server  and controlling the game through pushbuttons on ESP32 circuit board


1) A snake-game is hosted on an ESP32 server as shown in the code using **Webserver** library. 
2) Javascript game code can be found [here](https://github.com/yakkomajuri/brython-snake).
3) An ESP32 circuit is build with 4 pushbuttons on it.
4) These pushbuttons act as up-down-left-right arrow keys of a computer.
5) [BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) library is used for connecting ESP32 with device's bluetooth. Subsequent control signals from pushbuttons are sent to the device via bluetooth.
6) The assembly of the circuit and the game can be figured out from the image below.

![circuit]()  ![game]()
