<!DOCTYPE html>

<?php
// Initialize the session
session_start();
 
// Check if the user is already logged in, if yes then redirect him to welcome page
if(isset($_SESSION["loggedin"]) && $_SESSION["loggedin"] === true){
    header("location: admin.php");
    exit;  //記得要跳出來，不然會重複轉址過多次
}
?>

<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>LZZ家庭智慧智慧門鎖管理介面</title>
  <link rel="stylesheet" href="css/bootstrap.css"> <!--宣告 CSS-->
  <link rel="stylesheet" href="css/style.css">
  <script src="js/bootstrap.bundle.min.js"></script> <!--宣告 JS-->
</head>
<body>
<div class="container">
    <div class="row">
      <div class="col-md-6 offset-md-3">
        <h2 class="text-center text-dark mt-5">LZZ家庭智慧智慧門鎖管理介面</h2>
        <div class="text-center mb-5 text-dark">帶給您最專業的數據分析</div>
        <div class="card my-5">

          <form class="card-body cardbody-color p-lg-5" method="post" action="login.php">

            <div class="text-center">
              <img src="https://cdn.pixabay.com/photo/2016/03/31/19/56/avatar-1295397__340.png" class="img-fluid profile-image-pic img-thumbnail rounded-circle my-3"
                width="200px" alt="profile">
            </div>

            <div class="mb-3">
              <input type="text" class="form-control" name="username" aria-describedby="emailHelp"
                placeholder="管理者帳號">
            </div>
            <div class="mb-3">
              <input type="password" class="form-control" name="password" placeholder="管理者密碼">
            </div>
            <div class="text-center"><button type="submit" class="btn btn-color px-5 mb-5 w-100">登入</button></div>
			 <div id="emailHelp" class="form-text text-center mb-5 text-dark">你沒有帳號嗎? <a href="register.html" class="text-dark fw-bold"> 前往註冊</a>
            </div>
            </div>
          </form>
        </div>

      </div>
    </div>
  </div>
</body>
</html>
