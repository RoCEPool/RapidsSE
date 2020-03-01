/*============================================================================*/
/* Division.cpp -- Pipe Stage-Partition                                       */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Pipelines,Division,CN_DIVISION)
__aNewFunction(Division,CN_DIVISION)
__vDeleteFunction(Division,CN_DIVISION)
__aFooterFunction(Division)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Division::Division(StageSpec* pStageSpec, i8 i_Div)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  m_i_Div      = i_Div;
  
  // determine the node,slot coordinates of this division
  if (m_pStageSpec->bNodeTopo()) {
    // node only
    m_i_Nde = i_Div;
    m_i_Ptn = C_UNKNOWN;
    m_i_Slt = C_UNKNOWN;
  }
  else {
    // node,slot
    m_i_Ptn = i_Div;
    m_i_Nde = TablePtn::iPartitionToNode(m_i_Ptn);
    m_i_Slt = TablePtn::iPartitionToSlot(m_i_Ptn);
  }
  
  vSynthesiseName();

  _checkInitialised_(CN_DIVISION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Division::~Division(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Division::vDump(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  if (m_pStageSpec->bNodeTopo()) {
    vTrace("  [%02lld] Node-Division: Node %lld: \n"_Stz,
           m_i_Div,
           m_i_Nde);
  }
  else {
    vTrace("  [%02lld] Partition-Division: Node %lld Slot %lld: \n"_Stz,
           m_i_Div,
           m_i_Nde,
           m_i_Slt);
  }

  for (i8 i_Op = 0; i_Op < nItems(); i_Op++) {
    Op* pOp = pGet(i_Op);
  
    pOp->vDump(i_Op);
  }
}

/*----------------------------------------------------------------------------*/
/* get partition this division needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 Division::i_Nde(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Nde;
}

/*----------------------------------------------------------------------------*/
/* get partition this division needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 Division::i_Ptn(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* get slot this division needs to run on                                     */
/*----------------------------------------------------------------------------*/
i8 Division::i_Slt(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Slt;
}

/*----------------------------------------------------------------------------*/
/* get stage spec for this division                                           */
/*----------------------------------------------------------------------------*/
StageSpec* Division::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for partitions of given node                               */
/*----------------------------------------------------------------------------*/
void Division::vSpawnOperators(i8 iSessionId, i8 iNode)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  i8 iPtnNode = TablePtn::iPartitionToNode(m_i_Ptn);
  
  if (iPtnNode == iNode) {
    // for each operator in the stage ptn
    for (i8 i_Op = nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = pGet(i_Op);
      
      bool bSpawned = pOp->bSpawnJob(iSessionId);
      
      ASSERT(bSpawned);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* synthesise name of this division                                           */
/*----------------------------------------------------------------------------*/
void Division::vSynthesiseName(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
 
  if (m_pStageSpec->bNodeTopo()) {
    m_sDivisionNm.vSprint("Div_N%lld_of_%s"_Stz,
                          m_i_Nde,
                          m_pStageSpec->szStageNm());
  }
  else {
    m_sDivisionNm.vSprint("Div_N%lldS%lld_of_%s"_Stz,
                          m_i_Nde,
                          m_i_Slt,
                          m_pStageSpec->szStageNm());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
