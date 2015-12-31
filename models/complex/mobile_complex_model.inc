<?
       require_once(DIR_FRAMEWORK."packages/MobileDetect/Mobile_Detect.php");
			
		class BASE_CMODEL_MOBILE extends Mobile_Detect {
              
                public function is_mobile() { return $this->isMobile(); }
        }


