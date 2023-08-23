/*============================================================================*/
/* XF_Prolog_Shorten_queryNm.cpp -- Shorten Query Names                           */
/*============================================================================*/

#include "XF_Prolog_Shorten_queryNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__aNewFunction(XF_Prolog_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__vDeleteFunction(XF_Prolog_Shorten_queryNm, CN_XF_SHORTEN_QUERYNM)
__aFooterFunction(XF_Prolog_Shorten_queryNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Shorten_queryNm::XF_Prolog_Shorten_queryNm(CardIter* pCI)
: XF_PROLOG(CN_XF_SHORTEN_QUERYNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);

  _checkInitialised_(CN_XF_SHORTEN_QUERYNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Shorten_queryNm::~XF_Prolog_Shorten_queryNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Shorten_queryNm::vXF_Shorten_queryNm(CardIter* pCI)
{
  _static_

  XF_Prolog_Shorten_queryNm* pXF_Shorten_QueryNames = new XF_Prolog_Shorten_queryNm(pCI);

  pXF_Shorten_QueryNames->vXF_Shorten_queryNm();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Shorten_queryNm::vXF_Shorten_queryNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SHORTEN_QUERYNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_SHORTEN_QUERYNM, "shorten query names"_Stz);

  s32 sBoardNm = m_pCI->G.pBoard->szBoardNm();
  if (Symbol::bIs_Sqy_Nm(sBoardNm)) {
    i8 iSqNo = StrLib::iGleanInt(sBoardNm.azChar());
    Symbol xSymbol_sqNo(SF_SQY_NO,iSqNo);
    m_pCI->G.pBoard->vBoardNm(xSymbol_sqNo.szSF_symbol(SF_SQY_NO));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  auto bLambdaFn = [](Card* p_card) {
    if (!Symbol::bIs_Sqy_Nm(p_card->szSymbol(SF_NONE)))
      return C_CONTINUE_RECURSION;

    i8 iSqNo = StrLib::iGleanInt(p_card->szSymbol(SF_NONE)->azChar());

    Symbol xSymbol_sqNo(SF_SQY_NO,iSqNo);
    p_card->pSymbol()->vSym(&xSymbol_sqNo);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE,bLambdaFn);
  UNUSED(b);


  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
