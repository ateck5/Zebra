<?php
$file_source = $_GET['file']; 
header('Content-disposition: attachment; filename='.$file_source);
header('Content-type: application/pdf');
readfile($file_source);
?>