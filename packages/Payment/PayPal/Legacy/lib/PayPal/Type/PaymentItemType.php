<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * PaymentItemType
 * 
 * PaymentItemType Information about a Payment Item.
 *
 * @package PayPal
 */
class PaymentItemType extends XSDSimpleType
{
    /**
     * eBay Auction Transaction ID of the Item
     */
    var $EbayItemTxnId;

    /**
     * Item name set by you or entered by the customer.
     */
    var $Name;

    /**
     * Item number set by you.
     */
    var $Number;

    /**
     * Quantity set by you or entered by the customer.
     */
    var $Quantity;

    /**
     * Amount of tax charged on payment
     */
    var $SalesTax;

    /**
     * Amount of shipping charged on payment
     */
    var $ShippingAmount;

    /**
     * Amount of handling charged on payment
     */
    var $HandlingAmount;

    /**
     * Coupon ID Number
     */
    var $CouponID;

    /**
     * Amount Value of The Coupon
     */
    var $CouponAmount;

    /**
     * Currency of the Coupon Amount
     */
    var $CouponAmountCurrency;

    /**
     * Amount of Discount on this Loyality Card
     */
    var $LoyalityCardDiscountAmount;

    /**
     * Currency of the Discount
     */
    var $LoyalityCardDiscountCurrency;

    /**
     * Cost of item
     */
    var $Amount;

    /**
     * Item options selected in PayPal shopping cart
     */
    var $Options;

    function PaymentItemType()
    {
        parent::XSDSimpleType();
        $this->_namespace = 'urn:ebay:apis:eBLBaseComponents';
        $this->_elements = array_merge($this->_elements,
            array (
              'EbayItemTxnId' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Name' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Number' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Quantity' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'SalesTax' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ShippingAmount' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'HandlingAmount' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'CouponID' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'CouponAmount' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'CouponAmountCurrency' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'LoyalityCardDiscountAmount' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'LoyalityCardDiscountCurrency' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Amount' => 
              array (
                'required' => false,
                'type' => 'BasicAmountType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Options' => 
              array (
                'required' => false,
                'type' => 'OptionType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
            ));
    }

    function getEbayItemTxnId()
    {
        return $this->EbayItemTxnId;
    }
    function setEbayItemTxnId($EbayItemTxnId, $charset = 'iso-8859-1')
    {
        $this->EbayItemTxnId = $EbayItemTxnId;
        $this->_elements['EbayItemTxnId']['charset'] = $charset;
    }
    function getName()
    {
        return $this->Name;
    }
    function setName($Name, $charset = 'iso-8859-1')
    {
        $this->Name = $Name;
        $this->_elements['Name']['charset'] = $charset;
    }
    function getNumber()
    {
        return $this->Number;
    }
    function setNumber($Number, $charset = 'iso-8859-1')
    {
        $this->Number = $Number;
        $this->_elements['Number']['charset'] = $charset;
    }
    function getQuantity()
    {
        return $this->Quantity;
    }
    function setQuantity($Quantity, $charset = 'iso-8859-1')
    {
        $this->Quantity = $Quantity;
        $this->_elements['Quantity']['charset'] = $charset;
    }
    function getSalesTax()
    {
        return $this->SalesTax;
    }
    function setSalesTax($SalesTax, $charset = 'iso-8859-1')
    {
        $this->SalesTax = $SalesTax;
        $this->_elements['SalesTax']['charset'] = $charset;
    }
    function getShippingAmount()
    {
        return $this->ShippingAmount;
    }
    function setShippingAmount($ShippingAmount, $charset = 'iso-8859-1')
    {
        $this->ShippingAmount = $ShippingAmount;
        $this->_elements['ShippingAmount']['charset'] = $charset;
    }
    function getHandlingAmount()
    {
        return $this->HandlingAmount;
    }
    function setHandlingAmount($HandlingAmount, $charset = 'iso-8859-1')
    {
        $this->HandlingAmount = $HandlingAmount;
        $this->_elements['HandlingAmount']['charset'] = $charset;
    }
    function getCouponID()
    {
        return $this->CouponID;
    }
    function setCouponID($CouponID, $charset = 'iso-8859-1')
    {
        $this->CouponID = $CouponID;
        $this->_elements['CouponID']['charset'] = $charset;
    }
    function getCouponAmount()
    {
        return $this->CouponAmount;
    }
    function setCouponAmount($CouponAmount, $charset = 'iso-8859-1')
    {
        $this->CouponAmount = $CouponAmount;
        $this->_elements['CouponAmount']['charset'] = $charset;
    }
    function getCouponAmountCurrency()
    {
        return $this->CouponAmountCurrency;
    }
    function setCouponAmountCurrency($CouponAmountCurrency, $charset = 'iso-8859-1')
    {
        $this->CouponAmountCurrency = $CouponAmountCurrency;
        $this->_elements['CouponAmountCurrency']['charset'] = $charset;
    }
    function getLoyalityCardDiscountAmount()
    {
        return $this->LoyalityCardDiscountAmount;
    }
    function setLoyalityCardDiscountAmount($LoyalityCardDiscountAmount, $charset = 'iso-8859-1')
    {
        $this->LoyalityCardDiscountAmount = $LoyalityCardDiscountAmount;
        $this->_elements['LoyalityCardDiscountAmount']['charset'] = $charset;
    }
    function getLoyalityCardDiscountCurrency()
    {
        return $this->LoyalityCardDiscountCurrency;
    }
    function setLoyalityCardDiscountCurrency($LoyalityCardDiscountCurrency, $charset = 'iso-8859-1')
    {
        $this->LoyalityCardDiscountCurrency = $LoyalityCardDiscountCurrency;
        $this->_elements['LoyalityCardDiscountCurrency']['charset'] = $charset;
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
    function getOptions()
    {
        return $this->Options;
    }
    function setOptions($Options, $charset = 'iso-8859-1')
    {
        $this->Options = $Options;
        $this->_elements['Options']['charset'] = $charset;
    }
}
