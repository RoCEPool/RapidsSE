/*============================================================================*/
/* XF_Shorten_queryNm.cpp -- Shorten Query Names                           */
/*============================================================================*/

#include "XF_Shorten_queryNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__aNewFunction(XF_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__vDeleteFunction(XF_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__aFooterFunction(XF_Shorten_queryNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Shorten_queryNm::XF_Shorten_queryNm(CardIter* pCI)
: XF_TRANSFORM(CN_XF_SHORTEN_QUERYNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);

  _checkInitialised_(CN_XF_SHORTEN_QUERYNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Shorten_queryNm::~XF_Shorten_queryNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Shorten_queryNm::vXF_Shorten_queryNm(CardIter* pCI)
{
  _static_

  XF_Shorten_queryNm* pXF_Shorten_QueryNames = new XF_Shorten_queryNm(pCI);

  pXF_Shorten_QueryNames->vXF_Shorten_queryNm();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Shorten_queryNm::vXF_Shorten_queryNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "shorten query names"_Stz);

  s32 sBoardNm = m_pCI->G.pBoard->szBoardNm();
  if (Symbol::b_IsSubQueryName(sBoardNm)) {
    i8 iSqNo = StrLib::iGleanInt(sBoardNm.azChar());
    Symbol xSymbol_sqNo;
    xSymbol_sqNo.vSetSubQueryOrdinal(iSqNo);
    m_pCI->G.pBoard->vBoardNm(xSymbol_sqNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bLambdaFn = [pCI](Card* p_card) {
    if (!Symbol::b_IsSubQueryName(p_card->szSymbol()))
      return C_CONTINUE_RECURSION;

    i8 iSqNo = StrLib::iGleanInt(p_card->szSymbol()->azChar());

    Symbol xSymbol_sqNo;
    xSymbol_sqNo.vSetSubQueryOrdinal(iSqNo);
    p_card->vSymbol(xSymbol_sqNo);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE,bLambdaFn);


  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
