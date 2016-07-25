<?
	class BASE_ACTION_ARTICLE_ARTICLEIMAGEUPLOAD extends BASE_ACTION_ARTICLE_BASE {

		function process() {

			$article_id			= $this->post("aid");

			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			if(!$article_cmodel)
				throw new Exception("Invalid article");

			$temp_cmodel = CMODEL_TEMP::create();

			try {
				foreach(UPLOAD_UTIL::uploads("file", $temp_cmodel->dir(),array("png","jpg")) as $upload_util) {

					$upload_util->upload();

					$article_cmodel->commit_image($upload_util->file());
				}

				$this
					->data("image_url",$article_cmodel->get_image()->url("small"))
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


