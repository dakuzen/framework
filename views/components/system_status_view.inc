<?php

	class BASE_VIEW_COMPONENTS_SYSTEM_STATUS extends VIEW {

		function __construct() {
			$this->set_route("components","system_status");
			$this->disable_authorization();
		}

		function init() {
		
			$last_cron_pulse 	= CMODEL_SETTING::get_instance_cron_pulse();
			$last_db_backup		= CMODEL_SETTING::get_instance_db_backup();
			
			$cron = SYSTEM_MANAGER::get_cron();
			
			$is_cron_dead = false;
			
			$is_production = SYSTEM_MANAGER::is_system_mode_production();
			
			if($cron) {
				
				$is_cron_dead = $last_cron_pulse < (time() - CONSTANT::SECONDS_HOUR);
			
				if($is_cron_dead && $is_production) 
					APPLICATION::add_error_message("There is no cron pulse. Please notify your administrator");
			}
			
			$notify_recipients = SYSTEM_MANAGER::get_notify_recipients();
			
			$show_db_backup_warning = false;
			
			if($cron) {
				$show_db_backup_warning = $last_db_backup < time() - CONSTANT::SECONDS_DAY;
			
				if($show_db_backup_warning && $is_production)
					APPLICATION::add_error_message("There are no database backups for the past 24 hours. Please contact support.");
			}
			
			if(!$notify_recipients && $is_production)
				APPLICATION::add_error_message("There are no notify recipients. Please contact support.");
			
			$upgrade_cmodel = new CMODEL_UPGRADE();
			$version = $upgrade_cmodel->get_current_version();
			
			$is_valid_version = BASE_CMODEL_UPGRADE::is_valid_version($version);
			
			if(!$is_valid_version && $is_production)
				APPLICATION::add_error_message("The current version format is invalid. Please check the version file");	
	
			$upgrade_cmodel = new BASE_CMODEL_UPGRADE();
			$upgrade_versions = $upgrade_cmodel->get_upgrade_versions();

			$this->set_var("database_name",DB::get_instance()->get_db_name());
			$this->set_var("database_time",DB::instance()->one("SELECT NOW()"));
			$this->set_var("database_timezone",DB::instance()->one("SELECT @@session.time_zone"));
			$this->set_var("instance_directory",basename(dirname(MODEL_PATH::get_application_directory())));
			$this->set_var("is_production",$is_production);
			$this->set_var("versions",$upgrade_versions);
			$this->set_var("server_mode",SYSTEM_MANAGER::get_system_mode_name());
			$this->set_var("log_level",get_value(BASE_CMODEL_LOGGER::get_level_list(),SYSTEM_MANAGER::get_log_level(),SYSTEM_MANAGER::get_log_level()));
			$this->set_var("last_updated_time",CMODEL_SETTING::get_instance_upgraded());
			$this->set_var("is_cron_dead",$is_cron_dead);
			$this->set_var("last_cron_pulse",$last_cron_pulse);
			$this->set_var("version",$version);
			$this->set_var("notify_recipients",$notify_recipients);
			$this->set_var("last_db_backup_time",$last_db_backup);
			$this->set_var("show_db_backup_warning",$show_db_backup_warning);
			$this->set_var("system_host",SYSTEM_MANAGER::get_system_host());
			$this->set_var("ssl_mode",SYSTEM_MANAGER::get_ssl() ? "Enabled" : "Disabled");
			$this->set_var("cron_mode",$cron ? "Enabled" : "Disabled");			
		}
	}