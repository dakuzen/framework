<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * IdentityTokenInfoType
 *
 * @package PayPal
 */
class IdentityTokenInfoType extends XSDSimpleType
{
    /**
     * Identity Access token from merchant
     */
    var $AccessToken;

    function IdentityTokenInfoType()
    {
        parent::XSDSimpleType();
        $this->_namespace = 'urn:ebay:apis:eBLBaseComponents';
        $this->_elements = array_merge($this->_elements,
            array (
              'AccessToken' => 
              array (
                'required' => true,
                'type' => 'string',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
            ));
    }

    function getAccessToken()
    {
        return $this->AccessToken;
    }
    function setAccessToken($AccessToken, $charset = 'iso-8859-1')
    {
        $this->AccessToken = $AccessToken;
        $this->_elements['AccessToken']['charset'] = $charset;
    }
}
