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
