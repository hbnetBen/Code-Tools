<?php
/********************************************************************************************
dasersolomon@gmail.com 29th November, 2010

*********************************************************************************************/

/* database information */
define('HOST', 'localhost');
define('USER', 'root');
define('PASS','yahweh');
define('DB', 'teng');

/*Set Users Limits */
define('DAILYLIMIT', 3);	//public
define('WEEKLYLIMIT', 12);	//public
define('MONTHLYLIMIT', 32);	//public

/* Error Messages */
define('DAILYLIMITEXCEEDED', 1); //protected
define('WEEKLYLIMITEXCEEDED', 2); //protected
define('MONTHLYLIMITEXCEEDED', 3); //protected

/* Table Information. The names can be changed while their structure remains the same */
define('TBLDAYS','tengday');	
define('TBLWEEK','tengweek');
define('TBLMONTH','tengmonth');


define('DAY',1);
define('WEEK',2);
define('MONTH',3);

/* Global Variables */
$DASER_update = NULL;
$DASER_conn = NULL;
$DASER_status_error = NULL;
$DASERusername = NULL;

$DASER_DAY_unit = NULL;
$DASER_WEEK_unit = NULL;
$DASER_MONTH_unit = NULL;

/********************************************

1 = day
2 = weekly
3 = monthly

********************************************/

function daser_can_send($username){
global $DASER_update,$DASERusername;
$DASERusername = $username;
$validated = FALSE;

init_database() or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__ . " Error Establishing Connection with Database");


	if(validate_today($username) && validate_week($username) && validate_month($username)){
	//if(validate_today($username)){
	$DASER_update = TRUE;
	return TRUE;
	}else{
	return FALSE;
	}
	
	
}

function daser_tell_api(){
global $DASER_update;

	if($DASER_update){
	
	decrement_day();
	decrement_week();
	decrement_month();
		
	}else{
	die("Consult DaserSolomon@gmail.com: Call: \n bool daser_can_send( String username) \nThen a function
	that will Send whatever it is you want to send after which you call: \n void daser_tell_api(void)\n to update the
	database");
	}

}

function decrement_day(){
global $DASER_conn,$DASER_DAY_unit,$DASERusername;

if($DASER_DAY_unit > 0){
$current_unit = $DASER_DAY_unit - 1;
$sql = "UPDATE " . TBLDAYS . " SET `no_sent` = '$current_unit' WHERE `username` = '$DASERusername' ";
mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);;
}else{
//die("Inform Daser for this is a very Fatal Bug.");
}

}

function decrement_week(){
global $DASER_conn,$DASER_WEEK_unit,$DASERusername;

if($DASER_WEEK_unit > 0){
$current_unit = $DASER_WEEK_unit - 1;
$sql = "UPDATE " . TBLWEEK . " SET `no_sent` = '$current_unit' WHERE `username` = '$DASERusername' ";
mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);;
}else{
//die("Inform Daser for this is a very Fatal Bug.");
}

}

function decrement_month(){
global $DASER_conn,$DASER_MONTH_unit,$DASERusername;

if($DASER_MONTH_unit > 0){
$current_unit = $DASER_MONTH_unit - 1;
$sql = "UPDATE " . TBLMONTH . " SET `no_sent` = '$current_unit' WHERE `username` = '$DASERusername' ";
mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
}else{
//die("Inform Daser for this is a very Fatal Bug.");
}



}


function validate_today($username){
global $DASER_conn, $DASER_status_error,$DASER_DAY_unit;

$stack = FALSE;
$return_value = NULL;
do{


$sql = "SELECT `no_sent`, `date` FROM " . TBLDAYS . " WHERE username = '$username' ";
$resultset = mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);


	if($num = mysql_num_rows($resultset) == 0){
	//register the user
	
	$epoch_s = time();
	$epoch_e_w = time() + 604800; //604800 seconds == 1wk
	$epoch_e_m = time() + 2419200; //2419200 seconds == 4wks
	
	$sql = "INSERT INTO " . TBLWEEK . " (`username`, `no_sent`, `epoch_start`, `epoch_end`) VALUES ('$username'," .  WEEKLYLIMIT . ", '$epoch_s', '$epoch_e_w')";
	mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__ );
	
	$sql = "INSERT INTO " . TBLMONTH . " (`username`, `no_sent`, `epoch_start`, `epoch_end`) VALUES ('$username'," . MONTHLYLIMIT . ", '$epoch_s', '$epoch_e_m')";
	mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
	
	$sql = "INSERT INTO " . TBLDAYS . " (`username`, `no_sent`, `date`) VALUES ('$username'," . DAILYLIMIT . ", NOW() );";
	mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);

	$sql = "SELECT * FROM " . TBLDAYS . " WHERE username = '$username' ";
	$resultset = mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
	}


