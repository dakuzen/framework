<?
	class BHMODEL_FF_ARTICLE_SETTING extends HMODEL {
	
		protected $_select_fields = array("ff_article_settings.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE_SETTING::create());
		}
		
		public function gets() {			
			$ff_article_settings = $this->get_dbq()->select($this->_select_fields);

			$ff_article_setting_cmodels = array();			
			foreach($ff_article_settings as $ff_article_setting) {
				$ff_article_setting_cmodel = CMODEL_FF_ARTICLE_SETTING::create()->populate($ff_article_setting,true,false);
				$this->apply_properties($ff_article_setting_cmodel,$ff_article_setting);
				$ff_article_setting_cmodels[] = $ff_article_setting_cmodel;
			}
			
			if(!$ff_article_setting_cmodels)
				return array();
			
			return $ff_article_setting_cmodels;
		}
		
		public function set_ff_article_setting_ids($values)	{ return $this->where("ff_article_settings.ff_article_setting_id","IN",$values); }
		public function set_ff_article_setting_id($value)	{ return $this->where("ff_article_settings.ff_article_setting_id","=",$value); }
		

		public static function create($defaults=true) {
			return $defaults ? parent::create() : parent::create();
		}		
	}
	