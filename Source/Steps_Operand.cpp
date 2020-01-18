/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("Operand[].bide"_Stz,           &Step::vOperandBide);
  vAddStep("Operand[].csvData"_Stz,        &Step::vOperandCsvData);
  vAddStep("Operand[].delete"_Stz,         &Step::vOperandDelete);
  vAddStep("Operand[].firstRow@"_Stz,      &Step::vOperandFirstRowFetch);
  vAddStep("Operand[].get"_Stz,            &Step::vOperandGet);
  vAddStep("Operand[].handleAggEnd"_Stz,   &Step::vOperandHandleAggEnd);
  vAddStep("Operand[].isNull"_Stz,         &Step::vOperandIsNull);
  vAddStep("Operand[].itemCount@"_Stz,     &Step::vOperandItemCountFetch);
  vAddStep("Operand[].insertIntoTable"_Stz,&Step::vOperandInsertIntoTable);
  vAddStep("Operand[].invokePython"_Stz,   &Step::vOperandInvokePython);
  vAddStep("Operand[].kind"_Stz,           &Step::vOperandKind);
  vAddStep("Operand[].newEosMarker"_Stz,   &Step::vOperandNewEosMarker);
  vAddStep("Operand[].newErrorMarker"_Stz, &Step::vOperandNewErrorMarker);
  vAddStep("Operand[].newPulseMarker"_Stz, &Step::vOperandNewPulseMarker);
  vAddStep("Operand[].newStartMarker"_Stz, &Step::vOperandNewStartMarker);
  vAddStep("Operand[].newStartedMarker"_Stz, &Step::vOperandNewStartedMarker);
  vAddStep("Operand[].newStopMarker"_Stz,  &Step::vOperandNewStopMarker);
  vAddStep("Operand[].newStoppedMarker"_Stz, &Step::vOperandNewStoppedMarker);
  vAddStep("Operand[].newStreamDef"_Stz,   &Step::vOperandNewStreamDef);
  vAddStep("Operand[].next"_Stz,           &Step::vOperandNext);
  vAddStep("Operand[].pipeOut"_Stz,        &Step::vOperandPipeOut);
  vAddStep("Operand[].pipeToJob"_Stz,      &Step::vOperandPipeToJob);
  vAddStep("Operand[].rowCount@"_Stz,      &Step::vOperandRowCountFetch);
  vAddStep("Operand[].set"_Stz,            &Step::vOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vOperandUpsertIntoTable);
  vAddStep("Operand[].crwToCsv"_Stz,       &Step::vOperandCrwToCsv);
  vAddStep("Operand[].csvToCrw"_Stz,       &Step::vOperandCsvToCrw);
  vAddStep("Operand[].workDone"_Stz,       &Step::vOperandWorkDone);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBide(void)
{
  ENTRY(); // ENTRY("vOperandBide"_Stz);
  
  m_pRegSet->m_xpOperand = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pRegSet->m_xpOperand.bIsNull()) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvData -- return operand csv data                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvData(void)
{
  ENTRY(); // ENTRY("vOperandCsvData"_Stz);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperand);
  char* data = (char*)pCsvBuffer->ayDataArea();
  m_pDataStack->vPushStz((Stz*)data);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandDelete(void)
{
  ENTRY(); // ENTRY("vOperandDelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpOperand);
  
  m_pRegSet->m_xpOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].firstRow@ -- fetch operand first row                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandFirstRowFetch(void)
{
  ENTRY(); // ENTRY("vOperandFirstRowFetch"_Stz);
  
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperand);
  m_pRegSet->m_xpRow = pIrwBuffer->aFirstRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandGet(void)
{
  ENTRY(); // ENTRY("vOperandGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].handleAggEnd -- handle agg end                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandHandleAggEnd(void)
{
  ENTRY(); // ENTRY("vOperandHandleAggEnd"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  
  i8 nCount = m_pDataStack->iPopInt(__func__);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  if (pOp->nDecLeftEosMarkers() == 0) {
    if (pOp->bIsCancelling()) {
      AM::vDeleteOperand(*m_pRegSet->m_xpOperand);
      pOp->vPipeOut(new Marker(OM_CANCELMARKER));
    }
    else {
      
      // create an IrwBuffer
      IrwBuffer* pIrwBuffer = new IrwBuffer();
      
      Row* pRow = pIrwBuffer->aAllotRow(*m_pRegSet->m_xpStreamDef);
      pRow->m_waColVal[0] = nCount;
      
      // pipe out the streamdef
      pOp->vPipeOut(*m_pRegSet->m_xpStreamDef);
      
      // pipe out the buffer with the count
      pOp->vPipeOut(pIrwBuffer);
      
      pOp->vPipeOut(*m_pRegSet->m_xpOperand);
    }
  
    pOp->vAckJob();
    
  }
  else
    AM::vDeleteOperand(*m_pRegSet->m_xpOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsNull(void)
{
  ENTRY(); // ENTRY("vOperandIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperand.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].itemCount@ -- fetch the operand item count                       */
/*----------------------------------------------------------------------------*/
void Step::vOperandItemCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandItemCountFetch"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperand);
  
  m_pDataStack->vPushInt(pIrwBuffer->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].insertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandInsertIntoTable(void)
{
  ENTRY(); // ENTRY("vOperandInsertIntoTable"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  i8 iPartition = pOp->nPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  m_pRegSet->m_xpTable->vAppendBufferInsertEntries(C_UNKNOWN, iSlot, pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].invokePython -- invoke python script                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandInvokePython(void)
{
  ENTRY(); // ENTRY("vOperandInvokePython"_Stz);

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
/* Operand[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandKind(void)
{
  ENTRY(); // ENTRY("vOperandKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_xpOperand->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_IRWBUFFER:       eOperand = ZK_IRWBUFFER;       break;
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pRegSet->m_xpOperand);

       switch(pMarker->eOpMarker()) {
       
         case OM_EOSMARKER:       eOperand = ZK_EOSMARKER;       break;
         case OM_ERRORMARKER:     eOperand = ZK_ERRORMARKER;     break;
         case OM_CANCELMARKER:    eOperand = ZK_CANCELMARKER;    break;
         case OM_PULSEMARKER:     eOperand = ZK_PULSEMARKER;     break;
         case OM_STARTMARKER:     eOperand = ZK_STARTMARKER;     break;
         case OM_STARTEDMARKER:   eOperand = ZK_STARTEDMARKER;   break;
         case OM_STOPMARKER:      eOperand = ZK_STOPMARKER;      break;
         case OM_STOPPEDMARKER:   eOperand = ZK_STOPPEDMARKER;   break;
         default:  ASSERT(0);
       }
      break;
    }
    case CN_SRTBUFFER:       eOperand = ZK_SRTBUFFER;       break;
    case CN_STREAMDEF:       eOperand = ZK_STREAMDEF;       break;
      
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
void Step::vOperandNext(void)
{
  ENTRY(); // ENTRY("vOperandNext"_Stz);
  
  if (m_pRegSet->m_xpOperand.bIsNull())
    m_pRegSet->m_xpOperand = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pRegSet->m_xpOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEosMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEosMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewEosMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErrorMarker -- new an error marker                            */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewErrorMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewErrorMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_ERRORMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulseMarker -- new a pulse marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewPulseMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewPulseMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_PULSEMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartMarker -- new a start marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStartMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_STARTMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartedMarker -- new a started marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartedMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStartedMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStopMarker -- new a stop marker                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStopMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStopMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_STOPMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStopppedMarker -- new a stopped marker                        */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStoppedMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStoppedMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new Marker(OM_STOPPEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def from table[]                    */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStreamDef(void)
{
  ENTRY(); // ENTRY("vOperandNewStreamDef"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperand.bIsNull());
  
  m_pRegSet->m_xpOperand = new StreamDef(m_pRegSet->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeOut(*m_pRegSet->m_xpOperand);
  
  m_pRegSet->m_xpOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToJob -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeToJob(void)
{
  ENTRY(); // ENTRY("vOperandPipeToJob"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeToJob(*m_pRegSet->m_xpOperand);
  
  m_pRegSet->m_xpOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandRowCountFetch"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperand);
  
  m_pDataStack->vPushInt(pCrwBuffer->nRowCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandSet(void)
{
  ENTRY(); // ENTRY("vOperandSet"_Stz);
  
  ASSERT(m_pRegSet->m_xpOperand.bIsNull());
  m_pRegSet->m_xpOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].upsertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandUpsertIntoTable(void)
{
  ENTRY(); // ENTRY("vOperandUpsertIntoTable"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  i8 iPartition = pOp->nPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  m_pRegSet->m_xpTable->vUpsertBuffer(C_UNKNOWN, iSlot, pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwToCsv -- convert Crw operand to Csv                           */
/*----------------------------------------------------------------------------*/
void Step::vOperandCrwToCsv(void)
{
  ENTRY(); // ENTRY("vOperandCrwToCsv"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperand);
  m_pRegSet->m_xpOperand = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrw -- convert csv operand to Crw                           */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvToCrw(void)
{
  ENTRY(); // ENTRY("vOperandCsvToCrw"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperand);
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;
  Table* pTable = *m_pRegSet->m_xpTable;
  
  char* aCursor = (char*)pCsvBuffer->ayDataArea();
  CrwBuffer* pCrwBuffer = nullptr;
  
  bool bBlankLine = false;
  
  ExtractRow* pExtractRow = _pExtractRow(*m_pRegSet->m_xpRegister0);
  
  Row* pRow;
  
  i8 nRecords =pCsvBuffer->nRecords();
  for (i8 i_Record = 0; i_Record < nRecords; i_Record++) {
    
    bool bValid = pExtractRow->bExtractRow(&pRow, &aCursor, bBlankLine);
    
    if (bBlankLine)
      continue;
    
    if (! bValid ) {
      vTrace("Warning: Skipping row (invalid value)\n"_Stz);
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
      pOp->vPipeOut(_pOperand(pCrwBuffer));
      pCrwBuffer = nullptr;
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].workDone -- update work done                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandWorkDone(void)
{
  ENTRY(); // ENTRY("vOperandWorkDone"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  eCN eClass_op = m_pRegSet->m_xpOperand->eClass();
  
  switch (eClass_op) {
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperand);
      pOp->vAddWorkDone(pCrwBuffer->nRowCount());
      break;
    }
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperand);
      pOp->vAddWorkDone(pCsvBuffer->nRecords());
      break;
    }
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(*m_pRegSet->m_xpOperand);
      pOp->vAddWorkDone(pEntBuffer->nItems());
      break;
    }
      
    case CN_IRWBUFFER: {
      IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperand);
      pOp->vAddWorkDone(pIrwBuffer->nItems());
      break;
    }
      
    case CN_SRTBUFFER: {
      SrtBuffer* pSrtBuffer = _pSrtBuffer(*m_pRegSet->m_xpOperand);
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
void Step::vOperandWriteToFile0(void)
{
  ENTRY(); // ENTRY("vOperandWriteToFile0"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperand);
  ASSERT(*m_pRegSet->m_xpFile0);
  ASSERT(m_pRegSet->m_xpFile0->bIsOpen());
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperand) ;
  char* aData = (char*)pCsvBuffer->ayDataArea();
  
  m_pRegSet->m_xpFile0->vPrint("%s"_Stz,aData);
  m_pRegSet->m_xpFile0->vFlush();
  
  // delete the buffer 
  delete pCsvBuffer;
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