$row = mysql_fetch_array($resultset) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
$count = $row['no_sent'];
$date = $row['date']; //mysql date format


	$date_now = getdate();
	$year = $date_now['year'];
	$month = $date_now['mon'];
	$day = $date_now['mday'];
	
	if(strlen($day) == 1){
	$day = "0".$day;
	}
	
	if(strlen($month) == 1){
	$month = "0".$month;
	}
	
	$date_now = $year ."-".$month ."-". $day; //date format similar to mysql's
	//$date_now = mktime(0,0,0,$month,$day,$year);
	
	//die($date_now);
	
	if($date_now == $date){ //Today's SMS has been exausted
	
		if($count == 0){
		$DASER_status_error = DAILYLIMITEXCEEDED;
		$return_value =  FALSE;
		}else{
		$DASER_DAY_unit = $count; //global variable. will be used by debit function invoked by API user
		$return_value =  TRUE;
		}
		
	$stack = FALSE;  //just incase i forgot to put it later
		
	}else{
	$sql = "UPDATE " . TBLDAYS . " SET `no_sent` = '" . DAILYLIMIT ."', `date` = NOW() WHERE `username` = '$username'";
	mysql_query($sql);
	//validate_today($username);
	$stack = TRUE;
	}

	//the API user has to notifify API before the update to the DB

}while($stack);


return $return_value;
}//end func




function validate_week($username){
global $DASER_conn, $DASER_status_error,$DASER_WEEK_unit;

$stack = FALSE;
$return_value = NULL;

do{


$sql = "SELECT `no_sent`, `epoch_start`, `epoch_end` FROM " . TBLWEEK . " WHERE username = '$username' ";
$resultset = mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);


$row = mysql_fetch_array($resultset) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
$count = $row['no_sent'];
$epoch_start = $row['epoch_start']; //happy bufday unix
$epoch_end = $row['epoch_end']; //happy bufday unix. $epoch_start + 7DAYS


	$date_now = time();
	if($count == 0 && $date_now <= $epoch_end){ //weekly SMS has been exausted
	$DASER_status_error = WEEKLYLIMITEXCEEDED;
	$stack = FALSE;
	$return_value = FALSE;
	}else
	if($date_now > $epoch_end){  /*we dont care about the value of count at this point */
	$epoch_s = time();
	$epoch_w = time() + 604800; //604800 seconds == 1wk
	//$epoch_e_m = time() + 2419200; //2419200 seconds == 4wks
	
	$sql = "UPDATE " . TBLWEEK . " SET `no_sent` = ' " . WEEKLYLIMIT . "', `epoch_start` = '$epoch_s', `epoch_end` = '$epoch_w' WHERE `username` = '$username'";
	mysql_query($sql);
	//validate_week($username);
	$stack = TRUE;
	}else
	if($count > 0 && $date_now <= $epoch_end){
	$stack = FALSE;
	$DASER_WEEK_unit = $count;
	$return_value = TRUE;
	
	}else{
	$stack = FALSE;
	$return_value = FALSE;
	}

	//the API user has to notifify API before the update to the DB





}while($stack);


return $return_value;

}//end func


function validate_month($username){
global $DASER_conn, $DASER_status_error,$DASER_MONTH_unit;


$stack = FALSE;
$return_value = NULL;

do{



$sql = "SELECT `no_sent`, `epoch_start`, `epoch_end` FROM " . TBLMONTH . " WHERE username = '$username' ";
$resultset = mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);


$row = mysql_fetch_array($resultset) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
$count = $row['no_sent'];
$epoch_start = $row['epoch_start']; //happy bufday unix
$epoch_end = $row['epoch_end']; //happy bufday unix. $epoch_start + 31DAYS


	$date_now = time();
	
	if($count == 0 && $date_now <= $epoch_end){ //weekly SMS has been exausted
	$DASER_status_error = MONTHLYLIMITEXCEEDED;
	$stack = FALSE;
	$return_value = FALSE;
	}else
	if($date_now > $epoch_end){
	$epoch_s = time();
	//$epoch_w = time() + 604800; //604800 seconds == 1wk
	$epoch_m = time() + 2419200; //2419200 seconds == 4wks == 1months

	$sql = "UPDATE " . TBLMONTH . " SET `no_sent` = ' " . MONTHLYLIMIT . "', `epoch_start` = '$epoch_s', `epoch_end` = '$epoch_m' WHERE `username` = '$username'";
	mysql_query($sql);
	//validate_month($username);
	$stack = TRUE;
	}else
	if($count > 0 && $date_now <= $epoch_end){
	$stack = FALSE;
	
	$DASER_MONTH_unit = $count;
	$return_value = TRUE;
	
	}else{
	$stack = FALSE;
	$return_value = FALSE;
	}

	//the API user has to notifify API before the update to the DB






}while($stack);

return $return_value;
}//end func

function init_database(){
global $DASER_conn;
$DASER_conn = mysql_connect(HOST,USER,PASS) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);

	if(mysql_select_db(DB,$DASER_conn)){
	return TRUE;
	}else{
	die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);
	}

}



function daser_create_tables(){
global $DASER_conn;

$sql = "CREATE TABLE IF NOT EXISTS ". TBLDAYS . " (
  `username` varchar(255) NOT NULL,
  `no_sent` int(5) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;";

mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);

$sql = "CREATE TABLE IF NOT EXISTS ". TBLWEEK ." (
  `username` varchar(255) NOT NULL,
  `no_sent` int(5) NOT NULL,
  `epoch_start` int(50) NOT NULL,
  `epoch_end` int(50) NOT NULL,
  PRIMARY KEY (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;";

mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);

$sql = "CREATE TABLE IF NOT EXISTS ". TBLMONTH ." (
  `username` varchar(255) NOT NULL,
  `no_sent` int(5) NOT NULL,
  `epoch_start` int(50) NOT NULL,
  `epoch_end` int(50) NOT NULL,
  PRIMARY KEY (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;";

mysql_query($sql) or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__);

}
?>
