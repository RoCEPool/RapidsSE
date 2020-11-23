/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "Steps.h"

extern EnvFlags gxEnvFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("LogicUnit[].altIndex@"_Stz,     &Step::vLogicUnitAltIndexFetch);
  vAddStep("LogicUnit[].altTable@"_Stz,     &Step::vLogicUnitAltTableFetch);
  
  vAddStep("LogicUnit[].diaryAction"_Stz,   &Step::vLogicUnitDiaryAction);
  vAddStep("LogicUnit[].diaryDir"_Stz,      &Step::vLogicUnitDiaryDir);
  vAddStep("LogicUnit[].diary@"_Stz,        &Step::vLogicUnitDiaryFetch);
  vAddStep("LogicUnit[].diaryJobNote"_Stz,  &Step::vLogicUnitDiaryJobNote);
  vAddStep("LogicUnit[].diaryName"_Stz,     &Step::vLogicUnitDiaryName);
  vAddStep("LogicUnit[].diaryNote"_Stz,     &Step::vLogicUnitDiaryNote);
  vAddStep("LogicUnit[].diaryTitle"_Stz,    &Step::vLogicUnitDiaryTitle);

  vAddStep("LogicUnit[].dynFun"_Stz,        &Step::vLogicUnitDynFun);
  vAddStep("LogicUnit[].dynFunArgs"_Stz,    &Step::vLogicUnitDynFunArgs);
  vAddStep("LogicUnit[].genAsm"_Stz,        &Step::vLogicUnitGenAsm);
  vAddStep("LogicUnit[].genAsmTxt"_Stz,     &Step::vLogicUnitGenAsmTxt);
  vAddStep("LogicUnit[].genCode"_Stz,       &Step::vLogicUnitGenCode);
    
  vAddStep("LogicUnit[].callAgain@"_Stz,    &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz,    &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].calledAgain@"_Stz,  &Step::vLogicUnitCalledAgainFetch);
  vAddStep("LogicUnit[].calledAgain!"_Stz,  &Step::vLogicUnitCalledAgainStore);

  vAddStep("LogicUnit[].dev@"_Stz,          &Step::vLogicUnitDevFetch);
  vAddStep("LogicUnit[].dev!"_Stz,          &Step::vLogicUnitDevStore);
  
  vAddStep("LogicUnit[].handler@"_Stz,      &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,      &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].init"_Stz,          &Step::vLogicUnitInit);
  
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
  
  DevIXT* pDevIxt = _pDevIXT(m_pLogicUnit->m_xRluCommon.m_pDevAltIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].altTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitAltTableFetch(void)
{
  ENTRY();
  
  DevRSB* pDevRsb = _pDevRSB(m_pLogicUnit->m_xRluCommon.m_pDevAltRsb);
  
  m_pDataStack->vPushStr(pDevRsb->m_sTableNm);
  
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
  
  m_pLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pLogicUnit->m_oSince.iNanoseconds();
  
  i8 nQueueLength = m_pLogicUnit->m_xpOp->nQueueLength();

  m_pLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           i_Action,i_ActEnumGroup,i_ActEnum,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diary@ -- diary fetch                                          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_pLogicUnit->m_pRluDiary);

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
  
  m_pLogicUnit->vDiaryJobNote(sJobNote);

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
  
  m_pLogicUnit->vDiaryName(sName);

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
  
  m_pLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pLogicUnit->m_oSince.iNanoseconds();
  
  
  i8 nQueueLength = m_pLogicUnit->m_xpOp->nQueueLength();
  
  m_pLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
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
  
  m_pLogicUnit->vDiaryDir(sDir);

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
  
  m_pLogicUnit->vDiaryTitle(sTitle);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFun(void)
{
  ENTRY();
  
  i8 iRes = m_pLogicUnit->iDynFun(0,C_UNKNOWN);
  
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

  i8 iRes = m_pLogicUnit->iDynFun(nArgs,iArg0);
  
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
  
  File* pFile = new File(nullptr);
  
  bool b = pFile->bOpenRead(sFileName.szStz());
  RASSERT(b);
  
  s16KB sText;
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);
  
  //vTrace("FILE %s\n"_Stz, sFileName.azChar());
  
  bool bResult = m_pLogicUnit->bGenerateAsm(sText);
  
  m_pDataStack->vPushInt(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genAsmTxt -- generate asm text into file                       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenAsmTxt(void)
{
  ENTRY();
  
  s256 sFileName;
  m_pDataStack->vPopStr(sFileName, __func__);
  
  File* pFile = new File(nullptr);
  
  i8 iQueryNo = m_pDataStack->iPopInt(__func__);
  
  //bool b = pFile->bOpenRead(sFileName.szStz());
  //RASSERT(b);
  
  m_pLogicUnit->vGenerateAsmTxt(iQueryNo, sFileName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genCode -- generate code for given test                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenCode(void)
{
  ENTRY();
  
  i8 iTestNo = m_pDataStack->iPopInt(__func__);
      
  m_pLogicUnit->vGenCode(iTestNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCallAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain@ --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCalledAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain! --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCalledAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dev@ --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDevFetch(void)
{
  ENTRY();

  i8 i_Dev = m_pDataStack->iPopInt(__func__);
  
  void* aDev = m_pLogicUnit->m_aaDev[i_Dev];
  
  m_pDataStack->vPushPtr((Object*)aDev);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dev! --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDevStore(void)
{
  ENTRY();

  i8 i_Dev = m_pDataStack->iPopInt(__func__);
  
  void* aDev = m_pDataStack->pPopPtr(__func__);
  
  m_pLogicUnit->m_aaDev[i_Dev] = aDev;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpHandler = pHandler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].init -- init the logic unit                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitInit(void)
{
  ENTRY();
  
  m_pLogicUnit->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcIndex@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcIndexFetch(void)
{
  ENTRY();
  
  DevIXT* pDevIxt = _pDevIXT(m_pLogicUnit->m_xRluCommon.m_pDevSrcIxt);
  
  m_pDataStack->vPushStr(pDevIxt->m_sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].srcTable@ --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSrcTableFetch(void)
{
  ENTRY();
  
  DevRSB* pDevRsb = _pDevRSB(m_pLogicUnit->m_xRluCommon.m_pDevSrcRsb);
  
  m_pDataStack->vPushStr(pDevRsb->m_sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transform -- transform                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransform(void)
{
  ENTRY();
  
  i8 bResult = m_pLogicUnit->iTransform(0,C_UNKNOWN);
  
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

  i8 bResult = m_pLogicUnit->iTransform(nArgs,iArg0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
