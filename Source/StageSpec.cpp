/*============================================================================*/
/* StageSpec.cpp -- Stage Specification                                       */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StageSpec::operator new(size_t)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(HeapMem::pAllocate(CN_STAGESPEC));
  
  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StageSpec::operator delete(void* aAddr)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(aAddr);
  
  HeapMem::vFree(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpec::StageSpec(Stz* szName)
: Object(CN_STAGESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sName = szName;
  
   m_eTopology = PO_NONE;
  
   m_pOpSpecVec = new OpSpecVec;
  
   _checkInitialised_(CN_STAGESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpec::~StageSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  delete m_pOpSpecVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add op spec                                                                */
/*----------------------------------------------------------------------------*/
void StageSpec::vAddOpSpec(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  pOpSpec->v_OpSpec(m_pOpSpecVec->nItems());
  
  m_pOpSpecVec->vAdd(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* copy ops from given stage spec                                             */
/*----------------------------------------------------------------------------*/
void StageSpec::vCopyOps(StageSpec* pStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpecVec* pOpSpecVec_from = pStageSpec->pOpSpecVec();
  
  for (i8 i_Op = 0; i_Op < pOpSpecVec_from->nItems(); i_Op++) {
    OpSpec* pOpSpec_from = pOpSpecVec_from->pGet(i_Op);
    
    m_pOpSpecVec->vAdd(pOpSpec_from);
  }
}

/*----------------------------------------------------------------------------*/
/* set locus                                                                  */
/*----------------------------------------------------------------------------*/
void StageSpec::vLocus(Stz* szLocus)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sLocus = szLocus;
}

/*----------------------------------------------------------------------------*/
/* get opspec vec                                                             */
/*----------------------------------------------------------------------------*/
OpSpecVec* StageSpec::pOpSpecVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_pOpSpecVec;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* StageSpec::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* prepend a consumeOp spec                                                   */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrependConsumeOp(ePX eTopologyX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpec* pOpSpec;
  i8 iConsumeArg = C_UNKNOWN;
  
  pOpSpec = new OpSpec("consume"_Stz,this);
  
  // add argument to the consumeOp depending on the topology
  switch (eTopologyX) {
    case PX_ALL_NODES_TO_ORIGIN:
      iConsumeArg = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iConsumeArg = AM::pGrid()->nSlotsPerNode();
      break;
      
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iConsumeArg = AM::pGrid()->nPartitions();
      break;
      
    case PX_ORIGIN_TO_ORIGIN:
      iConsumeArg = 1;
      break;
      
    default:
      RASSERT(0);
  }
  
  pOpSpec->vAddEnamePair("<arg_0>"_Stz,iConsumeArg,TN_INTEGER);
  m_pOpSpecVec->vInsert(pOpSpec);
  
  // rework the m_i_OpSpec
  for (i8 i_Op = 0; i_Op < m_pOpSpecVec->nItems(); i_Op++) {
    OpSpec* pOpSpec = m_pOpSpecVec->pGet(i_Op);
    pOpSpec->v_OpSpec(i_Op);
  }
  
}

/*----------------------------------------------------------------------------*/
/* prepend a startOp spec                                                     */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrependStartOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpec* pOpSpec;
  
  pOpSpec = new OpSpec("start"_Stz,this);
  m_pOpSpecVec->vInsert(pOpSpec);
  
  // rework the m_i_OpSpec
  for (i8 i_Op = 0; i_Op < m_pOpSpecVec->nItems(); i_Op++) {
    OpSpec* pOpSpec = m_pOpSpecVec->pGet(i_Op);
    pOpSpec->v_OpSpec(i_Op);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get topology                                                               */
/*----------------------------------------------------------------------------*/
ePO StageSpec::eTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_eTopology;
}

/*----------------------------------------------------------------------------*/
/* set topology                                                               */
/*----------------------------------------------------------------------------*/
void StageSpec::vTopology(ePO eTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eTopology = eTopology;
}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void StageSpec::vToPipeStr(Stz* szPipeStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < m_pOpSpecVec->nItems(); i_Op++) {\
    OpSpec* pOpSpec = m_pOpSpecVec->pGet(i_Op);

    if (i_Op)
      szPipeStr->vConcat(" | "_Stz);
    
    szPipeStr->vConcat(pOpSpec->szName());
    
//    if (pOpSpec->szArgs()->iLength()) {
//      szPipeStr->vConcat(" "_Stz);
//      szPipeStr->vConcat(pOpSpec->szArgs());
//    }
  }

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("stage %s "_Stz,m_sName.szStz());
  
  switch (m_eTopology) {
    case PO_ALL_NODES:           vTrace("on all nodes"_Stz);
                                 break;
    case PO_ALL_PARTITIONS:      vTrace("on all partitions"_Stz);
                                 break;
    case PO_ORIGIN:              vTrace("on origin"_Stz);
                                 break;
    case PO_SPECIFIC_NODE:       vTrace("on node %s"_Stz,m_sLocus.szStz());
                                 break;
    case PO_SPECIFIC_PARTITION:  vTrace("on partition %s"_Stz,m_sLocus.szStz());
                                 break;
    default:
      ;
  }
  vTrace(":\n"_Stz);


  vTrace("  "_Stz);
  for (i8 i_Op = 0; i_Op < m_pOpSpecVec->nItems(); i_Op++) {
    if (i_Op)
      vTrace("| "_Stz);
    m_pOpSpecVec->pGet(i_Op)->vPrint();
  }
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StageSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
