<?
	class BASE_ACTION_ARTICLE_AUTHORIMAGEUPLOAD extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
			
			$article_author_cmodel = HMODEL_FF_ARTICLE_AUTHOR::get($this->post("aaid"));

			if(!$article_author_cmodel)
				throw new Exception("Invalid author");	
	
			$temp_cmodel = CMODEL_TEMP::create();

			try {

				foreach(UPLOAD_UTIL::uploads($temp_cmodel->dir(),array("jpg")) as $upload_util) {
					
					$upload_util->upload();

					$article_author_cmodel->commit_image($upload_util->file());
				}

				$this
					->data("image_url",$article_author_cmodel->get_image()->url("small"))
					->success();
			
			} catch(Exception $e) {
				$this->error($e);
				$temp_cmodel->cleanup();
			}
		
			$temp_cmodel->cleanup();
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to upload the image");
		}	
	}
	

