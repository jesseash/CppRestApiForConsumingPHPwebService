<?php
 
header("Content-Type:application/json");
//require "data.php";
header("HTTP/1.1 ",200);

$inputJSON = file_get_contents('php://input'); //captures the Json object
$input = json_decode($inputJSON, TRUE); //get hashmap from Json object
//extract hash values here
$response['data']=$input["name"]." was here..."; //concatenates a string verifying the service succesfully executed

$json_response = json_encode($response);
	echo $json_response;


?>
