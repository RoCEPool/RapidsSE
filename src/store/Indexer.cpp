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

  BoardIter xBI(this);

  while ( (m_pMainCard = xBI.pEachMain(CK_SCAN)) ) {
    m_p_table = m_pMainCard->pSub_table();
    if ( (m_p_indexList = m_p_table->pNew_indexList()) )
      m_pMainCard->vInsertAfter(m_p_indexList,m_p_table);
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

  BoardIter xBI(this);

  while (m_pMainCard = xBI.pEachMain(CK_SCAN)) {
    if (! (m_p_filter = m_pMainCard->pSub_filter()) )
      continue;
    m_p_filter->vVisitAllSub(CK_PREDCMP,bfPredBounds);
  }

}
/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
