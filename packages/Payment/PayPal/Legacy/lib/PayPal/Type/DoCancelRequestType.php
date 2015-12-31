<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/AbstractRequestType.php';

/**
 * DoCancelRequestType
 *
 * @package PayPal
 */
class DoCancelRequestType extends AbstractRequestType
{
    /**
     * Msg Sub Id that was used for the orginal operation.
     */
    var $CancelMsgSubID;

    /**
     * Original API's type
     */
    var $APIType;

    function DoCancelRequestType()
    {
        parent::AbstractRequestType();
        $this->_namespace = 'urn:ebay:api:PayPalAPI';
        $this->_elements = array_merge($this->_elements,
            array (
              'CancelMsgSubID' => 
              array (
                'required' => true,
                'type' => 'string',
                'namespace' => 'urn:ebay:api:PayPalAPI',
              ),
              'APIType' => 
              array (
                'required' => true,
                'type' => 'APIType',
                'namespace' => 'urn:ebay:api:PayPalAPI',
              ),
            ));
    }

    function getCancelMsgSubID()
    {
        return $this->CancelMsgSubID;
    }
    function setCancelMsgSubID($CancelMsgSubID, $charset = 'iso-8859-1')
    {
        $this->CancelMsgSubID = $CancelMsgSubID;
        $this->_elements['CancelMsgSubID']['charset'] = $charset;
    }
    function getAPIType()
    {
        return $this->APIType;
    }
    function setAPIType($APIType, $charset = 'iso-8859-1')
    {
        $this->APIType = $APIType;
        $this->_elements['APIType']['charset'] = $charset;
    }
}
