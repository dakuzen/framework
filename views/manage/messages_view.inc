<?

	class BASE_VIEW_MANAGE_MESSAGES extends VIEW {

		function __construct() {
			$this->set_route("manage","messages");
			$this->set_form("javascript:;");
		}

		function init() {

			if($this->post("action")=="messages") {

				echo APPLICATION::get_instance()->get_base_view_instance("manage","messagelist")->get_html();
				die;
			}

			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
		}
	}