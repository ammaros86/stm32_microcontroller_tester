
<?php

$response = array();

require_once __DIR__ . '/database_connect.php';

$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();
$query = "SELECT * FROM control_commands" ;
$result = mysqli_query($connection, $query);
echo json_encode($result->fetch_assoc());
$databaseConnection->closeDataBaseConnection();

?>
