<?
	/*
		CMODEL_IP::create(2130706433)->ip();
	*/

	class BCMODEL_IP extends CMODEL {
		
		protected $_ip = null;

		public function __construct($ip) {
			$this->ip($ip);
		}

		/*
		private function inet_to_bits($inet) {
		   $inet = inet_pton($inet);
		   $unpacked = unpack('A16', $inet);
		   $unpacked = str_split($unpacked[1]);
		   $binaryip = '';
		   foreach ($unpacked as $char)
		   	 $binaryip .= str_pad(decbin(ord($char)), 8, '0', STR_PAD_LEFT);

		   return $binaryip;
		}

		private function bits_to_inet($bits) {
		    $inet = "";
		    for($pos=0; $pos<128; $pos+=8) {
		        $inet .= chr(bindec(substr($bits, $pos, 8)));
		    }
		    return inet_ntop($inet);
		}
		*/

		public function add($networkmask) {

			// First find the length of the block: IPv6 uses 128 bits for the mask
			$networksize = pow(2, 128 - $networkmask);

			// Reduce network size by one as we really need last IP address in the range,
			// not first one of subsequent range
			$this->_ip = $this->_ip + $networksize - 1;

			return $this;
		}
		
		public function binary() { return CONVERT_UTIL::get_binary_ip_address($this->ip()); }

		public function ip($ip=null) { 
			if($ip) {

				if(!is_numeric($ip)) {
					$ip = preg_replace("/^::ffff:/","",$ip);
					$ip = ip2long($ip);
				}

				$this->_ip = $ip;
				return $this;
			}

			return long2ip($this->_ip); 
		}

	}