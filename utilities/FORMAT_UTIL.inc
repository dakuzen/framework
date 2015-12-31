<?php
        /**
        * Class FORMAT_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class FORMAT_UTIL {

		/**
		 * Returns a full address
		 *
		 * <code>
		 * FORMAT_UTIL::get_full_address("address1","address2","city","state","zip","country");
		 * </code>
		 *
		 * @param string $address1 string address
		 * @param string $address2 string address
		 * @param string $city string city
		 * @param string $state string state
		 * @param string $zip string zipcode
		 * @param string $country string country
		 * @param string $name string name
		 * @param string $delim string delimiter by default "\n"
		 *
		 * @return string
		 */

		function get_full_address($address1,$address2,$city,$state,$zip,$country,$name="",$delim="\n") {
			$address_str = "";

			if(!is_empty($name))
				$address_str .= $name;

			if(!is_empty($address1))
				$address_str .= $delim.$address1;

			if(!is_empty($address2))
				$address_str .= $delim.$address2;

			if(!is_empty($city))
				$address_str .= $delim.$city;

			if(!is_empty($state))
				$address_str .= ", ".$state;

			if(!is_empty($country)) {
				$country = get_value(MODEL_COUNTRY::get_country_list(),$country,$country);
				$address_str .= $delim.$country;
			}

			if(!is_empty($zip))
				$address_str .= $delim.$zip;

			return $address_str;
		}

		/**
		 * Returns a full address
		 *
		 * <code>
		 * FORMAT_UTIL::get_two_line_address("address1","address2","city","state","zip");
		 * </code>
		 *
		 * @param string $address1 string address
		 * @param string $address2 string address
		 * @param string $city string city
		 * @param string $state string state
		 * @param string $zip string zipcode
		 * @param string $country string country
		 * @param string $delim string delimiter by default "\n"
		 *
		 * @return string
		 */

		function get_two_line_address($address1,$address2,$city,$state,$zip,$country="",$delim="\n") {

			$address_str = "";

			$address_str .= $address1;

			if(!is_empty($address2))
				$address_str .= "  ".$address2;

			$address_str .= $delim;


			$address_str .= $city;

			if(!is_empty($state))
				$address_str .= "  ".$state;

			if(!is_empty($zip))
				$address_str .= "  ".$zip;

			if(!is_empty($country)) {
				$country = get_value(BASE_MODEL_COUNTRY::get_countries(),$country,$country);
				$address_str .= $delim.$country;
			}

			return $address_str;
		}


		/**
		 * Returns the long date format
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_long_date();
		 * </code>
		 * <pre>
		 * March 23rd, 2010
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */
		static function get_long_date($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {

			$format = "%B %e%0S, %Y";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%e %B, %Y";

			return FORMAT_UTIL::get_formatted_time($format,$time,$lang,$country);
		}

		static function get_time($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {

			$format = "%0g:%M %0a";

			return self::get_formatted_time($format,$time);
		}

        /**
         * Returns month date
         * @param integer $time
         * @param boolean $abrev
         * @param string $lang
         * @param string $country
         * @return string
         */
		static function get_month_date($time=-1,$abrev=false,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {

			$format = "%B %e%0S";

			if($abrev)
				$format = "%b %e%0S";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = $abrev ? "%e %b" : "%e %B";

			return self::get_formatted_time($format,$time,$lang,$country);
		}

		/**
		 * Returns the long date format with time
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_long_date_time();
		 * </code>
		 * <pre>
		 * March 23rd, 2010 11:12 am
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_long_date_time($time=-1) {

			$format = "%B %e%0S, %Y %0g:%M %0a";

			return self::get_formatted_time($format,$time);
		}


		static function get_long_date_long_time($time=-1) {

			$format = "%B %e%0S, %Y %0g:%M:%S %0a";

			return self::get_formatted_time($format,$time);
		}

		/**
		 * Returns the full date
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_full_date();
		 * </code>
		 * <pre>
		 * Tuesday March 23rd, 2010
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_full_date($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%A %B %e%0S, %Y";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%A %e %B, %Y";

			return self::get_formatted_time($format,$time,$lang,$country);
		}

		/**
		 * Returns the full date with time
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_full_date_time();
		 * </code>
		 * <pre>
		 * Tuesday March 23rd, 2010 11:15 am
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_full_date_time($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%A %B %e%0S, %Y %0g:%M %0a";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%A %e %B, %Y %H:%M:%S";

			return self::get_formatted_time($format,$time,$lang,$country);
		}

		/**
		 * Returns the short date
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_short_date();
		 * </code>
		 * <pre>
		 * 03/23/10
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_short_date($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%m/%d/%y";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%d/%m/%y";

			return self::get_formatted_time($format,$time,$lang,$country);
		}

		/**
		 * Returns the short date with time
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_short_date_time();
		 * </code>
		 * <pre>
		 * 03/23/10 11:25 am
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_short_date_time($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%m/%d/%y %0g:%M %0a";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%d/%m/%y %H:%M:%S";

			return FORMAT_UTIL::get_formatted_time($format,$time,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US);
		}

		/**
		 * Returns the short time
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_short_time();
		 * </code>
		 * <pre>
		 * 11:25 am
		 * </pre>
		 *
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		static function get_short_time($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%0g:%M %0a";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				$format = "%H:%M:%S";

			return self::get_formatted_time($format,$time,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US);
		}

		/**
		 * Returns the short date with time
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_formatted_time("%a");
		 * </code>
		 * <pre>
		 * Tue
		 * </pre>
		 *
		 * @param string $format format style
		 * @param time $time
		 * @param string $lang optional language which defaults to english
		 * @param string $country optional country which defaults to U.S.
		 *
		 * @return string
		 */

		/*
		Day	---	---
		%a	An abbreviated textual representation of the day	Sun through Sat
		%A	A full textual representation of the day	Sunday through Saturday
		%d	Two-digit day of the month (with leading zeros)	01 to 31
		%e	 Day of the month, with a space preceding single digits. Not implemented as described on Windows. See below for more information.	 1 to 31
		%j	Day of the year, 3 digits with leading zeros	001 to 366
		%u	ISO-8601 numeric representation of the day of the week	1 (for Monday) though 7 (for Sunday)
		%w	Numeric representation of the day of the week	0 (for Sunday) through 6 (for Saturday)
		Week	---	---
		%U	Week number of the given year, starting with the first Sunday as the first week	13 (for the 13th full week of the year)
		%V	ISO-8601:1988 week number of the given year, starting with the first week of the year with at least 4 weekdays, with Monday being the start of the week	01 through 53 (where 53 accounts for an overlapping week)
		%W	A numeric representation of the week of the year, starting with the first Monday as the first week	46 (for the 46th week of the year beginning with a Monday)
		Month	---	---
		%b	Abbreviated month name, based on the locale	Jan through Dec
		%B	Full month name, based on the locale	January through December
		%h	Abbreviated month name, based on the locale (an alias of %b)	Jan through Dec
		%m	Two digit representation of the month	01 (for January) through 12 (for December)
		Year	---	---
		%C	Two digit representation of the century (year divided by 100, truncated to an integer)	19 for the 20th Century
		%g	Two digit representation of the year going by ISO-8601:1988 standards (see %V)	Example: 09 for the week of January 6, 2009
		%G	The full four-digit version of %g	Example: 2008 for the week of January 3, 2009
		%y	Two digit representation of the year	Example: 09 for 2009, 79 for 1979
		%Y	Four digit representation for the year	Example: 2038
		Time	---	---
		%H	Two digit representation of the hour in 24-hour format	00 through 23
		%I	Two digit representation of the hour in 12-hour format	01 through 12
		%l (lower-case 'L')	Hour in 12-hour format, with a space preceeding single digits	 1 through 12
		%M	Two digit representation of the minute	00 through 59
		%p	UPPER-CASE 'AM' or 'PM' based on the given time	Example: AM for 00:31, PM for 22:23
		%P	lower-case 'am' or 'pm' based on the given time	Example: am for 00:31, pm for 22:23
		%r	Same as "%I:%M:%S %p"	Example: 09:34:17 PM for 21:34:17
		%R	Same as "%H:%M"	Example: 00:35 for 12:35 AM, 16:44 for 4:44 PM
		%S	Two digit representation of the second	00 through 59
		%T	Same as "%H:%M:%S"	Example: 21:34:17 for 09:34:17 PM
		%X	Preferred time representation based on locale, without the date	Example: 03:59:16 or 15:59:16
		%z	Either the time zone offset from UTC or the abbreviation (depends on operating system)	Example: -0500 or EST for Eastern Time
		%Z	The time zone offset/abbreviation option NOT given by %z (depends on operating system)	Example: -0500 or EST for Eastern Time
		Time and Date Stamps	---	---
		%c	Preferred date and time stamp based on local	Example: Tue Feb 5 00:45:10 2009 for February 5, 2009 at 12:45:10 AM
		%D	Same as "%m/%d/%y"	Example: 02/05/09 for February 5, 2009
		%F	Same as "%Y-%m-%d" (commonly used in database datestamps)	Example: 2009-02-05 for February 5, 2009
		%s	Unix Epoch Time timestamp (same as the time() function)	Example: 305815200 for September 10, 1979 08:40:00 AM
		%x	Preferred date representation based on locale, without the time	Example: 02/05/09 for February 5, 2009
		Miscellaneous	---	---
		%n	A newline character ("\n")	---
		%t	A Tab character ("\t")	---
		%%	A literal percentage character ("%")	---
		*/



		static function get_formatted_time($format,$time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {

			if(!$time)
				return "";

			if($lang==CONSTANT::LANGUAGE_FR && $country==CONSTANT::COUNTRY_CA)
				setlocale(LC_TIME, 'french');
			else
				setlocale(LC_TIME, 'en_EN');

			$time = $time==-1 ? time() : $time;

			$time = !is_numeric($time) ? strtotime($time) : $time;

			$format = str_replace("%O", date("S", $time), $format);

			$mapping["%e"] = date("j", $time);
			$mapping["%0S"] = date("S", $time);
			$mapping["%0a"] = date("a", $time);
			$mapping["%0g"] = date("g", $time);

			$format = str_replace(array_keys($mapping),array_values($mapping),$format);

			return strftime($format,$time);
		}

		/**
		 * Returns a formatted currency
		 *
		 * <code>
		 * $amount = 23.662323;
		 * echo FORMAT_UTIL::get_currency("%a");
		 * </code>
		 * <pre>
		 * 23.66
		 * </pre>
		 *
		 * @param integer $amount amount to be formatted
		 * @param integer $decimal_places number of decimal places
                 * @param boolean $currency_sign
		 * @return integer
		 */
		static function get_currency($amount,$decimal_places=2,$currency_sign=true) {

			$number = number_format(abs($amount),$decimal_places);
			$number = sprintf("%-0".$decimal_places."s",$number);

			return ($amount<0 ? "-" : "").($currency_sign ? "$" : "").$number;
		}

                /**
                 * Returns a number
                 * @param integer $number
                 * @param integer $decimal_places
                 * @return string
                 */
		static function get_number($number,$decimal_places=2) {
			return ($number<0 ? "-" : "").number_format(abs($number),$decimal_places);
		}

                /**
                 * Returns a number abr
                 * @param integer $number
                 * @param integer $decimal_places
                 * @return string
                 */
		static function get_number_abr($number,$decimal_places=1) {

			if($number<1000)
				return $number;

			if($number<1000000)
				return round($number/1000,$decimal_places)."k";

			if($number<1000000000)
				return round($number/1000000,$decimal_places)."m";

			if($number<10000000000)
				return round($number/1000000000)."b";

			return $number;
		}

		static function get_phone($phone) {
			return preg_replace('/(\d{3})[^\d]*(\d{3})[^\d]*(\d{4})$/','($1) $2-$3',$phone);
		}


		/**
		 * Returns a the amount of seconds in the date
		 *
		 * <code>
		 * echo FORMAT_UTIL::get_seconds("2010-04-19");
		 * </code>
		 * <pre>
		 * 1271649600
		 * </pre>
		 *
		 * @param int $date date to be formatted to seconds
		 *
		 * @return int
		 */

		static function get_seconds($date) {
			return @strtotime($date);
		}


		/**
		 * Returns the abbreviation that goes with the given day
		 *
		 * <code>
		 * $abbreviation = FORMAT_UTIL::day_abbreviation(1);
		 * echo "1".$abbreviation;
		 * </code>
		 * <pre>
		 * 1st
		 * </pre>
		 *
		 * @param int $number number of the day
		 *
		 * @return string
		 */

		static function day_abbreviation($number) {
			return self::get_ordinal($number);
		}

        /**
         * Returns an ordinal
         * @param string $cdnl
         * @return string
         * @todo review the types
         */
		function get_ordinal($cdnl){
			$cdnl = intval($cdnl);
			$test_c = abs($cdnl) % 10;
			$ext = ((abs($cdnl) %100 < 21 && abs($cdnl) %100 > 4) ? 'th'
			    : (($test_c < 4) ? ($test_c < 3) ? ($test_c < 2) ? ($test_c < 1)
			    ? 'th' : 'st' : 'nd' : 'rd' : 'th'));
			return $cdnl.$ext;
		}

		/**
		 * Returns a formatte filesize
		 *
		 * @param integer $bytes number of bytes
                 * @param integer $block_size
		 *
		 * @return string
		 */


		function get_formatted_filesize($bytes,$block_size=1024) {
			$bytes = is_empty($bytes) ? 0 : $bytes;

			if ($bytes >= pow(2,40)) {
				$strReturn = round($bytes / pow($block_size,4), 2);
				$strSuffix = "TB";
			} elseif ($bytes >= pow(2,30)) {
				$strReturn = round($bytes / pow($block_size,3), 2);
				$strSuffix = "GB";
			} elseif ($bytes >= pow(2,20)) {
				$strReturn = round($bytes / pow($block_size,2), 2);
				$strSuffix = "MB";
			} elseif ($bytes >= pow(2,10))	{
				$strReturn = round($bytes / pow($block_size,1), 2);
				$strSuffix = "KB";
			} else {
				$strReturn = $bytes;
				$strSuffix = "bytes";
			}

			if ($strReturn == 1)
				$strReturn .= " " . $strSuffix;
			else
				$strReturn .= " " . $strSuffix;
			return $strReturn;

		}

	}