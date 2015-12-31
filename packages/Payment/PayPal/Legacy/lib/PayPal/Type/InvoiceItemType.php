<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * InvoiceItemType
 * 
 * Describes an individual item for an invoice.
 *
 * @package PayPal
 */
class InvoiceItemType extends XSDSimpleType
{
    /**
     * a human readable item name
     */
    var $Name;

    /**
     * a human readable item description
     */
    var $Description;

    /**
     * The International Article Number or Universal Product Code (UPC) for the item.
     * Empty string is allowed. Character length and limits: 17 single-byte characters
     */
    var $EAN;

    /**
     * The Stock-Keeping Unit or other identification code assigned to the item.
     * Character length and limits: 64 single-byte characters
     */
    var $SKU;

    /**
     * A retailer could apply different return policies on different items. Each return
     * policy would be identified using a label or identifier. This return policy
     * identifier should be set here. This identifier will be displayed next to the
     * item in the e-Receipt. Character length and limits: 8 single-byte characters
     */
    var $ReturnPolicyIdentifier;

    /**
     * total price of this item
     */
    var $Price;

    /**
     * price per item quantity
     */
    var $ItemPrice;

    /**
     * quantity of the item (non-negative)
     */
    var $ItemCount;

    /**
     * Unit of measure for the itemCount
     */
    var $ItemCountUnit;

    /**
     * discount applied to this item
     */
    var $Discount;

    /**
     * identifies whether this item is taxable or not. If not passed, this will be
     * assumed to be true.
     */
    var $Taxable;

    /**
     * The tax percentage applied to the item. This is only used for displaying in the
     * receipt, it is not used in pricing calculations. Note: we have totalTax at
     * invoice level. It's up to the caller to do the calculations for setting that
     * other element.
     */
    var $TaxRate;

    /**
     * Additional fees to this item
     */
    var $AdditionalFees;

    /**
     * identifies whether this is reimbursable or not. If not pass, this will be
     * assumed to be true.
     */
    var $Reimbursable;

    /**
     * Manufacturer part number.
     */
    var $MPN;

    /**
     * International Standard Book Number. Reference http://en.wikipedia.org/wiki/ISBN
     * Character length and limits: 32 single-byte characters
     */
    var $ISBN;

    /**
     * Price Look-Up code Reference http://en.wikipedia.org/wiki/Price_Look-Up_code
     * Character length and limits: 5 single-byte characters
     */
    var $PLU;

    /**
     * Character length and limits: 32 single-byte characters
     */
    var $ModelNumber;

    /**
     * Character length and limits: 32 single-byte characters
     */
    var $StyleNumber;

