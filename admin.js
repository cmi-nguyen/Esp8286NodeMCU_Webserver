
window.addEventListener("load", onLoad);
document.addEventListener("DOMContentLoaded", function() {
  
});

var logs=[];

function onLoad(event) {
  updateLoginUI(checkSession());
  initButton();
}
function initButton() {
  document.getElementById("loginBtn").addEventListener("click", login_func);
  document.getElementById("logoutBtn").addEventListener("click", logout);
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
    $("#myTable").show();
    GetDataLogEntry();
  } else {
    $("#sessionUsername").hide();
    $("#loginFormBtn").show();
    $("#logoutBtn").hide();
    $("#myTable").hide();
  }
}
// function GetDataLogEntry() {
//   $.ajax({
//     url: "http://localhost:8080/logs",
//     type: "GET",
//     contentType: "application/json",
//     headers: {
//       "Access-Control-Allow-Origin": "*", // Allow cross-origin requests
//       "Access-Control-Allow-Headers": "Content-Type", // Allow headers
//     },
//     success: function (data) {
//         // Code to add rows to table
//     },
//     error: function (xhr, status, error) {
//       console.error("Error during get data:", error);
//       alert("An error occurred while getting data. Please try again.");
//     },
//   });
// }

function GetDataLogEntry() {
  $.ajax({
    url: "http://localhost:8080/logs",
    type: "GET",
    contentType: "application/json",
    headers: {
      "Access-Control-Allow-Origin": "*", // Allow cross-origin requests
      "Access-Control-Allow-Headers": "Content-Type", // Allow headers
    },
    success: function (data) {
      // Check if the response data is not empty
      if (data && data.length > 0) {
        // Loop through the data and add rows to the table
        data.forEach(function (log, index) {
          // Create a new row
          var newRow = $("<tr></tr>");
          logs.push(log);

          // Create table data for each column
          var rowNumber = $("<td></td>").text(index + 1);
          var action = $("<td></td>").text(log.action); // Assuming 'action' is part of the API response
          var date = $("<td></td>").text(log.date); // Assuming 'date' is part of the API response
          var user = $("<td></td>").text(log.user); // Assuming 'user' is part of the API response

          // Append the columns to the row
          newRow.append(rowNumber, action, date, user);

          // Append the row to the table body
          $("#myTable tbody").append(newRow);
        });
      } else {
        alert("No data available.");
      }
    },
    error: function (xhr, status, error) {
      console.error("Error during get data:", error);
      alert("An error occurred while getting data. Please try again.");
    },
  });
}

 function SearchFiler() {


  // Get the value from the search input and the datetime picker
  const searchTxt = document.getElementById("searchTxt").value.toLowerCase();
  const selectedDate = document.getElementById("datetimepicker").value;

  // Convert selected date to a Date object
  const selectedDateTime = new Date(selectedDate);

  // Filter data based on the search text and date
  const filteredData = logs.filter(item => {
    const itemDate = new Date(item.date);
    const actionMatch = item.action.toLowerCase().includes(searchTxt);
    const userMatch = item.user.toLowerCase().includes(searchTxt);
    const dateMatch = selectedDate ? itemDate > selectedDateTime : true; // If a date is selected, filter by date

    return (actionMatch || userMatch) && dateMatch;
  });

  // Call the function to display the filtered data
  displayData(filteredData);
};
// Function to display filtered data in the table
function displayData(filteredData) {
  const tableBody = document.querySelector("#myTable tbody");
  tableBody.innerHTML = ""; // Clear the table body before adding new rows

  filteredData.forEach((log, index) => {
    // Create a new row
    const newRow = document.createElement("tr");

    // Create cells for each column
    const rowNumber = document.createElement("td");
    rowNumber.textContent = index + 1;
    
    const action = document.createElement("td");
    action.textContent = log.action;

    const date = document.createElement("td");
    date.textContent = log.date;

    const user = document.createElement("td");
    user.textContent = log.user;

    // Append cells to the row
    newRow.append(rowNumber, action, date, user);

    // Append the row to the table body
    tableBody.appendChild(newRow);
  });
}