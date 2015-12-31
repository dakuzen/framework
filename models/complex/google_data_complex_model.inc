<?php   

        /**
        * BASE_CMODEL_GOOGLE_DATA class file
        *
        * @package Framework
        * @subpackage Cmodels
        */

	SERVER_UTIL::add_include_path(DIR_FRAMEWORK."packages/ZendGdata/library/");

	require_once 'Zend/Loader.php';
	Zend_Loader::loadClass('Zend_Gdata_YouTube');
	Zend_Loader::loadClass('Zend_Gdata_AuthSub');
	Zend_Loader::loadClass('Zend_Gdata_App_Exception');      
        
        /**
        * Class BASE_CMODEL_GOOGLE_DATA
        *
        * @package Framework
        * @subpackage Cmodels
        */
	
        class BASE_CMODEL_GOOGLE_DATA extends Zend_Gdata_YouTube {}  