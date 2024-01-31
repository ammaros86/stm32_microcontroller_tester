
<?php
// array for JSON response
$response = array();

// include db connect class
require_once __DIR__ . '/database_connect.php';

// connecting to db
$databaseConnection = new DataBaseConnection();
$connection = $databaseConnection->openDataBaseConnection();

    $board_id = $_POST['board_id'];

    $sql = "DELETE FROM test_commands WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["board_commands"] = 1;
    } else {
        $response["board_commands"] = 0;
    }

    $sql = "DELETE FROM board_flash WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["board_flash"] = 1;
    } else {
        $response["board_flash"] = 0;
    }

    $sql = "DELETE FROM board_temperature WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["board_temperature"] = 1;
    } else {
        $response["board_temperature"] = 0;
    }

    $sql = "DELETE FROM current_consumption WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["current_consumption"] = 1;
    } else {
        $response["current_consumption"] = 0;
    }

    $sql = "DELETE FROM gpio_continuity WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_continuity"] = 1;
    } else {
        $response["gpio_continuity"] = 0;
    }

    $sql = "DELETE FROM gpio_input_no_pull WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_input_no_pull"] = 1;
    } else {
        $response["gpio_input_no_pull"] = 0;
    }

    $sql = "DELETE FROM gpio_input_pull_down WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_input_pull_down"] = 1;
    } else {
        $response["gpio_input_pull_down"] = 0;
    }

    $sql = "DELETE FROM gpio_input_pull_up WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_input_pull_up"] = 1;
    } else {
        $response["gpio_input_pull_up"] = 0;
    }

    $sql = "DELETE FROM  gpio_output_no_pull WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_output_no_pull"] = 1;
    } else {
        $response["gpio_output_no_pull"] = 0;
    }

    $sql = "DELETE FROM  gpio_output_pull_down WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_output_pull_down"] = 1;
    } else {
        $response["gpio_output_pull_down"] = 0;
    }

    $sql = "DELETE FROM gpio_output_pull_up WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_output_pull_up"] = 1;
    } else {
        $response["gpio_output_pull_up"] = 0;
    }

    $sql = "DELETE FROM gpio_short_circuit WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["gpio_short_circuit"] = 1;
    } else {
        $response["gpio_short_circuit"] = 0;
    }

    $sql = "DELETE FROM analog_input WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["analog_input"] = 1;
    } else {
        $response["analog_input"] = 0;
    }

    $sql = "DELETE FROM power_gnd_pins WHERE board_id = '$board_id'";
    if ($connection->query($sql) === TRUE) {
        $response["power_gnd_pins"] = 1;
    } else {
        $response["power_gnd_pins"] = 0;
    }

   echo json_encode($response);
$databaseConnection->closeDataBaseConnection();
?>
