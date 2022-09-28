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

  BoardIter xBI(this);

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

      m_p_scanPred->vAppend(p_predCmp);

      return true;
    };

    // - - - - - - - - -

    m_p_filter->vVisitAllSub(CK_PREDCMP,bfX);

  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
