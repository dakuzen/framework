<?php

	class BASE_VIEW_MANAGE_EMAILLOG extends VIEW {
		
		private $_email_event_queue_cmodel 	= null;
		private $_email_event_log_cmodels 	= array();
		private $_resend_link			= "";
		private $_edit_link			= "";
		private $_delete_link			= "";
		private $_cancel_link			= "";
		private $_edit_link_html		= "";
		private $_resend_link_html		= "";
		private $_cancel_link_html 		= "";
		
		function __construct() {
			$this->set_title("Email Log");
			
			$this->_edit_link_html 		= BASE_MODEL_IMAGE_ICON::get_edit("Edit Email Event");
			$this->_resend_link_html 	= BASE_MODEL_IMAGE_ICON::get_generate("Re-Send Email");
			$this->_cancel_link_html 	= BASE_MODEL_IMAGE_ICON::get_delete("Cancel Email");
			
			$this->_email_event_queue_cmodel = new BASE_CMODEL_EMAIL_EVENT_QUEUE();
		}

		function init() {
			$registry = REGISTRY::get_instance();
			
			if(is_numeric($email_event_queue_id=$registry->get_request_parm("0")))
				$this->load_view($email_event_queue_id);
				
			if($action=$registry->get_request_parm("action")) {
				
				if($action=="resend") {
					
					$has_success = $this->_email_event_queue_cmodel->send(true);
					
					if($has_success)
						APPLICATION::add_notify_message("Successfully resent the email");
					else
						APPLICATION::add_error_message("There was a problem trying to resend the email");
					
				} elseif($action=="cancel") {
					
					$has_success = $this->_email_event_queue_cmodel->cancel();
					
					if($has_success)
						APPLICATION::add_notify_message("Successfully canceled the email");
					else
						APPLICATION::add_error_message("There was a problem trying to cancel the email");
					
				}
			}
			
			$from = "";
			if($this->_email_event_queue_cmodel->get_from_name())
				$from = $this->_email_event_queue_cmodel->get_from_name();
			
			if($this->_email_event_queue_cmodel->get_from_email())
				$from .= $this->_email_event_queue_cmodel->get_from_name() ? " &lt;".$this->_email_event_queue_cmodel->get_from_email()."&gt;" : $this->_email_event_queue_cmodel->get_from_email();
			
			$this->set_var("from",$from);
			$this->set_var("email_event_queue",$this->_email_event_queue_cmodel);
			$this->set_var("email_event_logs",$this->_email_event_log_cmodels);
			$this->set_var("resend_link",$this->_resend_link);
			$this->set_var("edit_link",$this->_edit_link);
			$this->set_var("cancel_link",$this->_cancel_link);
			$this->set_var("edit_link_html",$this->_edit_link_html);
			$this->set_var("resend_link_html",$this->_resend_link_html);
			$this->set_var("cancel_link_html",$this->_cancel_link_html);
		}
	
		
		function load_view($email_event_queue_id) {
			
			$email_event_queue_hmodel = new BASE_HMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_hmodel->set_email_event_queue_id($email_event_queue_id);
			$email_event_queue_hmodel->load_email_event_queue_attachments();
			$email_event_queue_cmodel = get_value($email_event_queue_hmodel->get_email_event_queues(),0);
			
			if($email_event_queue_cmodel) {
				$this->_email_event_queue_cmodel = $email_event_queue_cmodel;				

				$email_event_log_handler = new BASE_HMODEL_EMAIL_EVENT_LOG();
				$email_event_log_handler->set_email_event_queue_id($email_event_queue_id);
				
				$this->_email_event_log_cmodels = $email_event_log_handler->get_email_event_logs();				
			}	
		}		
		
		public function set_email_event_queue($email_event_queue_cmodel) 	{ $this->_email_event_queue_cmodel = $email_event_queue_cmodel; }
		public function set_resend_link($resend_link) 				{ $this->_resend_link = $resend_link; }
		public function set_cancel_link($cancel_link)				{ $this->_cancel_link = $cancel_link; }
		public function set_edit_link($edit_link) 				{ $this->_edit_link = $edit_link; }
		public function set_resend_link_html($resend_link_html) 		{ $this->_resend_link_html = $resend_link_html; }
		public function set_cancel_link_html($cancel_link_html)			{ $this->_cancel_link_html = $cancel_link_html; }
		public function set_edit_link_html($edit_link_html) 			{ $this->_edit_link_html = $edit_link_html; }		
		
		
		public function set_action_url($action_url) {
			$this->set_cancel_link($action_url."action:cancel/");
			$this->set_resend_link($action_url."action:resend/");
		}
	}				
	