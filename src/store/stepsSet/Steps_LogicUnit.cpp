/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "MY.hpp"
#include "TagConstants.h"


extern EnvFlags gxEnvFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("LogicUnit[].altIndex@"_Stz,     &Step::vLogicUnitAltIndexFetch);
  vAddStep("LogicUnit[].altTable@"_Stz,     &Step::vLogicUnitAltTableFetch);

  vAddStep("LogicUnit[].callAgain@"_Stz,    &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz,    &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].calledAgain@"_Stz,  &Step::vLogicUnitCalledAgainFetch);
  vAddStep("LogicUnit[].calledAgain!"_Stz,  &Step::vLogicUnitCalledAgainStore);
  vAddStep("LogicUnit[].cleanup"_Stz,       &Step::vLogicUnitCleanup);
  vAddStep("LogicUnit[].compilerToExit"_Stz,&Step::vLogicUnitCompilerToExit);

  vAddStep("LogicUnit[].diaryAction"_Stz,   &Step::vLogicUnitDiaryAction);
  vAddStep("LogicUnit[].diaryDir"_Stz,      &Step::vLogicUnitDiaryDir);
  vAddStep("LogicUnit[].diary@"_Stz,        &Step::vLogicUnitDiaryFetch);
  vAddStep("LogicUnit[].diaryJobNote"_Stz,  &Step::vLogicUnitDiaryJobNote);
  vAddStep("LogicUnit[].diaryName"_Stz,     &Step::vLogicUnitDiaryName);
  vAddStep("LogicUnit[].diaryNote"_Stz,     &Step::vLogicUnitDiaryNote);
  vAddStep("LogicUnit[].diaryTitle"_Stz,    &Step::vLogicUnitDiaryTitle);
  vAddStep("LogicUnit[].dynFun"_Stz,        &Step::vLogicUnitDynFun);
  vAddStep("LogicUnit[].dynFunArgs"_Stz,    &Step::vLogicUnitDynFunArgs);
  
  vAddStep("LogicUnit[].genAggregate"_Stz,  &Step::vLogicUnitGenAggregate);
  vAddStep("LogicUnit[].genAsm"_Stz,        &Step::vLogicUnitGenAsm);
  vAddStep("LogicUnit[].genFilter"_Stz,     &Step::vLogicUnitGenFilter);
  vAddStep("LogicUnit[].genFilterProject"_Stz,&Step::vLogicUnitGenFilterProject);
  vAddStep("LogicUnit[].genHash"_Stz,       &Step::vLogicUnitGenHash);
  vAddStep("LogicUnit[].genIndexJoin"_Stz,  &Step::vLogicUnitGenIndexJoin);
  vAddStep("LogicUnit[].genIndexScan"_Stz,  &Step::vLogicUnitGenIndexScan);
  vAddStep("LogicUnit[].genLimit"_Stz,      &Step::vLogicUnitGenLimit);
  vAddStep("LogicUnit[].genLoopJoin"_Stz,   &Step::vLogicUnitGenLoopJoin);
  vAddStep("LogicUnit[].genProjection"_Stz, &Step::vLogicUnitGenProjection);
    
  vAddStep("LogicUnit[].handler@"_Stz,      &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,      &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].init"_Stz,          &Step::vLogicUnitInit);
  vAddStep("LogicUnit[].IsLftCrwBuffer@"_Stz,  &Step::vLogicUnitIsLftCrwBufferFetch);
  vAddStep("LogicUnit[].IsLftCrwBuffer!"_Stz,  &Step::vLogicUnitIsLftCrwBufferStore);
  vAddStep("LogicUnit[].intStack@"_Stz,     &Step::vLogicUnitIntStackFetch);
  vAddStep("LogicUnit[].intStack!"_Stz,     &Step::vLogicUnitIntStackStore);

  vAddStep("LogicUnit[].jobMetrics@"_Stz,   &Step::vLogicUnitJobMetricsFetch);
  vAddStep("LogicUnit[].jobMetrics!"_Stz,   &Step::vLogicUnitJobMetricsStore);

  vAddStep("LogicUnit[].srcIndex@"_Stz,     &Step::vLogicUnitSrcIndexFetch);
  vAddStep("LogicUnit[].srcTable@"_Stz,     &Step::vLogicUnitSrcTableFetch);
  
  vAddStep("LogicUnit[].transform"_Stz,     &Step::vLogicUnitTransform);
  vAddStep("LogicUnit[].transformArgs"_Stz, &Step::vLogicUnitTransformArgs);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].altIndex@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitAltIndexFetch(void)
{
  ENTRY();
  
  DevIXT* pDevIxt = _pDevIXT(m_p_TaskLogicUnit->m_xRluCommon.m_pDevAltIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].altTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitAltTableFetch(void)
{
  ENTRY();
  
  DevROW* pDevRow = _pDevROW(m_p_TaskLogicUnit->m_xRluCommon.m_pDevAltRow);
  
  m_pDataStack->vPushStr(pDevRow->m_sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xRluCommon.m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopIntBol(__func__);
  
  m_p_TaskLogicUnit->m_xRluCommon.m_bCallAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain@ --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xRluCommon.m_bCalledAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain! --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopIntBol(__func__);
  
  m_p_TaskLogicUnit->m_xRluCommon.m_bCalledAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].cleanup --                                                     */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCleanup(void)
{
  ENTRY();

  m_p_TaskLogicUnit->vCleanup();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].compilerToExit --                                              */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCompilerToExit(void)
{
  ENTRY();

  bool bCompilerToExit = m_p_TaskLogicUnit->m_bCompilerToExit;

  m_pDataStack->vPushInt(bCompilerToExit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryAction -- add a longer diary action note                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryAction(void)
{
  ENTRY();
 
  i8 i_EnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_Enum      = m_pDataStack->iPopInt(__func__);
  i8 iItem       = m_pDataStack->iPopInt(__func__);

  i8 i_Action       = m_pDataStack->iPopInt(__func__);
  i8 i_ActEnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_ActEnum      = m_pDataStack->iPopInt(__func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_p_TaskLogicUnit->m_oSince.iNanoseconds();
  
  i8 nQueueLength = m_p_TaskLogicUnit->m_xpOp->nQueuedOperands();

  m_p_TaskLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           i_Action,i_ActEnumGroup,i_ActEnum,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diary@ -- diary fetch                                          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_p_TaskLogicUnit->m_pRluDiary);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryJobNote -- set diary job note                             */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryJobNote(void)
{
  ENTRY();
  
  s256 sJobNote;
  m_pDataStack->vPopStr(sJobNote, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->vDiaryJobNote(sJobNote);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryTitle -- set diary title                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryName(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->vDiaryName(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryNote -- add a diary note                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryNote(void)
{
  ENTRY();
  
  i8 i_EnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_Enum      = m_pDataStack->iPopInt(__func__);
  i8 iItem       = m_pDataStack->iPopInt(__func__);
  
  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_p_TaskLogicUnit->m_oSince.iNanoseconds();
  
  
  i8 nQueueLength = m_p_TaskLogicUnit->m_xpOp->nQueuedOperands();
  
  m_p_TaskLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           0,0,0,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryDir -- set diary dir                                      */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryDir(void)
{
  ENTRY();
  
  s256 sDir;
  m_pDataStack->vPopStr(sDir, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->vDiaryDir(sDir);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryTitle -- set diary title                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryTitle(void)
{
  ENTRY();
  
  s256 sTitle;
  m_pDataStack->vPopStr(sTitle, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_p_TaskLogicUnit->vDiaryTitle(sTitle);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFun(void)
{
  ENTRY();
  
  i8 iRes = m_p_TaskLogicUnit->iDynFun(0,C_UNKNOWN);
  UNUSED(iRes);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFunArgs(void)
{
  ENTRY();
  
  i8 nArgs = m_pDataStack->iPopInt(__func__);

  if (nArgs) {
    RASSERT(1 == nArgs);
  }
  i8 iArg0 = m_pDataStack->iPopInt(__func__);

  i8 iRes = m_p_TaskLogicUnit->iDynFun(nArgs,iArg0);
  UNUSED(iRes);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genAggregate -- generate aggregation asm text into file        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenAggregate(void)
{
  ENTRY();

  ExprEval* pExprEval;
  pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError;

  if (pExprEval->bHasGroupBy())
    eError = m_p_TaskLogicUnit->eGenerateGroupBy(pExprEval);
  else {
    eError = EC_INVALID_COMMAND;
  }

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genAsm -- generate asm code ffrom given file                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenAsm(void)
{
  ENTRY();
  
  s256 sFileName;
  m_pDataStack->vPopStr(sFileName, __func__);
  
  LocalFile oFile;
  
  bool b = oFile.bOpenRead(sFileName.szStz());
  RASSERT(b);
  
  s16KB sText;
  //MemLib::vZero(sText.azChar(),C_16KB,CN_STRING_T);

  i8 nSize = oFile.iSize();
  i8 nBytes = oFile.nReadBytes((void*)sText.azChar(), nSize);
  UNUSED(nBytes);

  //vTrace("FILE %s\n"_Stz, sFileName.azChar());

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = m_p_TaskLogicUnit->bGenerateAsm(sText);
  
  m_pDataStack->vPushInt(bResult);
  
  oFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genFilter -- generate filter asm text into file                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenFilter(void)
{
  ENTRY();

  ExprEval* pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateFilter(pExprEval);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genFilterProject -- generate filter & project asm text - file  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenFilterProject(void)
{
  ENTRY();

  ExprEval* pFilter = _pExprEval(m_pDataStack->pPopPtr(__func__));
  ExprEval* pProject = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateFilterProject(pFilter, pProject);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genHash -- generate hash asm text into file                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenHash(void)
{
  ENTRY();

  ExprEval* pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateHash(pExprEval);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genIndexJoin -- generate indexJoin asm text into file          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenIndexJoin(void)
{
  ENTRY();

  ExprEval* pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateIndexJoin(pExprEval);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genLimit -- generate limit asm text into file                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenLimit(void)
{
  ENTRY();

  i8 iLimit = m_pDataStack->iPopInt(__func__);

  eEC eError = m_p_TaskLogicUnit->eGenerateLimit(iLimit);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genLoopJoin -- generate loopJoin asm text into file            */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenLoopJoin(void)
{
  ENTRY();

  ExprEval* pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateLoopJoin(pExprEval);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genProjection -- generate projection asm text into file        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenProjection(void)
{
  ENTRY();

  ExprEval* pExprEval;
  pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError;

  if (pExprEval->bHasAggregate())
    eError = m_p_TaskLogicUnit->eGenerateAggregate(pExprEval);
  else {
    eError = m_p_TaskLogicUnit->eGenerateProjection(pExprEval);
  }

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genIndexScan -- generate indexScan asm text into file          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenIndexScan(void)
{
  ENTRY();

  ExprEval* pExprEval;
  pExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_p_TaskLogicUnit->eGenerateIndexScan(pExprEval);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_p_TaskLogicUnit->m_xpHandler = pHandler;
  
  PsxThd::vTlsSet(giTLS_HandlerKey,(void*)pHandler->szWordNm());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].init -- init the logic unit                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitInit(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IsLftCrwBuffer@ -- is the left buffer a crw buffer ?            */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIsLftCrwBufferFetch(void)
{
  ENTRY();

  bool bIsLftCrwBuffer = m_p_TaskLogicUnit->m_bIsLftCrwBuffer;

  m_pDataStack->vPushInt(bIsLftCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IsLftCrwBuffer! -- store is left buffer a crw buffer ?         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIsLftCrwBufferStore(void)
{
  ENTRY();

  i8 iIsLeftCrwBuffer = m_pDataStack->iPopIntBol(__func__);

  m_p_TaskLogicUnit->m_bIsLftCrwBuffer = iIsLeftCrwBuffer;

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* LogicUnit[].IntStack@ -- int stack fetch                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIntStackFetch(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpIntStack);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IntStack! -- int stack store                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIntStackStore(void)
{
  ENTRY();

  IntStack* pIntStack = _pIntStack(m_pDataStack->pPopPtr(__func__));

  m_p_TaskLogicUnit->m_xpIntStack = pIntStack;
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].jobMetrics@ -- job metrics fetch                               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitJobMetricsFetch(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpJobMetrics);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].jobMetrics! -- job metrics store                               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitJobMetricsStore(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_pDataStack->pPopPtr(__func__));

  m_p_TaskLogicUnit->m_xpJobMetrics = pJobMetrics;
  EXIT();
}




/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcIndex@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcIndexFetch(void)
{
  ENTRY();
  
  DevIXT* pDevIxt = _pDevIXT(m_p_TaskLogicUnit->m_xRluCommon.m_pDevLftIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcTableFetch(void)
{
  ENTRY();
  
  DevROW* pDevRow = _pDevROW(m_p_TaskLogicUnit->m_xRluCommon.m_pDevLftRow);
  
  m_pDataStack->vPushStr(pDevRow->m_sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transform -- transform                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransform(void)
{
  ENTRY();
  
  i8 bResult = m_p_TaskLogicUnit->iTransform(0,C_UNKNOWN);
  UNUSED(bResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transformArgs -- transform (with arguments)                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransformArgs(void)
{
  ENTRY();
  
  i8 nArgs = m_pDataStack->iPopInt(__func__);

  if (nArgs) {
    RASSERT(1 == nArgs);
  }
  i8 iArg0 = m_pDataStack->iPopInt(__func__);

  i8 bResult = m_p_TaskLogicUnit->iTransform(nArgs,iArg0);
  UNUSED(bResult);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
