<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/AbstractResponseType.php';

/**
 * BMSetInventoryResponseType
 *
 * @package PayPal
 */
class BMSetInventoryResponseType extends AbstractResponseType
{
    function BMSetInventoryResponseType()
    {
        parent::AbstractResponseType();
        $this->_namespace = 'urn:ebay:api:PayPalAPI';
    }

}
