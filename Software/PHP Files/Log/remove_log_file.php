<?php
$response = array();

if (isset($_POST['logfilename'])) {
    $logfilename= $_POST['logfilename'];
 
    $logfilePath = '/var/www/LogFiles/' . $logfilename ;
    if (file_exists($logfilePath)) {
        if (unlink($logfilePath)) {
 	    $response["response"] = "success";
        } else {
 	    $response["response"] = "error";
        }
 
    } else {
 
     $response["response"] = "unknown path";	
    }
} else {
 
     $response["response"] = "unknown logfilename";	

}
 echo json_encode($response);
?>
