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
/* table morphs to scan                                                       */
/*----------------------------------------------------------------------------*/
void Card_Scan::vMorph_table_To_scan(STORE::CardIter *pCI)
{
  _static_

    pCI->G.p_from->vCard(CK_SCAN);
//  pCI->G_vNewMember(CK_SCAN);
//  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_scan);
}

/*----------------------------------------------------------------------------*/
/* scan becomes table                                                         */
/*----------------------------------------------------------------------------*/
void Card_Scan::vScanBecomesTable(STORE::CardIter *pCI)
{
  _static_

//  pCI->G.p_level = pCI->G.p_scan->pSub_level();
//  pCI->G.p_table = pCI->G.p_scan->pSub_table();
//  pCI->G.p_from  = pCI->G.p_scan->pSub_from();
//
//  pCI->G.p_scan->vRemoveChild(pCI->G.p_table);
//  pCI->G.p_scan->vRemoveChild(pCI->G.p_level);
//  pCI->G.p_scan->vRemoveChild(pCI->G.p_from);
//
//  pCI->G.pDeck->vReplace(pCI->G.p_scan,pCI->G.p_from);
//
//  pCI->G.p_from->vAppend(pCI->G.p_level);
//  pCI->G.p_from->vAppend(pCI->G.p_table);


  pCI->G.p_scan->vCard(CK_FROM);

  //DELETE_CARD(pCI->G.p_scan);
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
  UNUSED(pCI);
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
 void Card_Scan::vEarlyProject_leaf(CardIter* pCI)
 {
   //vDump(0,(Stz*)__func__);

   _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

   pCI->Early.m_pOP_leaf = this;

   pCI->Prior.m_pProjection = pSub_projection();

   pCI->vPrintBoard();

   vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Scan::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
 _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_TABLE,CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
