#include <BleKeyboard.h>
#include <WiFi.h>
#include <WebServer.h>

BleKeyboard bleKeyboard;


WebServer server(80);
 
const char* ssid = "      "; // Add the ssid of your network
const char* password =  "       "; // Add the passowrd of your WiFi network.

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JavaScript Snake</title>

    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css"
        integrity="sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk" crossorigin="anonymous">

    <style>

        h1 {
            margin-top: 8%;
            color: rgb(83, 238, 36);
        }

        h3 {
            color: rgb(190, 187, 187);
        }

        h6 {
            color: rgb(117, 117, 117);
        }

        a {
            color: rgb(117, 117, 117);
        }

        button {
            display: block;
            margin-left: auto;
            margin-right: auto;
            background-color: transparent !important;
            border-color: rgb(155, 156, 155) !important;
            color: rgb(155, 156, 155) !important;
            font-size: 8px !important;
            padding: 3px !important;
        }

        button:hover {
            border-color: rgb(68, 241, 68) !important;
            color: rgb(194, 196, 194) !important;
        }

        body {
            background-color: black;
        }

        canvas {
            margin-top: 1%;
            padding-left: 0;
            padding-right: 0;
            margin-left: auto;
            margin-right: auto;
            display: block;
            border: 1px;
            border-style: solid;
            border-color: #535353;
        }
    </style>

</head>


<body>
    <h1 class="text-center">Snake built with JavaScript!</h1>
    <h6 class="text-center">Not that impressive... Check out <a href="brython-snake.html">snake built with Python - in browser!</a></h1>
    <canvas id="game-board" width="400" height="400"></canvas>
    <br>
    <h3 id="score" class="text-center">Score: 0</h3>
    <br>
    <h6 id="high-score" class="text-center">High Score: 0</h6>
    <br>
    <div class="text-center">
        <button id="instructions-btn" class="btn btn-info">Instructions</button>
    </div>
    <script>

        /*
         Original Code Credits: Chris DeLeon of HomeTeam GameDev 
         Modified by: Yakko Majuri
        */

        'use strict'

        let canvas, ctx;

        window.onload = function () {
            canvas = document.getElementById("game-board");
            ctx = canvas.getContext("2d");
            document.addEventListener("keydown", keyPush);
            setInterval(game, 1000 / 10);
            let instructionsBtn = document.getElementById("instructions-btn");
            instructionsBtn.addEventListener("click", showInstructions)
        }

        let score = 0;
        let highScore = 0;

        let px = 10;
        let py = 10;
        let gs = 20;
        let tc = 20;
        let ax = 15;
        let ay = 15;
        let xv = 0;
        let yv = 0;
        let trail = [];
        let tail = 5;

        let paused = false
        let pre_pause = [0, 0]



        function game() {
            px += xv;
            py += yv;
            if (px < 0) {
                px = tc - 1;
            }
            if (px > tc - 1) {
                px = 0;
            }
            if (py < 0) {
                py = tc - 1;
            }
            if (py > tc - 1) {
                py = 0;
            }
            ctx.fillStyle = "black";
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.fillStyle = "lime";
            for (var i = 0; i < trail.length; ++i) {
                ctx.fillRect(trail[i].x * gs, trail[i].y * gs, gs - 2, gs - 2);
                if (trail[i].x == px && trail[i].y == py) {
                    tail = paused ? tail : 5;
                    score = paused ? score : 0;
                }
            }
            trail.push({ x: px, y: py });
            while (trail.length > tail) {
                trail.shift();
            }

            if (ax == px && ay == py) {
                ++score;
                ++tail;
                ax = Math.floor(Math.random() * tc);
                ay = Math.floor(Math.random() * tc);
            }
            ctx.fillStyle = "red";
            ctx.fillRect(ax * gs, ay * gs, gs - 2, gs - 2);
            updateScore(score)
        }

        function updateScore(newScore) {
            document.getElementById("score").innerHTML = "Score: " + String(newScore);
            if (newScore > highScore) {
                document.getElementById("high-score").innerHTML = "High Score: " + String(newScore);
                highScore = newScore;
            }
        }

        function keyPush(evt) {
            if (!paused) {
                switch (evt.keyCode) {
                    case 37:
                        xv = -1; yv = 0;
                        break;
                    case 38:
                        xv = 0; yv = -1;
                        break;
                    case 39:
                        xv = 1; yv = 0;
                        break;
                    case 40:
                        xv = 0; yv = 1;
                        break;
                }
            }
            if (evt.keyCode === 32) {
                let temp = [xv, yv];
                xv = pre_pause[0];
                yv = pre_pause[1];
                pre_pause = [...temp];
                paused = !paused;
            }
        }

        function showInstructions() {
            window.alert("Use the arrow keys to move and press spacebar to pause the game.")
        }

    </script>

</body>
</html>
)rawliteral";

int up = 19; // GPIO 19 for up pushbutton
int down = 4; // GPIO 4 for down pushbutton
int left = 5; // GPIO 5 for left pushbutton
int right = 18;// GPIO 18 for right pushbutton.

 
 void setup(){
  Serial.begin(115200);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);

  bleKeyboard.begin();
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 

  server.on("/html", []() {
    server.send(200, "text/html", HTML);
  });

  server.begin();
  
}
 
void loop()
{
  server.handleClient();
  if(digitalRead(up)==LOW)
  {
    bleKeyboard.press(KEY_UP_ARROW);
  }

  if(digitalRead(down)==LOW)
  {
    bleKeyboard.press(KEY_DOWN_ARROW);
  }

  if(digitalRead(left)==LOW)
  {
    bleKeyboard.press(KEY_LEFT_ARROW);
  }

  if(digitalRead(right)==LOW)
  {
    bleKeyboard.press(KEY_RIGHT_ARROW);
  }


  delay(100);
  bleKeyboard.releaseAll();
}
