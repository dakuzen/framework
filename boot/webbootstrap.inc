<?php
	error_reporting( 2047 );
	ini_set("display_errors",1);
	
	define("DIR_APPLICATION",str_replace("\\","/",(isset($DIR_APPLICATION) ? $DIR_APPLICATION : dirname($_SERVER["DOCUMENT_ROOT"].dirname($_SERVER["SCRIPT_NAME"]))."/")));
	define("DIR_INSTANCE",str_replace("\\","/",dirname(dirname(dirname(__FILE__)))."/"));
	define("DIR_FRAMEWORK",DIR_INSTANCE."framework/");
	
	require_once(DIR_FRAMEWORK."libraries/framework/base_application.inc");

	BASE_APPLICATION::validate_server();
	BASE_APPLICATION::initialize_web_application();
?>