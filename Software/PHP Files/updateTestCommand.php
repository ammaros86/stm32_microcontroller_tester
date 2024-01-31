<?php
$response = array();
$y = 0;
require_once __DIR__ . '/database_connect.php';    
$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();
if (isset($_POST['isUpdate'])){
    $isUpdate = $_POST['isUpdate']; 
        if($isUpdate == $y){   
             if (isset($_POST['board_id']) && isset($_POST['measure_current']) && isset($_POST['measure_temperature'])&& isset($_POST['flash_board'])  && isset($_POST['power_gnd_test']) && isset($_POST['gpio_continuity']) && isset($_POST['gpio_output_test'])  && isset($_POST['gpio_input_test']) && isset($_POST['analog_input_test']) && isset($_POST['gpio_short_circuit_test'])&& isset($_POST['complete_test']) && isset($_POST['test_value']) && isset($_POST['errors'])) 
            {
                $board_id = $_POST['board_id'];
                $measure_current = $_POST['measure_current'];
                $measure_temperature = $_POST['measure_temperature'];
                $flash_board = $_POST['flash_board'];
                $power_gnd_test= $_POST['power_gnd_test'];
                $gpio_continuity= $_POST['gpio_continuity'];
                $gpio_output_test = $_POST['gpio_output_test'];
                $gpio_input_test = $_POST['gpio_input_test'];
                $analog_input_test = $_POST['analog_input_test'];
                $gpio_short_circuit_test = $_POST['gpio_short_circuit_test'];
                $complete_test = $_POST['complete_test'];
                $test_value = $_POST['test_value'];
                $errors = $_POST['errors']; 
                $query = "INSERT INTO test_commands(board_id, measure_current, measure_temperature, flash_board, power_gnd_test, gpio_continuity, gpio_output_test, gpio_input_test, analog_input_test, gpio_short_circuit_test, complete_test, test_value, errors)VALUES('$board_id', '$measure_current', '$measure_temperature', '$flash_board', '$power_gnd_test', '$gpio_continuity', '$gpio_output_test', '$gpio_input_test', '$analog_input_test', '$gpio_short_circuit_test','$complete_test', '$test_value', '$errors')";
                $result = mysqli_query($connection, $query);

                 if($result) {
                        $response["success"] = "done";
                        echo json_encode($response);
                } else {
                    $response["success"] = "failed to insert row";
                    echo json_encode($response);
                }
            } else {
                $response["success"] = "required field is missing";
                echo json_encode($response);
            }

        }else{
        if (isset($_POST['board_id'])){
            $board_id = $_POST['board_id'];
            $measure_current = $_POST['measure_current'];
            $measure_temperature = $_POST['measure_temperature'];
            $flash_board = $_POST['flash_board'];
            $power_gnd_test= $_POST['power_gnd_test'];
            $gpio_continuity= $_POST['gpio_continuity'];
            $gpio_output_test = $_POST['gpio_output_test'];
            $gpio_input_test = $_POST['gpio_input_test'];
            $analog_input_test = $_POST['analog_input_test'];
            $gpio_short_circuit_test = $_POST['gpio_short_circuit_test'];
            $complete_test = $_POST['complete_test'];
            $test_value = $_POST['test_value'];
            $errors = $_POST['errors']; 
             $query = "UPDATE test_commands SET measure_current = '$measure_current', measure_temperature = '$measure_temperature', flash_board = '$flash_board', power_gnd_test = '$power_gnd_test', gpio_continuity = '$gpio_continuity', gpio_output_test= '$gpio_output_test', gpio_input_test = '$gpio_input_test', analog_input_test = '$analog_input_test', gpio_short_circuit_test = '$gpio_short_circuit_test', complete_test = '$complete_test', test_value = '$test_value', errors = '$errors'  WHERE board_id = '$board_id' ";
             $result = mysqli_query($connection, $query);
             if($result) {
                $response["success"] = "done";
                echo json_encode($response);
                } else {
                $response["success"] = "failed to insert row";
                echo json_encode($response);
                }
           
         }else{
            $response["success"] = "failed";
            echo json_encode($response);
         }
        }
}else{
    $response["success"] = "failed";
    echo json_encode($response);
}
$databaseConnection->closeDataBaseConnection();
?>