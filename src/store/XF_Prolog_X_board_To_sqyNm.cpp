/*============================================================================*/
/* XF_Prolog_X_board_To_sqyNm.cpp -- Change boardNm to sqyNm                  */
/*============================================================================*/

#include "XF_Prolog_X_board_To_sqyNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_board_To_sqyNm, CN_XF_PROLOG_X_BOARD_TO_SQYNM)
__aNewFunction(XF_Prolog_X_board_To_sqyNm, CN_XF_PROLOG_X_BOARD_TO_SQYNM)
__vDeleteFunction(XF_Prolog_X_board_To_sqyNm, CN_XF_PROLOG_X_BOARD_TO_SQYNM)
__aFooterFunction(XF_Prolog_X_board_To_sqyNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_board_To_sqyNm::XF_Prolog_X_board_To_sqyNm(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_BOARD_TO_SQYNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_BOARD_TO_SQYNM, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_BOARD_TO_SQYNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_BOARD_TO_SQYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_board_To_sqyNm::~XF_Prolog_X_board_To_sqyNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_BOARD_TO_SQYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_board_To_sqyNm::vConvert(CardIter* pCI)
{
  _static_

  XF_Prolog_X_board_To_sqyNm* pXF_Shorten_QueryNames = new XF_Prolog_X_board_To_sqyNm(pCI);

  pXF_Shorten_QueryNames->vX_board_To_sqyNm();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_board_To_sqyNm::vX_board_To_sqyNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_BOARD_TO_SQYNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_X_BOARD_TO_SQYNM, "shorten query names"_Stz);

  s32 sPipelineNm = m_pCI->G.pBoard->szPipelineNm();
  if (Symbol::bIs_Sqy_Nm(sPipelineNm)) {
    i8 iSqyNo = StrLib::iGleanInt(sPipelineNm.azChar());
    Symbol xSymbol_sqyNm(SF_SQY_NM, iSqyNo);
    m_pCI->G.pBoard->vPipelineNm(xSymbol_sqyNm.szSF_chrSeq(SF_SQY_NM));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfTo_sqyNm = [](Card* p_card) {
    if (! Symbol::bIs_Sqy_Nm(p_card->szFacet(SF_NONE)))
      return C_CONTINUE_RECURSION;
    if (! p_card->pSymbol()->bHasFacet(SF_SQY_NM))
      vDebugPoint();

    i8 iSqyNo = StrLib::iGleanInt(p_card->szFacet(SF_NONE)->azChar());

    p_card->pSymbol()->vDeleteFacet(SF_NMD_COL);
    Symbol xSymbol_sqyNm(SF_SQY_NM, iSqyNo);
    p_card->pSymbol()->vCastFacets(&xSymbol_sqyNm);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE, bfTo_sqyNm);
  UNUSED(b);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
