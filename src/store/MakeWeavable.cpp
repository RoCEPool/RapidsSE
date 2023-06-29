/*============================================================================*/
/* MakeWeavable.cpp -- Make Weavable                                          */
/*============================================================================*/

#include "MakeWeavable.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "XF_Dedupe_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeWeavable, CN_MAKEWEAVABLE)
__aNewFunction(MakeWeavable, CN_MAKEWEAVABLE)
__vDeleteFunction(MakeWeavable, CN_MAKEWEAVABLE)
__aFooterFunction(MakeWeavable)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeWeavable::MakeWeavable(CardIter* pCI)
: Object(CN_MAKEWEAVABLE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKEWEAVABLE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeWeavable::~MakeWeavable(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vWeavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  vReinstateStages();

  vAddStageCasts();

  vTblColToStmCol();
  vStmColToColNmOfKeyExists();

  XF_Dedupe_colNm::vXF_Dedupe_colNm(m_pCI);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage next casts                                                       */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vAddStageCasts(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "add stage casts"_Stz);

  m_pCI->G.pDeck->vAddStageCasts(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vReinstateStages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "reinstate stages"_Stz);

  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_PROJECTION,0);
  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vPrintBoard(__LINE__);

  Card_Stage::vReinstate_stages(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vStmColToColNmOfKeyExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEWEAVABLE,_FOOTER_);

  m_pCI->vTransform(__func__, "((20))))"_Stz);

  m_pCI->vRestart();

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
           || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    m_pCI->G.p_constraint->vConvert_stmColOdn_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vTblColToStmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEWEAVABLE,_FOOTER_);

  m_pCI->vTransform(__func__, "vTblColToStmCol"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain_reverse()) {

    m_pCI->Analysis.m_pProjection_prior = Card_Projection::pResolvePriorProjection(m_pCI);

    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_tblColOdn_To_stmColOdn(m_pCI);

//    if (m_pCI->G.p_OP->pSub_projection())
//      m_pCI->Analysis.m_pProjection_prior =
//          m_pCI->G.p_OP->pSub_projection();
  }
  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
