<?
	class BASE_ACTION_ARTICLE_JS extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			try {

				$js_timestamp = time();
					
				$storage_cmodel = SYSTEM_MANAGER::storage();
				$storage_cmodel->put($storage_cmodel->sanitize($this->get_file("js/script_".$js_timestamp.".js")),$this->post("content"));

				$this->success();

				$response = $storage_cmodel->response();

				$js_url = $this->post("content") ? $response["ObjectURL"] : "";
				
				CMODEL_FF_ARTICLE_SETTING::create()
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_JS,$this->post("content"))
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_JS_URL,$js_url)
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_JS_TIMESTAMP,$js_timestamp)
						->save();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the js");
		}	
	}
	

