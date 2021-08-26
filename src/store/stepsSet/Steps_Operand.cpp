/*============================================================================*/
/* Steps_Operand.cpp -- Register Operand step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"
#include "EachRowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "Parcel.hpp"
#include "MY.hpp"


#ifdef WITH_PYTHON_SUPPORT
  #include "/usr/include/python2.7/Python.h"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("operand.toParcel"_Stz,         &Step::vOperandToParcel);

  vAddStep("Operand[]"_Stz,                &Step::vOperandGet);
  vAddStep("Operand[]\\clear"_Stz,         &Step::vOperandGetAndClear);
  vAddStep("Operand[].delete"_Stz,         &Step::vOperandDelete);
  vAddStep("Operand[].isNotNull"_Stz,      &Step::vOperandIsNotNull);
  vAddStep("Operand[].isNull"_Stz,         &Step::vOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vOperandIsReport);
  vAddStep("Operand[].morphClass"_Stz,     &Step::vOperandMorphClass);
  vAddStep("Operand[].pipeIn"_Stz,         &Step::vOperandPipeIn);
  vAddStep("Operand[].set"_Stz,            &Step::vOperandSet);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpOperandA);
  
  m_p_TaskLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].getAndClear -- fetch operand and set to nullptr                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOperandA);
  
  m_p_TaskLogicUnit->m_xpOperandA = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNotNull -- is operand not null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpOperandA.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpOperandA.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isReport -- is operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsReport(void)
{
  ENTRY();

  Operand* pOperand = *m_p_TaskLogicUnit->m_xpOperandA;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == PT_REPORT ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].morphClass -- change the class number of operand                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandMorphClass(void)
{
  ENTRY();
  
  i8 iClassNo = m_pDataStack->iPopInt(__func__);

  Operand* pOperand = *m_p_TaskLogicUnit->m_xpOperandA;
  
  pOperand->vSetClassNo((eCN)iClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeIn -- move pipein operand into operandA                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_p_TaskLogicUnit->m_xpPipein;

  m_p_TaskLogicUnit->m_xpPipein = nullptr;
    
  m_p_TaskLogicUnit->m_xpOperandA = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].report -- mark operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandReport(void)
{
  ENTRY();
  
  Operand* pOperand = *m_p_TaskLogicUnit->m_xpOperandA;
  pOperand->vTailProcessing(PT_REPORT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* operand.toParcel -- convert operand to parcel                              */
/*----------------------------------------------------------------------------*/
void Step::vOperandToParcel(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Parcel* pParcel =  new Parcel;

  pParcel->vAddItem(pOperand);
  
  pParcel->vLabel(pOp->eCrossTopo(),
                  pOp->pPipelineJob()->iJobId());
  
  m_pDataStack->vPushPtr(pParcel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].writeToFile0 -- write to file0[]                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandWriteToFile0(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
  ASSERT(*m_p_TaskLogicUnit->m_xpFile0);
  ASSERT(m_p_TaskLogicUnit->m_xpFile0->bIsOpen());
  
  if (CN_CSVBUFFER == (*m_p_TaskLogicUnit->m_xpOperandA)->eGetClassNo()) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
    char* aData = (char*)pCsvBuffer->ayDataArea();
  
    m_p_TaskLogicUnit->m_xpFile0->vPrint("%s"_Stz,aData);
    //m_p_TaskLogicUnit->m_xpFile0->vFlush();
  }
  else
  if (CN_CRWBUFFER == (*m_p_TaskLogicUnit->m_xpOperandA)->eGetClassNo()) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
    
    byte* aCrwProper = aAdd<byte*>(pCrwBuffer,sizeof(DataBuffer));
    i8 nSizeCrwProper = sizeof(CrwBuffer) - sizeof(DataBuffer);

    bool b = m_p_TaskLogicUnit->m_xpFile0->bWriteBytes(aCrwProper,nSizeCrwProper);
    ASSERT(b);
    UNUSED(b);
  }
  else {
    NEVER_REACH;
  }
  
  // delete the buffer 
  //delete pCsvBuffer;
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
