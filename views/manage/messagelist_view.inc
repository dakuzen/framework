<?

	class BASE_VIEW_MANAGE_MESSAGELIST extends VIEW {
		
		protected $_edit_url = "/manage/message/%s/";
			
		function __construct() {
			$this->set_route("manage","messagelist")->type_blank();
		}

		function init() {
			
			$message_hmodel = HMODEL_MESSAGE::create()
									->order_by("name")
									->set_state(BASE_DBQ_MESSAGE::STATE_ACTIVE);

			if($search=$this->post("search")) 
				$message_hmodel
					->join("messages","sms_messages","message_id","sms_message_id","LEFT")
					->join("messages","email_messages","message_id","email_message_id","LEFT")
					->group(DB_GROUP::create()
								->where("messages.tag","LIKE","%".$search."%","OR")
								->where("messages.name","LIKE","%".$search."%","OR")
								->where("sms_messages.message","LIKE","%".$search."%","OR")
								->where("email_messages.body","LIKE","%".$search."%","OR")
								->where("email_messages.subject","LIKE","%".$search."%","OR"));
			
			$this->set_var("messages",$message_hmodel->gets());				
			$this->set_var("edit_url",$this->_edit_url);		
		}
	}