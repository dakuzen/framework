<?php
    /**
    * Class BASE_CMODEL_OPEN_GRAPH_META
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_OPEN_GRAPH_META extends BASE_CMODEL_META {

        /**
         * The locale
         * @var string $locale 
         */
		protected $_locale 		= "en_US";
        
        /**
         * Constructor
         * @param string $title
         * @param string $site_name
         * @param string $description
         * @param string $image_url
         * @param string $caption
         * @param string $url 
         */
		function __construct($title,$site_name,$description="",$image_url="",$caption="",$url="") {
			
            $url = $url ? $url : SERVER_UTIL::get_request_url();

            $this->set_locale("en_US");
            $this->set_title($title);
            $this->set_site_name($site_name);
			$this->set_description($description);
			$this->set_image_url($image_url);
			$this->set_caption($caption);
			$this->set_url($url);		
		}
			
        /**
         * Sets the locale
         * @param string $locale 
         */
        function set_locale($locale)               { $this->add_meta("og:locale",$locale); }

        /**
         * Sets the title
         * @param string $title 
         */
        function set_title($title)          		{ $this->add_meta("og:title",$title); }
        
        /**
         * Sets the site name
         * @param string $site_name 
         */
		function set_site_name($site_name) 			{ $this->add_meta("og:site_name",$site_name); }
        
        /**
         * Sets the description
         * @param string $description 
         */
		function set_description($description)	 	{ $this->add_meta("og:description",$description); }
                
        /**
         * Sets the image url
         * @param string $image_url 
         */
		function set_image_url($image_url) {
                
                $image = URL_UTIL::remove_protocol($image_url);
                
                $meta[] = self::get_meta_tag("og:image:secure_url","https://".$image);
                $meta[] = self::get_meta_tag("og:image","http://".$image);
                        
                $ext = FILE_UTIL::get_file_extension($image);
                
                $type = get_value(array("gif"=>"gif","png"=>"png","jpg"=>"jpeg","jpeg"=>"jpeg"),$ext,"jpeg");
                
                $meta[] = self::get_meta_tag("og:image:type","image/".$type);
        }
        
        /**
         * Sets the caption
         * @param string $caption 
         */
		function set_caption($caption) 			{ $this->add_meta("og:caption",$caption); }
        
        /**
         * Sets the url
         * @param string $url 
         */
		function set_url($url) 				{ $this->add_meta("og:url",$url); }
                
        /**
         * Sets the facebook app id
         * @param integer $facebook_app_id 
         */
		function set_facebook_app_id($facebook_app_id)	{ $this->add_meta("fb:app_id",$facebook_app_id); }
                
        /**
         * Sets the facebook admins
         * @param array $facebook_admins 
         */
		function set_facebook_admins($facebook_admins)	{ $this->add_meta("fb:admins",implode(",",$facebook_admins)); }		
	}
	
	
	
	