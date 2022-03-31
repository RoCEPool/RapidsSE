/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "VertexSpec.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "Optimiser.hpp"
#include "TagConstants.h"
#include "Cue.hpp"

#include "Grid.hpp"
#include "Procedure.hpp"
#include "DeckSpec.hpp"

#include "SP_PipeSpec.hpp"
#include "SP_Token.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dynamo,CN_DYNAMO)
__aNewFunction(Dynamo,CN_DYNAMO)
__vDeleteFunction(Dynamo,CN_DYNAMO)
__aFooterFunction(Dynamo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(TagSet* p_JobTags)
: ListNL_t<VertexSpec>(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_p_JobTags      = p_JobTags;
  
  m_szPipelineStmt = nullptr;
  m_p_VertexSpecRoot = nullptr;
  m_bWasOptimised  = false;
  //m_sOptPipelineStmt
  
  m_p_Scanner = nullptr;
  m_pCurrVertexSpec = nullptr;
  
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  //delete m_pScanner;
  
  while (VertexSpec *pSupPipeSpec = pPop())
    delete pSupPipeSpec;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair a dynamo                                                            */
/*----------------------------------------------------------------------------*/
void Dynamo::vRepair(Object* pObject)
{
  _static_

  Dynamo* pDynamo = _pDynamo(pObject);

  delete pDynamo;
}

/*----------------------------------------------------------------------------*/
/* convert pipeline statement to pipeline spec                                */
/*----------------------------------------------------------------------------*/
VertexSpec* Dynamo::pToPipeSpec(Stz* szPipelineStmt, TagSet* p_JobTags)
{
  _static_
  
  Dynamo* pDynamo = new Dynamo(p_JobTags);
  
  VertexSpec* pPipeSpec = pDynamo->pCompile(szPipelineStmt);

  delete pDynamo;
  
  return pPipeSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recursively compile the tokenised pipeline statement                       */
/*----------------------------------------------------------------------------*/
VertexSpec* Dynamo::pCompile(Stz* szPipelineStmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
        
  m_szPipelineStmt = szPipelineStmt;
  
  Scanner* pScanner = new Scanner(m_szPipelineStmt);
  
  if (! (pScanner->pTkn()->bSame("PIPELINE"_Stz) ||
         pScanner->pTkn()->bSame("PIPELINELOCAL"_Stz))) {
    delete pScanner;
    return nullptr;
  }
  

  m_p_VertexSpecRoot = new VertexSpec(nullptr,pScanner,m_p_JobTags);

  
  if (m_p_VertexSpecRoot->bCompile()) {
    
    //vOptimise();
 
    delete pScanner;
    
    return m_p_VertexSpecRoot;
  }
  
  delete m_p_VertexSpecRoot;
  
  ASSERT(! m_p_PipeSpec);
  
  delete pScanner;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* optimise the pipeline statement                                            */
/*----------------------------------------------------------------------------*/
void Dynamo::vOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Job* pJob = MY::pJob();
      
  if (! bWillOptimise())
    return;
  
  vTrace("vOptimise: \n%s\n"_Stz,m_szPipelineStmt);
  Cue::vGetReturn("optimise >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Optimiser* pOptimiser = new Optimiser(m_p_JobTags);
  
  pOptimiser->vDissect(m_p_VertexSpecRoot);
                                        
  pOptimiser->vOptimise();

  Stz* szWeavedStmt = pOptimiser->szWeave();
  vTrace("\nWeave:\n%s\n"_Stz, szWeavedStmt);
  
  Cue::vGetReturn("weave >> "_Stz);

  pJob->vCmd(szWeavedStmt);
  
  m_bWasOptimised = true;
   
  m_p_JobTags->vDumpSet(true);

  delete pOptimiser;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
VertexSpec* Dynamo::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_VertexSpecRoot;
}

/*----------------------------------------------------------------------------*/
/* was the statement optimised                                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bWasOptimised(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_bWasOptimised;
}

/*----------------------------------------------------------------------------*/
/* will the pipeline statement get optimised ?                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bWillOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Job* pJob = MY::pJob();
  
  Stz* szPipelineNm = m_p_VertexSpecRoot->szVertexNm();
  
  i8 iOptimiserLvl = AM::SYSARGS::iOptimiserLvl();

  if (   ! iOptimiserLvl
      || ! pJob->bIsOriginator()
      ||   szPipelineNm->bSame("sqlCompiler"_Stz)
      || ! m_p_VertexSpecRoot->pJobTags()->bHasName(C_TAG_OPTIMISE)
      ||   m_p_VertexSpecRoot->pJobTags()->bHasName(C_TAG_OPTIMISED)
      )
    return false;
  else
    return true;
}


#if 0

/*----------------------------------------------------------------------------*/
/*                           Pass 1 : Collect                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 1: collect the pipeline statement                                     */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass1_collect(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! bCollectPipeline())
    return false;

  if (! (pTkn()->bSame("TAG"_Stz) ||
        pTkn()->bSame("BEGIN"_Stz)))
    return false;

  while (pTkn()->bSame("TAG"_Stz))
    if (! bCollectTag())
      return false;

  if (! bCollectBeginEnd())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect pipeline <name>                                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectPipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! (pTkn()->bSame("PIPELINE"_Stz) ||
         pTkn()->bSame("PIPELINELOCAL"_Stz)))
    return false;

  vTokenAccept();
  
  m_sVertexNm = pTkn()->szChars();

  vTokenAccept();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one tag                                                            */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTag(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! pTkn()->bSame("TAG"_Stz))
    return false;
  vTokenAccept();

  s32 sTagNm = pTkn()->szChars();
  vTokenAccept();

  if (! pTkn()->bSame("="_Stz))
    return false;
  vTokenAccept();

  sSTMT sTagValue = pTkn()->szChars();
  vTokenAccept();

  while (! pTkn()->bSame("ENDTAG"_Stz)) {
    vTokenAccept();
  }
  vTokenAccept();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_p_JobTags->vInsert(sTagNm, sTagValue);

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect a BEGIN { STAGE ... } END sequence                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectBeginEnd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! pTkn()->bSame("BEGIN"_Stz))
    return false;
  vTokenAccept();

  while (pTkn()->bSame("PIPELINE"_Stz))
    if (! bCollectSubPipeline())
      return false;

  while (pTkn()->bSame("STAGE"_Stz))
    if (! bCollectStage())
      return false;

  if (! pTkn()->bSame("END"_Stz))
    return false;
  vTokenAccept();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect sub pipeline                                                       */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectSubPipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  VertexSpec* pParentVertexSpec = m_pCurrVertexSpec;
  vPush(pParentVertexSpec);
  
  m_pCurrVertexSpec = new VertexSpec(pParentVertexSpec,m_p_Scanner,m_p_JobTags);

  if (! m_pCurrVertexSpec->bCompile()) {
    delete m_pCurrVertexSpec;
    
    return false;
  }
  
  pParentVertexSpec->vAppend(m_pCurrVertexSpec);
  m_pCurrVertexSpec = pPop();

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect stage                                                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! pTkn()->bSame("STAGE"_Stz))
    return false;
  vTokenAccept();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz *szStageNm = pTkn()->szChars();
  StageSpec *p_StageSpec = new StageSpec(m_pCurrVertexSpec,
                                         m_pCurrVertexSpec->pDeckSpec(),
                                         szStageNm);
  vTokenAccept();

  m_pCurrVertexSpec->vAddStageSpec(p_StageSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! bCollectTopology(p_StageSpec))
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (bTokenMaybeOp(pTkn()))
    bCollectOpSequence(p_StageSpec);

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect topology                                                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectTopology(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (! pTkn()->bSame("ON"_Stz))
    return false;
  vTokenAccept();

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes | partitions :

  if (pTkn()->bSame("ALL"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAll(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on next node :

  else if (pTkn()->bSame("NEXT"_Stz)) {
    vTokenAccept();
    return bRecogniseOnNextNode(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on origin :

  else if (pTkn()->bSame("ORIGIN"_Stz)) {
    vTokenAccept();
    return bRecogniseOnOrigin(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --

  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* collect operator-pipe sequence                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOpSequence(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  while (bTokenMaybeOp(pTkn())) {

    if (! bCollectOp(p_StageSpec))
      return false;

    if (! pTkn()->bSame("|"_Stz))
      break;
    vTokenAccept();
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect one operator (and optionally its arguments)                        */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOp(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  OpSpec *pOpSpec = new OpSpec(p_StageSpec, pTkn()->szChars());
  vTokenAccept();

  p_StageSpec->vAddOpSpec(pOpSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pTkn()->bSame("BROADCAST"_Stz)) {
    pOpSpec->vSendCast(XK_BROADCAST);
    vTokenAccept();
  }
  else if (pTkn()->bSame("ROBIN"_Stz)) {
    pOpSpec->vSendCast(XK_ROBINDIST);
    vTokenAccept();
  }
  else if (pTkn()->bSame("LOCALROBIN"_Stz)) {
    pOpSpec->vSendCast(XK_ROBINLOCAL);
    vTokenAccept();
  }
  else if (pTkn()->bSame("HASHCAST"_Stz)) {
    pOpSpec->vSendCast(XK_HASHCAST);
    vTokenAccept();
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! bCollectOpArgs(p_StageSpec, pOpSpec))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator arguments                                                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOpArgs(StageSpec *p_StageSpec, OpSpec *pOpSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  UNUSED(p_StageSpec);

  s32 sArgName;
  sSTMT sArgValue;
  
  for (i8 i_Arg = 0; /*none*/; i_Arg++) {

    if (! bTokenMaybeOpArg(pTkn()))
      break;
    
    if (pTkn()->bSame("expr"_Stz)) {
      vTokenAccept();
      sArgValue = ""_Stz;
      IGNORE_RETURN(bCollectOpArgsExpr(p_StageSpec,pOpSpec,sArgValue));
    }
    else {
      sArgValue = pTkn()->szChars();
      vTokenAccept();
    }
    
    sArgName.vSprint("<arg_%lld>"_Stz, i_Arg);
    pOpSpec->vArgTagAdd(sArgName, sArgValue, TN_STRING);

  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* collect operator argument expr tail as string                              */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCollectOpArgsExpr(StageSpec *p_StageSpec,
                                      OpSpec *pOpSpec,
                                      String* pArgVal)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  UNUSED(p_StageSpec);

  SP_Token spToken;
  
  i8 nTkns = 0;
  while (bTokenMaybeOpArg(pTkn())) {
    if (nTkns++)
      pArgVal->vConcat(" "_Stz);
    
    Token* pToken = pTkn();
    
//    vTrace("%s\n"_Stz,spToken.sz(pToken));
//    if (pToken->bIsString()) {
//      pArgVal->vConcat("'"_Stz);
//      pArgVal->vConcat(pTkn()->szChars());
//      pArgVal->vConcat("'"_Stz);
//    }
//    else {
      pArgVal->vConcat(pToken->szChars());
//    }
    vTokenAccept();
  }

//  vTrace("expr: %s\n"_Stz,pArgVal->szStz());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL ^_ [ NODES | PARTITIONS ]                 */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnAll(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :

  if (pTkn()->bSame("NODES"_Stz)) {
    vTokenAccept();
    return bRecogniseOnAllNodes(p_StageSpec);
  }

  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :

  else if (pTkn()->bSame("PARTITIONS"_Stz)) {
    vTokenAccept();
    p_StageSpec->vParcelForward(YF_ALL_PARTITIONS);
    return bRecogniseOnAllPartitions(p_StageSpec);
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL NODE ^_                                   */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnAllNodes(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all nodes :

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();

    p_StageSpec->vStageTopology(YG_ALL_NODES);
    p_StageSpec->vParcelForward(YF_ALL_NODES);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ALL PARTITIONS ^_                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnAllPartitions(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // on all partitions :

  if (pTkn()->bSame("ON"_Stz)) {
    vTokenAccept();
    if (pTkn()->bSame("ORIGIN"_Stz)) {

      vTokenAccept();
      if (pTkn()->bSame(":"_Stz)) {
        vTokenAccept();

        p_StageSpec->vStageTopology(YG_ALL_PARTITIONS_ON_ORIGIN);
        p_StageSpec->vParcelForward(YF_ALL_PARTITIONS_ON_ORIGIN);
        return true;
      }
    }
    else {
      return false;
    }
  }

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();
    p_StageSpec->vStageTopology(YG_ALL_PARTITIONS);
    p_StageSpec->vParcelForward(YF_ALL_PARTITIONS);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON NEXT NODE ^_                                  */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnNextNode(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);
  
  if (! pTkn()->bSame("NODE"_Stz))
    return false;

  vTokenAccept();

  if (! pTkn()->bSame(":"_Stz))
    return false;

  vTokenAccept();

  p_StageSpec->vStageTopology(YG_NEXT_NODE);
  p_StageSpec->vParcelForward(YF_NEXT_NODE);

  return true;
}

/*----------------------------------------------------------------------------*/
/* recognise stage topology: ON ORIGIN ^_                                     */
/*----------------------------------------------------------------------------*/
bool Dynamo::bRecogniseOnOrigin(StageSpec *p_StageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (pTkn()->bSame(":"_Stz)) {
    vTokenAccept();

    p_StageSpec->vStageTopology(YG_ORIGIN);
    p_StageSpec->vParcelForward(YF_ORIGIN);
    return true;
  }
  else {
    s32 sLocus(pTkn()->szChars());
    vTokenAccept();
  }

  return false;
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is token a potential operator?                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bTokenMaybeOp(Token *pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is token a potential operator argument?                                    */
/*----------------------------------------------------------------------------*/
bool Dynamo::bTokenMaybeOpArg(Token *pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  if (pTkn->bSame("|"_Stz)
      || pTkn->bSame("END"_Stz)
      || pTkn->bSame("PIPELINE"_Stz)
      || pTkn->bSame("STAGE"_Stz))
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                           Pass 2 : Piping                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 2 staging                                                             */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass2_piping(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // foreach stageSpec of pipeSpec
  i8 nStageSpecs = m_pCurrVertexSpec->nStageSpecs();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec *pStageSpec = m_pCurrVertexSpec->pDeckSpec()->pGet(i_Stage);
    RASSERT(pStageSpec->i_StageSpec() == i_Stage);

    // foreach opSpec of stageSpec
    i8 nOpSpecs = pStageSpec->nItems();
    for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
      OpSpec *pOpSpec = pStageSpec->pGet(i_Op);
      RASSERT(pOpSpec->iOpSpec() == i_Op);

      Stz *szName = pOpSpec->szOpNm();
      StageSpec *pDstStageSpec = nullptr;

      // can we find a stage matching the name in the OpSepc?

      if (szName->bSame("next"_Stz)) {
        pDstStageSpec = m_pCurrVertexSpec->pDeckSpec()->pGet(i_Stage + 1);
        if (! pDstStageSpec)
          return false;
      }
      else if (szName->bSame("first"_Stz)) {
        pDstStageSpec = m_pCurrVertexSpec->pDeckSpec()->pGet(0);
        if (! pDstStageSpec)
          return false;
      }
      else if (szName->bSame("last"_Stz)) {
        pDstStageSpec = m_pCurrVertexSpec->pDeckSpec()->pGet(nOpSpecs - 1);
        if (! pDstStageSpec)
          return false;
      }
      else {
        pDstStageSpec = m_pCurrVertexSpec->pFindStageSpec(szName);
      }

      // what did we find?

      if (pDstStageSpec) {
        // name in the opSpec is a stage
        vNameIsStage(pStageSpec, pOpSpec, pDstStageSpec);
      } else {
        // assume the name in the opSpec is an op
        vNameIsOp(pStageSpec, pOpSpec);
      }

    } // foreach op

  } // foreach stage

  return true;
}

/*----------------------------------------------------------------------------*/
/* name is a known stage                                                      */
/*----------------------------------------------------------------------------*/
void Dynamo::vNameIsStage(StageSpec *pSrcStageSpec,
                                OpSpec    *pSrcOpSpec_nameOfStage,
                                StageSpec *pDstStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  // is there a send already? This would be the case if one stage is
  // piped into multiple stage e.g a unionAll
  OpSpec *pOpSpecSend = pSrcStageSpec->pFindOpSpec("sendSE"_Stz);

  if (! pOpSpecSend)

    // spec for op send does not yet exist in stage spec
    vFirstSend(pSrcStageSpec, pSrcOpSpec_nameOfStage, pDstStageSpec);

  else

    // spec for op send already exists in stage spec
    vOtherSend(pSrcStageSpec, pSrcOpSpec_nameOfStage,
               pDstStageSpec, pOpSpecSend);
}

/*----------------------------------------------------------------------------*/
/* name is assumed to be an operator                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vNameIsOp(StageSpec *pStageSpec,
                             OpSpec *pOpSpec_nameOfOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  i8 i_Op = pOpSpec_nameOfOp->iOpSpec();
  i8 i_Stage = pStageSpec->i_StageSpec();

  eYS eStreamTopology
        = StageSpec::eStreamTopoCalc(pStageSpec->eStageTopology());

  pOpSpec_nameOfOp->vStreamTopology_op(eStreamTopology);
  pOpSpec_nameOfOp->vNextStageOp(i_Stage, i_Op + 1);
}


/*----------------------------------------------------------------------------*/
/* first send to stage                                                        */
/*----------------------------------------------------------------------------*/
void Dynamo::vFirstSend(StageSpec *pSrcStageSpec,
                              OpSpec    *pSrcOpSpec_nameOfStage,
                              StageSpec *pDstStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);
  
  i8 i_Op = pSrcOpSpec_nameOfStage->iOpSpec();

  i8 i_NextStage = pDstStageSpec->i_StageSpec();

  eYS eStreamTopology;
  eYF eParcelForward;

  eYX eCrossTopology =
    StageSpec::eCrossTopoCalc(pSrcStageSpec, pDstStageSpec,
                              eParcelForward, eStreamTopology);

  pSrcOpSpec_nameOfStage->vParcelForward(eParcelForward);
  
  // SPECIAL CASE (to discuss with Alka)
  if (eCrossTopology == YX_ORIGIN_TO_ORIGIN && i_NextStage == 0) {
    // add parameter to the prev sink op
    OpSpec *pPrevOpSpec = pSrcStageSpec->pGet(i_Op - 1);
    Stz *szStgNm = szStageNm(i_NextStage);
    pPrevOpSpec->vStreamTopology_op(eStreamTopology);
    pPrevOpSpec->vCrossTopology(eCrossTopology, szStgNm, i_NextStage, 0);
    pPrevOpSpec->vParcelForward(eParcelForward);

    RASSERT(pSrcStageSpec == pPrevOpSpec->pStageSpec());

    // remove the last (stage) OpSpec from the OpSpecVec
    RASSERT(i_Op == pSrcStageSpec->nItems() - 1);
    pSrcStageSpec->vRemove(pSrcOpSpec_nameOfStage);
    delete pSrcOpSpec_nameOfStage;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // change the op to a send
  vMorphToSend(pSrcOpSpec_nameOfStage, eCrossTopology, eStreamTopology, i_NextStage);

  // send op has <castkind> argument
  pSrcOpSpec_nameOfStage->vArgTagAdd(C_TAG_SENDKIND,
                              (i8) pSrcOpSpec_nameOfStage->eSendCast(), TN_INTEGER);

  // is there a consumeOp already in the destination stage ?
  OpSpec *pConsumeOpSpec = pDstStageSpec->pFindOpSpec("consumeOp"_Stz);
  if (pConsumeOpSpec) {

    // found a consume op
    // implies that there are 2 stages piping into this same stage
    // eg in unionAll
    // 2 scan stages piping into the output stage
    // add to the consume params
    vArgUpdate_sends(pConsumeOpSpec, eCrossTopology);
  }
  else {
    vAddConsumeOp(pDstStageSpec, eCrossTopology);
  }

}

/*----------------------------------------------------------------------------*/
/* morph the stageNm of the opSpec to send <sendKind>                         */
/*----------------------------------------------------------------------------*/
void Dynamo::vMorphToSend(OpSpec *pOpSpec,
                                eYX eCrossTopology,
                                eYS eStreamTopology,
                                i8 i_NextStage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  pOpSpec->vOpNm("sendSE"_Stz);

  Stz *szStgNm = szStageNm(i_NextStage);
  pOpSpec->vStreamTopology_op(eStreamTopology);
  pOpSpec->vCrossTopology(eCrossTopology, szStgNm, i_NextStage, 0);
}

/*----------------------------------------------------------------------------*/
/* second or subsequent send to stage                                         */
/*----------------------------------------------------------------------------*/
void Dynamo::vOtherSend(StageSpec *pSrcStageSpec,
                              OpSpec    *pSrcOpSpec,
                              StageSpec *pOtherStageSpec,
                              OpSpec    *pOtherOpSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  i8 i_OtherStage = pOtherStageSpec->i_StageSpec();

  eYS eIntraTopology;
  eYF eParcelForward;

  eYX eCrossTopology =
    StageSpec::eCrossTopoCalc(pSrcStageSpec, pOtherStageSpec,
                              eParcelForward, eIntraTopology);

  pSrcOpSpec->vParcelForward(eParcelForward);
  
  // we already have a send, so just remove the stage
  pSrcStageSpec->vRemove(pSrcOpSpec);
  delete pSrcOpSpec;

  Stz *szStgNm = szStageNm(i_OtherStage);

  pOtherOpSpec->vOtherStageOp(szStgNm, i_OtherStage, 0);

  // insert a consume op at the beginning of the stage
  vAddConsumeOp(pOtherStageSpec, eCrossTopology);
}


/*----------------------------------------------------------------------------*/
/* add <sends> arg to consume op                                              */
/*----------------------------------------------------------------------------*/
void Dynamo::vArgAdd_sends(OpSpec *pConsumeOpSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  i8 iSends = C_UNKNOWN;

  switch (eCrossTopology) {
      
    case YX_ALL_NODES_TO_ORIGIN:
      iSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_NODES_TO_ALL_PARTITIONS:
      iSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES:
    case YX_ALL_PARTITIONS_TO_ALL_NODES:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case YX_ALL_PARTITIONS_TO_ORIGIN:
      iSends = AM::PTNMAP::nPtnsInGrid();
      break;

    case YX_NEXT_NODE_TO_ORIGIN:
    case YX_ORIGIN_TO_ORIGIN:
    case YX_ORIGIN_TO_ALL_PARTITIONS:
    case YX_ORIGIN_TO_ALL_NODES:
    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case YX_ORIGIN_TO_NEXT_NODE:
      iSends = 1;
      break;

    default:
      RASSERT(0);
  }

  pConsumeOpSpec->vArgTagAdd(C_TAG_SENDS, iSends, TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* update the <sends> arg of consume op                                       */
/*----------------------------------------------------------------------------*/
void Dynamo::vArgUpdate_sends(OpSpec *pConsumeOpSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  i8 iIncSends = C_UNKNOWN;

  switch (eCrossTopology) {
      
    case YX_ALL_NODES_TO_ORIGIN:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_NODES_TO_ALL_PARTITIONS:
      iIncSends = AM::pGrid()->nKnownNodes();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_NODES:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      iIncSends = AM::PTNMAP::nPtnsPerNode();
      break;

    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
    case YX_ALL_PARTITIONS_TO_ORIGIN:
      iIncSends = AM::PTNMAP::nPtnsInGrid();
      break;

    case YX_ORIGIN_TO_ORIGIN:
    case YX_ORIGIN_TO_ALL_PARTITIONS:
    case YX_ORIGIN_TO_ALL_NODES:
    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
      iIncSends = 1;
      break;

    default:
      RASSERT(0);
  }

  TagSet *pOpArgTags = pConsumeOpSpec->pArgTags();
  i8 iOldSends = pOpArgTags->iValue(C_TAG_SENDS);

  pOpArgTags->vReplace(C_TAG_SENDS, iOldSends + iIncSends);
}

/*----------------------------------------------------------------------------*/
/* add a consumeOp at the beginning of given stage spec                       */
/*----------------------------------------------------------------------------*/
void Dynamo::vAddConsumeOp(StageSpec *pStageSpec, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  OpSpec *pConsumeOpSpec = new OpSpec(pStageSpec,"consumeOp"_Stz);

  pStageSpec->vAdd(pConsumeOpSpec);


  vArgAdd_sends(pConsumeOpSpec, eCrossTopology);
}


/*----------------------------------------------------------------------------*/
/* get name of given stage                                                    */
/*----------------------------------------------------------------------------*/
Stz *Dynamo::szStageNm(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  StageSpec *pStageSpec = m_pCurrVertexSpec->pDeckSpec()->pGet(i_Stage);
  Stz *szNm = pStageSpec->szStageNm();

  return szNm;
}

/*----------------------------------------------------------------------------*/
/*                           Pass 3 : Includes                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pass 3: include other pipelines                                            */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass3_includes(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  Stz *szIncludePipNm;

  TokenVec *pIncludeTknVec = nullptr;

  if (m_p_JobTags->bHasName("<include_0>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_0>"_Stz);
    if (! m_sVertexNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 0: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }

  if (m_p_JobTags->bHasName("<include_1>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_1>"_Stz);
    if (! m_sVertexNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 1: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }

  if (m_p_JobTags->bHasName("<include_2>"_Stz)) {
    szIncludePipNm = m_p_JobTags->szValue("<include_2>"_Stz);
    if (! m_sVertexNm.bSame(szIncludePipNm)) {
      vTrace("Has Include 2: %lls\n"_Stz, szIncludePipNm);
      bPass3_include(szIncludePipNm);
    }
  }


  return true;
}

/*----------------------------------------------------------------------------*/
/* include stage(s) from other pipeline                                       */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass3_include(Stz *szIncludePipNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  sSTMT sPipelineSmt;

  TokenVec *pIncludeTknVec = nullptr;
  Word *pwMatchedProc = nullptr;

  pIncludeTknVec = TokenVec::pTokeniseSen(szIncludePipNm);

  bool b = Procedure::bTranslateToPipeline(sPipelineSmt.pString(),
                                           pIncludeTknVec,
                                           &pwMatchedProc);

  vTrace("Including: %lls\n"_Stz, sPipelineSmt.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet *pWrdTagSet = pwMatchedProc->pWrdTagSet();
  pWrdTagSet->vDumpSet();

  Dynamo *pDynamo = new Dynamo(pWrdTagSet);

  VertexSpec *pIncludePipeSpec = pDynamo->pCompile(sPipelineSmt.szStz());

  delete pDynamo;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pWrdTagSet->bHasName("<as_first>"_Stz)) {
    bool b = bPass3_includeFirst();
  }
  else if (pWrdTagSet->bHasName("<as_last>"_Stz)) {
    bool b = bPass3_includeLast();
  }
  else {
    NEVER_REACH;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pIncludeTknVec;

  return true;
}

/*----------------------------------------------------------------------------*/
/* include a stage from another pipeline as a first stage                     */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass3_includeFirst(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  return true;
}

/*----------------------------------------------------------------------------*/
/* include a stage from another pipeline as a last stage                      */
/*----------------------------------------------------------------------------*/
bool Dynamo::bPass3_includeLast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  return true;
}


/*----------------------------------------------------------------------------*/
/*                            Token Scanning                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
Token *Dynamo::pTkn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  return m_p_Scanner->pTkn();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
TokenVec *Dynamo::pTknVec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  return m_p_Scanner->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void Dynamo::vTokenAccept(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DYNAMO, _FOOTER_);

  m_p_Scanner->vTokenAccept();
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
