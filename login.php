<?php
// Include config file
$conn = require_once "config.php";
 
// Define variables and initialize with empty values
$username = "";
$password = "";

// Processing form data when form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    $sql = "SELECT * FROM users WHERE username = '$username'";
    $result = mysqli_query($conn, $sql);

    if ($result && mysqli_num_rows($result) == 1) {
        $row = mysqli_fetch_assoc($result);
        if (password_verify($password, $row["password"])) {
            session_start();
            // Store data in session variables
            $_SESSION["loggedin"] = true;
            $_SESSION["id"] = $row["id"];
            $_SESSION["username"] = $row["username"];
            header("Location: welcome.php");
            exit();
        } else {
            function_alert("帳號或密碼錯誤"); 
        }
    } else {
        function_alert("帳號或密碼錯誤"); 
    }
} else {
    function_alert("Something wrong"); 
}

// Close connection
mysqli_close($conn);

function function_alert($message) { 
    // Display the alert box  
    echo "<script>alert('$message');
    window.location.href='index.php';
    </script>"; 
    return false;
} 
?>
