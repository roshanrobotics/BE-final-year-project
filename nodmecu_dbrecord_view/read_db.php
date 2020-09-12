<!DOCTYPE html>
<html>
	<head>
		<style>
			table {
				border-collapse: collapse;
				width: 100%;
				color: #1f5380;
				font-family: monospace;
				font-size: 20px;
				text-align: left;
			} 
			th {
				background-color: #1f5380;
				color: white;
			}
			tr:nth-child(even) {background-color: #f2f2f2}
		</style>
	</head>
	<?php
		//Creates new record as per request
		//Connect to database
		$hostname = "localhost";
		$username = "root";
		$password = "electricity123";
		$dbname = "electricityiot";
		// Create connection
		$conn = mysqli_connect($hostname, $username, $password, $dbname);
		// Check connection
		if (!$conn) {
			die("Connection failed !!!");
		} 
	?>
	<body>
		<table>
			<tr>
				<th>No</th> 
				<th>current</th> 
				<th>voltage</th>
				<th>power</th>
				<th>date</th>
				<th>time</th>
			</tr>	
			<?php
				$table = mysqli_query($conn, "SELECT No, current, voltage, power, date, time FROM electricity_consumption_table");
				while($row = mysqli_fetch_array($table))
				{
			?>
			<tr>
				<td><?php echo $row["No"]; ?></td>
				<td><?php echo $row["current"]; ?></td>
				<td><?php echo $row["voltage"]; ?></td>
				<td><?php echo $row["power"]; ?></td>
				<td><?php echo $row["date"]; ?></td>
				<td><?php echo $row["time"]; ?></td>
			</tr>
			<?php
				}
			?>
		</table>
	</body>
</html>