 /*============================================================================*/
/* Card_Scan.cpp -- Scan                                                      */
/*============================================================================*/

#include "Card_Scan.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Scan,CK_SCAN)
__aNewFunction(Card_Scan,CN_CARD)
__vDeleteFunction(Card_Scan,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Scan::Card_Scan(void)
: Card_OP(CK_SCAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Scan::~Card_Scan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* individuate filter                                                         */
/*----------------------------------------------------------------------------*/
 void Card_Scan::vIndividuateFilter(CardIter* pCI) {

   _static_

   while (pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
     pCI->G.p_scan->vRemoveChild(pCI->G.p_filter);
     pCI->G.pDeck->vInsertAfter(pCI->G.p_filter, pCI->G.p_scan);

     if (! pCI->G.p_filter->pSub_level()) {
       pCI->G.p_filter->vPush(pCI->G.p_scan->pSub_level()->pDupCardRecursive());
     }
   }
 }

/*----------------------------------------------------------------------------*/
/* table morphs to scan                                                       */
/*----------------------------------------------------------------------------*/
void Card_Scan::vMorph_table_To_scan(STORE::CardIter *pCI)
{
  _static_

  pCI->G_vNewMember(CK_SCAN);
  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_scan);
}

/*----------------------------------------------------------------------------*/
/* scan becomes table                                                         */
/*----------------------------------------------------------------------------*/
void Card_Scan::vScanBecomesTable(STORE::CardIter *pCI)
{
  _static_

  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->G.p_scan);
  DELETE_CARD(pCI->G.p_scan);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Scan::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
 void Card_Scan::vEarlyProject_leaf(CardIter* pCI)
 {
   //vDump(0,(Stz*)__func__);

   _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

   pCI->Early.m_pOP_leaf = this;

   pCI->Analysis.m_pProjection_prior = pSub_projection();

   pCI->vPrintBoard();

   vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
