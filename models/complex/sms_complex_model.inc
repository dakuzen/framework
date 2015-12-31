<?
    /**
    * Class BASE_CMODEL_SMS
    *
    * @package Framework
    * @subpackage Cmodels
    */

	abstract class BASE_CMODEL_SMS extends CMODEL {
		
		abstract function send($number,$message);
		
	}