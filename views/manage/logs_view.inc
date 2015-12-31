<?
	class BASE_VIEW_MANAGE_LOGS extends VIEW {

		function __construct() {
			return parent::__construct("manage","logs");
		}

		function init() {

			$this->get_web_asset_manager()->add_daterangepicker();

			if($action=$this->request("action")) {

				if($action=="list") 
					echo APPLICATION::get_base_view_instance("manage","loglist")
								->disable_authorization()
								->get_html();

				if($action=="view") {

					$view = APPLICATION::get_base_view_instance("manage","log")
								->disable_authorization();
					APPLICATION::get_instance()->process_view($view);
					
				}

				die;
			}
		}
	}