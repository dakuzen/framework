<?php
	/**
        * Class HTTP_URI  
        *
        * @package Library
        * @subpackage Transport
        */

	class HTTP_URI {
                
                /**
                 * The pieces
                 * @var array $pieces
                 */
		protected $pieces = array();
		
                /**
                 * The map
                 * @var array $map 
                 */
		protected $map = array(
				'url' => 'uri',
				'querystring' => 'query',
				'anchor' => 'fragment',
				'protocol' => 'scheme');
		
                /**
                 * the schemes
                 * @var array $schemes
                 */
		protected $schemes = array(
				'http' => 80,
				'https'=> 443,
				'ssl'=> 443
			);

                /**
                 * Constructor
                 * @param string $uri 
                 */
		public function __construct($uri) {
			
			$this->pieces = @parse_url($uri);
			$this->pieces['uri'] = $uri;
			
			if (!isset($this->pieces['port'])) {
				
				$scheme = get_value($this->pieces,'scheme');				
				
				if (isset($this->schemes[$scheme])) {
					$this->pieces['port'] = $this->schemes[$scheme];
				}
				else {
					$this->pieces['port'] = 80;
				}
			}
		}
		
                /**
                 * Returns hosts
                 * @return string 
                 */
		public function get_host() { return get_value($this->pieces,"host"); }
		
                /**
                 * Returns key
                 * @param string $key
                 * @return null 
                 */
		public function __get($key) {
			if (isset($this->map[$key])) {
				$key = $this->map[$key];
			}
			if (isset($this->pieces[$key])) {
				return $this->pieces[$key];
			}
			return null;
		}

                /**
                 * To array
                 * @return array 
                 */
		public function toArray() {
			return $this->pieces;
		}
	}
