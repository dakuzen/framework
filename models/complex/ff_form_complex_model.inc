<?
	class BCMODEL_FF_FORM extends CMODEL {

		public function __construct() {
			return parent::__construct(DBO_FF_FORM::create())
								->register_json("configs");
		}

		public function set_form_fields($value) { return $this->data("form_fields",$value); }

		public function get_form_fields($hmodel=true) {
			if($hmodel && !$this->has_data("form_fields") && $this->get_article_id()) {
				$hmodel = is_a($hmodel,"HMODEL") ? $hmodel : HMODEL_FF_FORM_FIELD::create(false);
				$this->data("form_fields",$hmodel
										->where("ff_form_fields.form_id","=",$this->get_form_id())
										->gets());
			}
			return $this->data("form_fields");
		}

		public function is_state_active()			{ return $this->get_state()==DBQ_FF_FORM::STATE_ACTIVE; }
		public function is_state_deleted()			{ return $this->get_state()==DBQ_FF_FORM::STATE_DELETED; }
		public function get_state_name()			{ return value(DBQ_FF_FORM::get_state_list(),$this->get_state()); }
		public function get_manage_url()			{ return "/manage/articles/view:form/fid:".$this->get_form_id()."/"; }
		public function get_url()					{ return "/ffforms/ffform/ffid:".$this->get_form_id()."/"; }
		public function delete()					{ return $this->set_state(DBQ_FF_FORM::STATE_DELETED)->save(); }
		public function get_shortcode()				{ return '{$form_'.$this->get_tag()."}"; }
		public function get_confirmation_message()	{ return $this->get_config("confirmation_message"); }
		public function get_config($name)			{ return value($this->get_configs(),$name); }
		public function set_config($name,$value)	{ return $this->set_configs(array_merge($this->get_configs(),array($name=>$value))); }

		public function submit($form) {

			$confirmation_email = "";
			$data = array();
			foreach($this->get_form_fields() as $form_field_cmodel) {

				$value = value($form,$form_field_cmodel->get_guid());

				if($form_field_cmodel->is_interface_email()) {
					$confirmation_email = $value;

				} elseif($form_field_cmodel->interface_has_options()) {

					$options = array();
					foreach((array)$value as $guid)
						$options[] = value($form_field_cmodel->get_field_option_list(),$guid);

					$value = implode(", ",$options);
				}

				$data[]= array($form_field_cmodel->get_name(),$value);
			}

			$html_table = HTML_TABLE_UTIL::create()
								->set_data($data)
								->disable_css();

			$text_table = TEXT_TABLE_UTIL::create()
								->set_data($data)
								->disable_borders()
								->set_column_attributes(0,array("width"=>50))
								->set_column_attributes(1,array("width"=>200));

			$tags = array(CMODEL_MESSAGE::TAG_FORM_CONFIRMATION,CMODEL_MESSAGE::TAG_FORM_NOTIFICATION);

			foreach($tags as $tag) {

				try {

					$message_cmodel = CMODEL_MESSAGE::get($tag);

					if(!$message_cmodel)
						throw new Exception("Invalid message");

					if($message_cmodel->get_email_message()->is_format_html())
						$message_cmodel->set_form($html_table->get());

					elseif($message_cmodel->get_email_message()->is_format_html())
						$message_cmodel->set_form($text_table->get());

					if($tag==CMODEL_MESSAGE::TAG_FORM_CONFIRMATION)
						$message_cmodel->send_email($confirmation_email);

					elseif($tag==CMODEL_MESSAGE::TAG_FORM_NOTIFICATION)
						$message_cmodel->send_email();

				} catch(Exception $e) {}
			}
		}

		public function save() {

			if(is_numeric(self::get_form_id())) {
				if(!$this->dbo("ff_form")->update())
					throw new Exception("Error updating ".get_class());

			} else {
				if(!$this->get_state())
					$this->set_state(DBQ_FF_FORM::STATE_ACTIVE);

				if(!$this->dbo("ff_form")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			return $this;
		}


		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_form_id",
													"get_confirmation_message_id",
													"get_notification_message_id",
													"get_state",
													"get_name",
													"get_tag",
													"get_confirmation_message",
													))->arry();

			return $arry;
		}
	}