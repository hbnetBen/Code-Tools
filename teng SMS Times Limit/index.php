<?php
include "module.php.inc";

/*******************************************************************************************


boolean daser_can_send(String Username)

This function returns true or false depending on weather the user can or cannot send SMS as the case may be.
The function automatically creates a record for Username. All you have to do is fill the neccessary information in module.php.inc and Run the install.php (trust me! NO MORE NO LESS).

void daser_tell_api(void)

After you must have called the function that sends SMS you must tell the API by calling this function
to do some internal modification to the User's Record
************************************************************************************************/


if(daser_can_send("daserfoss")){

echo "SMS sent";
daser_tell_api();

}else{

	/***************************************************************************************
	
	You can find the reason for beign FALSE by testing the global variable $DASER_status_error
	against the three constants.
	
	******************************************************************************************/

	if($DASER_status_error == DAILYLIMITEXCEEDED){
	echo "your daily SMS has expired";
	}else
	if($DASER_status_error == WEEKLYLIMITEXCEEDED){

	echo "Your weekly SMS has expired";

	}else
	if($DASER_status_error == MONTHLYLIMITEXCEEDED){
	echo "Your Monthly SMS unit has expired";
	}else{
	echo "Are you sure you called daser_can_send()tion";
	}

}










?>


