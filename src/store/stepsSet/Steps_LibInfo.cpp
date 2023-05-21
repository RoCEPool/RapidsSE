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

#include "LogicUnit.hpp"
#include "CompileLib.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LibInfo(void)
{
  vAddStep("LibInfo[].compile"_Stz,      &Step::vLibInfoCompile);
  vAddStep("LibInfo[].delete"_Stz,       &Step::vLibInfoDelete);
  vAddStep("LibInfo[].find"_Stz,         &Step::vLibInfoFind);
  vAddStep("LibInfo[].get"_Stz,          &Step::vLibInfoGet);
  vAddStep("LibInfo[].isNotNull"_Stz,    &Step::vLibInfoIsNotNull);
  vAddStep("LibInfo[].link"_Stz,         &Step::vLibInfoLink);
  vAddStep("LibInfo[].new"_Stz,          &Step::vLibInfoNew);
  vAddStep("LibInfo[].set"_Stz,          &Step::vLibInfoSet);
  vAddStep("LibInfo[].unlink"_Stz,       &Step::vLibInfoDelete);
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].compile -- compile given source using LibInfo                    */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoCompile(void)
{
  ENTRY();

  s4K szDynLibNm;
  m_pDataStack->vPopStr(szDynLibNm,__func__);

  s48K szCode;
  m_pDataStack->vPopStr(szCode,__func__);

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;

  bool bClean = CompileLib::bCompile(szCode.szStz(), szDynLibNm, pLibInfo, "clang"_Stz);
  if (bClean)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].delete -- delete LibInfo                                         */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoDelete(void)
{
  ENTRY();
    
  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  m_pLogicUnit->m_xpLibInfo = nullptr;

  delete pLibInfo;

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
  m_pDataStack->vPopStr(sFunNm,__func__);

  i8 (*afFun)(void*);

  *(void **)(&afFun) = CompileLib::pvSymbolAddr(sFunNm, pLibInfo);

  m_pDataStack->vPushInt((i8)afFun);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].get -- get LibInfo                                               */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoGet(void)
{
  ENTRY();

  LibInfo* pLibInfo = * m_pLogicUnit->m_xpLibInfo;
  m_pDataStack->vPushPtr(pLibInfo);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LibInfo[].isNotNull -- get LibInfo                                         */
/*----------------------------------------------------------------------------*/
void Step::vLibInfoIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpLibInfo.bIsNull() ? 0 : 1);

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
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

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
    _pLibInfo(m_pDataStack->pPopPtr(__func__));

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
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
