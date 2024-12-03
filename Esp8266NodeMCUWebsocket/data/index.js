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