#include <LittleFS.h>











/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include <I2S.h>

#include <SoftwareSerial.h>





// Replace with your network credentials
const char *ssid = "LAM HOUSE";
const char *password = "xinchaoban";
// Other variables
// Servo
Servo servo;
int pos = 0;
bool doorState = 0;
// LED
bool ledState = 0;
bool ledState2 = 0;
const int ledPin = 14;
const int ledPin2 = 12;
// Sensor
const int US100_TX = 4;
const int US100_RX = 5;
 
SoftwareSerial US100Serial(US100_RX, US100_TX);
 
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int mmDist = 0;
int temp = 0;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Web Client</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            text-align: center;
        }

        h1 {
            font-size: 1.8rem;
            color: white;
        }

        h2 {
            font-size: 1.5rem;
            font-weight: bold;
            color: #143642;
        }

        .topnav {
            overflow: hidden;
            background-color: #143642;
        }

        body {
            margin: 0;
        }

        .content {
            padding: 30px;
            max-width: 600px;
            margin: 0 auto;
        }

        .card {
            background-color: #F8F7F9;
            ;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
            padding-top: 10px;
            padding-bottom: 20px;
        }

        .button {
            padding: 15px 50px;
            font-size: 24px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #0f8b8d;
            border: none;
            border-radius: 5px;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        /*.button:hover {background-color: #0f8b8d}*/
        .button:active {
            background-color: #0f8b8d;
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
        }

        .state {
            font-size: 1.5rem;
            color: #8c8c8c;
            font-weight: bold;
        }
    </style>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
   

</head>

<body>
    <div class="topnav">
        <div style="display: flex; justify-content: space-between; align-items: center;">
            <div style="margin-left: 20px;">
                <h1>ESP WebSocket Client</h1>
            </div>
            <div id="LoginDiv" style="margin-right: 20px;">
                <button type="button" class="btn btn-primary" data-bs-toggle="modal" id="loginBtn"
                    data-bs-target="#exampleModal">
                    Login
                </button>
            </div>
        </div>


    </div>
    <div class="container">

        <div class="row" style="margin-top: 20px;">
            <div class="col">
                <div class="card">
                    <h2>Sensors</h2>
                    <p class="state">Current Tempurature: <span id="state1">24*C</span></p>
                    <p><button class="button" style="visibility: hidden;">Toggle</button></p>
                </div>
            </div>
            <div class="col">
                <div class="card">
                    <h2>Door</h2>
                    <p class="state">state: <span id="state">%STATE%</span></p>
                    <p><button id="button2" class="button">Toggle</button></p>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col">
                <div class="card">
                    <h2>LED 1</h2>
                    <p class="state">state: <span id="state1">%STATE1%</span></p>
                    <p><button id="button" class="button">Toggle</button></p>
                </div>
            </div>
            <div class="col">
                <div class="card">
                    <h2>LED 2</h2>
                    <p class="state">state: <span id="state">%STATE%</span></p>
                    <p><button id="button1" class="button">Toggle</button></p>
                </div>
            </div>
        </div>




    </div>

    <!-- Login Modal -->
    <div class="modal fade" id="exampleModal" tabindex="-1" aria-labelledby="exampleModalLabel" aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="exampleModalLabel">Login</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <div class="modal-body">
                    <label for="userNameInput" class="form-label">User Name</label>
                    <input type="text" class="form-control" id="userNameInput" placeholder="Username">
                    <label for="passwordInput" class="form-label">Password</label>
                    <input type="password" class="form-control" id="passwordInput" placeholder="Enter Password">
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                    <button type="button" class="btn btn-primary">Login</button>
                </div>
            </div>
        </div>
    </div>
    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener('load', onLoad);
        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            console.log(gateway);
            websocket = new WebSocket(gateway);
            websocket.onopen = onOpen;
            websocket.onclose = onClose;
            websocket.onmessage = onMessage; // <-- add this line
        }
        function onOpen(event) {
            console.log('Connection opened');
        }
        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }
        function onMessage(event) {
            var state;
            var state1;
            var tempurature;
            var doorState;
            if (event.data == "LED0 1") {
                state = "ON";
            } else if (event.data == "LED0 0") {
                state = "OFF";
            }

            if (event.data == "LED1 0") {
                state1 = "OFF";
            } else if (event.data == "LED1 1") {
                state1 = "ON";
            }



            if (state1 != undefined) {
                document.getElementById('state1').innerHTML = state1;
            }
            if (state != undefined) {
                document.getElementById('state').innerHTML = state;
            }

        }
        function onLoad(event) {
            initWebSocket();
            initButton();
        }
        function initButton() {
            document.getElementById('button').addEventListener('click', toggle);
            document.getElementById('button1').addEventListener('click', toggle1);
            document.getElementById('button2').addEventListener('click', toggle2);
            document.getElementById('loginBtn').addEventListener('click', login_func);
        }
        function toggle() {
            websocket.send('toggle');
        }
        function toggle1() {
            websocket.send('toggle1');
        }
        function toggle2() {
            websocket.send('toggle2');
        }
        function login_func() {
            // login goes here

        }
    </script>
   
    
