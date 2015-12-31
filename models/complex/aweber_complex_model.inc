<?php
       /**
        * BASE_CMODEL_AWEBER class file
        *
        * @package Framework
        * @subpackage Cmodels
        */

        require_once(MODEL_PATH::get_framework_packages_directory()."aweber/aweber_api/aweber.php");
        
       /**
        * Class BASE_CMODEL_SMARTY
        *
        * @package Framework
        * @subpackage Cmodels
        */

        class BASE_CMODEL_AWEBER extends AWeberAPI {
                
              
                /**
                 * the step
                 * @var integer $step 
                 */
                public $_step                   = 0;
                                
                /**
                 * Constructor
                 * @param string $compile_dir
                 * @param string $cache_dir 
                 */
                //public function __construct($compile_dir="",$cache_dir="") {
                //        parent::__construct();
                //}
        }
