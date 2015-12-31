<?
    /**
    * Class BASE_CMODEL_LOGGER_DB
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_LOGGER_DB extends BASE_CMODEL_LOGGER {

        /**
         * Create
         * @param string $level
         * @return object 
         */
		public static function create() {
			
			$log_cmodel = new BASE_CMODEL_LOGGER_DB();
			
			self::$_instance = $log_cmodel;
			
			return self::$_instance;
		}

        /**
         * Log
         * @param object $data
         * @param string $level
         * @param string $code
         * @return boolean 
         */
		public function log($message,$type,$data=array()) {

            $ff_log_cmodel = BCMODEL_FF_LOG::create()
                                ->set_type($type)
                                ->set_message($message)
                                ->set_line(value($data,"line"))
                                ->set_file(value($data,"file"))
                                ->set_notified(0)
                                ->set_create_date(DB::get_date_time())
                                ->set_backtrace(JSON_UTIL::encode(value($data,"backtrace")))
                                ->set_server(JSON_UTIL::encode($_SERVER))
                                ->set_post(JSON_UTIL::encode($_POST))
                                ->set_get(JSON_UTIL::encode($_GET))
                                ->set_url(SERVER_UTIL::get_request_url())
                                ->save();

            if($this->_notify) {

                $minute = CMODEL_TIME::create(time())
                                    ->subtract("minute")
                                    ->db();

                $count = DBQ::create("ff_logs")
                                ->where("notified","=",1)
                                ->where("notify_date",">",$minute)
                                ->count();

                if($count<3) {

                    $data["message"]    = $message;
                    $data["type"]       = $type;

                    DEBUG_UTIL::notify_error($data);
                    $ff_log_cmodel->notify();

                    $max_ff_log_id = DBQ::create("ff_logs")
                                        ->where("notified","=",0)
                                        ->one("MAX(ff_log_id)");

                    if($max_ff_log_id) {

                        try {

                            $logs = DBQ::create("ff_logs")
                                        ->where("notified","=",0)
                                        ->where("ff_log_id","<=",$max_ff_log_id)
                                        ->group_by("message")
                                        ->select(array("message","COUNT(ff_log_id) AS count"));
                            if($logs) {

                                $total = 0;
                                $message = "";
                                foreach($logs as $log) {
                                    $total += (int)value($log,"count");
                                    $message .= value($log,"message")." (".value($log,"count").")\n";
                                }

                                $system_name = SYSTEM_MANAGER::get_system_name() ? SYSTEM_MANAGER::get_system_name() : SYSTEM_MANAGER::get_system_host();
                                $subject = $total." Error Notifications for ".$system_name;
                                
                                $message = "The ".$system_name." instance has produced the following errors:\n\n".$message;

                                SYSTEM_MANAGER::get_notify_email($subject,$message)->send();
                            
                                DBQ::create("ff_logs")
                                    ->where("notified","=",0)
                                    ->where("ff_log_id","<=",$max_ff_log_id)
                                    ->update(array("notify_date"=>DB::get_date_time(),"notified"=>true));
                            }

                        } catch(Exception $e) {}
                    }                    
                }

            }
		}		
	}
	