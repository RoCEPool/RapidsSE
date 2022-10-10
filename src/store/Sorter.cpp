/*============================================================================*/
/* Sorter.cpp --                                                           */
/*============================================================================*/

#include "Sorter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Sorter,CN_SORTER)
__aNewFunction(Sorter,CN_SORTER)
__vDeleteFunction(Sorter,CN_SORTER)
__aFooterFunction(Sorter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sorter::Sorter(Bench* pBench)
: TransformerTool(CN_SORTER,pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SORTER,_FOOTER_);

  _checkInitialised_(CN_SORTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SORTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sorter::~Sorter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SORTER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull <sort> when <sbyElist> is same as <gbyElist>                          */
/*----------------------------------------------------------------------------*/
void Sorter::vCull_sby_SameAs_gby(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SORTER, _FOOTER_);

  vTransformation(OT_vCull_sby_SameAs_gby);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ( (m_p_sort = _pCard_Sort(xBI.pEachMain(CK_SORT))) ) {

    if (! (m_p_aggreg = _pCard_Aggreg(xBI.pPrevMainIf(CK_AGGREG))) )
      return;

    m_p_gbyElist = m_p_aggreg->pSub_gbyElist();
    m_p_sbyElist = m_p_sort->pSub_sbyElist();

    if (bRecursivelyTheSame(m_p_gbyElist, m_p_sbyElist)) {
      xBI.vRemoveDeleteCurr(CK_SORT);
      break;
    }
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sorter::vCull_sby_SameAs_gby"_Stz);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
