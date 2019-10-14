/*============================================================================*/
/* Stage.cpp -- Pipe Stage                                                    */
/*============================================================================*/

#include "Operators.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stage* Operators::_pStage(void* aAddr)
{
  Stage* pStage = reinterpret_cast<Stage*>(aAddr);
  
  RASSERT_TYPE(pStage,CN_STAGE);
  
  return pStage;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Stage::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STAGE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Stage::operator delete(void* aAddr)
{
  Stage* pStage = reinterpret_cast<Stage*>(aAddr);
  
  HeapMem::vFree(pStage);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stage::Stage(StageSpec* pStageSpec, Pipeline* pPipeLine)
: Object(CN_STAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  //m_oOpVec
  
  switch (m_pStageSpec->eTopology()) {
    case PO_ALL_NODES:
      RASSERT(0);
      break;
      
    case PO_ALL_PARTITIONS:
      for (i8 i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
        m_oStagePtnVec.vAdd(new StagePtn(pStageSpec, i_Ptn));
      }
      break;
      
    case PO_ORIGIN:
      m_oStageNdeVec.vAdd(new StageNde(pStageSpec, 0));
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      RASSERT(0);
      break;
      
    default:
      RASSERT(0);
  }
  
  OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
  
  for (i8 i_OpSpec = 0; i_OpSpec < pOpSpecVec->nItems(); i_OpSpec++ ) {
    
    OpSpec* pOpSpec = pOpSpecVec->pGet(i_OpSpec);
    OpDefn* aOpDefn = gxOpCatalog.aFindOp(pOpSpec->szName());
    
    // unknown
    if (! aOpDefn) {
      vTrace(" ? %s\n"_Stz,pOpSpec->szName());
      ASSERT(0);
      continue;
    }
    
    // add the operator to the stages of the stageNdeVec as well
    for (i8 i_Index = 0; i_Index < m_oStageNdeVec.nItems(); i_Index++) {
      Op* pOp = (aOpDefn->m_fpNewOp)(pOpSpec,pPipeLine);
      StageNde* pStageNde = m_oStageNdeVec.pGet(i_Index);
      pOp->m_nNode = pStageNde->m_i_Nde;
      m_oOpVec.vAdd(pOp);
      pStageNde->m_oOpVec.vAdd(pOp);
    }
    
    // add the operator to the stages of the stagePtnVec as well
    for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
      Op* pOp = (aOpDefn->m_fpNewOp)(pOpSpec,pPipeLine);
      pOp->m_nPartition = i_Ptn;
      m_oOpVec.vAdd(pOp);
      m_oStagePtnVec.pGet(i_Ptn)->m_oOpVec.vAdd(pOp);
    }
  }
  
  _checkInitialised_(CN_STAGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stage::~Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Stage::vDump(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    s32 sArgs;
    
    EnameSet* pEnameSet = pOp->m_pOpSpec->pEnameSet();
    pEnameSet->vSprint(sArgs);
    
    vTrace("  [%02lld] { %9s: %10s%-24s endMarkers(L%lld R%lld) :%-6s %s }\n"_Stz,
           i_Op,
           pOp->szLabel(),
           aCN[pOp->eClass()],
           sArgs.aStz(),
           pOp->m_nLeftEndMarkers,
           pOp->m_nRightEndMarkers,
           pOp->szNextLabel(),
           pOp->m_bAppendRight ? "appendRight"_Stz : "appendLeft "_Stz
           );
  }
}

/*----------------------------------------------------------------------------*/
/* get opeVec                                                                 */
/*----------------------------------------------------------------------------*/
OpVec* Stage::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* Stage::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StagePtnVec* Stage::pStagePtnVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oStagePtnVec;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageNdeVec* Stage::pStageNdeVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oStageNdeVec;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stage)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
