<?
	/**
	* Class RSS_READER_UTIL
	*
	* @package Framework
	* @subpackage Utilities
	*/
	
	class RSS_READER_UTIL {
                
        /**
         * Constructor.
         * 
         */
		public static function create() {
			return new RSS_READER_UTIL();
		}

        /**
         * Get the url address
         * 
         * @param string $url the url address
         */
		function get($url) {

			$rss = array();

			$items = array();
			
			$rss["encoding"] 	= ""; 
			$rss["language"] 	= ""; 
			$rss["title"] 		= "";
			$rss["link"]  		= "";
			$rss["image_title"]  	= "";
			$rss["image_url"]  	= "";
			$rss["image_link"]	= "";
			$rss["language"]	= "";
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$has_success = $http_transport_util->get($url);

			$xpath_util = new XPATH_UTIL($http_transport_util->get_body());

			$root = get_value($xpath_util->select("(//*)[1]"),0);

			if(!$root)
				throw new Exception("Invalid XML");				

			if(preg_match("/feedburner\.com/",$http_transport_util->get_http_request()->get_uri()->get_host())) {

				if($title=$xpath_util->select_value("/feed/title"))
					$rss["title"] = $title;

				if($title=$xpath_util->select_value("/rss/channel/title"))
					$rss["title"] = $title;

				foreach($xpath_util->select("/rss/channel/item") as $entry) {

					$item["title"] 			= (string)$entry->title;
					$item["link"] 			= (string)$entry->link;
					$item["guid"] 			= (string)$entry->guid;
					$item["date"] 			= (string)$entry->pubDate;
					$item["pubDate"] 		= (string)$entry->pubDate;
					$item["description"] 	= (string)$entry->description;
					$items[] 				= $item;
				}
				
				foreach($xpath_util->select("/feed/entry") as $entry) {

					$item["guid"] 			= (string)$entry->id;
					$item["pubDate"] 		= (string)$entry->published;
					$item["date"] 			= (string)$entry->published;
					$item["description"] 	= (string)$entry->content;
					$item["title"] 			= (string)$entry->title;
					$item["link"] 			= (string)$entry->origLink;
					$items[] 				= $item;
				}	

			} else {

				if($root->getName()=="feed") {

					//Atom

					if($title=$xpath_util->select_value("/feed/title"))
						$rss["title"] = $title;

					foreach($xpath_util->select("/feed/entry") as $entry) {

						$item["guid"] 			= (string)$entry->id;
						$item["date"] 			= (string)$entry->updated;
						$item["description"] 	= (string)$entry->content;
						$item["title"] 			= (string)$entry->title;
						$item["link"] 			= (string)$entry->link->attributes()->href;
						$items[] 				= $item;
					}

				} elseif($root->getName()=="rss") {
			
					// RSS 2.0

					$rss["language"] 	= $xpath_util->select_value("/rss/channel/language");
					$rss["title"] 		= $xpath_util->select_value("/rss/channel/title");
					$rss["link"]  		= $xpath_util->select_value("/rss/channel/link");
					$rss["image_title"] = $xpath_util->select_value("/rss/channel/image/title");
					$rss["image_url"]  	= $xpath_util->select_value("/rss/channel/image/url");
					$rss["image_link"]	= $xpath_util->select_value("/rss/channel/image/link");
					
					foreach($xpath_util->select("/rss/channel/item") as $entry) {

						$item["guid"] 			= (string)$entry->guid;
						$item["pubDate"] 		= (string)$entry->pubDate;
						$item["date"] 			= (string)$entry->pubDate;
						$item["description"] 	= (string)$entry->description;
						$item["comments"] 		= (string)$entry->comments;
						$item["title"] 			= (string)$entry->title;
						$item["link"] 			= (string)$entry->link;
						$items[] 				= $item;
					}
				}
			}

			$rss["items"]		= $items;
			$rss["items_count"] = count($items);
								
			return $rss;
		}

	}