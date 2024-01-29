/*============================================================================*/
/* XF_Prolog_X_pipe_To_sqyNm.cpp -- Change boardNm to sqyNm                  */
/*============================================================================*/

#include "XF_Prolog_X_pipe_To_sqyNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_pipe_To_sqyNm, CN_XF_PROLOG_X_PIPE_TO_SQYNM)
__aNewFunction(XF_Prolog_X_pipe_To_sqyNm, CN_XF_PROLOG_X_PIPE_TO_SQYNM)
__vDeleteFunction(XF_Prolog_X_pipe_To_sqyNm, CN_XF_PROLOG_X_PIPE_TO_SQYNM)
__aFooterFunction(XF_Prolog_X_pipe_To_sqyNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_pipe_To_sqyNm::XF_Prolog_X_pipe_To_sqyNm(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_PIPE_TO_SQYNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_PIPE_TO_SQYNM, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_PIPE_TO_SQYNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_PIPE_TO_SQYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_pipe_To_sqyNm::~XF_Prolog_X_pipe_To_sqyNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_PIPE_TO_SQYNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_pipe_To_sqyNm::vConvert_board(CardIter* pCI)
{
  _static_

  XF_Prolog_X_pipe_To_sqyNm* pXF_Shorten_QueryNames = new XF_Prolog_X_pipe_To_sqyNm(pCI);

  pXF_Shorten_QueryNames->vX_Board_pipe_To_sqyNm();

  delete pXF_Shorten_QueryNames;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_pipe_To_sqyNm::vConvert_deck(CardIter* pCI)
{
  _static_

  XF_Prolog_X_pipe_To_sqyNm* pXF_Shorten_QueryNames = new XF_Prolog_X_pipe_To_sqyNm(pCI);

  pXF_Shorten_QueryNames->vX_Deck_pipe_To_sqyNm();

  delete pXF_Shorten_QueryNames;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_pipe_To_sqyNm::vX_Board_pipe_To_sqyNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_PIPE_TO_SQYNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_X_PIPE_TO_SQYNM, "pipe to sqyNm"_Stz);

#ifdef Z_SQY_Qn

  Symbol xSymbol_sqyNm(SF_SQY_NO, m_pCI->G.pBoard->i_Board());
  m_pCI->G.pBoard->pSubQuery()->vSqyNm(xSymbol_sqyNm.szSF_chrSeq(SF_SQY_NO));

#else

  if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
    m_pCI->m_pBench->vDump(__func__);

  s32 sSubPipeNm = m_pCI->G.pBoard->szSubPipeNm();
//  s32 sSqyNm = m_pCI->G.pBoard->pSubQuery()->szSqyNm();
//  RASSERT(sSqyNm.bSame(sSubPipeNm));

  if (Symbol::bIs_Sqy_No(sSubPipeNm)) {
    i8 iSqyNo = StrLib::iGleanInt(sSubPipeNm.azChar());
    Symbol xSymbol_sqyNm(SF_SQY_NO, iSqyNo);
    m_pCI->G.pBoard->vSubPipeNm(xSymbol_sqyNm.szSF_chrSeq(SF_SQY_NO));
  }
  else {
    vDebugPoint();
  }

#endif

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* shorten query names                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_pipe_To_sqyNm::vX_Deck_pipe_To_sqyNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_PIPE_TO_SQYNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_X_PIPE_TO_SQYNM, "shorten query names"_Stz);

#ifdef Z_SQY_Qn

  CardIter* pCI = m_pCI;
  auto bfTo_sqyNm = [pCI](Card* p_card) {
    if (! (p_card->bIsCard(CK_o_CALL) ||
          (p_card->bIsCard(CK_TERM) && Symbol::bIs_Sqy_Nm(p_card->szFacet(SF_NONE)))))
       return C_CONTINUE_RECURSION;


    eSF eFacet = p_card->pSymbol()->eFacet(0);
    RASSERT(SF_PPE_NM == eFacet || SF_SQY_NO == eFacet);


    pCI->vPrintBoard(__func__,__LINE__,true);
    p_card->vDump(__func__);
    vDebugPoint();
    Symbol* pSymbol_card = p_card->pSymbol();
    if (SF_PPE_NM == eFacet) {
      Stz* szSubPipeNm =pSymbol_card->szFacet(SF_PPE_NM);
      Board* pBoard_subPipeNm = pCI->m_pBench->pFindBoard_By_subPipeNm(szSubPipeNm);

      Symbol xSymbol_sqyNm(SF_SQY_NO, pBoard_subPipeNm->i_Board());
      pSymbol_card->vCastFacets(&xSymbol_sqyNm);
    }
    else {
      Stz* szSqyNm = pSymbol_card->szFacet(SF_SQY_NO);
      Board* pBoard_sqyNm = pCI->m_pBench->pFindBoard_By_subPipeNm(szSqyNm);

      Symbol xSymbol_sqyNm(SF_SQY_NO, pBoard_sqyNm->i_Board());
      pSymbol_card->vCastFacets(&xSymbol_sqyNm);
    }
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE, bfTo_sqyNm);
  UNUSED(b);

#else


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;
  auto bfTo_sqyNm = [pCI](Card* p_card) {
    if (!Symbol::bIs_Sqy_No(p_card->szFacet(SF_NONE)))
      return C_CONTINUE_RECURSION;


    eSF eFacet = p_card->pSymbol()->eFacet(0);
    RASSERT(SF_PPE_NM == eFacet || SF_SQY_NO == eFacet);

    i8 iSqyNo = StrLib::iGleanInt(p_card->szFacet(SF_NONE)->azChar());

    Symbol xSymbol_sqyNm(SF_SQY_NO, iSqyNo);
    p_card->pSymbol()->vCastFacets(&xSymbol_sqyNm);

//    pCI->vPrintBoard(__func__,__LINE__,true);
//    p_card->vDump(__func__);
//    vDebugPoint();
    if (SF_PPE_NM == eFacet) {

      Stz* szSubPipeNm = p_card->pSymbol()->szFacet(SF_PPE_NM);
      Board* pBoard_subPipeNm = pCI->m_pBench->pFindBoard_By_subPipeNm(szSubPipeNm);

      Board* pBoard_sqyNm = pCI->m_pBench->pFindBoard_By_sqyNm(szSubPipeNm);
      RASSERT(pBoard_sqyNm == pBoard_subPipeNm);
    }
    else {
      Stz* szSqyNm = p_card->pSymbol()->szFacet(SF_SQY_NO);
      Board* pBoard_subPipeNm = pCI->m_pBench->pFindBoard_By_subPipeNm(szSqyNm);

      Board* pBoard_sqyNm = pCI->m_pBench->pFindBoard_By_sqyNm(szSqyNm);
      RASSERT(pBoard_sqyNm == pBoard_subPipeNm);
    }
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE, bfTo_sqyNm);
  UNUSED(b);

#endif

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
