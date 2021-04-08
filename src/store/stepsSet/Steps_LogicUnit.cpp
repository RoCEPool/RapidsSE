/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

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
  vAddStep("LogicUnit[].genLimit"_Stz,      &Step::vLogicUnitGenLimit);
  vAddStep("LogicUnit[].genProjection"_Stz, &Step::vLogicUnitGenProjection);
  vAddStep("LogicUnit[].genRange"_Stz,      &Step::vLogicUnitGenRange);
    
  vAddStep("LogicUnit[].handler@"_Stz,      &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,      &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].init"_Stz,          &Step::vLogicUnitInit);

  vAddStep("LogicUnit[].logOperand"_Stz,    &Step::vLogicUnitLogOperand);

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
  
  DevIXT* pDevIxt = _pDevIXT(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pDevAltIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].altTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitAltTableFetch(void)
{
  ENTRY();
  
  DevROW* pDevRow = _pDevROW(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pDevAltRow);
  
  m_pDataStack->vPushStr(pDevRow->m_sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xRluCommon.m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pJob->pLogicUnit_job()->m_xRluCommon.m_bCallAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain@ --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xRluCommon.m_bCalledAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain! --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pJob->pLogicUnit_job()->m_xRluCommon.m_bCalledAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].compilerToExit --                                              */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCompilerToExit(void)
{
  ENTRY();

  bool bCompilerToExit = m_pJob->pLogicUnit_job()->m_bCompilerToExit;

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
  
  m_pJob->pLogicUnit_job()->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pJob->pLogicUnit_job()->m_oSince.iNanoseconds();
  
  i8 nQueueLength = m_pJob->pLogicUnit_job()->m_xpOp->nQueueLength();

  m_pJob->pLogicUnit_job()->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           i_Action,i_ActEnumGroup,i_ActEnum,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diary@ -- diary fetch                                          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_pJob->pLogicUnit_job()->m_pRluDiary);

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
  
  m_pJob->pLogicUnit_job()->vDiaryJobNote(sJobNote);

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
  
  m_pJob->pLogicUnit_job()->vDiaryName(sName);

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
  
  m_pJob->pLogicUnit_job()->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pJob->pLogicUnit_job()->m_oSince.iNanoseconds();
  
  
  i8 nQueueLength = m_pJob->pLogicUnit_job()->m_xpOp->nQueueLength();
  
  m_pJob->pLogicUnit_job()->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
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
  
  m_pJob->pLogicUnit_job()->vDiaryDir(sDir);

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
  
  m_pJob->pLogicUnit_job()->vDiaryTitle(sTitle);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFun(void)
{
  ENTRY();
  
  i8 iRes = m_pJob->pLogicUnit_job()->iDynFun(0,C_UNKNOWN);
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

  i8 iRes = m_pJob->pLogicUnit_job()->iDynFun(nArgs,iArg0);
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

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  eEC eError;

  if (pExprEval->bHasGroupBy())
    eError = m_pJob->pLogicUnit_job()->eGenerateGroupBy(pExprEval, pStreamDefOut);
  else {
    eError = m_pJob->pLogicUnit_job()->eGenerateAggregate(pExprEval, pStreamDefOut);
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

  bool bResult = m_pJob->pLogicUnit_job()->bGenerateAsm(sText);
  
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

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_pJob->pLogicUnit_job()->eGenerateFilter(pExprEval, pStreamDefOut);

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

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_pJob->pLogicUnit_job()->eGenerateLimit(iLimit, pStreamDefOut);

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

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  eEC eError = m_pJob->pLogicUnit_job()->eGenerateProjection(pExprEval, pStreamDefOut);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genRange -- generabte projection asm text into file             */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenRange(void)
{
  ENTRY();

  Range* pRange = _pRange(m_pDataStack->pPopPtr(__func__));

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  m_pJob->pLogicUnit_job()->eGenerateRange(pRange, pStreamDefOut);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pJob->pLogicUnit_job()->m_xpHandler = pHandler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].init -- init the logic unit                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitInit(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].logOperand -- log operand                                      */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitLogOperand(void)
{
  ENTRY();
  
  i8 iPoint = m_pDataStack->iPopInt(__func__);
  
//  if ( ! AM::bRLUDiary() ) {
//    EXIT();
//    return;
//  }
  
  LogicUnit* pRLU = m_pJob->pLogicUnit_job();
  
  pRLU->m_oSince.vUpdate();
  i8 nNanoSeconds = pRLU->m_oSince.iNanoseconds();
  
  i8 nQueueLength = pRLU->m_xpOp->nQueueLength();
  
  vLogTI((i8)TI_OPERANDLOG,
           m_pJob->szJobNm(),
           m_pJob->i_TablePtn(),
           (i8)pRLU->m_ePipeInOperand,
           iPoint,
           nNanoSeconds,
           nQueueLength,
           C_VARGS_END
         )
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcIndex@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcIndexFetch(void)
{
  ENTRY();
  
  DevIXT* pDevIxt = _pDevIXT(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pDevSrcIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcTableFetch(void)
{
  ENTRY();
  
  DevROW* pDevRow = _pDevROW(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pDevSrcRow);
  
  m_pDataStack->vPushStr(pDevRow->m_sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transform -- transform                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransform(void)
{
  ENTRY();
  
  i8 bResult = m_pJob->pLogicUnit_job()->iTransform(0,C_UNKNOWN);
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

  i8 bResult = m_pJob->pLogicUnit_job()->iTransform(nArgs,iArg0);
  UNUSED(bResult);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
