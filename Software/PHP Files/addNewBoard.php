<?php

$response = array();
 
require_once __DIR__ . '/database_connect.php';    
$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();
if (isset($_POST['boardName'])){
 	$boardName = $_POST['boardName'];

	$query = "INSERT INTO test_commands(boardName)VALUES('$boardName')";
	$result = mysqli_query($connection, $query);
	if($result) {
 		$response["success"] = "done";
 		echo json_encode($response);
	} else {
 		$response["success"] = "failed to insert row";
 		echo json_encode($response);
	}
}else{
 	$response["success"] = "failed boardName missing";
 	echo json_encode($response);
}
       
$databaseConnection->closeDataBaseConnection();
?>