<?php
        /**
        * Class ENCRYPT_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class ENCRYPT_UTIL {
		
		const CRYPT_KEY = "j4nds032";
		
		/**
		 * Encrypts a string url
		 *
		 * <code>
		 * $sentence = "what are you doing now";
		 * $mystring = ENCRYPT_UTIL::encrypt_string_url($sentence);
		 * echo $mystring;
		 * </code>
		 *
		 * <pre>
		 * gBCh6jmCWPN/x0AINgK2Fo2giv6vouSr/Z6iiHWkVEKGO/UdgTtkhEJE42DoUepB
		 * </pre>
		 *
		 *
		 * @param string $plain_text string to be encrypted
		 * @return string
		 */
		
		function encrypt_string_url($plain_text) {
			return urlencode(ENCRYPT_UTIL::encrypt_string($plain_text));
		}

		/**
		 * Encrypts a string url
		 *
		 * <code>
		 * $sentence = "what are you doing now";
		 * $mystring = ENCRYPT_UTIL::encrypt_url($sentence);
		 * echo $mystring;
		 * </code>
		 *
		 * <pre>
		 * gBCh6jmCWPN/x0AINgK2Fo2giv6vouSr/Z6iiHWkVEKGO/UdgTtkhEJE42DoUepB
		 * </pre>
		 *
		 *
		 * @param string $plain_text string to be encrypted
		 * @return string
		 */		

		function encrypt_url($plain_text) { return self::encrypt_string_url($plain_text); }
		
		/**
		 * Encrypts a string
		 *
		 * <code>
		 * $sentence = "what are you doing now";
		 * $mystring = ENCRYPT_UTIL::encrypt($sentence);
		 * echo $mystring;
		 * </code>
		 *
		 * <pre>
		 * gBCh6jmCWPN/x0AINgK2Fo2giv6vouSr/Z6iiHWkVEKGO/UdgTtkhEJE42DoUepB
		 * </pre>
		 *
		 * @param string $plain_text string to be encrypted
                 * @param integer $iv_len
                 * @param string $crypt_key
		 * @return string
		 */
		function encrypt($plain_text, $iv_len=16, $crypt_key=null) { return self::encrypt_string($plain_text, $iv_len, $crypt_key); }
		
		/**
		 * Decrypts a string
		 *
                 * @param string $plain_text string to be decrypted
                 * @param integer $iv_len
                 * @param string $crypt_key
		 * @return string
		 */
		function decrypt($plain_text, $iv_len=16, $crypt_key=null) { return self::decrypt_string($plain_text, $iv_len, $crypt_key); }
		
		/**
		 * Encrypts a string
		 *
		 * @param string $plain_text string to be decrypted
                 * @param integer $iv_len
                 * @param string $crypt_key
		 * @return string
		 */
		function encrypt_string($plain_text, $iv_len=16, $crypt_key=null) {

			$crypt_key = $crypt_key ? $crypt_key : ENCRYPT_UTIL::CRYPT_KEY;
			
			$plain_text .= "\x13";
			$n = strlen($plain_text);
			
			if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
			
			$i = 0;
			$enc_text = ENCRYPT_UTIL::get_rnd_iv($iv_len);
			$iv = substr($crypt_key ^ $enc_text, 0, 512);
			
			while ($i < $n) {
				$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
				$enc_text .= $block;
				$iv = substr($block . $iv, 0, 512) ^ $crypt_key;
				$i += 16;
			}
			return base64_encode($enc_text);
		}
		
		/**
		 * Decrypts a string
		 *
		 * @param string $enc_text
                 * @param integer $iv_len
                 * @param string $crypt_key
		 * @return string
		 */
		function decrypt_string($enc_text, $iv_len = 16, $crypt_key=null) {
		
			$crypt_key = $crypt_key ? $crypt_key : ENCRYPT_UTIL::CRYPT_KEY;

			$enc_text = base64_decode($enc_text);
			$n = strlen($enc_text);
			$i = $iv_len;
			$plain_text = '';
			$iv = substr($crypt_key ^ substr($enc_text, 0, $iv_len), 0, 512);

			while ($i < $n) {
				$block = substr($enc_text, $i, 16);
				$plain_text .= $block ^ pack('H*', md5($iv));
				$iv = substr($block . $iv, 0, 512) ^ $crypt_key;
				$i += 16;
			}

			return preg_replace('/\\x13\\x00*$/', '', $plain_text);
		}

		/**
		 * Used internally for encrypting 
		 *
		 * @param integet $iv_len 
		 * @return string
		 */
		function get_rnd_iv($iv_len){
			$iv = '';
			while ($iv_len-- > 0) 
				$iv .= chr(mt_rand() & 0xff);
			return $iv;
		}
		
                /**
                 * Aes decrypt
                 * @todo review the description
                 * 
                 * @param integer $val
                 * @param string $ky
                 * @return string 
                 */
		function aes_decrypt($val,$ky) {
		
			if(!$val)
				return $val;
			
			$key = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; 
			for($a=0;$a<strlen($ky);$a++) 
				$key[$a%16]=chr(ord($key[$a%16]) ^ ord($ky[$a])); 
			
			$mode 	= MCRYPT_MODE_ECB; 
			$enc 	= MCRYPT_RIJNDAEL_128; 
			$dec 	= @mcrypt_decrypt($enc, $key, $val, $mode, @mcrypt_create_iv( @mcrypt_get_iv_size($enc, $mode), MCRYPT_DEV_URANDOM ) ); 
			
			return rtrim($dec,(( ord(substr($dec,strlen($dec)-1,1))>=0 and ord(substr($dec, strlen($dec)-1,1))<=16)? chr(ord( substr($dec,strlen($dec)-1,1))):null)); 
		} 

                /**
                 * Aes encrypt
                 * @param integer $val
                 * @param string $ky
                 * @return string 
                 */
		function aes_encrypt($val,$ky)  { 
			
			if(!$val)
				return true;
				
			$key = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; 

			for($a=0;$a<strlen($ky);$a++) 
				$key[$a%16] = chr(ord($key[$a%16]) ^ ord($ky[$a])); 

			$mode 	= MCRYPT_MODE_ECB; 
			$enc 	= MCRYPT_RIJNDAEL_128; 
			$val 	= str_pad($val, (16*(floor(strlen($val) / 16)+(strlen($val) % 16==0?2:1))), chr(16-(strlen($val) % 16))); 
			
			return mcrypt_encrypt($enc, $key, $val, $mode, mcrypt_create_iv( mcrypt_get_iv_size($enc, $mode), MCRYPT_DEV_URANDOM)); 
		} 


    
	}
