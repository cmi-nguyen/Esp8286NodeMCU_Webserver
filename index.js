var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener("load", onLoad);
// function initWebSocket() {
//     console.log('Trying to open a WebSocket connection...');
//     console.log(gateway);
//     websocket = new WebSocket(gateway);
//     websocket.onopen = onOpen;
//     websocket.onclose = onClose;
//     websocket.onmessage = onMessage; // <-- add this line
// }
function onOpen(event) {
  console.log("Connection opened");
}
function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
  var state;
  var state1;
  var tempurature;
  var doorState;
  if (event.data == "LED0 1") {
    state = "ON";
    SaveDataLogEntry("Bật đèn 1");
  } else if (event.data == "LED0 0") {
    state = "OFF";
    SaveDataLogEntry("Tắt đèn 1");
  }

  if (event.data == "LED1 0") {
    state1 = "OFF";
    SaveDataLogEntry("Tắt đèn 2");
  } else if (event.data == "LED1 1") {
    state1 = "ON";
    SaveDataLogEntry("Bật đèn 2");
  }

  if (state1 != undefined) {
    document.getElementById("state1").innerHTML = state1;
  }
  if (state != undefined) {
    document.getElementById("state").innerHTML = state;
  }
}
function onLoad(event) {
  // initWebSocket();
  updateLoginUI(checkSession());
  initButton();
}
function initButton() {
  document.getElementById("button").addEventListener("click", toggle);
  document.getElementById("button1").addEventListener("click", toggle1);
  document.getElementById("button2").addEventListener("click", toggle2);
  document.getElementById("loginBtn").addEventListener("click", login_func);
}
function toggle() {
  if (!checkSession()) {
    alert("You must Login  first");
  } else websocket.send("toggle");
}
function toggle1() {
  if (!checkSession()) {
    alert("You must Login  first");
  } else websocket.send("toggle1");
}
function toggle2() {
  if (!checkSession()) {
    alert("You must Login  first");
    return;
  } else websocket.send("toggle2");
}
function login_func() {
  const username = $("#userNameInput").val();
  const password = $("#passwordInput").val();
  // Validate inputs

  // Prepare the request body
  const requestBody = {
    accountName: username,
    password: password,
  };

  // Send the AJAX request
  $.ajax({
    url: "http://localhost:8080/accounts/login",
    type: "POST",
    contentType: "application/json",
    data: JSON.stringify(requestBody),
    headers: {
      "Access-Control-Allow-Origin": "*", // Allow cross-origin requests

      "Access-Control-Allow-Headers": "Content-Type", // Allow headers
    },
    success: function (data) {
      if (data) {
        // Save session
        sessionStorage.setItem("user", JSON.stringify(data));

        alert("Login successful!");

        updateLoginUI(checkSession());
      } else {
        alert("Invalid credentials!");
      }
    },
    error: function (xhr, status, error) {
      console.error("Error during login:", error);
      alert("An error occurred while logging in. Please try again.");
    },
  });
}
function logout() {
  sessionStorage.removeItem("user");
  alert("You have been logged out!");
  location.reload();
}
function checkSession() {
  const user = sessionStorage.getItem("user");
  if (user) {
    return true;
  } else return false;
}
function updateLoginUI(isLogin) {
  if (isLogin) {
    const user = JSON.parse(sessionStorage.getItem("user"));
    $("#sessionUsername").html(`Hello, ${user.accountName}`);
    $("#sessionUsername").show();
    $("#loginFormBtn").hide();
    $("#logoutBtn").show();
  } else {
    $("#sessionUsername").hide();
    $("#loginFormBtn").show();
    $("#logoutBtn").hide();
  }
}
function SaveDataLogEntry(action) {
  // Create request
  const user = JSON.parse(sessionStorage.getItem("user"));
  var date= new Date();
  const requestBody ={
    "Id": 0,
    "user": `${user.accountName}`,
    "action": action,
    "date": date.toLocaleString()
  }
  $.ajax({
    url: "http://localhost:8080/logs",
    type: "POST",
    contentType: "application/json",
    data: JSON.stringify(requestBody),
    headers: {
      "Access-Control-Allow-Origin": "*", // Allow cross-origin requests
      "Access-Control-Allow-Headers": "Content-Type", // Allow headers
    },
    success: function (data) {},
    error: function (xhr, status, error) {
      console.error("Error during posting data:", error);
      alert("An error occurred while posting data. Please try again.");
    },
  });
}
