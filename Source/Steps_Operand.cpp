/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  
  vAddStep("Operand[].csvAppend"_Stz,      &Step::vOperandCsvAppend);
  vAddStep("Operand[].bide"_Stz,           &Step::vOperandBide);
  vAddStep("Operand[].chunkFile"_Stz,      &Step::vOperandChunkFile);
  vAddStep("Operand[].csvData"_Stz,        &Step::vOperandCsvData);
  vAddStep("Operand[].crwToCsv"_Stz,       &Step::vOperandCrwToCsv);
  vAddStep("Operand[].csvToCrw"_Stz,       &Step::vOperandCsvToCrw);
  vAddStep("Operand[].csvToCrwH"_Stz,      &Step::vOperandCsvToCrwH);
  vAddStep("Operand[].delete"_Stz,         &Step::vOperandDelete);
  vAddStep("Operand[].firstRow@"_Stz,      &Step::vOperandFirstRowFetch);
  vAddStep("Operand[].get"_Stz,            &Step::vOperandGet);
  vAddStep("Operand[].handleAggEnd"_Stz,   &Step::vOperandHandleAggEnd);
  vAddStep("Operand[].isNull"_Stz,         &Step::vOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vOperandIsReport);
  vAddStep("Operand[].itemCount@"_Stz,     &Step::vOperandItemCountFetch);
  vAddStep("Operand[].insertIntoTable"_Stz,&Step::vOperandInsertIntoTable);
  vAddStep("Operand[].invokePython"_Stz,   &Step::vOperandInvokePython);
  vAddStep("Operand[].invokePythonShell"_Stz, &Step::vOperandInvokePythonShell);
  vAddStep("Operand[].kind"_Stz,           &Step::vOperandKind);
  vAddStep("Operand[].newCsvBuffer"_Stz,   &Step::vOperandNewCsvBuffer);
  vAddStep("Operand[].newEosMarker"_Stz,   &Step::vOperandNewEosMarker);
  vAddStep("Operand[].newErrorMarker"_Stz, &Step::vOperandNewErrorMarker);
  vAddStep("Operand[].newPulseMarker"_Stz, &Step::vOperandNewPulseMarker);
  vAddStep("Operand[].newSeekOffsets"_Stz, &Step::vOperandNewSeekOffsets);
  vAddStep("Operand[].newStartMarker"_Stz, &Step::vOperandNewStartMarker);
  vAddStep("Operand[].newStartedMarker"_Stz, &Step::vOperandNewStartedMarker);
  vAddStep("Operand[].newStopMarker"_Stz,  &Step::vOperandNewStopMarker);
  vAddStep("Operand[].newStoppedMarker"_Stz, &Step::vOperandNewStoppedMarker);
  vAddStep("Operand[].newStreamDef"_Stz,   &Step::vOperandNewStreamDef);
  vAddStep("Operand[].next"_Stz,           &Step::vOperandNext);
  vAddStep("Operand[].pipeOut"_Stz,        &Step::vOperandPipeOut);
  vAddStep("Operand[].pipeOutSet"_Stz,     &Step::vOperandPipeOutSet);
  vAddStep("Operand[].pipeToJob"_Stz,      &Step::vOperandPipeToJob);
  vAddStep("Operand[].readChunk"_Stz,      &Step::vOperandReadChunk);
  vAddStep("Operand[].report"_Stz,         &Step::vOperandReport);
  vAddStep("Operand[].rowCount@"_Stz,      &Step::vOperandRowCountFetch);
  vAddStep("Operand[].seekSets@"_Stz,      &Step::vOperandSeekSetsFetch);
  vAddStep("Operand[].set"_Stz,            &Step::vOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vOperandUpsertIntoTable);
  vAddStep("Operand[].workDone"_Stz,       &Step::vOperandWorkDone);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBide(void)
{
  ENTRY(); // ENTRY("vOperandBide"_Stz);
  
  m_pRegSet->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pRegSet->m_xpOperandA.bIsNull()) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].chunkFile -- operand chunk file                                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandChunkFile(void)
{
  ENTRY(); // ENTRY("vOperandChunkFile"_Stz);
  
  // get the seekOffsets
  SeekOffsets* pSeekOffsets = _pSeekOffsets(*m_pRegSet->m_xpOperandA);
  
  // get the file we are chunking
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  // get the last chunk location
  i8 i_ChunkLoc = m_pRegSet->m_iCounter0;
  
  // get the Eof location
  i8 i_EofLoc = m_pRegSet->m_iCounter1;
  
  char* pReadBlock = (char*)MemLib::aMallocBlock(C_CHUNKBLOCK);
  
  i8 nChunkBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process chunks till eof or till we get C_SET_SEEKOFFSETS
  
  while (true) {
    
    // compute the location of the final block of the current chunk
    i8 i_FinalBlockLoc = i_ChunkLoc + C_LOAD_READCHUNK - C_CHUNKBLOCK;
    
    // the final block will be past the eof if the chunk is
    // less than maximum chunk size
    if (i_FinalBlockLoc > i_EofLoc) {
      
      // compute the number of bytes which belong to the current chunk
      nChunkBytes = i_EofLoc - i_ChunkLoc;
      
      bool bAddOffset = pSeekOffsets->bAddOffset(i_ChunkLoc, nChunkBytes);
      ASSERT(bAddOffset);
    
      s32 sFileSize;
      MagLib::vByteMag(i_EofLoc, 3, sFileSize);
      vTrace("Loading %s %s in %lld chunks by %lld nodes %lld partitions\n"_Stz,
             pFile->sFilePath().szStz(),sFileSize.szStz(),
             pSeekOffsets->nOffsets(),
             AM::nDefinedNodes(),
             AM::nPartitions());
      
      // set the chunk location
      m_pRegSet->m_iCounter0 = i_ChunkLoc + nChunkBytes;
      
      break;
    }
    
    // read next block of the chunk
    pFile->vSeek(i_FinalBlockLoc);
    
    i8 nBytesRead = pFile->nReadBytes(pReadBlock,C_CHUNKBLOCK);
    
    // compute last actual byte of final block just read in
    char *aLastByte = &pReadBlock[nBytesRead - 1];
    
    // find the final end of line for the block (and therefore the chunk)
    char *aFinalEol = aLastByte;
    bool bFoundEol = StrLib::bBackward(&aFinalEol,pReadBlock,'\n');
    if (!bFoundEol)
      bFoundEol = StrLib::bBackward(&aFinalEol,pReadBlock,'\r');
    RASSERT(bFoundEol);
    
    // compute the number of bytes which belong to the current chunk
    i8 nPartialBytes = Correctness::nSub((void*)aFinalEol,pReadBlock) + 1;
    
    // compute the total chunk bytes
    nChunkBytes = nPartialBytes + C_WSP_DATACHUNK - 2048;
    
    // compute end location of chunk
    i8 i_EndChunkLoc = i_ChunkLoc + nChunkBytes - 1;
    
    bool bAddOffset = pSeekOffsets->bAddOffset(i_ChunkLoc, nChunkBytes);
    ASSERT(bAddOffset);
    
    // advance to start of next chunk
    i_ChunkLoc = i_EndChunkLoc + 1;
    
    // have we reached C_SET_OFFSETS ?
    if (pSeekOffsets->bIsFull()) {
      
      // save the current chunk location
      m_pRegSet->m_iCounter0 = i_ChunkLoc;
      
      // dump
      //pSeekOffsets->vDump();
      
      break;
    }
  }
  
  // dump
  //pSeekOffsets->vDump();
  
  // free the read block
  MemLib::vFreeBlock(pReadBlock, C_2K);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvAppend -- append a csv row to the csv operand                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvAppend(void)
{
  ENTRY(); // ENTRY("vOperandCsvAppend"_Stz);
  
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
void Step::vOperandCsvData(void)
{
  ENTRY(); // ENTRY("vOperandCsvData"_Stz);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
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
  
  AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].firstRow@ -- fetch operand first row                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandFirstRowFetch(void)
{
  ENTRY(); // ENTRY("vOperandFirstRowFetch"_Stz);
  
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperandA);
  m_pRegSet->m_xpRow = pIrwBuffer->aFirstRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandGet(void)
{
  ENTRY(); // ENTRY("vOperandGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].handleAggEnd -- handle agg end                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandHandleAggEnd(void)
{
  ENTRY(); // ENTRY("vOperandHandleAggEnd"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  i8 nCount = m_pDataStack->iPopInt(__func__);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  if (pOp->nDecLeftEosMarkers() == 0) {
    if (pOp->bIsCancelling()) {
      AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
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
      
      pOp->vPipeOut(*m_pRegSet->m_xpOperandA);
    }
  
    pOp->vAckJob();
    
  }
  else
    AM::vDeleteOperand(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsNull(void)
{
  ENTRY(); // ENTRY("vOperandIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperandA.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isReport -- is operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsReport(void)
{
  ENTRY(); // ENTRY("vOperandIsReport"_Stz);
  
  Operand* pOperand = *m_pRegSet->m_xpOperandA;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == TP_REPORT ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].itemCount@ -- fetch the operand item count                       */
/*----------------------------------------------------------------------------*/
void Step::vOperandItemCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandItemCountFetch"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  IrwBuffer* pIrwBuffer = _pIrwBuffer(*m_pRegSet->m_xpOperandA);
  
  m_pDataStack->vPushInt(pIrwBuffer->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].insertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandInsertIntoTable(void)
{
  ENTRY(); // ENTRY("vOperandInsertIntoTable"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  
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
/* Operand[].invokePythonShell -- invoke python script from shell             */
/*----------------------------------------------------------------------------*/
void Step::vOperandInvokePythonShell(void)
{
  ENTRY(); // ENTRY("vOperandInvokePython"_Stz);
  
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
void Step::vOperandKind(void)
{
  ENTRY(); // ENTRY("vOperandKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_xpOperandA->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_IRWBUFFER:       eOperand = ZK_IRWBUFFER;       break;
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pRegSet->m_xpOperandA);

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
    case CN_SEEKOFFSETS :    eOperand = ZK_SEEKOFFSETS;     break;
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
  
  if (m_pRegSet->m_xpOperandA.bIsNull())
    m_pRegSet->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCsbBuffer -- new a csv buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewCsvBuffer(void)
{
  ENTRY(); // ENTRY("vOperandNewCsvBuffer"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new CsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEosMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEosMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewEosMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErrorMarker -- new an error marker                            */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewErrorMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewErrorMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_ERRORMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulseMarker -- new a pulse marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewPulseMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewPulseMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_PULSEMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newSeekOffsets -- new a seek offset operand                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewSeekOffsets(void)
{
  ENTRY(); // ENTRY("vOperandNewSeekOffset"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new SeekOffsets;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartMarker -- new a start marker                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStartMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STARTMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartedMarker -- new a started marker                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStartedMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStartedMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStopMarker -- new a stop marker                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStopMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStopMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STOPMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStopppedMarker -- new a stopped marker                        */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStoppedMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewStoppedMarker"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new Marker(OM_STOPPEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def from table[]                    */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStreamDef(void)
{
  ENTRY(); // ENTRY("vOperandNewStreamDef"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandA.bIsNull());
  
  m_pRegSet->m_xpOperandA = new StreamDef(m_pRegSet->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeOut(*m_pRegSet->m_xpOperandA);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOutSet -- append operand set onto next operator in pipeline  */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOutSet(void)
{
  ENTRY(); // ENTRY("vOperandPipeOutSet"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut(*m_pRegSet->m_xpOperandA, true);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToJob -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeToJob(void)
{
  ENTRY(); // ENTRY("vOperandPipeToJob"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeToJob(*m_pRegSet->m_xpOperandA);
  
  m_pRegSet->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].readChunk -- reach a chunk from file given the seekOffsets       */
/*----------------------------------------------------------------------------*/
void Step::vOperandReadChunk(void)
{
  ENTRY(); // ENTRY("vOperandReadChunk"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  SeekOffsets* pSeekOffsets = _pSeekOffsets(*m_pRegSet->m_xpOperandA);
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  i8 iEntry = m_pDataStack->iPopInt(__func__);
  
  i8 nBytes = pSeekOffsets->nBytes(iEntry);
  i8 iOffset = pSeekOffsets->iOffset(iEntry);
  
  //vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  pFile->vSeek(iOffset);
  pCsvBuffer->vTake(nBytes);
  i8 nBytesRead = pFile->nReadBytes(pCsvBuffer->ayDataArea(),nBytes);
  ASSERT(nBytesRead == nBytes);
  
  m_pRegSet->m_xpOperandB = pCsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].report -- mark operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandReport(void)
{
  ENTRY(); // ENTRY("vOperandReport"_Stz);
  
  Operand* pOperand = *m_pRegSet->m_xpOperandA;
  pOperand->vTailProcessing(TP_REPORT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandRowCountFetch"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  
  m_pDataStack->vPushInt(pCrwBuffer->nRowCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandSeekSetsFetch(void)
{
  ENTRY(); // ENTRY("vOperandSeekSetsFetch"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  SeekOffsets* pSeekOffsets = _pSeekOffsets(*m_pRegSet->m_xpOperandA);
  
  m_pDataStack->vPushInt(pSeekOffsets->nOffsets());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandSet(void)
{
  ENTRY(); // ENTRY("vOperandSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpOperandA.bIsNull());
  m_pRegSet->m_xpOperandA = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].upsertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandUpsertIntoTable(void)
{
  ENTRY(); // ENTRY("vOperandUpsertIntoTable"_Stz);
  
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
void Step::vOperandCrwToCsv(void)
{
  ENTRY(); // ENTRY("vOperandCrwToCsv"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandA);
  m_pRegSet->m_xpOperandA = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrw -- convert csv operand to Crw                           */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvToCrw(void)
{
  ENTRY(); // ENTRY("vOperandCsvToCrw"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;

  Table* pTable = *m_pRegSet->m_xpTable;
  
  char* aCursor = (char*)pCsvBuffer->ayDataArea();
  CrwBuffer* pCrwBuffer = nullptr;
  
  bool bBlankLine = false;
  
  ExtractRow* pExtractRow = _pExtractRow(*m_pRegSet->m_xpRegister0);
  
  Row* pRow;
  
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
        pOp->vPipeOut(_pOperand(pCrwBuffer));
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
      pOp->vPipeOut(_pOperand(pCrwBuffer));
      pCrwBuffer = nullptr;
    }
    
  }
 
L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrwH -- convert csv operand to CrwH                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandCsvToCrwH(void)
{
  ENTRY(); // ENTRY("vOperandCsvToCrwH"_Stz);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  pLoader->bReadRecords((Stz*)pCsvBuffer->ayDataArea(), pCsvBuffer->nDataBytes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].workDone -- update work done                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandWorkDone(void)
{
  ENTRY(); // ENTRY("vOperandWorkDone"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
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
      
    case CN_SEEKOFFSETS: {
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
void Step::vOperandWriteToFile0(void)
{
  ENTRY(); // ENTRY("vOperandWriteToFile0"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  ASSERT(*m_pRegSet->m_xpFile0);
  ASSERT(m_pRegSet->m_xpFile0->bIsOpen());
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA) ;
  char* aData = (char*)pCsvBuffer->ayDataArea();
  
  m_pRegSet->m_xpFile0->vPrint("%s"_Stz,aData);
  m_pRegSet->m_xpFile0->vFlush();
  
  // delete the buffer 
  delete pCsvBuffer;
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
