<?	

	// http://www.httpuseragent.org/list/Robot

	class BASE_CMODEL_CRAWLER {
              
		public function is_crawler($user_agent=null) {

			$user_agent = is_null($user_agent) ? $user_agent : SERVER_UTIL::get_user_agent();
			
			$patterns = array(	"Bloglines subscriber",
								"Dumbot",
								"Sosoimagespider",
								"QihooBot",
								"FAST-WebCrawler",
								"Superdownloads Spiderman",
								"LinkWalker",
								"msnbot",
								"ASPSeek",
								"WebAlta Crawler",
								"Lycos",
								"FeedFetcher-Google",
								"Yahoo",
								"YoudaoBot",
								"AdsBot-Google",
								"Googlebot",
								"Scooter",
								"Gigabot",
								"Charlotte",
								"eStyle",
								"AcioRobot",
								"GeonaBot",
								"msnbot-media",
								"Baidu",
								"CocoCrawler",
								"Google",
								"Charlotte t",
								"Yahoo! Slurp China",
								"Sogou web spider",
								"YodaoBot",
								"MSRBOT",
								"AbachoBOT",
								"Sogou head spider",
								"AltaVista",
								"IDBot",
								"Sosospider",
								"Yahoo! Slurp",
								"Java VM",
								"DotBot",
								"LiteFinder",
								"Yeti",
								"Rambler",
								"Scrubby",
								"Baiduspider",
								"accoona");

			$pattern = "/".implode("|",$patterns)."/i";
			return preg_match($pattern,$user_agent)>0;
		}
    }