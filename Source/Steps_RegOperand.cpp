/*============================================================================*/
/* Steps_RegOperand.cpp -- Register Operand step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperand(void)
{
  vAddStep("Operand[]"_Stz,                &Step::vRegOperandGet);
  vAddStep("Operand[].addColumnDef"_Stz,   &Step::vRegOperandAddColumnDef);
  vAddStep("Operand[].bide"_Stz,           &Step::vRegOperandBide);
  vAddStep("Operand[].csvAppend"_Stz,      &Step::vRegOperandCsvAppend);
  vAddStep("Operand[].csvData"_Stz,        &Step::vRegOperandCsvData);
  vAddStep("Operand[].crwToCsv"_Stz,       &Step::vRegOperandCrwToCsv);
  vAddStep("Operand[].csvToCrw"_Stz,       &Step::vRegOperandCsvToCrw);
  vAddStep("Operand[].csvToCrwH"_Stz,      &Step::vRegOperandCsvToCrwH);
  vAddStep("Operand[].delete"_Stz,         &Step::vRegOperandDelete);
  vAddStep("Operand[].firstRow@"_Stz,      &Step::vRegOperandFirstRowFetch);
  vAddStep("Operand[].handleAggEnd"_Stz,   &Step::vRegOperandHandleAggEnd);
  vAddStep("Operand[].isNull"_Stz,         &Step::vRegOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vRegOperandIsReport);
  vAddStep("Operand[].itemCount@"_Stz,     &Step::vRegOperandItemCountFetch);
  vAddStep("Operand[].invokePython"_Stz,   &Step::vRegOperandInvokePython);
  vAddStep("Operand[].invokePythonShell"_Stz, &Step::vRegOperandInvokePythonShell);
  vAddStep("Operand[].kind"_Stz,           &Step::vRegOperandKind);
  vAddStep("Operand[].newCsvBuffer"_Stz,   &Step::vRegOperandNewCsvBuffer);
  vAddStep("Operand[].newEosMarker"_Stz,   &Step::vRegOperandNewEosMarker);
  vAddStep("Operand[].newErrorMarker"_Stz, &Step::vRegOperandNewErrorMarker);
  vAddStep("Operand[].newErroredMarker"_Stz,&Step::vRegOperandNewErroredMarker);
  vAddStep("Operand[].newExitMarker"_Stz,  &Step::vRegOperandNewExitMarker);
  vAddStep("Operand[].newExitedMarker"_Stz,&Step::vRegOperandNewExitedMarker);
  vAddStep("Operand[].newPulseMarker"_Stz, &Step::vRegOperandNewPulseMarker);
  vAddStep("Operand[].newPulsedMarker"_Stz,&Step::vRegOperandNewPulsedMarker);
  vAddStep("Operand[].newStartMarker"_Stz, &Step::vRegOperandNewStartMarker);
  vAddStep("Operand[].newStartedMarker"_Stz, &Step::vRegOperandNewStartedMarker);
  vAddStep("Operand[].newStreamDef"_Stz,   &Step::vRegOperandNewStreamDef);
  vAddStep("Operand[].next"_Stz,           &Step::vRegOperandNext);
  vAddStep("Operand[].pipeOut"_Stz,        &Step::vRegOperandPipeOut);
  vAddStep("Operand[].pipeOutSet"_Stz,     &Step::vRegOperandPipeOutSet);
  vAddStep("Operand[].pipeToJob"_Stz,      &Step::vRegOperandPipeToJob);
  vAddStep("Operand[].report"_Stz,         &Step::vRegOperandReport);
  vAddStep("Operand[].rowCount@"_Stz,      &Step::vRegOperandRowCountFetch);
  vAddStep("Operand[].set"_Stz,            &Step::vRegOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vRegOperandUpsertIntoTable);
  vAddStep("Operand[].workDone"_Stz,       &Step::vRegOperandWorkDone);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vRegOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].addColumnDef -- operand add column definition                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandAddColumnDef(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pRegSet->m_xpOperandA);
  
  s256 sColumnNm;
  m_pDataStack->vPopStr(sColumnNm, __func__);
  
  eCT eColumnType;
  eColumnType = (eCT)m_pDataStack->iPopInt(__func__);
  
  pStreamDef->vAddColumn(sColumnNm, eColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBide(void)
{
  ENTRY();
  
  m_pRegSet->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pRegSet->m_xpOperandA.bIsNull()) {
    m_pTask->vScheduleBide();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvAppend -- append a csv row to the csv operand                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCsvAppend(void)
{
  ENTRY();
  
  s256 sCsvRow;
  m_pDataStack->vPopStr(sCsvRow,__func__);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
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
void Step::vRegOperandCsvData(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  char* data = (char*)pCsvBuffer->ayDataArea();
  m_pDataStack->vPushStz((Stz*)data);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].firstRow@ -- fetch operand first row                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandFirstRowFetch(void)
{
  ENTRY();
  
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperandA);
  m_pRegSet->m_xpRow = pIrwBuffer->aFirstRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].handleAggEnd -- handle agg end                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandHandleAggEnd(void)
{
  ENTRY();
  
//  ASSERT(*m_pRegSet->m_xpOperandA);
//  
//  i8 nCount = m_pDataStack->iPopInt(__func__);
//  
//  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
//  
//  if (pOp->nDecLeftEosMarkers() == 0) {
//    if (pOp->bIsCancelling()) {
//      AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
//      pOp->vPipeOut(new Marker(OM_CANCELMARKER));
//    }
//    else {
//      
//      // create an IrwBuffer
//      IrwBuffer* pIrwBuffer = new IrwBuffer();
//      
//      Row* pRow = pIrwBuffer->aAllotRow(*m_pRegSet->m_xpStreamDef);
//      pRow->m_waColVal[0] = nCount;
//      
//      // pipe out the streamdef
//      pOp->vPipeOut(*m_pRegSet->m_xpStreamDef);
//      
//      // pipe out the buffer with the count
//      pOp->vPipeOut(pIrwBuffer);
//      
//      pOp->vPipeOut(*m_pRegSet->m_xpOperandA);
//    }
//  
//    pOp->vAckJob();
//    
//  }
//  else
//    AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperandA.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isReport -- is operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandIsReport(void)
{
  ENTRY();
  
  Operand* pOperand = *m_pRegSet->m_xpOperandA;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == TP_REPORT ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].itemCount@ -- fetch the operand item count                       */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandItemCountFetch(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperandA);
  
  m_pDataStack->vPushInt(pIrwBuffer->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].invokePython -- invoke python script                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandInvokePython(void)
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
void Step::vRegOperandInvokePythonShell(void)
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
  
  RASSERT(oSysCall.m_eStatus == SC_SUCCESS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandKind(void)
{
  ENTRY();
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_xpOperandA->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_INFOSET  : {
      InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpOperandA);
      
      switch(pInfoSet->eOpInfoSet()) {
        case OI_LOCKSTATS:      eOperand = ZK_LOCKSTATS;      break;
        case OI_NODEINFO:       eOperand = ZK_NODEINFO;       break;
        case OI_PROGRESSUNITS:  eOperand = ZK_PROGRESSUNITS;  break;
        case OI_SEEKOFFSETS:    eOperand = ZK_SEEKOFFSETS;    break;
        case OI_TOTALUNITS:     eOperand = ZK_TOTALUNITS;     break;
        default: ASSERT(0);
      }
      break;
    }
      
    case CN_IRWBUFFER:       eOperand = ZK_IRWBUFFER;       break;
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pRegSet->m_xpOperandA);

       switch(pMarker->eOpMarker()) {
       
         case OM_CANCELMARKER:    eOperand = ZK_CANCELMARKER;    break;
         case OM_EOSMARKER:       eOperand = ZK_EOSMARKER;       break;
         case OM_ERRORMARKER:     eOperand = ZK_ERRORMARKER;     break;
         case OM_ERROREDMARKER:   eOperand = ZK_ERROREDMARKER;   break;
         case OM_EXITMARKER:      eOperand = ZK_EXITMARKER;      break;
         case OM_EXITEDMARKER:    eOperand = ZK_EXITEDMARKER;    break;
         case OM_PULSEMARKER:     eOperand = ZK_PULSEMARKER;     break;
         case OM_PULSEDMARKER:    eOperand = ZK_PULSEDMARKER;    break;
         case OM_STARTMARKER:     eOperand = ZK_STARTMARKER;     break;
         case OM_STARTEDMARKER:   eOperand = ZK_STARTEDMARKER;   break;
         default:  ASSERT(0);
       }
      break;
    }
    case CN_SRTBUFFER:       eOperand = ZK_SRTBUFFER;       break;
    case CN_STREAMDEF:       eOperand = ZK_STREAMDEF;       break;
    case CN_STREAMSET:       eOperand = ZK_STREAMSET;       break;
      
    default:
      RASSERT(0);
      break;
  }
  
  m_pDataStack->vPushInt(eOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].next -- get next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNext(void)
{
  ENTRY();
  
  if (m_pRegSet->m_xpOperandA.bIsNull())
    m_pRegSet->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCsbBuffer -- new a csv buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewCsvBuffer(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new CsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEosMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewEosMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErrorMarker -- new an error marker                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewErrorMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_ERRORMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErroredMarker -- new an errored marker                        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewErroredMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_ERROREDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitMarker -- new an exit marker                              */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewExitMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_EXITMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitedMarker -- new an exited marker                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewExitedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_EXITEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulseMarker -- new a pulse marker                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewPulseMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_PULSEMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulsedMarker -- new a pulsed marker                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewPulsedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_PULSEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartMarker -- new a start marker                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStartMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STARTMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartedMarker -- new a started marker                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStartedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def from table[]                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStreamDef(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  if (m_pRegSet->m_xpTable.bIsNull())
    m_pRegSet->m_xpOperandA = new StreamDef();
  else
    m_pRegSet->m_xpOperandA = new StreamDef(m_pRegSet->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpOperandA,m_pTask);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOutSet -- append operand set onto next operator in pipeline  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeOutSet(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpOperandA,m_pTask, true);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToJob -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeToJob(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToJob(*m_pRegSet->m_xpOperandA);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].report -- mark operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandReport(void)
{
  ENTRY();
  
  Operand* pOperand = *m_pRegSet->m_xpOperandA;
  pOperand->vTailProcessing(TP_REPORT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandRowCountFetch(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  
  m_pDataStack->vPushInt(pCrwBuffer->nRowCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpOperandA.bIsNull());
  m_pRegSet->m_xpOperandA = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].upsertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandUpsertIntoTable(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  i8 iPartition = pOp->nPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  m_pRegSet->m_xpTable->vUpsertBuffer(C_UNKNOWN, iSlot, pCrwBuffer);
  
  // will delete this buffer
  pCrwBuffer->vIsPermanent(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwToCsv -- convert Crw operand to Csv                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCrwToCsv(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  m_pRegSet->m_xpOperandA = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrw -- convert csv operand to Crw                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCsvToCrw(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;

  Table* pTable = *m_pRegSet->m_xpTable;
  
  char* aCursor = (char*)pCsvBuffer->ayDataArea();
  CrwBuffer* pCrwBuffer = nullptr;
  
  bool bBlankLine = false;
  
  ExtractRow* pExtractRow = _pExtractRow(*m_pRegSet->m_xpRegister0);
  
  RowSingle* pRow;
  
  //i8 nRecords = pCsvBuffer->nRecords();
  //for (i8 i_Record = 0; i_Record < nRecords; i_Record++) {
  while(true) {
    bool bValid = pExtractRow->bExtractRow(&pRow, &aCursor, bBlankLine);
    
    if (bBlankLine)
      continue;
    
    if (! bValid ) {
      vTrace("Warning: Skipping row (invalid value)\n"_Stz);
      if (pCrwBuffer) {
        pCrwBuffer->vAppendClose();
        pOp->vPipeOut_nextOp(_pOperand(pCrwBuffer),m_pTask);
        pCrwBuffer = nullptr;
      }
      goto L_Exit;
    }
    
//    if (EC_MAX_ROW_LENGTH == eError) {
//      vTrace("Warning: Skipping row (Max row length)\n"_Stz);
//      continue;
//    }
    
    // use row hash to get partition
    i8 i_Ptn = pTable->iToPartition(pRow->m_iHash);
    
    // attempt to append row into a write-behind buffer
    eEC eError = pWriteBehind->eAppendRow(pRow, &pCrwBuffer, i_Ptn);
    
    // was there an error ?
    if (eError != EC_NONE) {
      vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eError]);
      continue;
    }
    
    if (pCrwBuffer) {
      pCrwBuffer->vAppendClose();
      pOp->vPipeOut_nextOp(_pOperand(pCrwBuffer),m_pTask);
      pCrwBuffer = nullptr;
    }
    
  }
 
L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrwH -- convert csv operand to CrwH                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCsvToCrwH(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Job* pPipelineJob = pOp->pPipeline()->pJob();
  
  pLoader->bReadRecords(pPipelineJob, (Stz*)pCsvBuffer->ayDataArea(), pCsvBuffer->nDataBytes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].workDone -- update work done                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandWorkDone(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  eCN eClass_op = m_pRegSet->m_xpOperandA->eClass();
  
  switch (eClass_op) {
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
      pOp->vAddWorkDone(pCrwBuffer->nRowCount());
      break;
    }
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
      pOp->vAddWorkDone(pCsvBuffer->nRecords());
      break;
    }
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(*m_pRegSet->m_xpOperandA);
      pOp->vAddWorkDone(pEntBuffer->nItems());
      break;
    }
      
    case CN_IRWBUFFER: {
      IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperandA);
      pOp->vAddWorkDone(pIrwBuffer->nItems());
      break;
    }
      
    case CN_SRTBUFFER: {
      SrtBuffer* pSrtBuffer = _pSrtBuffer(*m_pRegSet->m_xpOperandA);
      pOp->vAddWorkDone(pSrtBuffer->nItems());
      break;
    }
      
    case CN_STREAMDEF: {
      break;
    }
      
    default:
      RASSERT(0);
      break;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].writeToFile0 -- write to file0[]                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandWriteToFile0(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  ASSERT(*m_pRegSet->m_xpFile0);
  ASSERT(m_pRegSet->m_xpFile0->bIsOpen());
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA) ;
  char* aData = (char*)pCsvBuffer->ayDataArea();
  
  m_pRegSet->m_xpFile0->vPrint("%s"_Stz,aData);
  m_pRegSet->m_xpFile0->vFlush();
  
  // delete the buffer 
  //delete pCsvBuffer;
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
