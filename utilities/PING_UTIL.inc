<?php
        /**
        * Class PING_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class PING_UTIL {
	
                /**
                 * Gets the ip address from a domain
                 * 
                 * @param string $domain
                 * 
                 * @return integer 
                 */
		function get_ip_from($domain) {

			$cmd = "ping %s ".$domain;

			$options = SERVER_UTIL::is_os_windows() ? "" : "-c 1";

			$cmd = sprintf($cmd,$options);

			SERVER_UTIL::exec($cmd,$ping,true);

			preg_match("/[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/",implode("",$ping),$ip);
			
			return get_value($ip,0);
		}
	}