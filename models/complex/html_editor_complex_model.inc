<?php
        /**
        * Class BASE_CMODEL_HTML_EDITOR
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_HTML_EDITOR extends MODEL {

		const TYPE_FILE			= "files";
		const TYPE_IMAGE		= "images";
		const TYPE_FLASH		= "flash";
		const TYPE_MEDIA		= "media";
		
                /**
                 * Return type list
                 * @return array 
                 */
		static function get_type_list() { return array(self::TYPE_IMAGE=>"Images",self::TYPE_FILE=>"Files",self::TYPE_MEDIA=>"Media",self::TYPE_FLASH=>"Flash"); }

                /**
                 * Sets root directory
                 * @param string $value
                 */
		function set_root_directory($value) 	{ $this->set_value("root_directory",$value); }
                
                /**
                 * Sets root directory
                 * @param string $value
                 */
		function set_root_url($value) 		{ $this->set_value("root_url",$value); }
                
                /**
                 * Sets root directory
                 * @param string $value
                 */
		function set_image_extensions($value) 	{ $this->set_value("exts_image",$value); }
                
                /**
                 * Sets file extensions
                 * @param string $value
                 */
		function set_file_extensions($value) 	{ $this->set_value("exts_file",$value); }
                
                /**
                 * Sets media extensions
                 * @param string $value
                 */
		function set_media_extensions($value) 	{ $this->set_value("exts_media",$value); }
                
                /**
                 * Sets flash extensions
                 * @param string $value
                 */
		function set_flash_extensions($value) 	{ $this->set_value("exts_flash",$value); }
		
                /**
                 * Returns the root directory
                 * @return string 
                 */
		function get_root_directory() 		{ return $this->get_value("root_directory"); }
                
                /**
                 * Returns the root directory
                 * @return string 
                 */
		function get_root_url() 		{ return $this->get_value("root_url"); }
                
                /**
                 * Returns the root directory
                 * @return string 
                 */
		function get_image_extensions() 	{ return $this->get_value("exts_image"); }
                
                /**
                 * Returns the file extensions
                 * @return mixed 
                 */
		function get_file_extensions() 		{ return $this->get_value("exts_file"); }
                
                /**
                 * Returns the media extensions
                 * @return mixed 
                 */
		function get_media_extensions() 	{ return $this->get_value("exts_media"); }
                
                /**
                 * Returns the flash extensions
                 * @return mixed 
                 */
		function get_flash_extensions() 	{ return $this->get_value("exts_flash"); }
		
                /**
                 * Returns allowed resource types
                 * @return mixed 
                 */
		function get_allowed_resource_types() { 
			$defaults = array(BASE_CMODEL_HTML_EDITOR::TYPE_IMAGE,BASE_CMODEL_HTML_EDITOR::TYPE_FLASH,BASE_CMODEL_HTML_EDITOR::TYPE_FILE,BASE_CMODEL_HTML_EDITOR::TYPE_MEDIA);		
			return $this->get_value("resource_types",$defaults); 
		}
		
                /**
                 * Return allowed resource type list
                 * @return array 
                 */
		function get_allowed_resource_type_list() {
			$list = array();
			
			foreach($this->get_allowed_resource_types() as $type)
				$list[$type] = get_value(self::get_type_list(),$type);
			return $list;
		}
		
                /**
                 * Set value
                 * @param string $key
                 * @param string $value 
                 */
		function set_value($key,$value) {
			$_SESSION["htmleditor"][$key] = $value;
		}
		
                /**
                 * Get value
                 * @param string $key
                 * @param string $default
                 * @return mixed 
                 */
		function get_value($key,$default=null) {
			return get_value($_SESSION,array("htmleditor",$key),$default);
		}
	}