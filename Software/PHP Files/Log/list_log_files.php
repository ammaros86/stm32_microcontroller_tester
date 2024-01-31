<?php
 
$directory = '/var/www/LogFiles';
$files = scandir($directory);
$allFiles = [];
if ($files === false) {
    die('Fehler beim Scannen des Verzeichnisses: ' . error_get_last()['message']);
}
foreach ($files as $file) {   
   if ($file != "." && $file != "..") {
        $allFiles[] = $file;
    }
}

echo json_encode($allFiles);
?>