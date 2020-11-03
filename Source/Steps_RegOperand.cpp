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
  vAddStep("Operand[].asSrcCrwBuffer"_Stz, &Step::vRegOperandAsSrcCrwBuffer);
  vAddStep("Operand[].asSrcRowSlab"_Stz,   &Step::vRegOperandAsSrcRowSlab);
  vAddStep("Operand[].bide"_Stz,           &Step::vRegOperandBide);
  vAddStep("Operand[].csvAppend"_Stz,      &Step::vRegOperandCsvAppend);
  vAddStep("Operand[].csvData"_Stz,        &Step::vRegOperandCsvData);
  vAddStep("Operand[].csvToCrwH"_Stz,      &Step::vRegOperandCsvToCrwH);
  vAddStep("Operand[].csvToRsb"_Stz,       &Step::vRegOperandCsvToRsb);
  vAddStep("Operand[].crwDuplicate"_Stz,   &Step::vRegOperandCrwDuplicate);
  vAddStep("Operand[].crwToCsv"_Stz,       &Step::vRegOperandCrwToCsv);
  vAddStep("Operand[].crwToRsb"_Stz,       &Step::vRegOperandCrwToRsb);
  vAddStep("Operand[].delete"_Stz,         &Step::vRegOperandDelete);
  vAddStep("Operand[].insdelIntoTable"_Stz,&Step::vRegOperandInsdelIntoTable);
  vAddStep("Operand[].isNotNull"_Stz,      &Step::vRegOperandIsNotNull);
  vAddStep("Operand[].isNull"_Stz,         &Step::vRegOperandIsNull);
  vAddStep("Operand[].isReport"_Stz,       &Step::vRegOperandIsReport);
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
  vAddStep("Operand[].pipeIn"_Stz,         &Step::vRegOperandPipeIn);
  vAddStep("Operand[].pipeOut"_Stz,        &Step::vRegOperandPipeOut);
  vAddStep("Operand[].pipeOutSet"_Stz,     &Step::vRegOperandPipeOutSet);
  vAddStep("Operand[].pipeToJob"_Stz,      &Step::vRegOperandPipeToJob);
  vAddStep("Operand[].report"_Stz,         &Step::vRegOperandReport);
  vAddStep("Operand[].rowCount@"_Stz,      &Step::vRegOperandRowCountFetch);
  vAddStep("Operand[].rsbToCsv"_Stz,       &Step::vRegOperandRsbToCsv);
  vAddStep("Operand[].set"_Stz,            &Step::vRegOperandSet);
  vAddStep("Operand[].upsertIntoTable"_Stz,&Step::vRegOperandUpsertIntoTable);
  vAddStep("Operand[].writeToFile0"_Stz,   &Step::vRegOperandWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* Operand[].addColumnDef -- operand add column definition                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandAddColumnDef(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpOperandA);
  
  s256 sColumnNm;
  m_pDataStack->vPopStr(sColumnNm, __func__);
  
  eCT eColumnType;
  eColumnType = (eCT)m_pDataStack->iPopInt(__func__);
  
  pStreamDef->vAddColumn(sColumnNm, eColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].asSrcCrwBuffer -- use this operand as src crw buffer             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandAsSrcCrwBuffer(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);

  m_pLogicUnit->m_pDevSrcIxt->m_pCrwBuffer = pCrwBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].asSrcRowSlab -- use this operand as src row slab                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandAsSrcRowSlab(void)
{
  ENTRY();

  RsbBuffer* pRsbBuffer = _pRsbBuffer(*m_pLogicUnit->m_xpOperandA);

  m_pLogicUnit->m_pDevSrcRsb->m_pRowSlab = pRsbBuffer->pRowSlab();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBide(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pLogicUnit->m_xpOperandA.bIsNull()) {
    m_pTask->vScheduleBide();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwDuplicate -- duplicate crw buffer operand                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCrwDuplicate(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);

  CrwBuffer* pNewCrwBuffer = new CrwBuffer;
  pNewCrwBuffer->vCopy(pCrwBuffer);

  m_pDataStack->vPushPtr(pNewCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwToCsv -- convert Crw operand to Csv                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCrwToCsv(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
  m_pLogicUnit->m_xpOperandA = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].crwToRsb -- convert crw buffer operand to rsb buffer             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCrwToRsb(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  bool b = pToRsb->bConvertCrw(pCrwBuffer);
  
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
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
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
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  char* data = (char*)pCsvBuffer->ayDataArea();
  m_pDataStack->vPushStz((Stz*)data);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToCrwH -- convert csv operand to CrwH                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCsvToCrwH(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  Loader* pLoader = *m_pLogicUnit->m_xpLoader;
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Job* pPipelineJob = pOp->pPipeline()->pJob();
  
  pLoader->bCsvToCrw((Stz*)pCsvBuffer->ayDataArea(),
                     pCsvBuffer->nDataBytes(),
                     pPipelineJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].csvToRsb -- convert csv nuffer operand to rsb buffer             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCsvToRsb(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

  char* azCsvChunk = (char*) pCsvBuffer->szRecords();

  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  bool b = pToRsb->bConvertCsv(azCsvChunk,pCsvMeta);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpOperandA);
  
  m_pLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].insdelIntoTable -- insert-delete operand into the table          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandInsdelIntoTable(void)
{
//  ENTRY();
//
//  ASSERT(*m_pLogicUnit->m_xpOperandA);
//
//  Op* pOp = m_pTask->m_pJob->m_pOp;
//  i8 iPartition = pOp->iPartition();
//  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
//
//  if (CN_CRWBUFFER == (*m_pLogicUnit->m_xpOperandA)->eClass()) {
//    CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
//    // will delete this buffer
//    pCrwBuffer->vIsPermanent(false);
//    m_pLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, iSlot, pCrwBuffer);
//
//  }
//  else
//  if (CN_RSBBUFFER == (*m_pLogicUnit->m_xpOperandA)->eClass()) {
//    // will delete this buffer
//    RsbBuffer* pRsbBuffer = _pRsbBuffer(*m_pLogicUnit->m_xpOperandA);
//    pRsbBuffer->vIsPermanent(false);
//    m_pLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, iSlot, pRsbBuffer);
//  }
//  else {
//    NEVER_REACH;
//  }
//
//  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNotNull -- is operand not null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpOperandA.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpOperandA.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].isReport -- is operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandIsReport(void)
{
  ENTRY();
  
  Operand* pOperand = *m_pLogicUnit->m_xpOperandA;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == PT_REPORT ? true : false);
  
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
  
  eCN eClass_op = m_pLogicUnit->m_xpOperandA->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_INDEXDEF:        eOperand = ZK_INDEXDEF;        break;
    case CN_INFOSET  : {
      InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpOperandA);
      
      switch(pInfoSet->eOpInfoSet()) {
        case OI_CALLPIPELINE:   eOperand = ZK_CALLPIPELINE;   break;
        case OI_COUNTINFO:      eOperand = ZK_COUNTINFO;      break;
        case OI_LOCKSTATS:      eOperand = ZK_LOCKSTATS;      break;
        case OI_NODEINFO:       eOperand = ZK_NODEINFO;       break;
        case OI_PATHINFO:       eOperand = ZK_PATHINFO;       break;
        case OI_PROGRESSUNITS:  eOperand = ZK_PROGRESSUNITS;  break;
        case OI_RETURNPIPELINE: eOperand = ZK_RETURNPIPELINE; break;
        case OI_SEEKOFFSETS:    eOperand = ZK_SEEKOFFSETS;    break;
        case OI_TOTALUNITS:     eOperand = ZK_TOTALUNITS;     break;
        default: RASSERT(0);
      }
      break;
    }
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pLogicUnit->m_xpOperandA);

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
         default:  RASSERT(0);
       }
      break;
    }
    case CN_PROFILE:         eOperand = ZK_PROFILE;         break;
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
  
  if (m_pLogicUnit->m_xpOperandA.bIsNull())
    m_pLogicUnit->m_xpOperandA = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newCsbBuffer -- new a csv buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewCsvBuffer(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new CsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEosMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewEosMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErrorMarker -- new an error marker                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewErrorMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_ERRORMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newErroredMarker -- new an errored marker                        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewErroredMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_ERROREDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitMarker -- new an exit marker                              */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewExitMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_EXITMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newExitedMarker -- new an exited marker                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewExitedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_EXITEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulseMarker -- new a pulse marker                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewPulseMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_PULSEMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newPulsedMarker -- new a pulsed marker                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewPulsedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_PULSEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartMarker -- new a start marker                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStartMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_STARTMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStartedMarker -- new a started marker                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStartedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  m_pLogicUnit->m_xpOperandA = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def from Table[]                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNewStreamDef(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpOperandA.bIsNull());
  
  if (m_pLogicUnit->m_xpTable.bIsNull())
    m_pLogicUnit->m_xpOperandA = new StreamDef();
  else
    m_pLogicUnit->m_xpOperandA = new StreamDef(m_pLogicUnit->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeIn -- move pipein operand into operandA                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_pLogicUnit->m_xpPipein;

  m_pLogicUnit->m_xpPipein = nullptr;
    
  m_pLogicUnit->m_xpOperandA = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandA,m_pTask);
  
  m_pLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOutSet -- append operand set onto next operator in pipeline  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeOutSet(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandA,m_pTask, true);
  
  m_pLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToJob -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandPipeToJob(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToJob(*m_pLogicUnit->m_xpOperandA);
  
  m_pLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].report -- mark operand for report processing ?                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandReport(void)
{
  ENTRY();
  
  Operand* pOperand = *m_pLogicUnit->m_xpOperandA;
  pOperand->vTailProcessing(PT_REPORT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandRowCountFetch(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
  
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
  ASSERT(! pOperand || m_pLogicUnit->m_xpOperandA.bIsNull());
  m_pLogicUnit->m_xpOperandA = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].upsertIntoTable -- insert operand into the table                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandUpsertIntoTable(void)
{
//  ENTRY();
//  
//  ASSERT(*m_pLogicUnit->m_xpOperandA);
//  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
//  
//  Op* pOp = m_pTask->m_pJob->m_pOp;
//  i8 iPartition = pOp->iPartition();
//  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
//  
//  m_pLogicUnit->m_xpTable->vUpsertBuffer(C_UNKNOWN, iSlot, pCrwBuffer);
//  
//  // will delete this buffer
//  pCrwBuffer->vIsPermanent(false);
//  
//  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rsbToCsv -- convert rsbBuffer operand to csvBuffer               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandRsbToCsv(void)
{
  ENTRY();
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(*m_pLogicUnit->m_xpOperandA);
 
  m_pLogicUnit->m_xpOperandA = pRsbBuffer->pToCsvBuffer();
  
  delete pRsbBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].writeToFile0 -- write to file0[]                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandWriteToFile0(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  ASSERT(*m_pLogicUnit->m_xpFile0);
  ASSERT(m_pLogicUnit->m_xpFile0->bIsOpen());
  
  if (CN_CSVBUFFER == (*m_pLogicUnit->m_xpOperandA)->eClass()) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
    char* aData = (char*)pCsvBuffer->ayDataArea();
  
    m_pLogicUnit->m_xpFile0->vPrint("%s"_Stz,aData);
    //m_pLogicUnit->m_xpFile0->vFlush();
  }
  else
  if (CN_CRWBUFFER == (*m_pLogicUnit->m_xpOperandA)->eClass()) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
    
    byte* aCrwProper = aAdd<byte*>(pCrwBuffer,sizeof(DataBuffer));
    i8 nSizeCrwProper = sizeof(CrwBuffer) - sizeof(DataBuffer);

    bool b = m_pLogicUnit->m_xpFile0->bWriteBytes(aCrwProper,nSizeCrwProper);
    ASSERT(b);
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
