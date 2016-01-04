<?
  
  	class BASE_CMODEL_SERVER extends CMODEL {

		public function exec_stream($cmd,$callback=null,$options=["capture"=>true]) {

			if(value($options,"capture",true))
				$cmd = $cmd." 2>&1";

			$descriptorspec = array(
			   0 => array("pipe", "r"),   // stdin is a pipe that the child will read from
			   1 => array("pipe", "w"),   // stdout is a pipe that the child will write to
			   2 => array("pipe", "w")    // stderr is a pipe that the child will write to
			);

			$process = proc_open($cmd, $descriptorspec, $pipes, realpath('./'), null);
			
			if(!is_resource($process)) 
				throw new Exception("Failed to get process resource");
				
			$errors = [];
			while($s=fgets($pipes[2])) { 
	          $errors[] = $s;
	          $callback($s);
	        } 

		    while($s = fgets($pipes[1])) {
		    	$errors[] = $s;
		        $callback($s);
		    }

			$return = @proc_close($process);

			@fclose($pipes[0]);
			@fclose($pipes[1]);
			@fclose($pipes[2]);

			if($return)
				throw new Exception(implode("\n",$errors));

			return $this;
		}

		public function exec($cmd,&$output=null,$options=["capture"=>true]) {

			if($capture)
				$cmd = $cmd." 2>&1";

			$this->log($cmd);
			
			$return_var = null;
			exec($cmd,$output,$return_var);

			if($return_var)
				throw new Exception(implode("\n",$output),$return_var);
			
			return $this;
		}		
	}