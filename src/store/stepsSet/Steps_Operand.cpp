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
  vAddStep("Operand[].insdelIntoTable"_Stz,&Step::vOperandInsdelIntoTable);
  vAddStep("Operand[].isNotNull"_Stz,      &Step::vOperandIsNotNull);
  vAddStep("Operand[].isNull"_Stz,         &Step::vOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vOperandIsReport);
  vAddStep("Operand[].invokePython"_Stz,   &Step::vOperandInvokePython);
  vAddStep("Operand[].invokePythonShell"_Stz, &Step::vOperandInvokePythonShell);
  vAddStep("Operand[].morphClass"_Stz,     &Step::vOperandMorphClass);
  vAddStep("Operand[].newNextBufferMarker"_Stz,&Step::vOperandNewNextBufferMarker);
  vAddStep("Operand[].pipeIn"_Stz,         &Step::vOperandPipeIn);
  vAddStep("Operand[].pipeOutSet"_Stz,     &Step::vOperandPipeOutSet);
  vAddStep("Operand[].pipeToOutput"_Stz,   &Step::vOperandPipeToOutput);
  vAddStep("Operand[].set"_Stz,            &Step::vOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vOperandUpsertIntoTable);
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
/* Operand[].insdelIntoTable -- insert-delete operand into the table          */
/*----------------------------------------------------------------------------*/
void Step::vOperandInsdelIntoTable(void)
{
  ENTRY();

//  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
//
//  Op* pOp = m_pJob->m_pOp;
//  tPN tPtnNo = pOp->tPtnNo();
//  i8 i_TablePtn = Table::iPtnNoToTablePtn(tPtnNo);
//
//  if (CN_CRWBUFFER == (*m_p_TaskLogicUnit->m_xpOperandA)->eGetClassNo()) {
//    CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
//    // will delete this buffer
//    pCrwBuffer->vIsPermanent(false);
//    m_p_TaskLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, i_TablePtn, pCrwBuffer);
//
//  }
//  else
//  if (CN_ROWBUFFER == (*m_p_TaskLogicUnit->m_xpOperandA)->eGetClassNo()) {
//    // will delete this buffer
//    RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
//    pRowBuffer->vIsPermanent(false);
//    m_p_TaskLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, i_TablePtn, pRowBuffer);
//  }
//  else {
//    NEVER_REACH;
//  }

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
/* Operand[].invokePython -- invoke python script                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandInvokePython(void)
{
  ENTRY();

  s4K  pData;
  s256 pPythonFile;
  s256 pPythonFunction;
  
  // pop the python file name
  m_pDataStack->vPopStr(pPythonFile, __func__);
  
  // pop the python function name
  m_pDataStack->vPopStr(pPythonFunction, __func__);
  
  // pop the data to be printed
  m_pDataStack->vPopStr(pData, __func__);

#ifdef WITH_PYTHON_SUPPORT
  
  // Initialize the Python interpreter.
  Py_Initialize();
  PySys_SetPath((char*)("../Python"));
  
  // Create some Python objects
  PyObject *pModuleName, *pModule, *pDict, *pFunction, *pArgs, *pArgString;
  
  // Convert the python file name to a Python string.
  pModuleName = PyString_FromString(pPythonFile.azChar());
  
  // Import the file as a Python module.
  pModule = PyImport_Import(pModuleName);
  if (pModule == nullptr)
  {
    PyErr_Print();
  }
  // get the dictionary of the contents of the module.
  pDict = PyModule_GetDict(pModule);
  
  // Get the function from the dictionary.
  pFunction = PyDict_GetItemString(pDict, pPythonFunction.azChar());
  
  // Create a Python tuple to hold one argument to the method.
  pArgs = PyTuple_New(1);

  // covert the data argument to a python string and add to teh arg tuple
  pArgString = PyString_FromString(pData.azChar());
  PyTuple_SetItem(pArgs, 0, pArgString);
  
  // Call the function with the arguments.
  PyObject* pResult = PyObject_CallObject(pFunction, pArgs);
  
  // Print a message if calling the method failed.
  if(pResult == NULL) {
    printf("Calling the print method failed.\n");
    PyErr_Print();
  }
  
  // Convert the result to a long from a Python object.
  i8 iResult = PyInt_AsLong(pResult);
  RASSERT(iResult);
  
  // Destroy the Python interpreter.
  Py_Finalize();
  
#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].invokePythonShell -- invoke python script from shell             */
/*----------------------------------------------------------------------------*/
void Step::vOperandInvokePythonShell(void)
{
  ENTRY();
  
  s4K  pData;
  s256 pPythonFile;
  
  // pop the python file name
  m_pDataStack->vPopStr(pPythonFile, __func__);
  
  // pop the data to be printed
  m_pDataStack->vPopStr(pData, __func__);
  
  SysCall oSysCall;
  s4K sCommand;
  
  sCommand.vSprint("python %s %s"_Stz, pPythonFile.azChar(), pData.azChar());
  
  i8 iExitStatus = Shell::iExecute(sCommand.szStz(),oSysCall);
  UNUSED(iExitStatus);

  RASSERT(oSysCall.m_eStatus == SC_SUCCESS);
  
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
/* Operand[].newNextBufferMarker -- new a next buffer marker                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewNextBufferMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_NEXTBUFFERMARKER);

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
/* Operand[].pipeOutSet -- append operand set onto next operator in pipeline  */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOutSet(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToNextOp(*m_p_TaskLogicUnit->m_xpOperandA,this);
  
  m_p_TaskLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToOutput -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_p_TaskLogicUnit->m_xpOperandA);
  
  m_p_TaskLogicUnit->m_xpOperandA = nullptr;
  
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
/* Operand[].upsertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandUpsertIntoTable(void)
{
  ENTRY();
  
//  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
//  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
//
//  Op* pOp = m_pJob->m_pOp;
//  tPN tPtnNo = pOp->tPtnNo();
//  i8 i_TablePtn = Table:iPtnNoToTablePtn(tPtnNo);
//
//  m_p_TaskLogicUnit->m_xpTable->vUpsertBuffer(C_UNKNOWN, i_TablePtn, pCrwBuffer);
//
//  // will delete this buffer
//  pCrwBuffer->vIsPermanent(false);
  
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
