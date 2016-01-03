<?
  
  	class BASE_CMODEL_SERVER extends CMODEL {

		public function exec($cmd,&$output=array(),$capture_output=true) {

			if($capture_output)
				$cmd = $cmd." 2>&1";

			$this->log($cmd);
			
			$return_var = null;
			exec($cmd,$output,$return_var);

			if($return_var)
				throw new Exception(implode("\n",$output),$return_var);
			
			return $this;
		}		
	}