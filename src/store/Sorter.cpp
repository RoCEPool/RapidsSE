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

  vTransformation(OT_Cull_sby_SameAs_gby);

  BoardIter xBI(m_pBoard);

  while ( (m_p_sort = xBI.pEach_sort()) ) {

    if (! (m_p_aggreg = xBI.pPrev_aggreg()) )
      return;

    m_p_gbyElist = m_p_aggreg->pSub_gbyElist();
    m_p_sbyElist = m_p_sort->pSub_sbyElist();

    if (bRecursivelyTheSame(m_p_gbyElist, m_p_sbyElist)) {
      xBI.vRemoveDeleteCurr(CK_SORT);
      break;
    }
  }
}
/*----------------------------------------------------------------------------*/
//void Sorter::vCull_sby_SameAs_gby(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_SORTER, _FOOTER_);
//
//  BoardIter xBI(m_pBoard);
//
//  vBeginIteration();
//
//  while (Card_Sort *pCard_sort = _pCard_Sort(pEachMain(CK_SORT))) {
//
//    Card_Aggreg* pCard_aggreg =
//        _pCard_Aggreg(m_pBoard->pFindPrevMain(pCard_sort,CK_AGGREG));
//
//    if (! pCard_aggreg)
//      return;
//
//    Card_GbyElist* pCard_GbyElist = _pGetSub_Card_GbyElist(pCard_aggreg);
//    Card_SbyElist* pCard_SbyElist = _pGetSub_Card_SbyElist(pCard_sort);
//
//    if (Card::bCardsSameRecursive(pCard_GbyElist,pCard_SbyElist)) {
//      m_pBoard->vRemoveMainCard(pCard_sort);
//      delete pCard_sort;
//      break;
//    }
//  }
//
//  vEndIteration();
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
