<?
	$server_mode			= !$is_production ? HTML_UTIL::span($server_mode,array("style"=>"color:red")) : $server_mode;

	$last_updated_date 		= $last_updated_time ? CMODEL_TIME::create($last_updated_time)->long_datetime() : HTML_UTIL::span("N/A",array("style"=>"color:red"));
	
	$last_cron_pulse_date 	= $last_cron_pulse ? CMODEL_TIME::create($last_cron_pulse)->long_datetime() : "N/A";
	
	$last_cron_pulse_date 	= $is_cron_dead ? HTML_UTIL::span($last_cron_pulse_date,array("style"=>"color:red")) : $last_cron_pulse_date;
	
	$last_db_backup_date 	= $last_db_backup_time ? CMODEL_TIME::create($last_db_backup_time)->long_datetime() : "N/A";
	
	$last_db_backup_date	= $show_db_backup_warning ? HTML_UTIL::span($last_db_backup_date,array("style"=>"color:red")) : $last_db_backup_date;
	
	$notify_recipients		= !$notify_recipients ? HTML_UTIL::span("N/A",array("style"=>"color:red")) : implode(", ",$notify_recipients);
	
	$version				= str_replace("_",".",$version);
	
	$table_data[] = array("System Time: ",		HTML_UTIL::span(CMODEL_FORMAT::create(time())->iso8601(["seconds"=>true]),array("style"=>"font-weight:bold;")));
	$table_data[] = array("System Timezone: ",		HTML_UTIL::span(SERVER_UTIL::shell_exec("date +'%Z %:z'"),array("style"=>"font-weight:bold;")));	
	$table_data[] = array("App Timezone: ",		HTML_UTIL::span(CMODEL_TIME::create(time())->timezone_abr(),array("style"=>"font-weight:bold;")));
	$table_data[] = array("App Mode: ",		HTML_UTIL::span($server_mode,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Server Hostname: ",		HTML_UTIL::span(SERVER_UTIL::get_hostname(),array("style"=>"font-weight:bold;")));	
	$table_data[] = array("App Hostname: ",		HTML_UTIL::span($system_host,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Installed Directory: ",		HTML_UTIL::span($instance_directory,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Database Name: ",		HTML_UTIL::span($database_name,array("style"=>"font-weight:bold;")));	
	$table_data[] = array("Database Time: ",		HTML_UTIL::span(CMODEL_FORMAT::create($database_time)->iso8601(),array("style"=>"font-weight:bold;")));	
	$table_data[] = array("Database Timezone: ",		HTML_UTIL::span($database_timezone,array("style"=>"font-weight:bold;")));	
	$table_data[] = array("Log Level: ",		HTML_UTIL::span($log_level,array("style"=>"font-weight:bold;")));
	$table_data[] = array("SSL: ",			HTML_UTIL::span($ssl_mode,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Cron: ",			HTML_UTIL::span($cron_mode,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Version: ",		HTML_UTIL::span($version,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Last Update: ",		HTML_UTIL::span($last_updated_date,array("style"=>"font-weight:bold;")));

	$table_data[] = array("Notify Recipients: ",	HTML_UTIL::span($notify_recipients,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Last DB Backup: ",	HTML_UTIL::span($last_db_backup_date,array("style"=>"font-weight:bold;")));
	$table_data[] = array("Last Cron Pulse: ",	HTML_UTIL::span($last_cron_pulse_date,array("style"=>"font-weight:bold;")));
	
	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->disable_css()
		->render();