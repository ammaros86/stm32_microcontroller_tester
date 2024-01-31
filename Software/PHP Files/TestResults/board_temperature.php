
<?php
// array for JSON response
$response = array();

// include db connect class
require_once __DIR__ . '/../database_connect.php';


// connecting to db
$connection = new DataBaseConnection();
$conn = $connection->openDataBaseConnection();

	$board_id = $_POST['board_id'];
	$query = "SELECT * FROM board_temperature WHERE board_id = '$board_id'" ;
	$result = mysqli_query($conn, $query);
	echo json_encode($result->fetch_assoc());
$connection->closeDataBaseConnection();

?>
