<?php
	/**
        * Smarty Modifier Money Format Function file
        * @package Library
        * @subpackage Smarty
        */

        /**
        * Smarty money_format modifier plugin
        *
        * Type:     modifier<br>
        * Name:     money_format<br>
        * Purpose:  Formats a number as a currency string
        * @link http://www.php.net/money_format
        * @param string format (default %n)
        * @return string
        */
        function smarty_modifier_money_format($number) {
                $number = (float)$number;
                return FORMAT_UTIL::get_currency($number,2,true);
        }
