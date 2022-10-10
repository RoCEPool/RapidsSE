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

  m_p_stage_new = nullptr;

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
void Stager::vAdd_stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  vTransformation(OT_vAdd_stages);
  vExplain("Blah"_Stz);

  i8 iFilter    = 0;
  i8 iIndexJoin = 0;
  i8 iOther     = 0;
  i8 iProject   = 0;
  i8 iScan      = 0;
  i8 iSort      = 0;
  i8 iStage     = 0;

  eYG eStageTopo = YG_NONE;
  Card_StageTopo* pCard_StageTopo = nullptr;

  BoardIter xBI(m_pBoard);
  while ( (m_pMainCard = xBI.pEachMain()) ) {

    if (! xBI.bMore())
      break;

    m_pNextMain = xBI.pNextMain();
    m_eNextMain = m_pNextMain->eCard();
    if (CK_STAGE == m_eNextMain)
      continue;

    switch (eDecodeMain(m_pMainCard)) {

      case CK_FILTER:
        RASSERT(m_p_filter == m_pMainCard);
        iFilter++;

        if (CK_INDEXJOIN == m_eNextMain)
          break;

        m_p_stage_new = m_p_stage->pDupCardAndSubCards();
        m_pBoard->vInsertMainCard_after(m_p_stage_new, m_p_filter);
        xBI.vForward();
        break;

      case CK_INDEXJOIN:
        RASSERT(m_p_indexJoin == m_pMainCard);
        iIndexJoin++;

        if (CK_PROJECT == m_eNextMain || CK_FILTER == m_eNextMain)
          break;

        m_p_stage_new = m_p_stage->pDupCardAndSubCards();
        m_pBoard->vInsertMainCard_after(m_p_stage_new, m_p_indexJoin);
        xBI.vForward();
        break;

      case CK_PROJECT:
        RASSERT(m_p_project == m_pMainCard);
        iProject++;

        m_p_stage_new = m_p_stage->pDupCardAndSubCards();
        m_pBoard->vInsertMainCard_after(m_p_stage_new, m_p_project);
        xBI.vForward();
        break;

      case CK_SCAN:
        RASSERT(m_p_scan == m_pMainCard);
        iScan++;
        RASSERT(1 == iScan);
        break;

      case CK_SORT:
        RASSERT(m_p_sort == m_pMainCard);
        iSort++;

        m_p_stage_new = m_p_stage->pDupCardAndSubCards();
        m_pBoard->vInsertMainCard_after(m_p_stage_new, m_p_sort);
        xBI.vForward();
        break;

      case CK_STAGE:
        RASSERT(m_p_stage == m_pMainCard);
        iStage++;

        m_p_stageTopo = m_p_stage->pSub_stageTopo();
        m_eStageTopo = m_p_stageTopo->eStageTopo();
        break;

      default:
        iOther++;
        break;
    }
  }

  i8 i_Stage = 1;
  xBI.vRestart();
  while ( (m_p_stage = _pCard_Stage(xBI.pEachMain(CK_STAGE))) )
    m_p_stage->vRenameStage(i_Stage++);

  vPrintBoard("Stager::vAdd_stages"_Stz);
}

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Stager::vLinearize_stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGER, _FOOTER_);

  vTransformation(OT_vLinearize_stages);
  vExplain("Linearize all the stages"_Stz);

  BoardIter xBI(m_pBoard);

  if (! (m_p_stage_a = _pCard_Stage(xBI.pEachMain(CK_STAGE))))
    goto L_Exit;
  if (! (m_p_stageTopo_a = m_p_stage_a->pSub_stageTopo()) )
    goto L_Exit;
  if (YG_ALL_PARTITIONS != m_p_stageTopo_a->eStageTopo())
    goto L_Exit;

  while ((m_p_stage_b = _pCard_Stage(xBI.pEachMain(CK_STAGE)))) {
    if (! (m_p_stageTopo_b = m_p_stage_b->pSub_stageTopo()) )
      break;
    if (YG_ALL_PARTITIONS != m_p_stageTopo_b->eStageTopo())
      break;
    xBI.vRemoveDeleteCurr(CK_STAGE);
  }

  L_Exit:

  xBI.vRestart();
  while ((m_p_stage = _pCard_Stage(xBI.pEachMain(CK_STAGE)))) {
    if (! (m_p_stageTopo = m_p_stage->pSub_stageTopo()) )
      continue;
    if (YG_ALL_PARTITIONS == m_p_stageTopo->eStageTopo())
      continue;

    m_p_stage->vAddProperty_nameOnly(C_PPT_Exclude);
  }


  m_pBoard->vRenumberMainCards();

  vSetPrintSubCards(CK_AGGELIST, false);
  vSetPrintSubCards(CK_GBYELIST, false);
  vSetPrintSubCards(CK_PROJELIST, false);

  vPrintBoard("Stager::vLinearize_stages"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
