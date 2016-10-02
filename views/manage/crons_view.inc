<?
	class BASE_VIEW_MANAGE_CRONS extends VIEW {

		protected $_processes 		= array();
		protected $_cron_cmodels	= array();
		protected $_cron_cmodel 	= null;

		function __construct($roles=array()) {
			return $this
					->set_route("manage","crons")
					->set_roles($roles);
		}

		function init() {

			$this->_cron_cmodels = ARRAY_UTIL::index(HMODEL_FF_CRON::create()->gets(),"get_process");

			$this->_cron_cmodel = CMODEL_CRON::create();
			$this->_processes = $this->_cron_cmodel->get_processes();

			sort($this->_processes);

			$action 	= $this->request("action");
			$process 	= $this->request("process");

			if($process && $action) {

				$response_cmodel = CMODEL_RESPONSE::create()->render_content_type();
				DEBUG_UTIL::format_text();

				if($action=="reset") {

					$has_success = CMODEL_FF_CRON::create_cron($process)->reset();

					$response_cmodel
						->data("message","Successfully reset the cron")
						->success();

				} elseif($action=="queue") {

					$cron_cmodel = CMODEL_FF_CRON::create_cron($process)->queue();

					$response_cmodel
						->data("message","Successfully queued the cron")
						->success();

				} elseif($action=="process") {

					if(!$process)
						throw new Exception("Process not specified");

					$this->_cron_cmodel->process($process,array("force"=>true));
				}

				$response_cmodel->render(true);
			}

			$task_url = rtrim(APPLICATION::get_instance()->get_uri()->get_uri(),"/")."/";

			$this->set_var("processes",$this->_processes);
			$this->set_var("settings",$this->_cron_cmodel->get_settings());
			$this->set_var("crons",$this->_cron_cmodels);
			$this->set_var("cron",$this->_cron_cmodel);
			$this->set_var("task_url",$task_url);
		}
	}