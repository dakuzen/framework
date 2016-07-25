<?
	abstract class BASE_CMODEL_PROCESS extends MODEL_COMPLEX {
		
		protected $_debug		= false;
		protected $_is_notify 	= true;
		
		public function __construct() {
			$this->register_dbo(new BASE_DBO_PROCESS());					
		}
		
		public static function create() { 
			$class = get_called_class();
			return new $class(); 
		}

		public function is_state_active()				{ return $this->get_state()==BASE_DBQ_PROCESS::STATE_ACTIVE; }
		public function is_state_idle()					{ return $this->get_state()==BASE_DBQ_PROCESS::STATE_COMPLETED; }
		public function is_state_completed()			{ return $this->get_state()==BASE_DBQ_PROCESS::STATE_COMPLETED; }
		public function is_state_running()				{ return $this->get_state()==BASE_DBQ_PROCESS::STATE_RUNNING; }
		public function is_state_deleted()				{ return $this->get_state()==BASE_DBQ_PROCESS::STATE_DELETED; }
		public function is_debug() 						{ return $this->_debug; }
		public function get_directory() 				{ return MODEL_PATH::get_process_directory().$this->get_name()."/"; }
		public function create_directory() 				{ FILE_UTIL::mkdir($this->get_directory()); }
		public function get_guid_filename($extension) 	{ return $this->get_guid().".".$extension; }

		public function disable_notify() 				{ $this->_is_notify = false; }
		public function is_notify() 					{ return $this->_is_notify; }

		public function get_state_name()				{ return get_value(BASE_DBQ_PROCESS::get_state_list(),$this->get_state()); }
		public function get_type_name()					{ return get_value(BASE_DBQ_PROCESS::get_type_list(),$this->get_type()); }
				
		public function object($index=null) 			{ return !is_null($index) ? get_value(parent::get_object(),$index) : parent::get_object(); }
		public function get_object($index=null) 		{ return $this->object($index); }
		public function get_file() 						{ return $this->get_directory().$this->get_guid().($this->get_extension() ? ".".$this->get_extension() : ""); }
		public function has_file()	 					{ return is_file($this->get_file()); }

		public function get_duration() 					{ return $this->get_end_time() - $this->get_start_time(); }
		
		public function get_created_age() 				{ return TIME_UTIL::get_time_age($this->get_created_time()); }
		public function get_formatted_duration() 		{ return TIME_UTIL::get_pretty_length($this->get_duration()); }
		public function get_formatted_percent() 		{ return $this->get_percent()."%"; }

		public function get_percent() 					{ return $this->is_state_completed() ? "100" : parent::get_percent(); }

		public function debug($debug=true) { 
			
			if(!SYSTEM_MANAGER::is_system_mode_production()) {
				$this->_debug = $debug;
				
				if($debug)
					DB::debug();
			}
		}

		public function save_file($filename,$content) {

			FILE_UTIL::mkdir($this->get_directory());
			FILE_UTIL::put($this->get_file(),$content);

			$this
				->set_filename($filename)
				->set_extension(FILE_UTIL::get_file_extension($filename))
				->save();
		}		

		public function delete() {
			$this->set_state(BASE_DBQ_PROCESS::STATE_DELETED);
			return $this->get_dbo("process")->update();
		}

		public function register_notifier() {}
	
		public function queue($name,$object) {
			
			$this->register_notifier();

			$this->set_name($name);
			$this->set_object($object);

			$this->save();

			return $this;
		}

		public function stream_file() {
			if($this->has_file())
				HTTP_UTIL::stream_file($this->get_file(),$this->get_filename());
		}

		public function process() {

			$this->create_directory();

			if(!$this->is_debug()) {
				
				$exists = BASE_DBQ_PROCESS::create()
									->where("state","=",BASE_DBQ_PROCESS::STATE_QUEUED)
									->where("process_id","=",$this->get_process_id())
									->exists();
				
				if(!$exists)
					return false;

				$this->set_state(BASE_DBQ_PROCESS::STATE_RUNNING);
				$this->set_start_time(time());
				$this->set_end_time(time());
				$this->save();
			}

			$has_success = false;

			try {
				
				$func = "process_".$this->get_name();

				if(!method_exists($this,$func))
					throw new Exception("Invalid process function");
					
				$this->$func();

				if(!$this->is_debug()) {

					$subject = "The process ".str_replace("_"," ",$this->get_name())." has completed on ".SYSTEM_MANAGER::get_system_id();

					if($this->is_notify())
						$this->notify($subject,$subject,$this->get_file());

					$this->set_state(BASE_DBQ_PROCESS::STATE_COMPLETED);
				}

			} catch(Exception $e) {
				if(!$this->is_debug()) {
					$this->set_state(BASE_DBQ_PROCESS::STATE_FAILED);
					$this->set_message($e->getMessage());
				}
			}

			if(!$this->is_debug()) {
				$this->set_end_time(time());
				$this->set_percent(100);
				$this->save();
			}

			return $has_success;
		}

		public function progress($current,$total) {

			if($total) {
				$chunk = floor($total / 100);
				
				if($chunk && ($current % $chunk === 0))
					$this->save_progress(($current/$total)*100);
			}
		}

		public function save_progress($percent) {

			$this->set_percent($percent);
			$this->set_end_time(time());
			
			if(!$this->is_debug())
				$this->save();
		}

		public function notify($subject,$message,$file=null) {
			
			if($this->get_notify()) {
				
				try {

					$recipients = explode(",",$this->get_notify());

					$email_cmodel = SYSTEM_MANAGER::get_email($subject,$message);
					
					if($file)
						$email_cmodel->add_attachment($file);

					$email_cmodel->send($recipients);
					
				} catch(Exception $e) {}
			}
		}
		
		public function save() {

			$is_object_tagged = $this->is_column_tagged("object");

			if($is_object_tagged)
				$this->set_object(serialize($this->get_object()));

			$has_success = false;
			if(is_numeric(self::get_process_id())) {
				$has_success = $this->get_dbo("process")->update();
				
			} else {
				
				if($this->get_percent()===null)
					$this->set_percent(0);

				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());
					
				$this->set_created_time(time());

				if(!$this->get_state())
					$this->set_state(BASE_DBQ_PROCESS::STATE_QUEUED);
					
				$has_success = $this->get_dbo("process")->insert();
			}
			
			return $has_success;
		}

		static function process_queued($process_hmodel=null,$debug=null) {

			$process_hmodel = $process_hmodel ? $process_hmodel : BASE_HMODEL_PROCESS::create(false);
			$process_hmodel->set_state(BASE_DBQ_PROCESS::STATE_QUEUED);

			foreach($process_hmodel->get_processes() as $process_cmodel) {
				if(!is_null($debug))
					$process_cmodel->debug($debug);
				$process_cmodel->process();				
			}

			return true;
		}

		public static function is_process_running($name) {
			return BASE_DBQ_PROCESS::create()
							->where("state","=",BASE_DBQ_PROCESS::STATE_RUNNING)
							->where("name","=",$name)
							->exists();
		}
		
		public function populate($data,$untag=false) {

			if(@array_key_exists("object",$data)) {
				$this->set_object(unserialize($data["object"]));
				unset($data["object"]);
			}

			return parent::populate($data,$untag);
		}
	}


