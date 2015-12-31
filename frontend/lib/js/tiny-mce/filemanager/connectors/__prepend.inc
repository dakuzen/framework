<?php 

	$has_bootstrap = include_once(realpath(dirname(__FILE__)."/../../../../../../../")."/boot/bootstrap.inc");
	
	if(!$has_bootstrap)  
		die;
		
	BASE_APPLICATION::initialize_web_application();	

	$application = APPLICATION::get_instance();
	$application->initialize();