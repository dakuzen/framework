<?
	/**
    * Class HTTP_ADAPTER  
    *
    * @package Library
    * @subpackage Transport
    */
	abstract class HTTP_ADAPTER {

        /**
         * send Request
         */
		abstract public function send($http_request);
        
        /**
         * write Request
         * @param string $file 
         */
		abstract public function write($http_request, $file);

        /**
         * parse Response Code
         * @param string $line
         * @return array
         * @throws HTTP_EXCEPTION 
         */
		protected function parseResponseCode($line) {
			if (sscanf($line, "HTTP/%s %s", $http_version, $returncode) != 2) {
				throw new HTTP_EXCEPTION("Malformed response");
			} else {
				return array("code" => intval($returncode), "httpVersion" => $http_version);
			}
		}
	}