<?
	class BASE_VIEW_MANAGE_LOGLIST extends VIEW {
		
		protected $_log_cmodels = array();

		function __construct() {
			$this->set_route("manage","loglist")->type_blank();
		}
		
		function init() {

			if(SERVER_UTIL::is_post())
				$this->load();
			
			$this->set_var("logs",$this->_log_cmodels);	
		}

		function load() {

			$page_index	= $this->request("page_index") ? $this->request("page_index") : 0;
			$page_limit	= $this->request("page_limit") ? $this->request("page_limit") : 15;

			$search		= (array)$this->request("search");
			$state 		= value($search,"state") ? value($search,"state") : "";
			$type 		= value($search,"type") ? value($search,"type") : "";

			$log_hmodel = HMODEL_FF_LOG::create()
									->order("create_date","DESC");

			if($start=value($search,"start_date"))
				$log_hmodel->where("create_date",">=",DB::get_date($start));

			if($end=value($search,"end_date"))
				$log_hmodel->where("create_date","<=",CMODEL_TIME::create($end)->add_day()->db());

			if($type)
				$log_hmodel->where("type","=",$type);
			
			if($state)
				$log_hmodel->set_state($state);
			
			if($keyword=strtolower(value($search,"keyword"))) {
				$log_hmodel
					->where("url","LIKE","%".$keyword."%","OR","searchgroup")
					->where("message","LIKE","%".$keyword."%","OR","searchgroup");
			}
			
			$record_count = $log_hmodel->count();

			$log_hmodel->set_limit($page_limit,$page_limit * $page_index,$record_count);

			$this->_log_cmodels = $log_hmodel->gets();

			$paging = APPLICATION::get_base_view_instance("components","paging")
							->populate($page_index,$page_limit,$record_count,'$("#log-form .list").trigger("load")');
			
			$this->set_view("paging",$paging);

			$this->set_var("search",$search);
		}	
	}
	
	
				
				