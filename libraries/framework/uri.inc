<?php
	/**
    * Class URI  
    *
    * @package Library
    * @subpackage Web
    */
	class URI {
            
        /**
         * The uri
         * @var string $uri 
         */
		private $_uri 			= "";
                
        /**
         * The arguments
         * @var array $arguments 
         */
		private $_arguments 		= array();
                
        /**
         * named arguments
         * @var array $named_arguments 
         */
		private $_named_arguments 	= array();
        
        /**
         * index arguments
         * @var array $index_arguments 
         */
		private $_index_arguments 	= array();
                
        /**
         * The controller
         * @var string $controller 
         */
		private $_controller 		= "";
                
        /**
         * The task
         * @var string $task 
         */
		private $_task	 		= "";
                
        /**
         * The parts
         * @var array $parts 
         */
		private $_parts		 	= array();
		
        /**
         * Constructor
         * @param string $uri 
         */
		function __construct($uri) {
			
			$this->populate($uri);
			
			$this->_controller = is_empty($this->_controller) ? "index" : $this->_controller;
			$this->_task       = is_empty($this->_task) ? "index" : $this->_task;
		}
		
        /**
         * Reutnr arguments
         * @return array 
         */
		function get_arguments() 		{ return $this->_arguments; }
                
        /**
         * Return argument
         * @param mixed $index
         * @return mixed 
         */
		function get_argument($index) 		{ return get_value($this->_arguments,$index); }
        
        /**
         * Returns the uri
         * @return string 
         */
		function get_uri() 			{ return $this->_uri; }
                
        /**
         * Return controller
         * @return object 
         */
		function get_controller() 		{ return $this->_controller; }
        
        /**
         * Return task
         * @return string 
         */
		function get_task()			{ return $this->_task ? $this->_task : "index"; }
        
        /**
         * Return parts
         * @return string 
         */
		function get_part($index)		{ return get_value($this->_parts,$index); }	

        /**
         * Return parts
         * @return array 
         */
		function get_parts()			{ return $this->_parts; }			

        /**
         * set controller
         * @param object $controller
         * @return null 
         */
		function set_controller($controller)	{ return $this->_controller = $controller; }	

        /**
         * is controller
         * @param object $controller
         * @return boolean 
         */
		function is_controller($controllers) {
			$controllers = (array)$controllers;
			
			foreach($controllers as $controller)
				if($this->get_controller()==$controller)
					return true;

			return false;
		}

		function is_task($tasks) {
			$tasks = (array)$tasks;
			
			foreach($tasks as $task)
				if($this->get_task()==$task)
					return true;

			return false;
		}		

		function is_route($controller,$task) {
			return $this->is_controller($controller) && $this->is_task($task);
		}

		function is_uri($uri) 			{ return trim($this->_uri,"/")==trim($uri,"/"); }

		function has_controllers($controllers)	{ return in_array($this->get_controller(),$controllers); }	        

        /**
         * Set task
         * @param string $task
         * @return null 
         */
		function set_task($task)		{ return $this->_task = $task; }
	
        /**
         * Checks whether action is task
         * @return boolean 
         */
		function is_action_task() 		{ return $this->is_action(); }
		function is_action() 			{ return stripos($this->get_task(),"do")===0; }
			
        /**
         * Returns expanded url
         * @return string
         */
		function get_expanded_url() {
			
			$parms = ARRAY_UTIL::get_imploded($this->_named_arguments,"&","","=");
			
			return "/".$this->_controller."/".$this->_task."/?".$parms;			
		}

		function get_path()		{ return "/".$this->get_controller()."/".$this->get_task()."/"; }	
		


		function populate($uri) {

			$parts = self::parse($uri,2);

			$this->_uri 			= get_value($parts,"uri");
			$this->_controller 		= get_value($parts,array("folders",0));
			$this->_task 			= get_value($parts,array("folders",1));
			$this->_arguments 		= get_value($parts,"arguments");
			$this->_named_arguments = get_value($parts,"named_arguments");
			$this->_index_arguments = get_value($parts,"index_arguments");
			$this->_parts			= get_value($parts,"parts");
		}

        /**
         * Populates
         * @param string $uri 
         */
		public static function parse($uri,$folder_count=0) {
			
			$folders = $index_arguments = $named_arguments = $args = $arguments = $parts = array();
			
			preg_match("/(.*?)(?:\?([^?]*))?$/",$uri,$matches);
			
			$uri = get_value($matches,1);
			
			$parts = explode("/",trim($uri,"/"));

			for($i=0;$i<$folder_count;$i++)
				if(strpos(current($parts),":")===false) 
					$folders[] = strtolower(array_shift($parts));
				
			foreach($parts as $part) {
				
				if(is_empty($part)) continue;

				if(strpos($part,":")!==false) {					
					
					$hash_args = explode(":",$part);
					if(!is_empty($key=$hash_args[0])) {
						$args[$key] 	= $hash_args[1];
						$args[] 	= $hash_args[1];
						
						$named_arguments[$key] 	= $hash_args[1];
						$index_arguments[] 	= $hash_args[1];
					}
						
				} else {
					$args[] = $part;
					$index_arguments[] = $part;
				}
			}
			
			if($query=get_value($matches,2)) {

				parse_str($query,$queries);

				foreach($queries as $name=>$query) {
					$args[$name] = $query;
					$args[] = $query;
					
					$named_arguments[$name] = $query;
					$index_arguments[] = $query;
				}	
			}
			
			$arguments = str_replace("%20"," ",$args);

			return array(	"uri"=>$uri,
							"parts"=>$parts,
							"folders"=>$folders,
							"arguments"=>$arguments,
							"named_arguments"=>$named_arguments,
							"index_arguments"=>$index_arguments);
		}
	}