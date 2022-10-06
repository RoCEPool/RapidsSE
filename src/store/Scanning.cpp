/*============================================================================*/
/* Scanning.cpp -- scan tool                                                    */
/*============================================================================*/

#include "Scanning.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Scanning, CN_SCANNING)
__aNewFunction(Scanning, CN_SCANNING)
__vDeleteFunction(Scanning, CN_SCANNING)
__aFooterFunction(Scanning)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scanning::Scanning(Bench* pBench)
: TransformerTool(CN_SCANNING, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  _checkInitialised_(CN_SCANNING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Scanning::~Scanning(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scanning::vAdd_scanPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  BoardIter xBI(m_pBoard);

  while ( (m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN))) ) {

    m_p_filter = m_p_scan->pSub_filter();
    if (! m_p_filter)
      continue;

    m_p_indexList = m_p_scan->pSub_indexList();
    if (! m_p_indexList)
      continue;

    m_p_scanPred = nullptr;

    // - - - - - - - - -
    auto bfX = [this](Card* pCard) {
      Card_PredCmp* p_predCmp = _pCard_PredCmp(pCard);
      if (! ('L' == p_predCmp->cMark1() || 'U' == p_predCmp->cMark1()) )
        return true;

      if (! this->m_p_scanPred) {
        m_p_scanPred = new Card_ScanPred;
        m_p_scan->vAppend(m_p_scanPred);
      }

      if ('L' == p_predCmp->cMark1()) {
        m_p_lBound = m_p_scanPred->pSub_lBound();
        if (! m_p_lBound) {
          m_p_lBound = new Card_LBound;
          m_p_scanPred->vInsert(m_p_lBound);
        }
        m_p_lBound->vAppend(p_predCmp);
      }
      else
      if ('U' == p_predCmp->cMark1()) {
        m_p_uBound = m_p_scanPred->pSub_uBound();
        if (! m_p_uBound) {
          m_p_uBound = new Card_UBound;
          m_p_scanPred->vAppend(m_p_uBound);
        }
        m_p_uBound->vAppend(p_predCmp);
      }

      return true;
    };

    // - - - - - - - - -

    m_p_filter->vVisitAllSub(CK_PREDCMP,bfX);

  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Scanning::vAdd_scanPred"_Stz);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scanning::vCull_markedPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  BoardIter xBI(m_pBoard);

  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    m_p_filter = m_p_scan->pSub_filter();
    if (!m_p_filter)
      continue;

    m_p_filter->vCullPred('L');
    m_p_filter->vCullPred('U');

    if (! m_p_filter->bIsEmptyFilter())
      continue;

    m_p_scan->vRemoveSubCard(m_p_filter);

    delete m_p_filter;
    m_p_filter = nullptr;
  }

  m_pCardCtl->vPrintForm(CK_STREAM, CP_CARDFORM);
  m_pCardCtl->vPrintByKind(CK_STREAM, true);
  m_pCardCtl->vPrintSubCards(CK_STREAM, false);

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Scanning::vCull_markedPred"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scanning::vPromote_scan_to_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SCANNING, _FOOTER_);

  BoardIter xBI(m_pBoard);

  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    m_p_scanPred = m_p_scan->pSub_scanPred();
    if (!m_p_scanPred)
      continue;

    m_p_scan->vCard(CK_INDEXSCAN);
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Scanning::vPromote_scan_to_indexScan"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
