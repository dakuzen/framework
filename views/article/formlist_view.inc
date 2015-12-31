<?
	class BASE_VIEW_ARTICLE_FORMLIST extends BASE_VIEW_ARTICLE_BASE {
		
		protected $_ff_form_cmodels = array();

		function __construct() {
			parent::__construct("article","formlist",array(SECURITY_MANAGER::ROLE_BACKEND));

			$this->type_blank();
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
		}
		
		function init() {

		if(SERVER_UTIL::is_post())
				$this->load();
			$this->set_var("ff_forms",$this->_ff_form_cmodels);	
		}

		function load() {
			$page_index	= $this->request("page_index") ? $this->request("page_index") : 0;
			$page_limit	= $this->request("page_limit") ? $this->request("page_limit") : 25;

						
			$ff_form_hmodel = HMODEL_FF_FORM::create(false);
			$ff_form_hmodel->set_state(DBQ_FF_FORM::STATE_ACTIVE);
			$record_count = $ff_form_hmodel->count();

			$ff_form_hmodel->set_limit($page_limit,$page_limit * $page_index,$record_count);

			$this->_ff_form_cmodels = $ff_form_hmodel->gets();

			$this->get_view("paging")->populate($page_index,$page_limit,$record_count,'$("#ff-form-list").trigger("load")');
		}	
	}
	
	
				
				