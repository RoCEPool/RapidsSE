/*============================================================================*/
/* Strand.cpp -- Strand of Stage of Pipeline                                  */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Pipelines,Strand,CN_STRAND)
__aNewFunction(Strand,CN_STRAND)
__vDeleteFunction(Strand,CN_STRAND)
__aFooterFunction(Strand)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Strand::Strand(StageSpec* pStageSpec, i8 i_Strand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STRAND,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  m_i_Strand   = i_Strand;
  
  // determine the node,tablePtn coordinates of this strand
  if (m_pStageSpec->bNodeTopo()) {
    // node only
    m_i_Node = i_Strand;
    m_tPtnNo = C_UNKNOWN;
    m_i_TablePtn = C_UNKNOWN;
  }
  else {
    // node,tablePtn
    m_tPtnNo = i_Strand;
    m_i_Node = PtnMap::i_PtnNoToNode(m_tPtnNo);
    m_i_TablePtn = Table::iPtnNoToTablePtn(m_tPtnNo);
  }
  
  vSynthesiseName();

  _checkInitialised_(CN_STRAND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Strand::~Strand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Strand::vDump(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  if (m_pStageSpec->bNodeTopo()) {
    vTrace("  [%02lld] (N%lld) Node-Strand %s:\n"_Stz,
           m_i_Strand,
           m_i_Node,
           m_sStrandNm.szStz());
  }
  else {
    vTrace("  [%02lld] (N%lld,S%02lld) Partition-Strand %s:\n"_Stz,
           m_i_Strand,
           m_i_Node,
           m_i_TablePtn,
           m_sStrandNm.szStz());
  }

  for (i8 i_Op = 0; i_Op < nItems(); i_Op++) {
    Op* pOp = pGet(i_Op);
  
    pOp->vDump(i_Op);
  }
}

/*----------------------------------------------------------------------------*/
/* get partition this strand needs to run on                                  */
/*----------------------------------------------------------------------------*/
i8 Strand::i_Node(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  return m_i_Node;
}

/*----------------------------------------------------------------------------*/
/* get partition in grid this strand needs to run on                          */
/*----------------------------------------------------------------------------*/
i8 Strand::tPtnNo(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* get tablePtn this strand needs to run on                                   */
/*----------------------------------------------------------------------------*/
i8 Strand::i_TablePtn(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  return m_i_TablePtn;
}

/*----------------------------------------------------------------------------*/
/* get stage spec for this strand                                             */
/*----------------------------------------------------------------------------*/
StageSpec* Strand::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for partitions of given node                               */
/*----------------------------------------------------------------------------*/
void Strand::vSpawnOperatorJobs(i8 iSessionId, i8 i_ThisNode)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
  
  i8 i_OnNode;
  if (m_pStageSpec->bNodeTopo())
    i_OnNode = m_i_Node;
  else
    i_OnNode = PtnMap::i_PtnNoToNode(m_tPtnNo);
  
  if (i_OnNode == i_ThisNode) {
    // for each operator in the stage ptn
    for (i8 i_Op = nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = pGet(i_Op);
      
      pOp->vSpawnOperatorJob(iSessionId);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* synthesise name of this strand                                             */
/*----------------------------------------------------------------------------*/
void Strand::vSynthesiseName(void)
{
  _precondition_(_MAGIC_,CN_STRAND,_FOOTER_);
 
  if (m_pStageSpec->bNodeTopo()) {
    m_sStrandNm.vSprint("_strand_N%lld_of_%s"_Stz,
                          m_i_Node,
                          m_pStageSpec->szStageNm());
  }
  else {
    m_sStrandNm.vSprint("_strandv_N%lldS%lld_of_%s"_Stz,
                          m_i_Node,
                          m_i_TablePtn,
                          m_pStageSpec->szStageNm());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
