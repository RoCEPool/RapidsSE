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
  i8 iFilter = 0;
  i8 iProject = 0;
  i8 iSort = 0;

  eYG eStageTopo = YG_NONE;
  Card_StageTopo* pCard_StageTopo = nullptr;

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card *pMainCard = m_pBoard->pGetMainCard(i_Main);

    eCK eCard = pMainCard->eCard();

    if (CK_STAGE == eCard) {
      i8 i_Child;
      pCard_StageTopo
          = _pCard_StageTopo(pMainCard->pFindChildCard(CK_STAGETOPO,i_Child));
      continue;
    }

    if (CK_SCAN == eCard) {
      iScan++;
      RASSERT(1 == iScan);
      continue;
    }

    if (CK_INDEXJOIN == eCard) {
      iIndexJoin++;

      Card *pMainCard_next = m_pBoard->pGetMainCard(i_Main + 1);
      eCK eCard_next = pMainCard_next->eCard();
      if (CK_PROJECT == eCard_next ||
          CK_FILTER == eCard_next)
        continue;

      Card_Stage *pCard_Stage_new = new Card_Stage;
      Card_StageTopo *pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_after(pCard_Stage_new, pMainCard);
      // skip stage just inserted
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }

    if (CK_FILTER == eCard) {
      iIndexJoin++;

      Card *pMainCard_next = m_pBoard->pGetMainCard(i_Main + 1);
      eCK eCard_next = pMainCard_next->eCard();

      if (CK_INDEXJOIN != eCard_next)
        continue;

      Card_Stage *pCard_Stage_new = new Card_Stage;
      Card_StageTopo *pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_after(pCard_Stage_new, pMainCard);
      // skip stage just inserted
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }

    if (CK_PROJECT == eCard) {
      iProject++;

      if (i_Main + 1 >= nMainCards)
        continue;

      Card *pMainCard_next = m_pBoard->pGetMainCard(i_Main + 1);
      eCK eCard_next = pMainCard_next->eCard();
      if (CK_STAGE == eCard_next)
        continue;

      Card_Stage *pCard_Stage_new = new Card_Stage;
      Card_StageTopo *pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_after(pCard_Stage_new, pMainCard);
      // skip stage just inserted
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }

    if (CK_SORT == eCard) {
      iSort++;

      Card_Stage *pCard_Stage_new = new Card_Stage;
      Card_StageTopo *pCard_StageTopo_new
          = _pCard_StageTopo(pCard_StageTopo->pDupCardAndSubCards());
      pCard_Stage_new->vAppend(pCard_StageTopo_new);
      m_pBoard->vInsertMainCard_before(pCard_Stage_new, pMainCard);
      // skip stage just inserted
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }
  }
}
/*----------------------------------------------------------------------------*/
void Stager::vAdd_Stages2(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  i8 iStage = 0;
  i8 iScan = 0;
  i8 iIndexJoin = 0;
  i8 iFilter = 0;
  i8 iProject = 0;
  i8 iSort = 0;

  eYG eStageTopo = YG_NONE;
  Card_StageTopo* pCard_StageTopo = nullptr;

  BoardIter xBI(this);
  while (m_pMainCard = xBI.pEachMain()) {

  }
}

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Stager::vLinearize_Stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  BoardIter xBI(this);

  if (! (m_p_stage_a = xBI.pFirst_stage()) )
    return;
  if (! (m_p_stageTopo_a = m_p_stage_a->pSub_stageTopo()) )
    return;
  if (YG_ALL_PARTITIONS != m_p_stageTopo_a->eStageTopo())
    return;

  for (;;) {
    if (! (m_p_stage_b = xBI.pNext_stage()) )
      return;
    if (! (m_p_stageTopo_b = m_p_stage_b->pSub_stageTopo()) )
      return;
    if (YG_ALL_PARTITIONS != m_p_stageTopo_b->eStageTopo())
      return;

    xBI.vRemoveDeleteCurr(CK_STAGE);

    m_pBoard->vRenumberMainCards();
  }

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
