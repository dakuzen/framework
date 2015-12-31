<?php
	error_reporting( 2047 );
	ini_set("display_errors",1);
	
	define("DIR_APPLICATION",(isset($DIR_APPLICATION) ? $DIR_APPLICATION : dirname($_SERVER["DOCUMENT_ROOT"])."/"));
	define("DIR_INSTANCE",dirname(dirname(dirname(__FILE__)))."/");
	define("DIR_FRAMEWORK",DIR_INSTANCE."framework/");
	
	require_once(DIR_FRAMEWORK."libraries/framework/base_application.inc");	
?>