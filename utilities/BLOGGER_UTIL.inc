<?php
        /**
        * Class BLOGGER_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 	
	class BLOGGER_UTIL {
	
		/**
                 * Create a client
		 * @param string $username
                 * @param string $password
                 * @param integer $blog_id
                 * 
                 * @return object
	 	 */
		static function create_client($username, $password,$blog_id=null) {
			$client = new SimpleCRUD($username, $password);
			
			if($blog_id)
				$client->blogID = $blog_id;
			
			return $client;
		}
		
                /**
                 * Get the blog id
		 * @param string $username
                 * @param string $password
                 * 
                 * @return integer
	 	 */
		static function get_blog_id($username, $password) {
			$client = self::create_client($username, $password);
			
			$query = new Zend_Gdata_Query("http://www.blogger.com/feeds/default/blogs");
			$feed = $client->gdClient->getFeed($query);
						
			$entry = isset($feed->entries[0]) ? $feed->entries[0] : null;
			
			if($entry) {
				$text = isset($entry->id->text) ? $entry->id->text : "";

				preg_match("/\d+$/",$text,$matches);
				return get_value($matches,0);
			}
			
			return null;
		}
		
                /**
                 * Get the blog information
		 * @param string $username
                 * @param string $password
                 * 
                 * @return array
	 	 */
		static function get_blog_info($username, $password) {
			$client = self::create_client($username, $password);
			
			$query = new Zend_Gdata_Query("http://www.blogger.com/feeds/default/blogs");
			$feed = $client->gdClient->getFeed($query);
			
			$entry = isset($feed->entries[0]) ? $feed->entries[0] : null;

			$info["blog_id"] 	= null;
			$info["blog_url"] 	= null;
			
			if($entry) {
				$text = isset($entry->id->text) ? $entry->id->text : "";
				$links = (isset($entry->link) && is_array($entry->link)) ? $entry->link : array();
				
				foreach($links as $link) {
					
					if($link->getType()=="text/html")
						$info["blog_url"] = $link->getHref();
					
				}

				preg_match("/\d+$/",$text,$matches);
				$info["blog_id"] = get_value($matches,0);
			}
			
			return $info;
		}		
	}
        
        SERVER_UTIL::add_include_path(DIR_FRAMEWORK."packages/ZendGdata/library/");

	require_once DIR_FRAMEWORK."packages/blogger/SimpleCRUD.inc";

	Zend_Loader::loadClass('Zend_Gdata_Feed');