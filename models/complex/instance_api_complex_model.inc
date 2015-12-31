<?
   /**
    * Class BASE_CMODEL_INSTANCE_API
    *
    * @package Framework
    * @subpackage Cmodels
    */

    class BASE_CMODEL_INSTANCE_API extends BASE_CMODEL_API {

        /**
         * Listen 
         */
        public function listen() { 
                
            $method = $this->parm(array("args","m"));

            $this->process($method);
        }

        /**
         * Process
         * @param string $method 
         */
        public function process($method) {
            
            try {

                if(!$this->is_method_exist($method))
                    throw new Exception("The method does not exist",404);
                
                call_user_func(array($this,preg_replace("/[\.]/","_",$method)));
               
            } catch(Exception $e) {
                 $this->get_api_response()->error($e->getMessage(),$e->getCode());
            }

            $this->get_api_response()->render();
        }

        public function is_method_exist($method) {
            return method_exists($this,preg_replace("/[\.]/","_",$method));   
        }

        /**
         * Api instance complete
         */
        public function instance_complete() {

            $upgrade_cmodel = new BASE_CMODEL_UPGRADE();
            $has_success = $upgrade_cmodel->mark_all_completed();

            $this->get_api_response()->success($has_success);
        }

        /**
         * Api instance init
         */
        public function instance_init() {

            $init_cmodel = new CMODEL_INIT();
            $has_success = $init_cmodel->init();    

            $this->get_api_response()->success($has_success);
        }

        public function method_exists() {

            $method = $this->parm(array("args","method"));

            if(!$this->is_method_exist($method))
                throw new Exception("The method does not exist",404);

            $this->get_api_response()->success();
        }

        public function instance_backup() {

            $db_pass = $this->parm(array("args","pass"));
            $db_user = $this->parm(array("args","user"));

            $instance_cmodel = CMODEL_SYSTEM_INSTANCE::create(MODEL_PATH::get_instance_dir());

            $options = array("port"=>$instance_cmodel->get_db_port(),"sock"=>$instance_cmodel->get_db_sock());
            $database_archive_cmodel = CMODEL_DAILY_DATABASE_ARCHIVE::create($instance_cmodel->get_db_host(),$instance_cmodel->get_db_name(),$db_user,$db_pass,$instance_cmodel->get_backups_directory(),$options);

            $ff_backup_cmodel = CMODEL_FF_BACKUP::create()
                                        ->set_start_time(microtime(true))
                                        ->set_filename($database_archive_cmodel->get_archive_filename());
            try {
                
                $database_archive_cmodel->archive();
                 
                CMODEL_SETTING::set_instance_db_backup(time());
                
                $ff_backup_cmodel
                    ->set_filesize($database_archive_cmodel->get_archive_filesize())
                    ->set_end_time(time())
                    ->set_state(BASE_DBQ_FF_BACKUP::STATE_SUCCESS)
                    ->save(); 

                $this->get_api_response()->success();

            } catch(Exception $e) {

                $ff_backup_cmodel
                    ->set_end_time(time())
                    ->set_message($e->getMessage())
                    ->set_state(BASE_DBQ_FF_BACKUP::STATE_FAILED)
                    ->save();

                throw $e;
            }        
        }                     

        /**
         * Api instance upgrade
         */
        public function instance_upgrade() {
                           
            $db_user = $this->parm(array("args","user"));
            $db_pass = $this->parm(array("args","pass"));
            $version = $this->parm(array("args","version"));

            if($db_user && $db_pass) {

                $ini_util = new INI_UTIL(SYSTEM_MANAGER::get_config_file());
                
                $db_host = $ini_util->get("DB_HOST");
                $db_name = $ini_util->get("DB_NAME");
                $db_type = $ini_util->get("DB_TYPE");
                $db_port = $ini_util->get("DB_PORT");
                $db_sock = $ini_util->get("DB_SOCK");
                 
                try {
                    if($db_user || $db_pass) {
                            $db = DB::get_db_connection($db_host,$db_user,$db_pass,$db_name,$db_type,array("port"=>$db_port,"sock"=>$db_sock));
                            DB::set_default_db_connection($db);
                    } else
                            $db = DB::get_instance();
                    
                    $application_version = null;

                    if($version && BASE_CMODEL_UPGRADE::is_valid_version($version))
                            $application_version = $version;
                    
                    if(!$application_version) {
                            $upgrade_cmodel = new CMODEL_UPGRADE();
                            
                            if(BASE_CMODEL_UPGRADE::is_valid_version($upgrade_cmodel->get_current_version()))
                                    $application_version = $upgrade_cmodel->get_current_version();        
                    }
                    
                    if($application_version) {
                            
                            $upgrade_cmodel = new CMODEL_UPGRADE();
                            $upgrade_cmodel->set_callback_handler(array($this,"api_instance_upgrade_handler"));
                            $has_success = $upgrade_cmodel->process($application_version);

                            $this->get_api_response()->success($has_success);

                            if($has_success) {
                                    $init_cmodel = new CMODEL_INIT();
                                    $init_cmodel->init();           
                            } else
                                    $this->get_api_response()->set_error(implode(", ",$upgrade_cmodel->get_error_messages()));
                            
                    } else
                            $this->get_api_response()->set_error("Invalid version `".$version."`");                                 
                    
                } catch(Exception $e) {
                        $this->get_api_response()->set_error($e->getMessage());
                }
            } else {

                if(!$db_user)
                        $this->get_api_response()->set_error("No database user name supplied. Use switch -user");
                        
                if(!$db_pass)
                        $this->get_api_response()->set_error("No database user password supplied. Use switch -pass");                        
            }
        }

        function api_instance_upgrade_handler($message,$has_success) {
            if(!$has_success)
                throw new Exception($message);
        }
    }