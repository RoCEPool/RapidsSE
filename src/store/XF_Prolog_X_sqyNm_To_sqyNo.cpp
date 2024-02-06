/*============================================================================*/
/* XF_Prolog_X_sqyNm_To_sqyNo.cpp -- Change boardNm to sqyNm                  */
/*============================================================================*/

#include "XF_Prolog_X_sqyNm_To_sqyNo.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_sqyNm_To_sqyNo, CN_XF_PROLOG_X_SQYNM_TO_SQYNO)
__aNewFunction(XF_Prolog_X_sqyNm_To_sqyNo, CN_XF_PROLOG_X_SQYNM_TO_SQYNO)
__vDeleteFunction(XF_Prolog_X_sqyNm_To_sqyNo, CN_XF_PROLOG_X_SQYNM_TO_SQYNO)
__aFooterFunction(XF_Prolog_X_sqyNm_To_sqyNo)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_sqyNm_To_sqyNo::XF_Prolog_X_sqyNm_To_sqyNo(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_SQYNM_TO_SQYNO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SQYNM_TO_SQYNO, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_SQYNM_TO_SQYNO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SQYNM_TO_SQYNO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_sqyNm_To_sqyNo::~XF_Prolog_X_sqyNm_To_sqyNo(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SQYNM_TO_SQYNO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_sqyNm_To_sqyNo::vConvert_deck_sqyNm_toSqyNo(CardIter* pCI)
{
  _static_

  XF_Prolog_X_sqyNm_To_sqyNo* pXF_Shorten_QueryNames = new XF_Prolog_X_sqyNm_To_sqyNo(pCI);

  pXF_Shorten_QueryNames->vX_Deck_sqyNm_To_sqyNo();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_sqyNm_To_sqyNo::vX_Deck_sqyNm_To_sqyNo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SQYNM_TO_SQYNO, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_X_SQYNM_TO_SQYNO, "shorten query names"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;
  auto bfTo_sqyNm = [pCI](Card* p_card) {
    UNUSED(pCI);
    Symbol* pSymbol_card = p_card->pSymbol();
    if (! pSymbol_card->nFacets())
      return C_CONTINUE_RECURSION;

    Stz* szFacet_symbol = pSymbol_card->szFacet(0);
    Board* pBoard = pCI->m_pBench->pFindBoard_By_sqyNm(szFacet_symbol);
    if (! pBoard)
      return C_CONTINUE_RECURSION;

    SubQuery* pSubQuery = pBoard->pSubQuery();

    Symbol xSymbol_sqyNo(SF_SQY_NO, pSubQuery->szSqyNo());

    pSymbol_card->vDeleteFacet(SF_SQY_NM);
    pSymbol_card->vCastFacets(&xSymbol_sqyNo);

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
