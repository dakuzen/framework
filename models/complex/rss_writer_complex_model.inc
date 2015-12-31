<?
    /**
    * Class BASE_CMODEL_SETTING_INITIALIZE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_RSS_WRITER extends MODEL_COMPLEX {
		
		protected $_rss 		= array("xmlns:atom"=>"http://www.w3.org/2005/Atom",
										"version"=>"2.0",
										);

		protected $_channel 	= array("title"=>"",
										"link"=>"",
										"description"=>"",
										"ttl"=>40,
										"language"=>"en-us");

		protected $_items		= array();

		function __construct() {
			
		}

		function set_channel($title,$link,$description,$channel=array()) {

			$this->_channel["title"] 		= $title;
			$this->_channel["link"] 		= $link;
			$this->_channel["description"] 	= $description;

			return $this;
		}

		function item($title,$description,$date,$link,$guid,$item=array()) {

			$date = @strtotime($date);

			$date = $date ? date('D, d M Y H:i:s T',$date) : "";

			$item["title"] 			= $title;
			$item["description"]	= $description;
			$item["date"] 			= $date;
			$item["guid"] 			= $guid;
			$item["link"] 			= $link;

			$this->_items[] = $item;

			return $this;
		}

		function generate() {

			$xml_writer_util = XML_WRITER_UTIL::create()
										->open("rss",$this->_rss);

			$xml_writer_util->open("channel");

			foreach($this->_channel as $name=>$value)
				$xml_writer_util->add($name,$value);	

			$xml_writer_util->close("channel");

			foreach($this->_items as $item) {
				$xml_writer_util
					->open("item");

				foreach($item as $name=>$value)
					$xml_writer_util->add($name,$value);
				
				$xml_writer_util->close("item");
			}


			$xml_writer_util->close("rss");

			return $xml_writer_util->string();


		}
        
		function stream() {
			header("Content-Type: application/xml; charset=ISO-8859-1");
			echo $this->generate();
		}
        
	}

/*

<?xml version="1.0" encoding="UTF-8"?>
<rss xmlns:atom="http://www.w3.org/2005/Atom" version="2.0" xmlns:twitter="http://api.twitter.com" xmlns:georss="http://www.georss.org/georss">
  <channel>
    <title>Twitter / MortgageOgre</title>
    <link>http://twitter.com/MortgageOgre</link>
    <atom:link type="application/rss+xml" rel="self" href="https://api.twitter.com/1/statuses/user_timeline.rss?screen_name=mortgageogre"/>
    <description>Twitter updates from MortgageOgre.com / MortgageOgre.</description>
    <language>en-us</language>
    <ttl>40</ttl>
  <item>
    <title>MortgageOgre: east side mario's</title>
    <description>MortgageOgre: east side mario's</description>
    <pubDate>Tue, 09 Apr 2013 12:47:08 +0000</pubDate>
    <guid>http://twitter.com/MortgageOgre/statuses/321605196025307137</guid>
    <link>http://twitter.com/MortgageOgre/statuses/321605196025307137</link>
    <twitter:source>web</twitter:source>
    <twitter:place/>
  </item>
  <item>
    <title>MortgageOgre: sadads</title>
    <description>MortgageOgre: sadads</description>
    <pubDate>Sat, 09 Mar 2013 17:46:14 +0000</pubDate>
    <guid>http://twitter.com/MortgageOgre/statuses/310446441959129088</guid>
    <link>http://twitter.com/MortgageOgre/statuses/310446441959129088</link>
    <twitter:source>&lt;a href=&quot;http://twitter.com/tweetbutton&quot; rel=&quot;nofollow&quot;&gt;Tweet Button&lt;/a&gt;</twitter:source>
    <twitter:place/>
  </item>
  </channel>
</rss>
*/