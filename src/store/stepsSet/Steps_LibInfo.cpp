/*============================================================================*/
/* Steps_LibInfo.cpp -- LibInfo step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"
#include "ExtendedText.hpp"

#include "LogicUnit.hpp"
#include "CompileLib.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LibInfo(void)
{
  vAddStep("LibInfo[].compile"_Stz,      &Step::vLibInfoCompile);
  vAddStep("LibInfo[].compileFile"_Stz,  &Step::vLibInfoCompileFile);
  vAddStep("LibInfo[].delete"_Stz,       &Step::vLibInfoDelete);
  vAddStep("LibInfo[].error"_Stz,        &Step::vLibInfoError);
  vAddStep("LibInfo[].find"_Stz,         &Step::vLibInfoFind);
  vAddStep("LibInfo[].get"_Stz,          &Step::vLibInfoGet);
  vAddStep("LibInfo[].isNotNull"_Stz,    &Step::vLibInfoIsNotNull);
  vAddStep("LibInfo[].link"_Stz,         &Step::vLibInfoLink);
  vAddStep("LibInfo[].new"_Stz,          &Step::vLibInfoNew);
  vAddStep("LibInfo[].set"_Stz,          &Step::vLibInfoSet);
  vAddStep("LibInfo[].unlink"_Stz,       &Step::vLibInfoUnlink);
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].compile -- compile given source using LibInfo                    */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoCompile(void)
{
  ENTRY();

  s4K szDynLibNm;
  m_oDataStack.vPopStr(szDynLibNm, __func__);

  ExtendedText* pCodeBlock;
  pCodeBlock = _pExtendedText(m_oDataStack.pPopPtr(__func__));

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;

  bool bClean = CompileLib::bCompile(pCodeBlock, szDynLibNm, pLibInfo);

  if (bClean)
    m_oDataStack.vPushInt(true);
  else
    m_oDataStack.vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].compileFile -- compile source file using LibInfo                 */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoCompileFile(void)
{
  ENTRY();


  s4K szDynLibNm;
  m_oDataStack.vPopStr(szDynLibNm, __func__);

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;

  bool bClean = CompileLib::bCompileFile(szDynLibNm, pLibInfo);

  if (bClean)
    m_oDataStack.vPushInt(true);
  else
    m_oDataStack.vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].delete -- delete LibInfo                                         */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoDelete(void)
{
  ENTRY();

  bool bDeleteFile = m_oDataStack.iPopBol(__func__);

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  m_pLogicUnit->m_xpLibInfo = nullptr;

  CompileLib::vDelete(pLibInfo, bDeleteFile);

  delete pLibInfo;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].error -- get error from LibInfo                                  */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoError(void)
{
  ENTRY();

  s2K sErrDetails;

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  sErrDetails = pLibInfo->szErrDetails();
  m_oDataStack.vPushStr(sErrDetails);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].find -- find symbol                                              */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoFind(void)
{
  ENTRY();

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;

  s32 sFunNm;
  m_oDataStack.vPopStr(sFunNm, __func__);

  i8 (*afFun)(void*);

  *(void **)(&afFun) = CompileLib::pvSymbolAddr(sFunNm, pLibInfo);

  m_oDataStack.vPushInt((i8)afFun);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].get -- get LibInfo                                               */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoGet(void)
{
  ENTRY();

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  m_oDataStack.vPushPtr(pLibInfo);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].isNotNull -- get LibInfo                                         */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoIsNotNull(void)
{
  ENTRY();

  m_oDataStack.vPushInt(m_pLogicUnit->m_xpLibInfo.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].link -- link LibInfo                                             */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoLink(void)
{
  ENTRY();

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  bool bResult = CompileLib::bDynamicLink(pLibInfo);

  if (bResult)
    m_oDataStack.vPushInt(true);
  else
    m_oDataStack.vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].new -- new LibInfo                                               */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoNew(void)
{
  ENTRY();
  
  LibInfo* pLibInfo = new LibInfo(false);
  m_pLogicUnit->m_xpLibInfo = pLibInfo;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].set -- set LibInfo                                               */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpLibInfo =
    _pLibInfo(m_oDataStack.pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].unlink -- unlink LibInfo                                         */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoUnlink(void)
{
  ENTRY();

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  bool bResult = CompileLib::bDynamicUnlink(pLibInfo);

  if (bResult)
    m_oDataStack.vPushInt(true);
  else
    m_oDataStack.vPushInt(false);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
