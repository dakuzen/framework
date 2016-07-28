<?
	class BASE_ACTION_ARTICLE_CSS extends BASE_ACTION_ARTICLE_BASE {

		function process() {

			try {

				$css_timestamp = time();

				$storage_cmodel = SYSTEM_MANAGER::storage();

				$css = CMODEL_SCSS::create(array(CMODEL_SCSS::OPTION_COMPRESS,CMODEL_SCSS::OPTION_MINIFY))->compile($this->post("content"));

				$storage_cmodel->put($css, $storage_cmodel->sanitize($this->get_file("css/style_".$css_timestamp.".css")));

				$response = $storage_cmodel->response();

				$css_url = $this->post("content") ? $response["ObjectURL"] : "";

				CMODEL_FF_ARTICLE_SETTING::create()
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_CSS,$this->post("content"))
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_CSS_URL,$css_url)
						->set(CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_CSS_TIMESTAMP,$css_timestamp)
						->save();

				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}

			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the css");
		}
	}


