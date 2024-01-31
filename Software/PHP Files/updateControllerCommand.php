<?php

$response = array();
$y = 0;

require_once __DIR__ . '/database_connect.php';

$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();
 

$copy_logs_usb= $_POST['copy_logs_usb'];
$turn_usb_port_on= $_POST['turn_usb_port_on'];
$power_off= $_POST['power_off'];
$test_value = $_POST['test_value'];

$query = "UPDATE control_commands SET copy_logs_usb= '$copy_logs_usb', turn_usb_port_on = '$turn_usb_port_on', power_off = '$power_off' ";
$result = mysqli_query($connection, $query);
if($result) {
     $response["message"] = "done";
}else{
    $response["message"] = "failed";
   
}
 echo json_encode($response);
$databaseConnection->closeDataBaseConnection();

?>