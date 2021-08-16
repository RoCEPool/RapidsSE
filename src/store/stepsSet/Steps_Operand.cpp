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
  vAddStep("Operand[].csvAppend"_Stz,      &Step::vOperandCsvAppend);
  vAddStep("Operand[].csvData"_Stz,        &Step::vOperandCsvData);
  vAddStep("Operand[].csvToRow"_Stz,       &Step::vOperandCsvToRow);
  vAddStep("Operand[].crwDuplicate"_Stz,   &Step::vOperandCrwDuplicate);
  vAddStep("Operand[].crwRowCount@"_Stz,   &Step::vOperandCrwRowCountFetch);
  vAddStep("Operand[].crwToCsv"_Stz,       &Step::vOperandCrwToCsv);
  vAddStep("Operand[].crwToEachRow"_Stz,   &Step::vOperandCrwToEachRow);
  vAddStep("Operand[].delete"_Stz,         &Step::vOperandDelete);
  vAddStep("Operand[].insdelIntoTable"_Stz,&Step::vOperandInsdelIntoTable);
  vAddStep("Operand[].isNotNull"_Stz,      &Step::vOperandIsNotNull);
  vAddStep("Operand[].isNull"_Stz,         &Step::vOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vOperandIsReport);
  vAddStep("Operand[].invokePython"_Stz,   &Step::vOperandInvokePython);
  vAddStep("Operand[].invokePythonShell"_Stz, &Step::vOperandInvokePythonShell);
  vAddStep("Operand[].kind"_Stz,           &Step::vOperandKind);
  vAddStep("Operand[].morphClass"_Stz,     &Step::vOperandMorphClass);
  vAddStep("Operand[].newCancelling"_Stz,&Step::vOperandNewCancelling);
  vAddStep("Operand[].newCancelledMarker"_Stz,&Step::vOperandNewCancelledMarker);
  vAddStep("Operand[].newCsvBuffer"_Stz,   &Step::vOperandNewCsvBuffer);
  vAddStep("Operand[].newCleaningMarker"_Stz,&Step::vOperandNewCleaningMarker);
  vAddStep("Operand[].newCleanedupMarker"_Stz,&Step::vOperandNewCleanedupMarker);
  vAddStep("Operand[].newDataPulse"_Stz,   &Step::vOperandNewDataPulse);
  vAddStep("Operand[].newDestroyMarker"_Stz, &Step::vOperandNewDestroyMarker);
  vAddStep("Operand[].newEachScanEosMarker"_Stz,   &Step::vOperandNewEachScanEosMarker);
  vAddStep("Operand[].newEndMarker"_Stz, &Step::vOperandNewEndMarker);
  vAddStep("Operand[].newEndingMarker"_Stz, &Step::vOperandNewEndingMarker);
  vAddStep("Operand[].newEndedMarker"_Stz, &Step::vOperandNewEndedMarker);
  vAddStep("Operand[].newEndCrwRhtMarker"_Stz, &Step::vOperandNewEndCrwRhtMarker);
  vAddStep("Operand[].newEndRowRhtMarker"_Stz, &Step::vOperandNewEndRowRhtMarker);
  vAddStep("Operand[].newEosMarker"_Stz,   &Step::vOperandNewEosMarker);
  vAddStep("Operand[].newErroringMarker"_Stz, &Step::vOperandNewErroringMarker);
  vAddStep("Operand[].newErroredMarker"_Stz,&Step::vOperandNewErroredMarker);
  vAddStep("Operand[].newExitingMarker"_Stz,  &Step::vOperandNewExitingMarker);
  vAddStep("Operand[].newExitedMarker"_Stz,&Step::vOperandNewExitedMarker);
  vAddStep("Operand[].newLeaveMarker"_Stz,&Step::vOperandNewLeaveMarker);
  vAddStep("Operand[].newLeavingMarker"_Stz,&Step::vOperandNewLeavingMarker);
  vAddStep("Operand[].newLeftMarker"_Stz,&Step::vOperandNewLeftMarker);
  vAddStep("Operand[].newNextBufferMarker"_Stz,&Step::vOperandNewNextBufferMarker);
  vAddStep("Operand[].newStartedMarker"_Stz, &Step::vOperandNewStartedMarker);
  vAddStep("Operand[].newStartMarker"_Stz, &Step::vOperandNewStartMarker);
  vAddStep("Operand[].newStartCrwRhtMarker"_Stz, &Step::vOperandNewStartCrwRhtMarker);
  vAddStep("Operand[].newStartRowRhtMarker"_Stz, &Step::vOperandNewStartRowRhtMarker);
  vAddStep("Operand[].newStreamDef"_Stz,   &Step::vOperandNewStreamDef);
  vAddStep("Operand[].newTrigger"_Stz,     &Step::vOperandNewTrigger);
  vAddStep("Operand[].pipeIn"_Stz,         &Step::vOperandPipeIn);
  vAddStep("Operand[].pipeOutSet"_Stz,     &Step::vOperandPipeOutSet);
  vAddStep("Operand[].pipeToOutput"_Stz,   &Step::vOperandPipeToOutput);
  vAddStep("Operand[].report"_Stz,         &Step::vOperandReport);
  vAddStep("Operand[].rowRowCount@"_Stz,      &Step::vOperandRowRowCountFetch);
  vAddStep("Operand[].rowToCsv"_Stz,       &Step::vOperandRowToCsv);
  vAddStep("Operand[].rowToEachRow"_Stz,   &Step::vOperandRowToEachRow);
  vAddStep("Operand[].set"_Stz,            &Step::vOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vOperandUpsertIntoTable);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwDuplicate -- duplicate crw buffer operand                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandCrwDuplicate(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);

  CrwBuffer* pNewCrwBuffer = new CrwBuffer;
  pNewCrwBuffer->vCopy(pCrwBuffer);

  m_pDataStack->vPushPtr(pNewCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwRowCount@ -- get no of rows in the crw buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandCrwRowCountFetch(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  i8 nRows = pCrwBuffer->nRows();

  m_pDataStack->vPushInt(nRows);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Operand[].crwToCsv -- convert Crw operand to Csv                           */
/*----------------------------------------------------------------------------*/
void Step::vOperandCrwToCsv(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  m_p_TaskLogicUnit->m_xpOperandA = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwToEachRow -- convert a row of Crw operand to eachRowBuffer    */
/*----------------------------------------------------------------------------*/
void Step::vOperandCrwToEachRow(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  i8 iRow = m_pDataStack->iPopInt(__func__);

  EachRowBuffer* pEachRowBuffer = pCrwBuffer->pToEachRowBuffer(iRow);
  m_pDataStack->vPushPtr(pEachRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvAppend -- append a csv row to the csv operand                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvAppend(void)
{
  ENTRY();
  
  s256 sCsvRow;
  m_pDataStack->vPopStr(sCsvRow,__func__);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  i8 nBytes = sCsvRow.nSize();
  if (pCsvBuffer->nFreeBytes() < nBytes + 1)
    ASSERT(0);
  
  Stz* szRecord = pCsvBuffer->szAllotRecord(nBytes - 1);
  StrLib::vCopyLen(szRecord->azChar(),sCsvRow.azChar(),nBytes - 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvData -- return operand csv data                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvData(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  char* data = (char*)pCsvBuffer->ayDataArea();
  m_pDataStack->vPushStz((Stz*)data);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToRow -- convert csv nuffer operand to row buffer             */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvToRow(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer =
    _pCsvBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  
  ToRow* pToRow = *m_p_TaskLogicUnit->m_xpToRow;
  
  bool b = pToRow->bConvertCsv(pCsvBuffer);
  UNUSED(b);

  EXIT();
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
/* Operand[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandKind(void)
{
  ENTRY();
  
//  Operand* pOperand = *m_p_TaskLogicUnit->m_xpOperandA;
//
//  eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
  
  eZK ePipeInOperand = m_p_TaskLogicUnit->m_ePipeInOperand;

  m_pDataStack->vPushInt(ePipeInOperand);
  
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
/* Operand[].newCancelling -- new a cancelling marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCancelling(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_CANCELLING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCancelledMarker -- new a cancelled marker                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCancelledMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_CANCELLEDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCsbBuffer -- new a csv buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCsvBuffer(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new CsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCleaningMarker -- new a cleanup marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCleaningMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_CLEANING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCleanedupMarker -- new a cleanedup marker                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCleanedupMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_CLEANED);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newDataPulse -- new a data pulse                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewDataPulse(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_DATAPULSE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newDestroyMarker -- new a destry marker                          */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewDestroyMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_DESTROYMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEachScanEosMarker -- new an each scan end marker              */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEachScanEosMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_EACHSCANEOSMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEnd -- new an end marker                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ENDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEnded -- new an ended marker                                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndedMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ENDEDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEnding -- new an ending marker                                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndingMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ENDINGMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEndCrwRhtMarker -- new an end crw right marker                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ENDCRWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEndRowRhtMarker -- new an end row right marker                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ENDROWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEosMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEosMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErroringMarker -- new an erroring marker                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewErroringMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ERRORINGMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErroredMarker -- new an errored marker                        */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewErroredMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_ERROREDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitingMarker -- new an exiting marker                        */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewExitingMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_EXITING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitedMarker -- new an exited marker                          */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewExitedMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_EXITED);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newLeaveMarker -- new a leave marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewLeaveMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_LEAVEMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newLeavingMarker -- new a leaving marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewLeavingMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_LEAVINGMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newLeftMarker -- new a left marker                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewLeftMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_LEFTMARKER);

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
/* Operand[].newStartedMarker -- new a started marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartedMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartMarker -- new a start marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = new Marker(OM_STARTMARKER);
  m_p_TaskLogicUnit->m_xpOperandA = pOperand;

  pOp->vOpTrace(pOperand,(Stz*)__func__,true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartCrwRhtMarker -- new a start crw right marker             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_STARTCRWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartRowRhtMarker -- new a start crw right marker             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_STARTROWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def from Table[]                    */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStreamDef(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());
  
  if (m_p_TaskLogicUnit->m_xpTable.bIsNull())
    m_p_TaskLogicUnit->m_xpOperandA = new StreamDef();
  else
    m_p_TaskLogicUnit->m_xpOperandA = new StreamDef(m_p_TaskLogicUnit->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newTrigger -- new a trigger marker                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewTrigger(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpOperandA.bIsNull());

  m_p_TaskLogicUnit->m_xpOperandA = new Marker(OM_TRIGGER);

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
/* Operand[].rowRowCount@ -- fetch the operand row count of rowBuffer         */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowRowCountFetch(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpOperandA);
  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  
  m_pDataStack->vPushInt(pRowBuffer->nRows());
  
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
/* Operand[].rowToCsv -- convert rowBuffer operand to csvBuffer               */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowToCsv(void)
{
  ENTRY();
  
  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
 
  m_p_TaskLogicUnit->m_xpOperandA = pRowBuffer->pToCsvBuffer();
  
  delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowToEachRow -- convert a row of RowBuffer to eachRowBuffer      */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowToEachRow(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpOperandA);
  i8 iRow = m_pDataStack->iPopInt(__func__);

  EachRowBuffer* pEachRowBuffer = pRowBuffer->pToEachRowBuffer(iRow);
  m_pDataStack->vPushPtr(pEachRowBuffer);

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
