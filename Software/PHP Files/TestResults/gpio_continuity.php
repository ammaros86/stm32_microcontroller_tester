
<?php
// array for JSON response
$response = array();

// include db connect class
require_once __DIR__ . '/../database_connect.php';

// connecting to db
$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection ->openDataBaseConnection();

	$board_id = $_POST['board_id'];
	$query = "SELECT * FROM gpio_continuity WHERE board_id = '$board_id'" ;
	$result = mysqli_query($connection, $query);
	$myArray = array();
	while($row = $result->fetch_assoc()) {
		$myArray[] = $row;
	}
	echo json_encode($myArray);
$databaseConnection->closeDataBaseConnection();

?>