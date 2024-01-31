<?php

$response = array();

require_once __DIR__ . '/database_connect.php';

$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();
$query = "SELECT * FROM test_commands" ;
$result = mysqli_query($connection , $query);
$myArray = array();
while($row = $result->fetch_assoc()) {
	$myArray[] = $row;
}
echo json_encode($myArray);
$databaseConnection->closeDataBaseConnection();

?>
