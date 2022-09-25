/*============================================================================*/
/* Stager.cpp --                                                           */
/*============================================================================*/

#include "Stager.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Stager, CN_STAGER)
__aNewFunction(Stager, CN_STAGER)
__vDeleteFunction(Stager, CN_STAGER)
__aFooterFunction(Stager)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stager::Stager(Bench* pBench)
: TransformerTool(CN_STAGER, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  _checkInitialised_(CN_STAGER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stager::~Stager(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Stager::vAdd_Stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  i8 iStage = 0;
  i8 iScan = 0;
  i8 iIndexJoin = 0;
  i8 iProject = 0;
  eYG eStageTopo = YG_NONE;
  Card_StageTopo* pCard_StageTopo = nullptr;

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = m_pBoard->pGetMainCard(i_Main);

    eCK eCard = pMainCard->eCard();

    if (CK_STAGE == eCard) {
      if (iStage++)
        break;
      pCard_StageTopo
          = _pCard_StageTopo(pMainCard->pFindChildCard(CK_STAGETOPO));

    }
    else
    if (CK_SCAN == eCard) {
      iScan++;
      RASSERT(1 == iScan);
      continue;
    }
    else
    if (CK_INDEXJOIN == eCard) {
      iIndexJoin++;

      Card* pMainCard_next = m_pBoard->pGetMainCard(i_Main+1);
      eCK eCard_next = pMainCard_next->eCard();
      if (CK_PROJECT == eCard_next)
        continue;

      Card_Stage* pCard_Stage_new = new Card_Stage;
      Card_StageTopo* pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_after(pCard_Stage_new,pMainCard);
      // skip stage just inserted
      i_Main++;
      continue;
    }
    else
    if (CK_PROJECT == eCard) {
      iProject++;

      Card* pMainCard_next = m_pBoard->pGetMainCard(i_Main+1);
      eCK eCard_next = pMainCard_next->eCard();
      if (CK_STAGE == eCard_next)
        continue;

      Card_Stage* pCard_Stage_new = new Card_Stage;
      Card_StageTopo* pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_after(pCard_Stage_new,pMainCard);
      // skip stage just inserted
      i_Main++;
    }

  }
}

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Stager::vLinearize_Stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  Card_Stage* pCard_stage_a =
      _pCard_Stage(m_pBoard->pFindMain(CK_STAGE));
  if (! pCard_stage_a)
    return;
  Card_StageTopo* pCard_stageTopo_a =
      _pCard_StageTopo(pCard_stage_a->pFindSubCard(CK_STAGETOPO));
  if (! pCard_stageTopo_a)
    return;

  if (YG_ALL_PARTITIONS != pCard_stageTopo_a->eStageTopo())
    return;

  for (;;) {
    Card_Stage* pCard_stage_b =
        _pCard_Stage(m_pBoard->pFindNextMain(pCard_stage_a,CK_STAGE));
    if (! pCard_stage_b)
      return;
    Card_StageTopo* pCard_stageTopo_b =
        _pCard_StageTopo(pCard_stage_b->pFindSubCard(CK_STAGETOPO));
    if (! pCard_stageTopo_b)
      return;

    if (YG_ALL_PARTITIONS != pCard_stageTopo_b->eStageTopo())
      return;

    m_pBoard->vRemoveMainCard(pCard_stage_b);

    delete pCard_stage_b;

    m_pBoard->vRenumberMainCards();
  }

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
