<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * DiscountType
 * 
 * Describes discount information
 *
 * @package PayPal
 */
class DiscountType extends XSDSimpleType
{
    /**
     * Item name
     */
    var $Name;

    /**
     * description of the discount
     */
    var $Description;

    /**
     * amount discounted
     */
    var $Amount;

    /**
     * offer type
     */
    var $RedeemedOfferType;

    /**
     * offer ID
     */
    var $RedeemedOfferID;

    function DiscountType()
    {
        parent::XSDSimpleType();
        $this->_namespace = 'urn:ebay:apis:eBLBaseComponents';
        $this->_elements = array_merge($this->_elements,
            array (
              'Name' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Description' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Amount' => 
              array (
                'required' => true,
                'type' => 'BasicAmountType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'RedeemedOfferType' => 
              array (
                'required' => false,
                'type' => 'RedeemedOfferType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'RedeemedOfferID' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
            ));
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
    function getDescription()
    {
        return $this->Description;
    }
    function setDescription($Description, $charset = 'iso-8859-1')
    {
        $this->Description = $Description;
        $this->_elements['Description']['charset'] = $charset;
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
    function getRedeemedOfferType()
    {
        return $this->RedeemedOfferType;
    }
    function setRedeemedOfferType($RedeemedOfferType, $charset = 'iso-8859-1')
    {
        $this->RedeemedOfferType = $RedeemedOfferType;
        $this->_elements['RedeemedOfferType']['charset'] = $charset;
    }
    function getRedeemedOfferID()
    {
        return $this->RedeemedOfferID;
    }
    function setRedeemedOfferID($RedeemedOfferID, $charset = 'iso-8859-1')
    {
        $this->RedeemedOfferID = $RedeemedOfferID;
        $this->_elements['RedeemedOfferID']['charset'] = $charset;
    }
}
