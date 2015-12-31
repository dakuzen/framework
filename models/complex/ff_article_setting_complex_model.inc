<?
	class BCMODEL_FF_ARTICLE_SETTING extends CMODEL {

		const SETTING_SITE_POST_URL 		= "SITE_POST_URL";
		const SETTING_SITE_PAGE_URL 		= "SITE_PAGE_URL";
		const SETTING_SITE_CSS 				= "SITE_CSS";
		const SETTING_SITE_JS 				= "SITE_JS";
		const SETTING_SITE_JS_URL 			= "SITE_JS_URL";
		const SETTING_SITE_CSS_URL			= "SITE_CSS_URL";
		const SETTING_SITE_CSS_TIMESTAMP	= "SITE_CSS_TIMESTAMP";
		const SETTING_SITE_JS_TIMESTAMP		= "SITE_JS_TIMESTAMP";		
	
		public function __construct() {
			return parent::__construct(DBO_FF_ARTICLE_SETTING::create());
		}
		
		public function get_manage_url()	{ return "/manage/ffarticlesetting/fasid:".$this->get_ff_article_setting_id()."/"; }
		public function get_url()			{ return "/ffarticlesettings/ffarticlesetting/fasid:".$this->get_ff_article_setting_id()."/"; }

		public function get_list()	{
			return [	self::SETTING_SITE_PAGE_URL=>[ "name"=>"Site Page URL","placeholder"=>"http://www.site.com/", "cache"=>true],
						self::SETTING_SITE_POST_URL=>[ "name"=>"Site Post URL","placeholder"=>"http://www.site.com/blog/", "cache"=>true],
						self::SETTING_SITE_CSS=>[ "name"=>"Site CSS", "editable"=>false ],
						self::SETTING_SITE_JS=>[ "name"=>"Site JS", "editable"=>false ],
						];
		}

		public function save() {

			$this->set_article_setting_id(BASE_DBQ_FF_ARTICLE_SETTING::create()
												->where("type","=",$this->get_type()) 
												->one("article_setting_id"));
			
			if(is_numeric(self::get_article_setting_id())) {
				if(!$this->dbo("ff_article_setting")->update())
					throw new Exception("Error updating ".get_class());
				
			} else {

				$this->set_cache(value(self::get_list(),array($this->get_type(),"cache")));

				if(!$this->dbo("ff_article_setting")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			//	}
			//	if(!DB::complete_transaction())
			//		throw new Exception("Error saving ".get_class());
			//} catch(Exception $e) {
			//	DB::complete_transaction();
			//	throw $e;
			//}
	
			return $this;
		}

		public static function get($type) {

			if(is_array($type))
				return BASE_DBQ_FF_ARTICLE_SETTING::create()
									->where("type","IN",$type)
									->column("value","type");
			else 
				return BASE_DBQ_FF_ARTICLE_SETTING::create()
									->where("type","=",$type)
									->one("value");
		}

		public static function set($type,$value) {
			return CMODEL_FF_ARTICLE_SETTING::create()
						->set_type($type) 
						->set_value($value)
						->save();
		}

		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_article_setting_id",
													"get_name",
													"get_value",
													))->arry();

			return $arry;
		}		
	}