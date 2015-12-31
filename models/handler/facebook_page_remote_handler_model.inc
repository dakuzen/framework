<? 
	class BASE_HMODEL_FACEBOOK_PAGE_REMOTE extends MODEL_HANDLER {
		
		protected $_facebook_user_id 	= null;
		protected $_class	 	= "CMODEL_FACEBOOK_PAGE";
		protected $_select_fields 	= array("page_id","name","type","pic_small","page_url","pic_square","pic","pic_large","fan_count","has_added_app");
		
		public function __construct($facebook_client_cmodel=null) {
			$this->_facebook_client_cmodel = $facebook_client_cmodel ? $facebook_client_cmodel : MODEL_FACEBOOK::create_client();			
			$this->_facebook_user_id = MODEL_FACEBOOK::get_active_user_id();
		}
		
		public function get_dbq() {
		
			$page_admin_dbq = new DBQ("page_admin");
			$page_admin_dbq->add_where("uid","=",$this->_facebook_user_id,"AND",false,true);
			$page_admin_sql = $page_admin_dbq->get_select_sql("page_id");
						
			$page_dbq = new DBQ("page");
			$page_dbq->add_where("page_id","IN",$page_admin_sql,"AND",false);
			
			$this->apply_constraints($page_dbq);
			
			return $page_dbq;
		}
		
		function get_facebook_pages() {
		
			if(!$this->_facebook_user_id)
				throw new Exception("Failed to load Facebook User ID");
						
			$page_dbq = $this->get_dbq();
			$query = $page_dbq->get_select_sql($this->_select_fields);
			
			$pages = $this->_facebook_client_cmodel->select($query);
			
			$facebook_page_cmodels = array();
			
			foreach($pages as $page) {

				$facebook_page_id = get_value($page,"page_id");
				
				unset($page["page_id"]);
				
				$page["likes"] = (int)get_value($page,"fan_count");
				
				$facebook_page_cmodel = new $this->_class();
				$facebook_page_cmodel->populate($page);
				$facebook_page_cmodel->set_facebook_page_id($facebook_page_id);
				$facebook_page_cmodel->set_state(DBQ_FACEBOOK_PAGE::STATE_ACTIVE);
				
				$facebook_page_cmodels[] = $facebook_page_cmodel;
			}

			return $facebook_page_cmodels;
		}
		
		function set_facebook_user_id($facebook_user_id) 	{ $this->_facebook_user_id = $facebook_user_id; }
		function set_class($class) 				{ $this->_class = $class; }
		function set_not_type($type) 				{ $this->add_filter("type","<>",$type,"","AND",false,true); }		
		function set_type($type) 				{ $this->add_filter("type","=",$type,"","AND",false,true); }		
		
		
		function set_not_page_ids($page_ids) 	{ 
			
			$db_group = new DB_GROUP();
			
			foreach($page_ids as $page_id)
				$db_group->add_where("page_id","!=",$page_id,"AND",false);
			
			$this->add_group($db_group);
		}
		
	}