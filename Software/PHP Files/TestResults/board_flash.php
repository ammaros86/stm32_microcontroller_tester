
<?php
// array for JSON response
$response = array();

// include db connect class
require_once __DIR__ . '/../database_connect.php';

// connecting to db
$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();

	$board_id = $_POST['board_id'];
	$query = "SELECT * FROM board_flash WHERE board_id = '$board_id'" ;
	$result = mysqli_query($connection, $query);
	echo json_encode($result->fetch_assoc());
$databaseConnection->closeDataBaseConnection();

?>
