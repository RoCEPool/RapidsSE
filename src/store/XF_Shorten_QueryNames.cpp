/*============================================================================*/
/* XF_Shorten_QueryNames.cpp -- Shorten Query Names                           */
/*============================================================================*/

#include "XF_Shorten_QueryNames.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Shorten_QueryNames, CN_XF_SHORTEN_QUERYNAMES)
__aNewFunction(XF_Shorten_QueryNames, CN_XF_SHORTEN_QUERYNAMES)
__vDeleteFunction(XF_Shorten_QueryNames, CN_XF_SHORTEN_QUERYNAMES)
__aFooterFunction(XF_Shorten_QueryNames)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Shorten_QueryNames::XF_Shorten_QueryNames(CardIter* pCI)
: XF_TRANSFORM(CN_XF_SHORTEN_QUERYNAMES, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNAMES, _FOOTER_);

  _checkInitialised_(CN_XF_SHORTEN_QUERYNAMES);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNAMES, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Shorten_QueryNames::~XF_Shorten_QueryNames(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNAMES, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Shorten_QueryNames::vXF_Shorten_QueryNames(CardIter* pCI)
{
  _static_

  XF_Shorten_QueryNames* pXF_Shorten_QueryNames = new XF_Shorten_QueryNames(pCI);

  pXF_Shorten_QueryNames->vXF_Shorten_QueryNames();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Shorten_QueryNames::vXF_Shorten_QueryNames(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNAMES, _FOOTER_);


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
