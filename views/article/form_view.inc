<?
	class BASE_VIEW_ARTICLE_FORM extends BASE_VIEW_ARTICLE_BASE {
	
		protected $_ff_form_cmodel = null;
		
		function __construct() {
			parent::__construct("article","form",array(SECURITY_MANAGER::ROLE_BACKEND));
			$this->type_popup();			
			$this->_ff_form_cmodel = CMODEL_FF_FORM::create();
		}
		
		function init() {
			
			if($ff_form_id=$this->get("fid"))
				$this->load($ff_form_id);
	
			$message_list = ARRAY_UTIL::listing(HMODEL_MESSAGE::create()
								->order("name")
								->gets(),"get_name","get_message_id");		
			
			$this->set_var("message_list",$message_list);	
			$this->set_var("form",$this->_ff_form_cmodel);
		}	

		function load($ff_form_id) {

			$ff_form_cmodel = HMODEL_FF_FORM::create(false)->get($ff_form_id);

			if($ff_form_cmodel)
				$this->_ff_form_cmodel = $ff_form_cmodel;
		}		
	}