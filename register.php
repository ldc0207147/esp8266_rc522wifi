<?php
$conn = require_once("config.php");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];
    
    // 檢查帳號是否重複
    $check = "SELECT * FROM users WHERE username='" . $username . "'";
    if (mysqli_num_rows(mysqli_query($conn, $check)) == 0) {
        $password_hash = password_hash($password, PASSWORD_DEFAULT);
        $sql = "INSERT INTO users (id, username, password) VALUES (NULL, '$username', '$password_hash')";
        
        if (mysqli_query($conn, $sql)) {
            echo "註冊成功!3秒後將自動跳轉頁面<br><a href='index.php'>未成功跳轉頁面請點擊此</a>";
            header("refresh:3;url=register.html");
            exit;
        } else {
            echo "Error creating table: " . mysqli_error($conn);
        }
    } else {
        echo "該帳號已有人使用!<br>3秒後將自動跳轉頁面<br><a href='register.html'>未成功跳轉頁面請點擊此</a>";
        header("refresh:3;url=register.html");
        exit;
    }
}

mysqli_close($conn);

function function_alert($message) {
    // Display the alert box  
    echo "<script>alert('$message');
     window.location.href='index.php';
    </script>";
    return false;
}
?>
