<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * OfferCouponInfoType
 * 
 * OffersAndCouponsInfoType Information about a Offers and Coupons.
 *
 * @package PayPal
 */
class OfferCouponInfoType extends XSDSimpleType
{
    /**
     * Type of the incentive
     */
    var $Type;

    /**
     * ID of the Incentive used in transaction
     */
    var $ID;

    /**
     * Amount used on transaction
     */
    var $Amount;

    /**
     * Amount Currency
     */
    var $AmountCurrency;

    function OfferCouponInfoType()
    {
        parent::XSDSimpleType();
        $this->_namespace = 'urn:ebay:apis:eBLBaseComponents';
        $this->_elements = array_merge($this->_elements,
            array (
              'Type' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ID' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Amount' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'AmountCurrency' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
            ));
    }

    function getType()
    {
        return $this->Type;
    }
    function setType($Type, $charset = 'iso-8859-1')
    {
        $this->Type = $Type;
        $this->_elements['Type']['charset'] = $charset;
    }
    function getID()
    {
        return $this->ID;
    }
    function setID($ID, $charset = 'iso-8859-1')
    {
        $this->ID = $ID;
        $this->_elements['ID']['charset'] = $charset;
    }
    function getAmount()
    {
        return $this->Amount;
    }
    function setAmount($Amount, $charset = 'iso-8859-1')
    {
        $this->Amount = $Amount;
        $this->_elements['Amount']['charset'] = $charset;
    }
    function getAmountCurrency()
    {
        return $this->AmountCurrency;
    }
    function setAmountCurrency($AmountCurrency, $charset = 'iso-8859-1')
    {
        $this->AmountCurrency = $AmountCurrency;
        $this->_elements['AmountCurrency']['charset'] = $charset;
    }
}
