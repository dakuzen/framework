<?
	class BASE_CMODEL_SITEMAP extends CMODEL {
	   
        protected $_xml_writer_util = null;

        public function __construct() {
            $this->_xml_writer_util = XML_WRITER_UTIL::create()
                                    ->open("urlset",array(  "xmlns"=>"http://www.sitemaps.org/schemas/sitemap/0.9",
                                                            "xmlns:xsi"=>"http://www.w3.org/2001/XMLSchema-instance",
                                                            "xsi:schemaLocation"=>"http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd"));
        }

        public function add($url,$modified="",$priority=1,$frequency="") {

            $this->_xml_writer_util->open("url");

            $this->_xml_writer_util->add("loc",$url);

            if($modified)
                $this->_xml_writer_util->add("lastmod",$modified);

            $frequency = strtolower($frequency);
            if(in_array($frequency,array("always","hourly","daily","weekly","monthly","yearly","never")))
                $this->_xml_writer_util->add("changefreq",$frequency);

            if($priority)
                $this->_xml_writer_util->add("priority",$priority);


            $this->_xml_writer_util->close("url");

            return $this;
        }
        
        public function get() {
            return $this->_xml_writer_util->close("urlset")->string();
        }
		
	}