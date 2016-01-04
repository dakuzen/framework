<?php

	class BASE_VIEW_MANAGE_BACKUPS extends VIEW {
		
		protected $_backup_url = "";
		
		function __construct() {
			$this->set_route("manage","backups");
			$this->set_title("Database Backups");
			
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
		}

		function init() {

			if($this->get("action")=="download") {

				if(SYSTEM_MANAGER::is_system_mode_debug()) {
					
					$ff_backup_cmodel = HMODEL_FF_BACKUP::create()
											->where("ff_backup_id","=",$this->get("bid"))
											->get();

					if($ff_backup_cmodel) {
						$file = MODEL_PATH::get_backups_directory().$ff_backup_cmodel->get_filename();
					
						if(is_file($file))
							HTTP_UTIL::stream_file($file);
						else
							APPLICATION::add_error("Failed to locate backup file");
						
					} else {
						APPLICATION::add_error("Failed to load backup");
					}

				} else {
					APPLICATION::add_error("Downloading of backup files are only allowed on development installs");					
				}
			}
			
			$ff_backup_cmodels = HMODEL_FF_BACKUP::create()
											->order("create_date","DESC")
											->gets();

			$this->set_var("ff_backups",$ff_backup_cmodels);
		}
	}