/*============================================================================*/
/* Stream.cpp -- Stream (instance of StageSpec within a Pipeline)             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "LglPtnMap.hpp"


#include "OpSpec.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "StageSpec.hpp"
#include "Stage.hpp"
#include "Vertex.hpp"
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
Stream::Stream(Stage* p_Stage, StageSpec* p_StageSpec,
               LglPtn tLglPtn, i8 iNodeId, i8 iPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
//  if (m_i_Stream != m_tLglPtn)
//    vTrace("m_i_Stream %lld ; m_tLglPtn %lld"_Stz,m_i_Stream,m_tLglPtn.iAsI8());

//  vTrace("Stream: L %lld N %lld P %lld\n"_Stz,
//         tLglPtn.iAsI8(),
//         iNodeId,
//         iPtnNo
//         );


  m_p_StageSpec = p_StageSpec;
  m_p_Stage     = p_Stage;

  m_tLglPtn  = tLglPtn;
  m_iNodeId  = iNodeId;
  m_iPtnNo   = iPtnNo;
  
  vSynthesiseName();

//  RASSERT(    (0 == m_i_Stream && -1 == m_tLglPtn.iAsI8())
//           || (m_i_Stream == m_tLglPtn));

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
  
//  i8 nOps = this->nItems();
//  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
//    Op* pOp = pGet(i_Op);
//
//    delete pOp;
//  }
//
//  vClear();
  
  while (Op* pOp = pPop())
    delete pOp;
  vCheckEmpty();
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
  
  for (i8 i_OpSpec = 0; i_OpSpec < m_p_StageSpec->nItems(); i_OpSpec++ ) {
    
    OpSpec* pOpSpec = m_p_StageSpec->pGet(i_OpSpec);
  
    Op* pOp_New = new Op(this,pOpSpec);
    vAppend(pOp_New);

    if (m_iPtnNo > 0) {
      
      Stream* pStream_0 = m_p_Stage->pGet(0);
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
/* get deck of this stream                                                    */
/*----------------------------------------------------------------------------*/
Deck* Stream::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAM,_FOOTER_);
  
  Deck* pDeck = m_p_Stage->pDeck();
  
  return pDeck;
}

/*----------------------------------------------------------------------------*/
/* get partition this stream needs to run on                                  */
/*----------------------------------------------------------------------------*/
i8 Stream::iNodeId(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_iNodeId;
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
  
  return m_p_Stage->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* get partition in grid this stream needs to run on                          */
/*----------------------------------------------------------------------------*/
LglPtn Stream::tLglPtn(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* get stage for this stream                                                  */
/*----------------------------------------------------------------------------*/
Stage* Stream::pStage(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_p_Stage;
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
  
  return m_p_StageSpec;
}

/*----------------------------------------------------------------------------*/
/* spawn tasks for all operators for partitions of given node                 */
/*----------------------------------------------------------------------------*/
void Stream::vSpawnOpTasks(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);

  i8 iThisNodeId = MY::iNodeId();
  
  i8 iOnNodeId = -1;
  
  if (m_p_StageSpec->bNodeTopo()) {
    
    if (YG_NEXT_NODE == m_p_StageSpec->eStageTopology()) {
      if (iThisNodeId != pPipeline()->iOriginNodeId()) {
        iOnNodeId = m_iNodeId;
      }
    }
    else {
      iOnNodeId = m_iNodeId;
    }
    
  }
  else {
    iOnNodeId = AM::pLglPtnMap()->iNodeId(m_tLglPtn);
  }
  
  if (iOnNodeId == iThisNodeId) {
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
i8 Stream::iPtnNo(void)
{
  _precondition_(_MAGIC_,CN_STREAM,_FOOTER_);
  
  return m_iPtnNo;
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
 
  if (m_p_StageSpec->bNodeTopo()) {
    m_sStreamNm.vSprint("_stream_N%lld_of_%s"_Stz,
                          m_iNodeId,
                          m_p_StageSpec->szStageNm());
  }
  else {
    m_sStreamNm.vSprint("_streamv_N%lldS%lld_of_%s"_Stz,
                          m_iNodeId,
                          m_iPtnNo,
                          m_p_StageSpec->szStageNm());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
