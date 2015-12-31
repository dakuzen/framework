<?php
/**
 * @package PayPal
 */

/**
 * Make sure our parent class is defined.
 */
require_once 'PayPal/Type/XSDSimpleType.php';

/**
 * IdentificationInfoType
 *
 * @package PayPal
 */
class IdentificationInfoType extends XSDSimpleType
{
    /**
     * Mobile specific buyer identification.
     */
    var $MobileIDInfo;

    /**
     * Contains login bypass information.
     */
    var $RememberMeIDInfo;

    /**
     * Identity Access Token.
     */
    var $IdentityTokenInfo;

    function IdentificationInfoType()
    {
        parent::XSDSimpleType();
        $this->_namespace = 'urn:ebay:apis:eBLBaseComponents';
        $this->_elements = array_merge($this->_elements,
            array (
              'MobileIDInfo' => 
              array (
                'required' => false,
                'type' => 'MobileIDInfoType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'RememberMeIDInfo' => 
              array (
                'required' => false,
                'type' => 'RememberMeIDInfoType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
              'IdentityTokenInfo' => 
              array (
                'required' => false,
                'type' => 'IdentityTokenInfoType',
                'namespace' => 'urn:ebay:apis:eBLBaseComponents',
              ),
            ));
    }

    function getMobileIDInfo()
    {
        return $this->MobileIDInfo;
    }
    function setMobileIDInfo($MobileIDInfo, $charset = 'iso-8859-1')
    {
        $this->MobileIDInfo = $MobileIDInfo;
        $this->_elements['MobileIDInfo']['charset'] = $charset;
    }
    function getRememberMeIDInfo()
    {
        return $this->RememberMeIDInfo;
    }
    function setRememberMeIDInfo($RememberMeIDInfo, $charset = 'iso-8859-1')
    {
        $this->RememberMeIDInfo = $RememberMeIDInfo;
        $this->_elements['RememberMeIDInfo']['charset'] = $charset;
    }
    function getIdentityTokenInfo()
    {
        return $this->IdentityTokenInfo;
    }
    function setIdentityTokenInfo($IdentityTokenInfo, $charset = 'iso-8859-1')
    {
        $this->IdentityTokenInfo = $IdentityTokenInfo;
        $this->_elements['IdentityTokenInfo']['charset'] = $charset;
    }
}
