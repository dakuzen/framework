<?
	class BCMODEL_INSTANCE_API_CLIENT extends BASE_CMODEL_API_CLIENT {

		protected $_instance_dir = null;

		public function __construct($instance_dir) { 
			$this->_instance_dir = $instance_dir; 
		}

		public function get($method,$parms=array()) {

			$parms = array_merge(array("m"=>$method),$parms);

			foreach($parms as $index=>$parm)
				if(!is_numeric($index))
					$parms[$index] = "-".$index." ".$parm;
			
			$api_file = $this->_instance_dir."backend/command/api.php";

			//Legacy
			if(!is_file($api_file))
				$api_file = $this->_instance_dir."application/command/api.php";

			$cmd = "php ".$api_file." ".implode(" ",$parms);

			$this->log($cmd);
			
			$data = array();
			SERVER_UTIL::exec($cmd,$data);

			$data = implode("\n",$data);
			$this->log($data);

			$data = $this->process_response($data);

			return $data;
		}
	}