<?php
include "module.php.inc";


/*********************************************************************

make sure you fill up the database information before running this script

this script creates the tables for the provided databame defined in modile.php.inc

**********************************************************************/


init_database() or die(mysql_error() . " Check " . __FILE__ . " on line " . __LINE__ . " Error Establishing Connection with Database");

daser_create_tables();

?>
