/*============================================================================*/
/* CarryForward.cpp -- CarryForward                                           */
/*============================================================================*/

#include "CarryForward.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, CarryForward, CN_CARRYFORWARD)
__aNewFunction(CarryForward, CN_CARRYFORWARD)
__vDeleteFunction(CarryForward, CN_CARRYFORWARD)
__aFooterFunction(CarryForward)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CarryForward::CarryForward(void)
: Object(CN_CARRYFORWARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table.m_bSeen      = false;
  Table.m_nTables    = 0;
  Table.m_bReproject = false;

  Where_preAgg.m_bSeen  = false;
  Where_preAgg.m_bMorph_sqlOp_To_EQ = false;
  Where_preAgg.bReplace_sqCall_With_result = false;

  Aggreg.m_bSeen   = false;
  Aggreg.m_bMorph_To_gbyAgg = false;

  GbyAgg.m_bSeen   = false;
  GbyAgg.m_bInject_foldExpr = false;

  Project_postAgg.m_bSeen   = false;

  Having.m_bSeen                 = false;
  Having.m_bCorrect_After_Inject = false;

  Sort.m_bSeen                 = false;
  Sort.m_bCorrect_After_Inject = false;

  Unproject.m_bSeen               = false;
  Unproject.m_bUnproject_foldExpr = false;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CarryForward::~CarryForward(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vSetSeen(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  pCI->vBeginEach();

  RASSERT(pCI->bEachOp(CK_c_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_c_FOLD));

  // SubQuery
  if (pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->pAdvanceChild()->bIsCard(CK_SUBQUERY));

  // From*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    Table.m_bSeen = true;
    Table.m_nTables++;
  }

  // [Where]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE)) {
    Where_preAgg.m_bSeen = true;
  }

  // Aggreg
  while (pCI->pPeekNextIf(CK_o_AGGREG) && pCI->bEachOp(CK_o_AGGREG)) {
    GbyAgg.m_bSeen = true;
  }
  // GbyAgg
  while (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    GbyAgg.m_bSeen = true;
  }

  // [project]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
    Project_postAgg.m_bSeen = true;
  }

  // Having
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING)) {
    Having.m_bSeen = true;
  }

  // Sort
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    Sort.m_bSeen = true;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
