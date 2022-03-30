/*============================================================================*/
/* Stream.cpp -- Stream (instance of StageSpec within a Pipeline)             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "PtnMap.hpp"


#include "OpSpec.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "StageSpec.hpp"
#include "Stage.hpp"
#include "PipeWork.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"
#include "Node.hpp"

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
Stream::Stream(Stage* pStage, StageSpec* pStageSpec,
               i8 i_Stream, PtnId tPtnId, i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
  m_sPipelineNm = pStage->pPipeWork()->pPipeline()->szPipelineNm();
  m_sStageNm    = pStage->szStageNm();

  m_pStageSpec = pStageSpec;
  m_pStage     = pStage;
  m_pPipeline  = pStage->pPipeWork()->pPipeline();

  m_tPtnId     = tPtnId;
  m_iNodeNo    = iNodeNo;
  m_i_Stream   = i_Stream;
  
  vSynthesiseName();

  _checkInitialised_(CN_STREAM);
  
  //vCreateOps();
  
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
    
    delete pOp;
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
  
    Op* pOp_New = new Op(this,pOpSpec);
    vAppend(pOp_New);

    if (m_i_Stream > 0) {
      
      Stream* pStream_0 = m_pStage->pGet(0);
      Op*     pOp_0_0   = pStream_0->pGet(i_OpSpec);
      
      TagSet* pTagSet_Op_0_0 = pOp_0_0->pOpTags();

      TagSet* pTagSet_Op_New = _pTagSet(pTagSet_Op_0_0->pMemoryCopy());
      
      pOp_New->vOpTags(pTagSet_Op_New);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Op* pOp_0 = pOp_New;
    
    pOp_0->vSetTags();

    // form and parse op command string
    
    sSTMT sOpCmd;
    pOp_0->vToCmdStr(sOpCmd);
    
    //vTrace("<><><> %s\n"_Stz,sOpCmd.azChar());

    TokenVec* pTknVec = new TokenVec;
    pTknVec->vTokeniseSen(sOpCmd);

    bool b = AM::bParseCmd(pTknVec,pOp_0->pOpTags());
    //RASSERT(b);
    
    pTknVec->vSetAllTokens();
    delete pTknVec;

  } // for

}

/*----------------------------------------------------------------------------*/
/* get partition this stream needs to run on                                  */
/*----------------------------------------------------------------------------*/
i8 Stream::iNodeNo(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Stream::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
  Op* pOp0 = pGet(0);

  return pOp0;
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
/* spawn tasks for all operators for partitions of given node                 */
/*----------------------------------------------------------------------------*/
void Stream::vSetInvoker(Task* pInvoker)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  // for each operator in the stage ptn
  for (i8 i_Op = nItems() - 1; i_Op >= 0; i_Op--) {
    
    Op* pOp = pGet(i_Op);
    
    if (pOp)
      pOp->vSetInvoker(pInvoker);
  }
  
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
void Stream::vSpawnOpTasks(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  i8 iThisNodeNo = MY::iNodeNo();
  
  i8 i_OnNode = -1;
  
  if (m_pStageSpec->bNodeTopo()) {
    
    if (YG_NEXT_NODE == m_pStageSpec->eStageTopology()) {
      if (iThisNodeNo != pPipeline()->iOriginNodeNo()) {
        i_OnNode = m_iNodeNo;
      }
    }
    else {
      i_OnNode = m_iNodeNo;
    }
    
  }
  else {
    i_OnNode = AM::pPtnMap()->iWhichNodeNo(m_tPtnId);
  }
  
  if (i_OnNode == iThisNodeNo) {
    // for each operator in the stage ptn
    for (i8 i_Op = nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = pGet(i_Op);
      
      pOp->vSpawnTask();
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
/* get stage name for this stream                                             */
/*----------------------------------------------------------------------------*/
Stz* Stream::szStageNm(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  return m_sStageNm;
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
                          m_iNodeNo,
                          m_pStageSpec->szStageNm());
  }
  else {
    m_sStreamNm.vSprint("_streamv_N%lldS%lld_of_%s"_Stz,
                          m_iNodeNo,
                          m_i_Stream,
                          m_pStageSpec->szStageNm());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
