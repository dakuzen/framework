<?
    /**
    * Class BASE_CMODEL_MESSAGE_QUEUE_ATTACHMENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_MESSAGE_QUEUE_ATTACHMENT extends MODEL_COMPLEX {
                
    
        /**
         * Constructor 
         */
		public function __construct() {
			$this->register_dbo(new BASE_DBO_MESSAGE_QUEUE_ATTACHMENT());
		}
		
        /**
         * Returns the local file
         * @return string
         */
		public function get_local_file() 	{ return MODEL_PATH::get_temp_directory().$this->get_local_filename(); }

        /**
         * Returns the local filename
         * @return string 
         */
        public function get_local_filename() 	{ return $this->get_guid().".".$this->get_extension(); }

        /**
         * Returns the extension
         * @return string 
         */
        public function get_extension() 	{ return FILE_UTIL::get_file_extension($this->get_filename()); }

        /**
         * Returns the state name
         * @return string 
         */
		public function get_state_name()	{ return value(BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::get_state_list(),$this->get_state()); }
        public function is_type_attachment(){ return $this->get_type()==BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::TYPE_ATTACHMENT; }
        public function is_type_embed()     { return $this->get_type()==BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::TYPE_EMBED; }

        /**
         * Deletes the email event queue attachment
         * @return boolean 
         */
		public function delete() {
			$this->set_state(BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::STATE_DELETED);
			return $this->get_dbo("message_queue_attachment")->update();
		}

		public function purge_file() {
			FILE_UTIL::delete($this->get_local_file());
		}         
		       
        /**
         * Saves
         * @return boolean 
         */
		public function save() {
				
			$has_success = false;
			if(is_numeric(self::get_message_queue_attachment_id())) {
				$has_success = $this->get_dbo("message_queue_attachment")->update();
			} else {
				
				if(!$this->get_guid())
                    $this->set_guid(MISC_UTIL::get_guid());

                if(!$this->get_type())
                    $this->set_type(BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::TYPE_ATTACHMENT);
				
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::STATE_ACTIVE);
				
				$has_success = $this->get_dbo("message_queue_attachment")->insert();
			}
			
			return $this;
		}
		
        /**
         * Commits the file
         * @param string $file
         * @return boolean
         */
		public function commit_file($file) {
			return FILE_UTIL::copy($file,$this->get_local_file());					
		}

        /**
         * Populates
         * @param array $data
         * @param boolean $untag
         * @return mixed
         * @todo review return type 
         */
		public function populate($data,$untag=false) {								
			return $this->populate_dbo($data,$untag);
		}
	}