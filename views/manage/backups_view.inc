<?php

	class BASE_VIEW_MANAGE_BACKUPS extends VIEW {
		
		protected $_backup_url = "";
		
		function __construct() {
			$this->set_route("manage","backups");
			$this->set_title("Database Backups");
			
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
		}

		function init() {
			
			$ff_backup_cmodels = HMODEL_FF_BACKUP::create()
											->order("create_date","DESC")
											->gets();

			$this->set_var("ff_backups",$ff_backup_cmodels);
		}
	}