<?php 
//http://127.0.0.1/log.php?card=23563ggfhfgf&state=true
DEFINE ('DBServer', '127.0.0.1'); 
DEFINE ('DBName', 'lzzlock');
DEFINE ('DBUser', 'root'); 
DEFINE ('DBPw', '');  

$conDb = mysqli_connect(DBServer,DBUser,DBPw);
if (!$conDb) {
    die("Can not connect to DB: " . mysqli_error($conDb));
    exit();
}

$selectDb = mysqli_select_db($conDb, DBName);
if (!$selectDb) {
    die("Database selection failed: " . mysqli_error($conDb));
    exit(); 
}


$card = mysqli_real_escape_string($conDb, $_GET['card']);
$state = mysqli_real_escape_string($conDb,$_GET['state']);
$datetime = date_create()->format('Y-m-d H:i:s');

$query = "INSERT INTO locklog (card, state, datetime) VALUES ('$card', '$state', '$datetime')";

$result = mysqli_query($conDb, $query) or trigger_error("query error " . mysqli_error($conDb)); 

mysqli_close($conDb); 

?>