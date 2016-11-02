<?

  	class BASE_CMODEL_SERVER extends CMODEL {

		public function exec_stream($cmd) {

			$callback = function($line) {
							echo $line;
						};

			SERVER_UTIL::exec_stream($cmd,$callback);

			return $this;
		}

		public function exec($cmd,&$output=null,$options=["capture"=>true]) {

			if(value($options,"capture",true))
				$cmd = $cmd." 2>&1";

			$this->log($cmd);

			$return_var = null;
			exec($cmd,$output,$return_var);

			$this->log($output);

			if($return_var)
				throw new Exception(implode("\n",$output),$return_var);

			return $this;
		}

		public function get($cmd,$options=["capture"=>true]) {
			$output = "";
			$this->exec($cmd,$output,$options);
			return $output;
		}
	}