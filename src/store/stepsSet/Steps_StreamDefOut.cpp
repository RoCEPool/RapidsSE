/*============================================================================*/
/* Steps_StreamDefOut.cpp -- Stream Definition (out-going) step group         */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefOut(void)
{
  vAddStep("StreamDefOut[]"_Stz,            &Step::vStreamDefOutGet);
  vAddStep("StreamDefOut[].addColumn"_Stz,  &Step::vStreamDefOutAddColumn);
  vAddStep("StreamDefOut[].addColumns"_Stz, &Step::vStreamDefOutAddColumns);
  vAddStep("StreamDefOut[].delete"_Stz,     &Step::vStreamDefOutDelete);
  vAddStep("StreamDefOut[].duplicate"_Stz,  &Step::vStreamDefOutDuplicate);
  vAddStep("StreamDefOut[].isNull"_Stz,     &Step::vStreamDefOutIsNull);
  vAddStep("StreamDefOut[].name@"_Stz,      &Step::vStreamDefOutNameFetch);
  vAddStep("StreamDefOut[].name!"_Stz,      &Step::vStreamDefOutNameStore);
  vAddStep("StreamDefOut[].new"_Stz,        &Step::vStreamDefOutNew);
  vAddStep("StreamDefOut[].pipeout"_Stz,    &Step::vStreamDefOutPipeout);
  vAddStep("StreamDefOut[].print"_Stz,      &Step::vStreamDefOutPrint);
  vAddStep("StreamDefOut[].set"_Stz,        &Step::vStreamDefOutSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].addColumn -- add a column                                   */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutAddColumn(void)
{
  ENTRY();
    
  s32 sColNm;
  m_pDataStack->vPopStr(sColNm,__func__);

  eCT eColType = (eCT)m_pDataStack->iPopInt(__func__);
  i8 eColLength = (eCT)m_pDataStack->iPopInt(__func__);
  i8 eColScale = (eCT)m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDefOut = * ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumn(sColNm,eColType,eColLength,eColScale);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].addColumns -- add columns from given stream def             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutAddColumns(void)
{
  ENTRY();
    
  StreamDef* pStreamDefSrc = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  StreamDef* pStreamDefOut = * ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumns(pStreamDefSrc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].delete -- delete stream definition out                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDelete(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = * ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut;
  
  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut = nullptr;
  
  delete pStreamDefOut;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].duplicate -- new a duplicate stream def                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDuplicate(void)
{
  ENTRY();
  
  ASSERT (* ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut);
  
  StreamDef* pStreamDef = new StreamDef(* ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut);
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[] -- get stream definition out                                */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name@ -- get name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameFetch(void)
{
  ENTRY();
    
  s32 sName = ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut->szName();
  
  m_pDataStack->vPushStz(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name! -- set name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameStore(void)
{
  ENTRY();
    
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].new -- new a stream definition out                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNew(void)
{
  ENTRY();
    
  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut = new StreamDef;
  (* ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut)->vAddDefSet();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].pipeOut -- append streamDef onto next operator in pipeline  */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutPipeout(void)
{
  ENTRY();
  
  ASSERT(*( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut,this);
  
  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].print -- print streamDefOut                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutPrint(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = _pStreamDef(*( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut);
  
  pStreamDefOut->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].set -- set streamDefOut                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutSet(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

