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
  
  m_i_Div = i_Div;
  //m_oOpVec
  
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
/* get opVec                                                                  */
/*----------------------------------------------------------------------------*/
OpVec* Division::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* get partition this stageNde needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 Division::i_Nde(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Nde;
}

/*----------------------------------------------------------------------------*/
/* get partition this stageNde needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 Division::i_Ptn(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Ptn;
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
    for (i8 i_Op = m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = m_oOpVec.pGet(i_Op);
      
      bool bSpawned = pOp->bSpawnJob(iSessionId);
      
      ASSERT(bSpawned);
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
