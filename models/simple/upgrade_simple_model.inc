<?
	class BASE_MODEL_UPGRADE extends MODEL {
		
		function process_upgrade() {
			
		$ini_util = INI_UTIL::create(SYSTEM_MANAGER::get_config_file());
			
			$db_host = $ini_util->get("DB_HOST");
			$db_name = $ini_util->get("DB_NAME");
			$db_type = $ini_util->get("DB_TYPE");
			$db_user = $ini_util->get("DB_USER");
			$db_pass = $ini_util->get("DB_PASS");
			
			$override_db_user = REGISTRY::instance()->get_parm(array("args","user"));
			$override_db_pass = REGISTRY::instance()->get_parm(array("args","pass"));
			$version = REGISTRY::instance()->get_parm(array("args","version"));
							
			if($override_db_user)
				$db_user = $override_db_user;
			else
				APPLICATION::display_warning_message("Defaulting to config.ini DB_USER. Use switch --user to override the DB user name.");

			if($override_db_pass)
				$db_pass = $override_db_pass;
			else
				APPLICATION::display_warning_message("Defaulting to config.ini DB_PASS. Use switch --pass to override the DB password.");
			
			APPLICATION::display_newline();
			
			try {
				if($db_user || $db_pass) {
					$db = DB::get_db_connection($db_host,$db_user,$db_pass,$db_name,$db_type);
					DB::set_default_db_connection($db);
				} else
					$db = DB::get_instance();
				
				if(!$version)
					$version = CMODEL_UPGRADE::create()->get_current_version();
				
				if(BASE_CMODEL_UPGRADE::is_valid_version($version)) {
					
					$upgrade_cmodel = new EXTEND_CMODEL_UPGRADE();
					$upgrade_cmodel->process($version);
				
				} else
					APPLICATION::display_message("Invalid version `".$version."`");					
				
			} catch(Exception $e) {
				APPLICATION::display_message("Ugrade failed: ".$e->getMessage());
			}			
		}
		
		static function get_current_version($version_file) {	
			
			$version = FILE_UTIL::get_file_contents($version_file); 
			
			if($version) {
				preg_match_all("/(\d+_\d+_\d+),?(\d+_\d+_\d+)?/",$version,$matches);
				$version = array_pop(get_value($matches,1,array()));
			}
			
			return $version; 
		}		
	}
	

	class EXTEND_CMODEL_UPGRADE extends BASE_CMODEL_UPGRADE {
	
		function process_message_callback($message,$has_success) {
			
			if($has_success)
				APPLICATION::display_message($message);
			else
				APPLICATION::display_message($message);
		}
	}