<?php
class DataBaseConnection {

    private $connection; 
    
    function __construct() {
     
    }

    function __destruct() {
 
    }

 
    function openDataBaseConnection() {
        require_once __DIR__ . '/database_config.php';
        $this->$connection= mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD, DB_DATABASE);
        if (!$this->$connection) {
            echo json_encode("failed to open Connection");
        } 
        $db = mysqli_select_db(DB_DATABASE);
        return $this->$connection;
    }
 
    function closeDataBaseConnection() {
        mysqli_close($this->$connection);
    }
 
}
 
?>
