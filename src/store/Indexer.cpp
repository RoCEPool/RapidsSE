/*============================================================================*/
/* Indexer.cpp -- scan tool                                                    */
/*============================================================================*/

#include "Indexer.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Indexer, CN_SCANNING)
__aNewFunction(Indexer, CN_SCANNING)
__vDeleteFunction(Indexer, CN_SCANNING)
__aFooterFunction(Indexer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Indexer::Indexer(Bench* pBench)
: TransformerTool(CN_SCANNING, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  _checkInitialised_(CN_SCANNING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Indexer::~Indexer(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Indexer::vAppend_indexList_to_scan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_SCAN)) ) {
    m_p_table = m_pMainCard->pSub_table();
    m_p_tablePtr = m_p_table->pSub_tablePtr();
    if ( (m_p_indexList = m_p_tablePtr->pNew_indexList()) )
      m_pMainCard->vInsertAfter(m_p_indexList, m_p_table);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Indexer::vMark_scan_pred_bounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  auto bfPredBounds = [](Card* pCard) {
    Card_PredCmp* p_predCmp = _pCard_PredCmp(pCard);
    p_predCmp->vMarkBound();
    return true;
  };

  // - - - - - - - - -

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_SCAN)) ) {
    if (! (m_p_filter = m_pMainCard->pSub_filter()) )
      continue;
    m_p_filter->vVisitAllSub(CK_PREDCMP,bfPredBounds);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Indexer::vMark_scan_pred_ordinals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);



  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_SCAN)) ) {
    if (! (m_p_filter = m_pMainCard->pSub_filter()) )
      continue;

    Card_Stream* p_stream = m_pMainCard->pSub_stream();
    if (! p_stream)
      continue;

    Card_IndexList* p_indexList = m_pMainCard->pSub_indexList();
    if (! p_indexList)
      continue;

    Card_IndexPtr* p_indexPtr = _pCard_IndexPtr(p_indexList->pGetChild(0));
    if (! p_indexPtr)
      continue;

    // - - - - - - - - -
    auto bfPredBounds = [p_stream,p_indexPtr](Card* pCard) {
      Card_Term* p_term = _pCard_Term(pCard);

      Stz* szTerm = p_term->szTerm();
      if (! szTerm->bIsOrdinal())
        return true;

      vTrace("vMark_scan_pred_ordinals:: %s\n"_Stz,p_term->szTerm());

      Card_StreamCol* p_streamCol = p_stream->pGet_streamCol(szTerm);
      if (! p_streamCol)
        return true;

      Stz* szColNm = p_streamCol->szStreamColNm();
      vTrace("vMark_scan_pred_ordinals:: %s\n"_Stz,szColNm);

      i8 i_IdxCol = C_UNKNOWN;
      Card_IdxColNm* p_idxCol = p_indexPtr->pFind_indexCol(szColNm, i_IdxCol);

      if (! p_idxCol)
        return true;

      vTrace("vMark_scan_pred_ordinals:: %lld\n"_Stz,i_IdxCol);

      pCard->vMark1('I');
      pCard->vMark2(i_IdxCol);
      return true;
    };
    // - - - - - - - - -

    m_p_filter->vVisitAllSub(CK_TERM,bfPredBounds);
  }

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
