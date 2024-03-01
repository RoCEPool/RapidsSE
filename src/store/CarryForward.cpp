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

  m_p_sqCrlKey = nullptr;
  m_bCrlKeyActive = false;

  m_p_sqResult = nullptr;
  m_bResultActive = false;

  m_pCF = nullptr;

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

  delete m_pCF;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bCrlKeyActive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_bCrlKeyActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vCrlKeyActive(bool bCrlKeyActive)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_bCrlKeyActive = bCrlKeyActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bIsCorrelated(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return (m_p_sqCrlKey ? true : false);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bResultActive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_bResultActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vResultActive(bool bResultActive)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_bResultActive = bResultActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CarryForward::p_sqCrlKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_p_sqCrlKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::v_sqCrlKey(Card_Expr* p_sqCrlKey)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_p_sqResult = p_sqCrlKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CarryForward::p_sqResult(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_p_sqResult;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::v_sqResult(Card_Expr* p_sqResult)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_p_sqResult = p_sqResult;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bWorkToDo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  if (m_bResultActive || m_bCrlKeyActive)
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vSetSeen(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  pCI->vBeginEach();

  RASSERT(pCI->bEachMain(CK_c_FOLD));
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_c_FOLD));

  // SubQuery
  if (pCI->bIsNextMain(CK_SUBQUERY))
    RASSERT(pCI->pAdvanceChild()->bIsCard(CK_SUBQUERY));

  // From*
  while (pCI->bIsNextMain(CK_o_FROM) && pCI->bEachMain(CK_o_FROM)) {
    Table.m_bSeen = true;
    Table.m_nTables++;
  }

  // [Where]
  if (pCI->bIsNextMain(CK_o_WHERE) && pCI->bEachMain(CK_o_WHERE)) {
    Where_preAgg.m_bSeen = true;
  }

  // Aggreg
  while (pCI->bIsNextMain(CK_o_AGGREG) && pCI->bEachMain(CK_o_AGGREG)) {
    GbyAgg.m_bSeen = true;
  }
  // GbyAgg
  while (pCI->bIsNextMain(CK_o_GBYAGG) && pCI->bEachMain(CK_o_GBYAGG)) {
    GbyAgg.m_bSeen = true;
  }

  // [project]
  if (pCI->bIsNextMain(CK_o_PROJECT) && pCI->bEachMain(CK_o_PROJECT)) {
    Project_postAgg.m_bSeen = true;
  }

  // Having
  if (pCI->bIsNextMain(CK_o_HAVING) && pCI->bEachMain(CK_o_HAVING)) {
    Having.m_bSeen = true;
  }

  // Sort
  if (pCI->bIsNextMain(CK_o_SORT) && pCI->bEachMain(CK_o_SORT)) {
    Sort.m_bSeen = true;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
