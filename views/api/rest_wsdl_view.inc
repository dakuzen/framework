<?
	class BASE_VIEW_API_REST_WSDL extends VIEW {

		protected $_config = array();
	
		public function __construct() {
			$this->disable_authorization();
			$this->type_blank();
		}

		public function init() {

			if($model=$this->get(0)) {
				
				$model_class = "CMODEL_".$model;

				if(class_exists($model_class)) {

					$cmodel = $model_class::create();

					$arry = [	"name"=>$model,
								"plural"=>LANG_UTIL::plural($model)];
					$fields = [];
					foreach($cmodel->get_dbos() as $dbo) {

						foreach($dbo->get_columns() as $name=>$column)
							$fields[$name] = ["name"=>$name,"type"=>$column->get_data_type()];
					}
					
					$arry["fields"] = array_values($fields);
									
					JSON_UTIL::stream($arry,true);
				}
			}

			$this->set_var("config",$this->_config);
		}

		public function config($config) { $this->_config = $config; return $this; }
		
	}