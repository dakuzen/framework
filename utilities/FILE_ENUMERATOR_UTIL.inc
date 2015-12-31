<?php
    /**
    * Class FILE_ENUMERATOR_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	abstract class FILE_ENUMERATOR_UTIL {
        
        /**
         * File
         * @var string $file 
         */
		protected $_file 		= null;
                
        /**
         * Pre skip count
         * @var integer $pre_skip_count
         */
		protected $_pre_skip_count	= 0;
                
        /**
         * Bundle count
         * @var integer $bundle_count
         */
		protected $_bundle_count	= 1;
                
        /**
         * Mapping
         * @var array $mapping 
         */
		protected $_mapping		= array();
		
        /**
         * Constructor.
         * @param string $file the file.
         * @param integer $pre_skip_count the pre skip count.
         */
		public function __construct($file=null,$pre_skip_count=0) {
			$this->_file = $file;
			$this->_pre_skip_count = $pre_skip_count;
		}
		
        /**
         * Set the bundle count
         * @param integer $bundle_count 
         */
		public function set_bundle_count($bundle_count) { $this->_bundle_count = $bundle_count; }
                
        /**
         * Set the mapping
         * @param array $mapping 
         */
		public function set_mapping($mapping) 		{ $this->_mapping = $mapping; }
                
        /**
         * Add mapping
         * @param string $name
         * @param integer $index 
         */
		public function add_mapping($name,$index)	{ $this->_mapping[] = array($name,$index); }
		
        /**
         * Process of data
         * @param string $data 
         */
		abstract function process($data);
		
		//TODO: try this str_getcsv()

        /**
         * Enumerate
         * @return boolean 
         */
		public function enumerate() {
			
			$handler = @fopen($this->_file,"r");
			
			if($handler) {
				$count = 0;
				
				$data = array();
				
				while(!feof($handler)) {
					
					$line = fgets($handler); 
					
					if($this->_pre_skip_count>0) {
						$this->_pre_skip_count--;
						continue;
					}
					
					$line = preg_replace("/[\n\r]+$/","",$line);
					
					preg_match_all("/\"?([^\",]*)\"?(?:,|$)/",$line,$matches);
					
					if($this->_mapping) {
					
						
						foreach($this->_mapping as $mapping) {
						
							$key 	= get_value($mapping,0);
							$index 	= get_value($mapping,1);
							
							if($key===null || $key==="")
								$row[] = get_value($matches,array(1,$index));
							else
								$row[$key] = get_value($matches,array(1,$index));
						}
						
						$data[] = $row;
						
					} else
						$data[] = get_value($matches,1);$row = array();
					
					$count++;
					
					if($count>=$this->_bundle_count) {
						
						$data = $this->_bundle_count==1 ? get_value($data,0,array()) : $data;
						$this->process($data);
						
						$data = array();
						$count = 0;
					}
					
					
				}
				
				if($data) 
					$this->process($data);				
				
				fclose($handler);
				
				return true;
			} else
				return false;
		}
	}