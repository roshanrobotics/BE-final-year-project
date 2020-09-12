<?php
//Creates new record as per request
    //Connect to database
    $servername = "localhost";
    $username = "root";
    $password = "electricity123";
    $dbname = "electricityiot";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }

    //Get current date and time
    date_default_timezone_set('Asia/kolkata');
    $d = date("Y-m-d");
    $t = date("H:i:s");

    if(!empty($_POST['currentval']) && !empty($_POST['voltageval']) && !empty($_POST['powerval']))
    {
		$currentval = $_POST['currentval'];
		$voltageval = $_POST['voltageval'];
		$powerval = $_POST['powerval'];
		
	    $sql = "INSERT INTO electricity_consumption_table (current, voltage, power, date, time)
		
		VALUES ('".$currentval."', '".$voltageval."', '".$powerval."', '".$d."', '".$t."')";

		if ($conn->query($sql) === TRUE) {
		    echo "OK";
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
	}


	$conn->close();
?>