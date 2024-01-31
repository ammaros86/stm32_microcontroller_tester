<?php
if (isset($_GET['filename'])) {
    $filename = $_GET['filename'];
    $file_path = '/var/www/LogFiles/' . $filename;
    if (file_exists($file_path)) {
        $file_contents = file_get_contents($file_path);
        echo $file_contents;
    } else {
        // Die Datei existiert nicht oder der Pfad ist falsch
        echo "Die Datei konnte nicht gefunden werden.";
    }
} else {
    echo "Es wurde kein Dateiname angegeben.";
}
?>