<?
    /**
    * Class BASE_CMODEL_GRAPHIC_BLOCK
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_GRAPHIC_BLOCK extends MODEL_COMPLEX {
		
		protected $_image_file;

		/**
		* Constructor
		*/
		public function __construct($dbo=null) {
			$this->register_dbo(new BASE_DBO_GRAPHIC_BLOCK());
			if($dbo)
				$this->register_dbo($dbo);
		}

	    /**
	     * Checks whether type is text
	     * @return boolean 
	     */
		public function is_type_text() 					{ return $this->get_type()==BASE_DBQ_GRAPHIC_BLOCK::TYPE_TEXT; }
                
	    /**
	     * Checks whether type is image
	     * @return boolean 
	     */
		public function is_type_image() 				{ return $this->get_type()==BASE_DBQ_GRAPHIC_BLOCK::TYPE_IMAGE; }

		/**
		 * Checks whether type is barcode
		 * @return boolean 
		 */
		public function is_type_barcode() 				{ return $this->get_type()==BASE_DBQ_GRAPHIC_BLOCK::TYPE_BARCODE; }

		/**
		 * Checks whether halign is left
		 * @return boolean 
		 */
		public function is_halign_left() 				{ return $this->get_halign()==BASE_DBQ_GRAPHIC_BLOCK::HALIGN_LEFT; }

		/**
		 * Checks whether halign is center
		 * @return boolean 
		 */
		public function is_halign_center() 				{ return $this->get_halign()==BASE_DBQ_GRAPHIC_BLOCK::HALIGN_CENTER; }

		/**
		 * Checks whether halign is right
		 * @return boolean 
		 */
		public function is_halign_right() 				{ return $this->get_halign()==BASE_DBQ_GRAPHIC_BLOCK::HALIGN_RIGHT; }

		/**
		 * Returns the width
		 * @return integer 
		 */
		public function get_width() 					{ return $this->get_x2() - $this->get_x1(); }

		/**
		 * Returns the height
		 * @return integer 
		 */
		public function get_height() 					{ return $this->get_y2() - $this->get_y1(); }

		/**
		 * Returns the width
		 * @return integer 
		 */
		public function get_font_size()					{ return parent::get_font_size() ? parent::get_font_size() : PDF_WRITER_UTIL::get_default_font_size(); }

		/**
		 * Returns the font name
		 * @return string 
		 */
		public function get_font_name()					{ return parent::get_font_name() ? parent::get_font_name() : PDF_WRITER_UTIL::get_default_font_name(); }

		/**
		 * Generate guid
		 * @return string
		 * @review return type 
		 */
		public function generate_guid()					{ return $this->set_guid(MISC_UTIL::get_guid()); }

		public function has_image_file()				{ return is_file($this->get_image_file()); }

		/* Override this function for custom image files */
		public function get_image_file() 				{ return $this->_image_file; }
		
		public function set_image_file($file) 			{ $this->_image_file = $file; }	
		
		public function get_x1($round=null)				{ return is_null($round) ? parent::get_x1() : round(parent::get_x1(),$round); }
		public function get_y1($round=null)				{ return is_null($round) ? parent::get_y1() : round(parent::get_y1(),$round); }
		public function get_x2($round=null)				{ return is_null($round) ? parent::get_x2() : round(parent::get_x2(),$round); }
		public function get_y2($round=null)				{ return is_null($round) ? parent::get_y2() : round(parent::get_y2(),$round); }

        /**
         * Returns image dimensions
         * @return array 
         */
		public function get_image_dimensions() { 

			if($this->has_image_file()) 
				return IMAGE_UTIL::get_image_size($this->get_image_file());

			return array();
		}	

		/**
		 * Save
		 * @return boolean 
		 */
		public function save() {
			
			DB::start_transaction();
			
			$has_success = false;
			
			if(is_numeric(self::get_graphic_block_id())) {
				$has_success = $this->get_dbo("graphic_block")->update();
			} else {
				
				$this->generate_guid();

				if(!$this->get_state())
					$this->set_state(BASE_DBQ_GRAPHIC_BLOCK::STATE_ACTIVE);
				
				$has_success = $this->get_dbo("graphic_block")->insert();
			}
			
			$has_success = DB::has_transaction_success();
			DB::complete_transaction();
			
			return $has_success ? $this : false;
		}

        /**
         * Return response data
         * @return array 
         */
		public function get_response_data() {
			$data["x1"] = $this->get_x1();
			$data["y1"] = $this->get_y1();
			$data["x2"] = $this->get_x2();
			$data["y2"] = $this->get_y2();
			return $data;
		}

        /**
         * Delete
         * @return boolean 
         */
		public function delete() {
			$this->set_state(BASE_DBQ_GRAPHIC_BLOCK::STATE_DELETED);
			return $this->get_dbo("graphic_block")->update();
		}
		
        /**
         * Populate
         * @param array $data
         * @param boolean $untag
         * @return boolean 
         */
		public function populate($data,$untag=false) {								
			return $this->populate_dbo($data,$untag);
		}

	}