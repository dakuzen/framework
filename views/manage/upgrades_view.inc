<?php

	class BASE_VIEW_MANAGE_UPGRADES extends VIEW {
		
		protected $_version = null;
		
		function __construct() {
			$this->set_route("manage","upgrades");
			$this->set_title("Upgrades");
			
			$upgrade_cmodel = new CMODEL_UPGRADE();
			$this->_version = $upgrade_cmodel->get_current_version();
		}

		function init() {
	
			if($version=$this->get("version"))
				$this->_version = $version;
			
			if($this->get("action")=="mac") {
				$upgrade_cmodel = new BASE_CMODEL_UPGRADE();
				$upgrade_cmodel->mark_completed($this->_version);
				die;
			}
			
			$upgrade_base_cmodel = new BASE_CMODEL_UPGRADE();
			
			$items = array();			
			try {
				
				$upgrade_functions = $upgrade_base_cmodel->get_upgrade_functions($this->_version);
				$upgrade_items = $upgrade_base_cmodel->get_upgrade_items($this->_version);

				foreach($upgrade_functions as $index=>$upgrade_function) {
					
					$upgrade_item = value($upgrade_items,$upgrade_function);
					
					$status = in_array(value($upgrade_item,"name"),$upgrade_functions) ? "Completed" : "Queued";
					
					$items[] = ["name"=>preg_replace("/^upgrade_/","",$upgrade_function),
								"version"=>value($upgrade_item,"version"),
								"create_time"=>CMODEL_TIME::create(value($upgrade_item,'create_date')),
								"status"=>$status];
				}

				usort($items, function($a, $b) {
				    return $a['create_time']->lesser($b['create_time']);
				});

			} catch(Exception $e) {
				APPLICATION::add_error_message($e->getMessage());
			}
			
			$upgrade_cmodel = new BCMODEL_UPGRADE();
			$upgrade_versions = $upgrade_cmodel->get_upgrade_versions();
			
			$this->set_var("versions",$upgrade_versions);			
			$this->set_var("items",$items);
			$this->set_var("version",$this->_version);
		}
		
		function set_version($version) { $this->_version = $version; }
	}