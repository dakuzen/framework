<?php
	class BASE_VIEW_MANAGE_EMAILLOGS extends VIEW {
		
		protected $_view_link 		= "";
		protected $_view_link_html 	= "";
	
		function __construct() {
			$this->set_title("Email Logs");
			
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
			$this->set_view("calendar",APPLICATION::get_base_view_instance("components","calendar"));
			
			$this->_view_link_html = BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {

			$registry 	= REGISTRY::get_instance();
			$state 		= $registry->get_request_parm("state");
			$email_event_id = $registry->get_request_parm("email_event_id");
			$keywords 	= $registry->get_request_parm("keywords");
			$page_index	= is_numeric($registry->get_request_parm("page_index")) ? $registry->get_request_parm("page_index") : 0;
			$page_limit	= is_numeric($registry->get_request_parm("page_limit")) ? $registry->get_request_parm("page_limit") : 15;
		
			$end_date 	= BASE_MODEL_CALENDAR::get_seconds($registry->get_request_parm("end_date"));
			$start_date 	= BASE_MODEL_CALENDAR::get_seconds($registry->get_request_parm("start_date"));
			
			$email_event_queue_handler = new BASE_HMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_handler->join("email_event_queues","email_events","email_event_id");
			$email_event_queue_handler->where("email_events.state","=","A");
			$email_event_queue_handler->add_order_by("create_date","DESC");
			
			if($keywords) {
				$email_event_queue_handler->add_filter("subject","LIKE","%".$keywords."%","OR","keyword");
				$email_event_queue_handler->add_filter("body","LIKE","%".$keywords."%","OR","keyword");
				$email_event_queue_handler->add_filter("to_recipients","LIKE","%".$keywords."%","OR","keyword");
				$email_event_queue_handler->add_filter("cc_recipients","LIKE","%".$keywords."%","OR","keyword");
				$email_event_queue_handler->add_filter("bcc_recipients","LIKE","%".$keywords."%","OR","keyword");
			}
			
			if($state)
				$email_event_queue_handler->add_filter("state","=",$state);
			
			if($email_event_id)
				$email_event_queue_handler->add_filter("email_event_id","=",$email_event_id);
				
			if($start_date)
				$email_event_queue_handler->add_filter("schedule_date",">",DB::get_date_time($start_date));
				
			if($end_date)
				$email_event_queue_handler->add_filter("schedule_date","<",DB::get_date_time($end_date + CONSTANT::SECONDS_DAY - 1));				
				
			
			$record_count = $email_event_queue_handler->get_count();
			
			$email_event_queue_handler->set_limit($page_limit,$page_limit * $page_index);

			$this->get_view("paging")->set_page_index($page_index);
			$this->get_view("paging")->set_record_count($record_count);
			$this->get_view("paging")->set_page_limit($page_limit);
			
			$email_event_queue_cmodels = $email_event_queue_handler->get_email_event_queues();
			
			$email_event_hmodel = new BASE_HMODEL_EMAIL_EVENT();
			$email_event_hmodel->where("state","=","A");
			$email_event_hmodel->add_order_by("name");
			$email_event_list = ARRAY_UTIL::get_list_from_objects($email_event_hmodel->get_email_events(),"get_name","get_email_event_id");
			
			$this->set_var("email_event_list",$email_event_list);
			$this->set_var("email_event_queues",$email_event_queue_cmodels);
			$this->set_var("keywords",$keywords);
			$this->set_var("state",$state);
			$this->set_var("end_date",$end_date);
			$this->set_var("email_event_id",$email_event_id);
			$this->set_var("start_date",$start_date);
			$this->set_var("view_link_html",$this->_view_link_html);
			$this->set_var("view_link",$this->_view_link);			
		}
		
		function set_view_link($view_link) 		{ $this->_view_link = $view_link; }
		function set_view_link_html($view_link_html) 	{ $this->_view_link_html = $view_link_html; }
	}