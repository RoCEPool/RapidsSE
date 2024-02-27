/*============================================================================*/
/* XF_Utility_Verify_dic.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Utility_Verify_dic.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aNewFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__vDeleteFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aFooterFunction(XF_Utility_Verify_dic)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::XF_Utility_Verify_dic(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_VALIDATE_BOARD, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_VALIDATE_BOARD);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::~XF_Utility_Verify_dic(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify(CardIter* pCI)
{
  _static_

  XF_Utility_Verify_dic* pXF_Validate = new XF_Utility_Verify_dic(pCI);

  try {
    pXF_Validate->vVerify();
  }
  catch (Error xError) {
    delete pXF_Validate;
    throw xError;
  }
  delete pXF_Validate;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  m_pCI->vTransform(__func__, XF_UTILITY_VERIFY_DIC, "verify dic"_Stz);


  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
