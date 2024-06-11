/*============================================================================*/
/* Steps_StreamDefOut.cpp -- Stream Definition (out-going) step group         */
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
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefOut(void)
{
  vAddStep("StreamDefOut[]"_Stz,            &Step::vStreamDefOutGet);
  vAddStep("StreamDefOut[].delete"_Stz,     &Step::vStreamDefOutDelete);
  vAddStep("StreamDefOut[].duplicate"_Stz,  &Step::vStreamDefOutDuplicate);
  vAddStep("StreamDefOut[].isNull"_Stz,     &Step::vStreamDefOutIsNull);
  vAddStep("StreamDefOut[].name@"_Stz,      &Step::vStreamDefOutNameFetch);
  vAddStep("StreamDefOut[].name!"_Stz,      &Step::vStreamDefOutNameStore);
  vAddStep("StreamDefOut[].new"_Stz,        &Step::vStreamDefOutNew);
  vAddStep("StreamDefOut[].newFromTable"_Stz,&Step::vStreamDefOutNewFromTable);
  vAddStep("StreamDefOut[].print"_Stz,      &Step::vStreamDefOutPrint);
  vAddStep("StreamDefOut[].set"_Stz,        &Step::vStreamDefOutSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].delete -- delete stream definition out                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDelete(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  
  m_pLogicUnit->m_xpStreamDefOut = nullptr;

  if (pStreamDefOut->eGetClassNo() == CN_STREAMDEF)
    delete pStreamDefOut;
  else {
    StreamDefRht* pStreamDefRht = _pStreamDefRht(pStreamDefOut);
    delete pStreamDefRht;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].duplicate -- new a duplicate stream def                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDuplicate(void)
{
  ENTRY();
  
  ASSERT (* m_pLogicUnit->m_xpStreamDefOut);
  
  StreamDef* pStreamDef = new StreamDef(* m_pLogicUnit->m_xpStreamDefOut);
  
  m_oDataStack.vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[] -- get stream definition out                                */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGet(void)
{
  ENTRY();
    
  m_oDataStack.vPushPtr(* m_pLogicUnit->m_xpStreamDefOut);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutIsNull(void)
{
  ENTRY();
  
  m_oDataStack.vPushInt(m_pLogicUnit->m_xpStreamDefOut.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name@ -- get name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameFetch(void)
{
  ENTRY();
    
  s32 sName = m_pLogicUnit->m_xpStreamDefOut->szStreamDefNm();
  
  m_oDataStack.vPushStz(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name! -- set name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameStore(void)
{
  ENTRY();
    
  s32 sName;
  m_oDataStack.vPopStr(sName, __func__);

  m_pLogicUnit->m_xpStreamDefOut->vStreamDefNm(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].new -- new a stream definition out                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNew(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpStreamDefOut = new StreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].newFromTable -- new a stream definition out from table      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNewFromTable(void)
{
  ENTRY();

  Table* pTable = _pTable(m_oDataStack.pPopPtr(__func__));

  m_pLogicUnit->m_xpStreamDefOut = new StreamDef(pTable->pStreamDef());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].print -- print streamDefOut                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutPrint(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = _pStreamDef(*m_pLogicUnit->m_xpStreamDefOut);
  
  pStreamDefOut->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].set -- set streamDefOut                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpStreamDefOut = _pStreamDef(m_oDataStack.pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

