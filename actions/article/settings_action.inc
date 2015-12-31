<?
	class BASE_ACTION_ARTICLE_SETTINGS extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$settings = (array)$this->post("settings");
			
			try {
				
				foreach($settings as $type=>$value) {

					if($type==CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_PAGE_URL || $type==CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_POST_URL)
						$value = rtrim($value,"/")."/";

					CMODEL_FF_ARTICLE_SETTING::create()
							->set_type($type) 
							->set_value($value)
							->save();
				}

				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the settings");
		}	
	}
	

