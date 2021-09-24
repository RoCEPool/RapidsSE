/*============================================================================*/
/* Stream.cpp -- Stream (instance of StageSpec within a Pipeline)             */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "PtnMap.hpp"


#include "OpSpec.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "StageSpec.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"

#include "SP_Stream.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stream,CN_STREAM)
__aNewFunction(Stream,CN_STREAM)
__vDeleteFunction(Stream,CN_STREAM)
__aFooterFunction(Stream)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stream::Stream(StageSpec* pStageSpec, Stage* pStage, i8 i_Stream,
               PtnId tPtnId, i8 i_Node)
: LifeGuard_t<Stream>(this,CN_STREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
  m_sPipelineNm = pStage->pPipeline()->szPipelineNm();
  m_sStageNm    = pStage->szStageNm();

  m_pStageSpec = pStageSpec;
  m_pStage     = pStage;
  m_pPipeline  = pStage->pPipeline();
  
  m_tPtnId     = tPtnId;
  m_i_Node     = i_Node;
  m_i_Stream   = i_Stream;
  
  vSynthesiseName();

  _checkInitialised_(CN_STREAM);
  
  vCreateOps();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stream::~Stream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
  i8 nOps = this->nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pGet(i_Op);
    
    pOp->vGuardedDelete(CN_OP,&pOp,
                        CN_STREAM,this,(Stz*)__func__);
  }

  vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the ops for this stream                                             */
/*----------------------------------------------------------------------------*/
void Stream::vCreateOps(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  for (i8 i_OpSpec = 0; i_OpSpec < m_pStageSpec->nItems(); i_OpSpec++ ) {
    
    OpSpec* pOpSpec = m_pStageSpec->pGet(i_OpSpec);
  
    Op* pOp = new Op(pOpSpec,this);
    
    pOp->vSetTags();
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form and parse op command string
    
    sSTMT sOpCmd;
    pOp->vToCmdStr(sOpCmd);
    
    //vTrace("<><><> %s\n"_Stz,sOpCmd.azChar());

    TokenVec* pTknVec = new TokenVec;
    pTknVec->vTokenise(sOpCmd);

    bool b = AM::bParseCmd(pTknVec,pOp->pOpTags());
    RASSERT(b);
    
    pTknVec->vSetAllTokens();
    delete pTknVec;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    

    vAdd(pOp);
  }

}

/*----------------------------------------------------------------------------*/
/* get partition this stream needs to run on                                  */
/*----------------------------------------------------------------------------*/
i8 Stream::i_Node(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_i_Node;
}

/*----------------------------------------------------------------------------*/
/* get pipeline for this stream                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Stream::pPipeline(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* get partition in grid this stream needs to run on                          */
/*----------------------------------------------------------------------------*/
PtnId Stream::tPtnId(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  return m_tPtnId;
}

/*----------------------------------------------------------------------------*/
/* get stage for this stream                                                  */
/*----------------------------------------------------------------------------*/
Stage* Stream::pStage(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_pStage;
}

/*----------------------------------------------------------------------------*/
/* get stage spec for this stream                                             */
/*----------------------------------------------------------------------------*/
StageSpec* Stream::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* spawn tasks for all operators for partitions of given node                 */
/*----------------------------------------------------------------------------*/
void Stream::vSpawnOpTasks(i8 iSessionId, i8 i_ThisNode)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  i8 i_OnNode;
  if (m_pStageSpec->bNodeTopo())
    i_OnNode = m_i_Node;
  else
    i_OnNode = AM::pPtnMap()->iWhichNodeNo(m_tPtnId);
  
  if (i_OnNode == i_ThisNode) {
    // for each operator in the stage ptn
    for (i8 i_Op = nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = pGet(i_Op);
      
      pOp->vSpawnOpTask(iSessionId);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* getm_i_Stream                                                              */
/*----------------------------------------------------------------------------*/
i8 Stream::i_Stream(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_i_Stream;
}

/*----------------------------------------------------------------------------*/
/* get stage spec for this stream                                             */
/*----------------------------------------------------------------------------*/
Stz* Stream::szStreamNm(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_sStreamNm;
}

/*----------------------------------------------------------------------------*/
/* synthesise name of this stream                                             */
/*----------------------------------------------------------------------------*/
void Stream::vSynthesiseName(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
 
  if (m_pStageSpec->bNodeTopo()) {
    m_sStreamNm.vSprint("_stream_N%lld_of_%s"_Stz,
                          m_i_Node,
                          m_pStageSpec->szStageNm());
  }
  else {
    m_sStreamNm.vSprint("_streamv_N%lldS%lld_of_%s"_Stz,
                          m_i_Node,
                          m_i_Stream,
                          m_pStageSpec->szStageNm());
  }
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Stream::vWalk(void* aArg)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  eCN eClassNo = eGetClassNo();
  
  vTrace("Walk %8s:%ll03d %p\n"_Stz
         
         ,aCN[eClassNo]
         ,(i8)eClassNo
         ,this
         );

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  i8 nOps = this->nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pGet(i_Op);
    
    pOp->vWalk(aArg);
  }
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Stream::vWalkLambda(
               i8 iDepth,
               std::function<void(Object* pObject, i8 iDepth)> vfClosure)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  vfClosure(this,iDepth);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  i8 nOps = this->nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pGet(i_Op);
    
    pOp->vWalkLambda(iDepth+1,vfClosure);
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
