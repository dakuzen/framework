<?
	class BASE_VIEW_ARTICLE_VIEW extends VIEW {
		
		protected $_article_cmodel = null;
		
		function __construct() {
			$this->set_route("article","view")->disable_authorization();
		}

		function init() {
		
			$content = "";		
			
			if($this->_article_cmodel) {
			
				/*
				$this->_article_cmodel->get_smarty()
							->assign("head",$this->_article_cmodel->get_head())
							->assign("path",$this->_article_cmodel->get_path())
							->assign("title",$this->_article_cmodel->get_title());
				
				try {
					$content = $this->_article_cmodel->get_content(true);
				} catch(Exception $e) {
					
					if(!SYSTEM_MANAGER::is_system_mode_production())
						APPLICATION::add_error_message($e->getMessage());
				}
				*/

				$content = $this->_article_cmodel->get_content(true);
				
				//if($this->_article_cmodel->is_body_wrap_none())					
				//	die($content);
			}

			$this->set_var("content",$content);
		}
		
		function set_article($article_cmodel) 	{ $this->_article_cmodel = $article_cmodel; return $this; }
	}