    function InvoiceItemType()
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
              'EAN' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'SKU' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ReturnPolicyIdentifier' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Price' => 
              array (
                'required' => false,
                'type' => 'BasicAmountType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ItemPrice' => 
              array (
                'required' => false,
                'type' => 'BasicAmountType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ItemCount' => 
              array (
                'required' => false,
                'type' => 'double',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ItemCountUnit' => 
              array (
                'required' => false,
                'type' => 'UnitOfMeasure',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Discount' => 
              array (
                'required' => false,
                'type' => 'DiscountType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Taxable' => 
              array (
                'required' => false,
                'type' => 'boolean',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'TaxRate' => 
              array (
                'required' => false,
                'type' => 'double',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'AdditionalFees' => 
              array (
                'required' => false,
                'type' => 'AdditionalFeeType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'Reimbursable' => 
              array (
                'required' => false,
                'type' => 'boolean',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'MPN' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ISBN' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'PLU' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'ModelNumber' => 
              array (
                'required' => false,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'StyleNumber' => 
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
    function getEAN()
    {
        return $this->EAN;
    }
    function setEAN($EAN, $charset = 'iso-8859-1')
    {
        $this->EAN = $EAN;
        $this->_elements['EAN']['charset'] = $charset;
    }
    function getSKU()
    {
        return $this->SKU;
    }
    function setSKU($SKU, $charset = 'iso-8859-1')
    {
        $this->SKU = $SKU;
        $this->_elements['SKU']['charset'] = $charset;
    }
    function getReturnPolicyIdentifier()
    {
        return $this->ReturnPolicyIdentifier;
    }
    function setReturnPolicyIdentifier($ReturnPolicyIdentifier, $charset = 'iso-8859-1')
    {
        $this->ReturnPolicyIdentifier = $ReturnPolicyIdentifier;
        $this->_elements['ReturnPolicyIdentifier']['charset'] = $charset;
    }
    function getPrice()
    {
        return $this->Price;
    }
    function setPrice($Price, $charset = 'iso-8859-1')
    {
        $this->Price = $Price;
        $this->_elements['Price']['charset'] = $charset;
    }
    function getItemPrice()
    {
        return $this->ItemPrice;
    }
    function setItemPrice($ItemPrice, $charset = 'iso-8859-1')
    {
        $this->ItemPrice = $ItemPrice;
        $this->_elements['ItemPrice']['charset'] = $charset;
    }
    function getItemCount()
    {
        return $this->ItemCount;
    }
    function setItemCount($ItemCount, $charset = 'iso-8859-1')
    {
        $this->ItemCount = $ItemCount;
        $this->_elements['ItemCount']['charset'] = $charset;
    }
    function getItemCountUnit()
    {
        return $this->ItemCountUnit;
    }
    function setItemCountUnit($ItemCountUnit, $charset = 'iso-8859-1')
    {
        $this->ItemCountUnit = $ItemCountUnit;
        $this->_elements['ItemCountUnit']['charset'] = $charset;
    }
    function getDiscount()
    {
        return $this->Discount;
    }
    function setDiscount($Discount, $charset = 'iso-8859-1')
    {
        $this->Discount = $Discount;
        $this->_elements['Discount']['charset'] = $charset;
    }
    function getTaxable()
    {
        return $this->Taxable;
    }
    function setTaxable($Taxable, $charset = 'iso-8859-1')
    {
        $this->Taxable = $Taxable;
        $this->_elements['Taxable']['charset'] = $charset;
    }
    function getTaxRate()
    {
        return $this->TaxRate;
    }
    function setTaxRate($TaxRate, $charset = 'iso-8859-1')
    {
        $this->TaxRate = $TaxRate;
        $this->_elements['TaxRate']['charset'] = $charset;
    }
    function getAdditionalFees()
    {
        return $this->AdditionalFees;
    }
    function setAdditionalFees($AdditionalFees, $charset = 'iso-8859-1')
    {
        $this->AdditionalFees = $AdditionalFees;
        $this->_elements['AdditionalFees']['charset'] = $charset;
    }
    function getReimbursable()
    {
        return $this->Reimbursable;
    }
    function setReimbursable($Reimbursable, $charset = 'iso-8859-1')
    {
        $this->Reimbursable = $Reimbursable;
        $this->_elements['Reimbursable']['charset'] = $charset;
    }
    function getMPN()
    {
        return $this->MPN;
    }
    function setMPN($MPN, $charset = 'iso-8859-1')
    {
        $this->MPN = $MPN;
        $this->_elements['MPN']['charset'] = $charset;
    }
    function getISBN()
    {
        return $this->ISBN;
    }
    function setISBN($ISBN, $charset = 'iso-8859-1')
    {
        $this->ISBN = $ISBN;
        $this->_elements['ISBN']['charset'] = $charset;
    }
    function getPLU()
    {
        return $this->PLU;
    }
    function setPLU($PLU, $charset = 'iso-8859-1')
    {
        $this->PLU = $PLU;
        $this->_elements['PLU']['charset'] = $charset;
    }
    function getModelNumber()
    {
        return $this->ModelNumber;
    }
    function setModelNumber($ModelNumber, $charset = 'iso-8859-1')
    {
        $this->ModelNumber = $ModelNumber;
        $this->_elements['ModelNumber']['charset'] = $charset;
    }
    function getStyleNumber()
    {
        return $this->StyleNumber;
    }
    function setStyleNumber($StyleNumber, $charset = 'iso-8859-1')
    {
        $this->StyleNumber = $StyleNumber;
        $this->_elements['StyleNumber']['charset'] = $charset;
    }
}
