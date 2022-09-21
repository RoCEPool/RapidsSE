/*============================================================================*/
/* Curator.cpp --                                                           */
/*============================================================================*/

#include "Curator.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Curator, CN_CURATOR)
__aNewFunction(Curator, CN_CURATOR)
__vDeleteFunction(Curator, CN_CURATOR)
__aFooterFunction(Curator)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Curator::Curator(Bench* pBench)
: TransformerTool(CN_CURATOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);

  _checkInitialised_(CN_CURATOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Curator::~Curator(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Curator::vLinearize_Stages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);

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
