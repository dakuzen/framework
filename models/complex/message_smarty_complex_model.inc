<?
	class BASE_CMODEL_MESSAGE_SMARTY extends CMODEL_SMARTY {

		protected $_plugin_enabled = true;

	    public static function create($compile_dir="",$cache_dir="") {
			return new CMODEL_MESSAGE_SMARTY($compile_dir,$cache_dir);
	    }	

	    public function plugin_enabled($enabled=false) {

	    	if(!func_num_args())
	    		return $this->_plugin_enabled;

			$this->_plugin_enabled = $enabled; 

			return $this;
	    }	    
	}


