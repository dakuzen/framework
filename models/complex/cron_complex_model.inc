<?
    /**
    * Class BASE_CMODEL_CRON
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_CRON extends CMODEL {
		
        /**
         * The debug
         * @var boolean $debug 
         */
		protected $_cron_setting_cmodels	= array();
		protected $_debug 					= false;    
		protected $_exceptions				= array();
		
		public function __construct($cron_setting_cmodels=[]) {
			$this->_cron_setting_cmodels = $cron_setting_cmodels;
		}		

        /**
         * Debug
         */
		public function debug() { $this->_debug = true; }

        /**
         * Process
         * @return boolean 
         */

        public function pre_process() {}
        
        public function init($cron_setting_cmodels) {
        	$this->_cron_setting_cmodels = $cron_setting_cmodels;
        }

		public function process($process=null,$options=array("force"=>false,"time"=>null)) {

			$this->pre_process();

			//Avoid the issue with timeouts such as "MySQL server has gone away"
			DB::instance()->_query("SET SESSION wait_timeout = 7200;");
			DB::instance()->_query("SET SESSION interactive_timeout = 7200;");
		
			if(!$process) {
				DEBUG_UTIL::print_stack_trace();
				die;
			}

			CMODEL_SETTING::set_instance_cron_pulse(time());

			$processes = $process ? array($process) : $this->get_processes();

			$cron_cmodels = ARRAY_UTIL::index(HMODEL_FF_CRON::create()
													->where("process","IN",$processes)
													->gets(),"get_process");

			$force 	= value($options,"force",false) || $this->_debug;
			$time 	= value($options,"time",time());

			$cron_setting_cmodels = $this->get_settings();

			foreach($processes as $process) {

				$cron_setting_cmodel 	= value($cron_setting_cmodels,$process,CMODEL_CRON_SETTING::create());
				$cron_cmodel 			= value($cron_cmodels,$process,CMODEL_FF_CRON::create()->set_process($process));

				//update the process state for stagnant or long running processes. For long running processes
				$this->update_stagnant_process($cron_cmodel,$cron_setting_cmodel->get_timeout());

				if(!$force) {
					
					if(!$cron_setting_cmodel->is_valid($time) && !$cron_cmodel->is_state_queued())
						continue;

					//Run any process that in not currently active/running
					if($cron_cmodel->is_state_active()) 
						continue;
				}

				$start_time = time();

				if(!$force)
					$cron_cmodel
						->set_create_date(DB::get_date_time())
						->set_process_id(getmypid())
						->set_state(CMODEL_FF_CRON::STATE_ACTIVE)
						->save();

				try {

					if(!method_exists($this,$process))
						throw new Exception("Invalid cron process: ".$process);

					$this->$process();

					$cron_cmodel->reset();
				
				} catch(Exception $exception) {

					$this->add_exception($exception);

					$message = $process." - ".$exception->getMessage()." on ".DATE_UTIL::get_medium_date_time();

					$cron_cmodel
						->set_state(CMODEL_FF_CRON::STATE_FAILED)
						->set_message($message);

					if(SYSTEM_MANAGER::is_system_mode_debug())
						throw $exception;
				}
			
				$cron_cmodel
					->set_process_id(null)
					->set_duration(time() - $start_time)
					->save();
			}

			if($this->_exceptions) {
				if(count($this->_exceptions)==1)
					throw value($this->_exceptions,0);
				else 
					throw new CMODEL_EXCEPTIONS($this->_exceptions);
			}
				
			return true;
		}

		public function run() {

			if(SERVER_UTIL::is_web_application()) {
				$time = time();
				foreach($this->get_processes() as $process)
					$this->process($process,array("time"=>$time));
			}
		
			if(SERVER_UTIL::is_command_application()) {

				$process 	= value($_SERVER,array("argv",1));
				$time 		= value($_SERVER,array("argv",2));
		
				if($process) {

					try {
						
						$this->process($process,array("time"=>$time));

					} catch(Exception $e) {

						if(!is_a($e,"CMODEL_EXCEPTIONS"))
							$e = CMODEL_EXCEPTIONS::create($e);

						foreach($e->exceptions() as $e) {
							printf("Cron Exception on ".SERVER_UTIL::get_hostname()."\n\n%s:%d %s (%d) [%s]\n\n", $e->getFile(), $e->getLine(), $e->getMessage(), $e->getCode(), get_class($e));
							print_r($e);
							echo "\n\n";
							print_r($_SERVER);
						}
					} 
				
				} else {
					
					$time = time();
					foreach($this->get_processes() as $process) {
						
						$cron_file = APPLICATION::get_instance_directory()."backend/command/cron.php";

						//Legacy
						if(!is_file($cron_file))
							$cron_file = APPLICATION::get_instance_directory()."application/command/cron.php";

						$cmd = "php ".$cron_file." ".$process." ".$time;	

						echo SERVER_UTIL::shell_exec($cmd,true);
					}
				}
			}
		}

        /**
         * Update process state
         * @param string $process 
         */
		public function update_stagnant_process($cron_cmodel,$max_hours=1) {

			if($max_hours) {
				
				//Refetch the CMODEL to get the latest object state
				$cron_cmodel = HMODEL_FF_CRON::create(false)
									->where("state","=",CMODEL_FF_CRON::STATE_ACTIVE)
									->where("process","=",$cron_cmodel->get_process())
									->get();

				if($cron_cmodel) {

					$create_time_cmodel = $cron_cmodel->get_create_time();

					if($create_time_cmodel->valid() && $max_hours) {						

						$stagnant 	= $create_time_cmodel->lesser(CMODEL_TIME::create(time())->subtract_hour($max_hours));
						$warning 	= $create_time_cmodel->lesser(CMODEL_TIME::create(time())->subtract_hour($max_hours/2)) && !$cron_cmodel->get_long_running();

						if($stagnant || $warning) {				

							$system_name 	= SYSTEM_MANAGER::get_system_name() ? SYSTEM_MANAGER::get_system_name() : SYSTEM_MANAGER::get_system_host();
							$system_name 	= $system_name ? $system_name : MODEL_PATH::get_instance_directory_name();
							$name 			= preg_replace("/^process_/","",$cron_cmodel->get_process());

							$message = $subject = "";
							if($stagnant) {																						
								
								$subject = 	"Stagnant Cron Process: ".ucwords(str_replace("_"," ",$name))." on ".$system_name;
								$message = 	"The ".ucwords(str_replace("_"," ",$cron_cmodel->get_process()))." Cron Process has been running for ".$create_time_cmodel->age(false,false).".\n".
											"The server has terminated the stagnant process and a new process has been respawned.";

								SERVER_UTIL::exec("kill -9 ".$cron_cmodel->get_process_id());
								$cron_cmodel->reset(CMODEL_FF_CRON::STATE_IDLE);

							} elseif($warning) {
								
								$subject = 	"Long Running Cron Process: ".ucwords(str_replace("_"," ",$name))." on ".$system_name;
								$message = 	"The ".ucwords(str_replace("_"," ",$cron_cmodel->get_process()))." Cron Process has been running for ".$create_time_cmodel->age(false,false)." which is abnormal.\n".
											"Please keep an eye on this process as it might end up stagnant and will be terminted.";

								$cron_cmodel
									->set_long_running(true)
									->set_message($message)
									->save();											
							}
							
							$message .= "\n\nProcess Started: ".$create_time_cmodel->long_datetime()." UTC\n".
										"Current Date Time: ".CMODEL_TIME::create(time())->long_datetime()." UTC\n".
										"System Name: ".$system_name."\n".
										"Server Name: ".SERVER_UTIL::get_hostname();

							$notify_recipients 	= SYSTEM_MANAGER::get_notify_recipients();
							$email_cmodel		= SYSTEM_MANAGER::get_email();				

							if($email_cmodel && $notify_recipients) {

								foreach($notify_recipients as $notify_recipient)
									$email_cmodel->add_to($notify_recipient);

								$email_cmodel
									->enable_text()
									->set_subject($subject)
									->set_body($message)
									->send();
							} else 
								echo $subject."\n\n".$message;
						}
					}
				}
			}

			return true;
		}

		
        /**
         * Return processes
         * @return array 
         */
		public function get_processes() {
		
			$class_methods = array();
			foreach(get_class_methods($this) as $class_method)
				if(preg_match("/^process_/",$class_method)) 
					$class_methods[] = $class_method;
					
			return $class_methods;
		}

        /**
         * Returns process reoccurance
         * @param string $process
         * @return BASE_CMODEL_REOCCURANCE 
         */
		public function get_process_reoccurance($process) {
			return new BASE_CMODEL_REOCCURANCE();
		}

		public function create_reoccurance($process) {
			return CMODEL_CRON::get_process_reoccurance($process);
		}
		
		public function add_exception($exception) {
			$this->_exceptions[] = $exception; return $this;
		}

        public function get_settings() {
        	$cron_setting_cmodels = [];
        	foreach($this->get_processes() as $process) {

        		$cron_setting_cmodel = value($this->_cron_setting_cmodels,$process);

        		if(!$cron_setting_cmodel) {

        			$reoccurance_cmodel = CMODEL_CRON::get_process_reoccurance($process);

        			if($reoccurance_cmodel)
        				$cron_setting_cmodel = MISC_UTIL::cast($reoccurance_cmodel,"CMODEL_CRON_SETTING");
        		}

        		if(!$cron_setting_cmodel)
        			$cron_setting_cmodel = CMODEL_CRON_SETTING::create();

        		$cron_setting_cmodels[$process] = $cron_setting_cmodel;
        	}

        	return $cron_setting_cmodels;
        }

	}
	