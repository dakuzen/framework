<?php

	$has_bootstrap = include_once(realpath(dirname(__FILE__)."/../../../../../../../")."/boot/bootstrap.inc");
	
	if(!$has_bootstrap)  
		die;
		
	BASE_APPLICATION::initialize_web_application();	

	$application = APPLICATION::get_instance();
	$application->initialize();
	
	// General settings
	//$config['general.engine'] = 'GoogleSpell';
	//$config['general.engine'] = 'PSpell';
	$config['general.engine'] = 'PSpellShell';
	//$config['general.remote_rpc_url'] = 'http://some.other.site/some/url/rpc.php';

	
	$pspell_mode = defined("PSPELL_FAST") ? PSPELL_FAST : "";
	
	// PSpell settings
	$config['PSpell.mode'] 		= $pspell_mode;
	$config['PSpell.spelling'] 	= "";
	$config['PSpell.jargon'] 	= "";
	$config['PSpell.encoding'] 	= "";
	
	$config['PSpellShell.tmp'] = MODEL_PATH::get_temporary_directory();	

	if(SERVER_UTIL::is_os_windows()) {
		$config['PSpellShell.aspell'] = 'aspell';
		
	} else {
		$config['PSpellShell.mode'] 	= $pspell_mode;
		$config['PSpellShell.aspell'] 	= '/usr/bin/aspell';
	}
	
	DEBUG_UTIL::enable_format_text();