</body>

</html>
)rawliteral";

void notifyClients() {
  ws.textAll("LED0 " + String(ledState));
  ws.textAll("LED1 " + String(ledState2));
  ws.textAll("DOOR " +String(doorState));
  ws.textAll("Temp "+String(temp));
  Serial.println("LED0 " + String(ledState)+"|"+"LED1 " + String(ledState2)+"|"+"DOOR " +String(doorState)+"|"+"Temp "+String(temp));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char *)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
    if (strcmp((char *)data, "toggle1") == 0) {
      ledState2 = !ledState2;
      notifyClients();
    }
    if (strcmp((char *)data, "toggle2") == 0) {
      doorState = !doorState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String &var) {
  Serial.println(var);
  String data;
  if (var == "STATE") {
    if (ledState) {
      data+= "ON,";
    } else {
      data+= "OFF,";
    }
    if (ledState2) {
      data+= "ON,";
    } else {
      data+= "OFF,";
    }
    if(doorState){
      data+= "OPEN,";
    }else{
      data+="CLOSED,";
    }
    data+=temp;
  }
  
 
  
  
  return data;
}
// String processor2(const String &var) {
//   Serial.println(var);
//   if (var == "STATE1") {
//     if (ledState2) {
//       return "ON";
//     } else {
//       return "OFF";
//     }
//   }
//   return String();
// }

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  US100Serial.begin(9600);

   if(LittleFS.begin()==false){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
 
   
    File fileToRead = LittleFS.open("/index.js", "r");
    if (!fileToRead) {
        Serial.println("Failed to open /index.js");
    } else {
        Serial.println("File opened successfully:");
        while (fileToRead.available()) {
            Serial.write(fileToRead.read());
        }
        fileToRead.close();
    }
  
File fileToRead2 = LittleFS.open("/index.html", "r");
    if (!fileToRead2) {
        Serial.println("Failed to open /index.html");
    } else {
        Serial.println("File opened successfully:");
        while (fileToRead2.available()) {
            Serial.write(fileToRead2.read());
        }
        fileToRead2.close();
    }

 


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);
  servo.attach(D4);
  servo.write(-1);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
     request->send(LittleFS, "/index.html","text/html", false,processor);
    //request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    // request->send(LittleFS, "./index.html","text/html", false,processor);
    request->send(LittleFS, "/index.js", "text/javascript");
  });


  // Start server
  server.begin();
}

void loop() {
 

  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
  digitalWrite(ledPin2, ledState2);
  US100Serial.flush();
  US100Serial.write(0x55);

  delay(300);
  if (US100Serial.available() >= 2) {
    MSByteDist = US100Serial.read();
    LSByteDist = US100Serial.read();
    mmDist = MSByteDist * 256 + LSByteDist;
    if ((mmDist > 1) && (mmDist < 10000)) {
      // Serial.print("Distance: ");
      // Serial.print(mmDist, DEC);
      // Serial.println(" mm");
      
    }
    if (mmDist < 50) {
      if (ledState2){
        ledState2 =!ledState2;
        digitalWrite(ledPin2, ledState2);
        notifyClients();
      }
      
    }
  }

  US100Serial.flush();
  US100Serial.write(0x50);

  delay(300);
  if (US100Serial.available() >= 1) {
    temp = US100Serial.read();
    if ((temp > 1) && (temp < 130))  // temprature is in range
    {
      temp -= 45;  // correct 45º offset
      // Serial.print("Temp: ");
      // Serial.print(temp, DEC);
      // Serial.println(" ºC.");
    }
  }

  delay(500);
  if (doorState){
    closeDoor();
    //Serial.print(pos);
  }
  if(!doorState){
    openDoor();
    //Serial.print(pos);
  }

}
void openDoor() {

for (pos = 0; pos < 180; pos += 10) {  // rotate from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo.write(pos);  // tell servo to go to position in variable 'pos'
      delay(10);         // waits 10ms for the servo to reach the position
    }
  
}
void closeDoor() {

for (pos = 180; pos > 0; pos -= 10) {  // rotate from 180 degrees to 0 degrees
      servo.write(pos);                    // tell servo to go to position in variable 'pos'
      delay(10);                           // waits 10ms for the servo to reach the position
    }

    
}
