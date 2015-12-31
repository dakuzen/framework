<?php
        /**
        * Class BASE_CMODEL_SALESFORCE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_SALESFORCE extends MODEL_COMPLEX {
		
                /**
                 * webtolead
                 * @param integer $oid
                 * @param array $fields
                 * @return boolean
                 * @throws Exception 
                 */
		public function webtolead($oid,$fields) {

			$fields["oid"] 		= $oid;
			$fields["debug"]	= 1;
			$fields["debugEmail"]	= 1;
			
			foreach($fields as $index=>$value)
				$fields[$index] = urlencode($value);			
			
			$fields_string = ARRAY_UTIL::get_imploded_string($fields,"&","","=");
		
			$url = "https://www.salesforce.com/servlet/servlet.WebToLead?encoding=UTF-8";
			 
		        $ch = curl_init();
			curl_setopt($ch,CURLOPT_URL,$url);
			curl_setopt($ch,CURLOPT_POST,count($fields));
			curl_setopt($ch,CURLOPT_POSTFIELDS,$fields_string);
			curl_setopt($ch,CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_setopt($ch,CURLOPT_RETURNTRANSFER, TRUE);
			curl_setopt($ch,CURLOPT_FOLLOWLOCATION, TRUE);

			$result = curl_exec($ch);
			
			if(preg_match("/Reason:\s(.*)/",$result,$matches)) 			
				throw new Exception(get_value($matches,1));
			
			curl_close($ch);
			
			return $result!==false;
		}
		
                /**
                 * test webtolead
                 * @param integer $oid
                 * @return boolean 
                 */
		public function test_webtolead($oid) {
			
			$fields["first_name"] 	= "John";
			$fields["last_name"] 	= "Smith";
			$fields["email"]	= "jon@gmailer.com";
			$fields["street"] 	= "Street";
			$fields["city"] 	= "City";
			$fields["state"] 	= "State";
			$fields["zip"] 		= "Zip";
			$fields["company"] 	= "Company";
			$fields["phone"] 	= "555-555-555";
			$fields["description"] 	= "Description";
			
			return $this->webtolead($oid,$fields);
		}
	}