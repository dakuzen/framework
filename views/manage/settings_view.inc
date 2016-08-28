<?
	class BASE_VIEW_MANAGE_SETTINGS extends VIEW {

		protected $_setting_cmodels 			= array();
		protected $_group_list					= array();
		protected $_cancel_link					= "";
		protected $_cancel_link_html			= "Cancel";
		protected $_setting_initialize_cmodel	= null;

		function init() {

			if(SERVER_UTIL::is_post()) {

				$action = $this->post("action");

				$response_cmodel = new CMODEL_RESPONSE();

				try {

					if($action=="save") {

						$settings = $this->post("settings");

						$setting_cmodels = HMODEL_SETTING::create()->gets();

						DB::start_transaction();

						foreach($setting_cmodels as $setting_cmodel) {

							if($setting_cmodel->is_interface_static())
								continue;

							$value = value($settings,$setting_cmodel->get_name());

							$setting_cmodel->set_value($value);

							if($setting_cmodel->is_data_type_file()) {

								$upload = new UPLOAD_UTIL($setting_cmodel->get_name());
								$upload->disable_check_extension();

								if($upload->is_file_upload_exist()) {

									$temp_dir = MODEL_PATH::get_temporary_directory();
									FILE_UTIL::mkdir($temp_dir);

									$upload->set_store_path($temp_dir);
									$has_success = $upload->upload();

									if($has_success) {
										$has_success = $setting_cmodel->commit_file($upload->get_store_file());

										if(!$has_success)
											throw new Exception("Failed to save the ".$setting_cmodel->get_label()." name");
									} else
										throw new Exception($upload->get_error_message());

									FILE_UTIL::rmdir($temp_dir);
								}

							} else
								$setting_cmodel->save();
						}

						if(!DB::complete_transaction())
							throw new Exception("There was a problem trying to save the settings");

					} elseif($action=="download") {

						$setting_cmodel = BASE_HMODEL_SETTING::get_setting($this->post("name"));

						if($setting_cmodel) {
							if(is_file($file=$setting_cmodel->get_value()))
								HTTP_UTIL::stream_file($file,basename($file));
						}
					} else
						throw new Exception("Invalid action");


				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel
					->success()
					->render(true);
			}

			$this->_setting_initialize_cmodel = new CMODEL_SETTING_INITIALIZE();
			$group_list = $this->_setting_initialize_cmodel->get_group_list();

			if(!$this->_setting_cmodels) {

				$this->_setting_initialize_cmodel->init();

				$this->_setting_cmodels = $this->_setting_initialize_cmodel->get_settings();

				$setting_dbq = new BASE_DBQ_SETTING();
				$settings = $setting_dbq->select(array("value"),"name");

				foreach($this->_setting_cmodels as $setting_cmodel)
					$setting_cmodel->populate(get_value($settings,$setting_cmodel->get_name()));
			}

			$grouped_settings = array();
			foreach($this->_setting_cmodels as $setting_cmodel)
				$grouped_settings[$setting_cmodel->get_property("group")][$setting_cmodel->get_label()] = $setting_cmodel;

			ksort($grouped_settings);

			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("grouped_settings",$grouped_settings);
			$this->set_var("group_descriptions",$this->_setting_initialize_cmodel->get_group_descriptions());
			$this->set_var("group_list",$group_list);
			$this->set_var("cancel_link",$this->_cancel_link);
			$this->set_var("cancel_link_html",$this->_cancel_link_html);
			$this->set_var("view_url",APPLICATION::get_instance()->get_current_view()->get_url());
		}

		function set_settings($setting_cmodels)		{ $this->_setting_cmodels = $setting_cmodels; }
		function set_cancel_link($cancel_link)		{ $this->_cancel_link = $cancel_link; }
		function set_cancel_link_html($cancel_link_html){ $this->_cancel_link_html = $cancel_link_html; }

	}