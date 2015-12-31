<?
	class BASE_VIEW_MANAGE_MESSAGEQUEUELIST extends VIEW {
		
		protected $_view_link 		= "";
		protected $_view_link_html 	= "";
	
		function __construct() {
			$this->disable_authorization();
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
			
			$this->_view_link_html = BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
		
			$registry 		= REGISTRY::get_instance();
			$state 			= $registry->get_request_parm("state");
			$message_id 	= $registry->get_request_parm("message_id");
			$keywords 		= $registry->get_request_parm("keywords");
			$page_index		= is_numeric($registry->get_request_parm("page_index")) ? $registry->get_request_parm("page_index") : 0;
			$page_limit		= is_numeric($registry->get_request_parm("page_limit")) ? $registry->get_request_parm("page_limit") : 15;
		
			$end_date 		= BASE_MODEL_CALENDAR::get_seconds($registry->get_request_parm("search","end_date"));
			$start_date 	= BASE_MODEL_CALENDAR::get_seconds($registry->get_request_parm("search","start_date"));
			
			$message_queue_handler = new BASE_HMODEL_MESSAGE_QUEUE();
			$message_queue_handler->add_order_by("created_date","DESC");
			$message_queue_handler->add_join("message_queues","messages","message_id");
			$message_queue_handler->add_filter("messages.state","=",BASE_DBQ_MESSAGE::STATE_ACTIVE);

			if($keywords) {
				$message_queue_handler->add_join("message_queues","email_message_queues","message_queue_id","email_message_queue_id","LEFT");
				$message_queue_handler->add_join("message_queues","sms_message_queues","message_queue_id","sms_message_queue_id","LEFT");

				$message_queue_handler->add_filter("subject","LIKE","%".$keywords."%","OR","keyword");
				$message_queue_handler->add_filter("email_message_queues.body","LIKE","%".$keywords."%","OR","keyword");
				$message_queue_handler->add_filter("sms_message_queues.body","LIKE","%".$keywords."%","OR","keyword");
				$message_queue_handler->add_filter("to_recipients","LIKE","%".$keywords."%","OR","keyword");
				$message_queue_handler->add_filter("cc_recipients","LIKE","%".$keywords."%","OR","keyword");
				$message_queue_handler->add_filter("bcc_recipients","LIKE","%".$keywords."%","OR","keyword");
			}
			
			if($state)
				$message_queue_handler->add_filter("message_queues.state","=",$state);
			
			if($message_id)
				$message_queue_handler->add_filter("message_queues.message_id","=",$message_id);
				
			if($start_date)
				$message_queue_handler->add_filter("message_queues.scheduled_date",">",DB::get_date_time($start_date));
				
			if($end_date)
				$message_queue_handler->add_filter("message_queues.scheduled_date","<",DB::get_date_time($end_date + CONSTANT::SECONDS_DAY - 1));				
				
			
			$record_count = $message_queue_handler->get_count();
			
			$message_queue_handler->set_limit($page_limit,$page_limit * $page_index);

			$this->get_view("paging")->set_page_index($page_index);
			$this->get_view("paging")->set_record_count($record_count);
			$this->get_view("paging")->set_page_limit($page_limit);
			
			$message_queue_cmodels = $message_queue_handler->get_message_queues();
			
			$this->set_var("message_queues",$message_queue_cmodels);
			$this->set_var("keywords",$keywords);
			$this->set_var("state",$state);
			$this->set_var("end_date",$end_date);
			$this->set_var("message_id",$message_id);
			$this->set_var("start_date",$start_date);
			$this->set_var("view_link_html",$this->_view_link_html);
			$this->set_var("view_link",$this->_view_link);			
		}
		
		function set_view_link($view_link) 				{ $this->_view_link = $view_link; }
		function set_view_link_html($view_link_html) 	{ $this->_view_link_html = $view_link_html; }
	}