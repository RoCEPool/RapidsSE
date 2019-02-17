/*============================================================================*/
/* Job_Export.cpp -- Export Table Job                                         */
/*============================================================================*/

// Job Short: Export

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Export =
{
  "Job_Export"_Stz,     // name of the job class
  CN_JOB_EXPORT,        // class enum
  JK_DIST2PC,           // job kind
  "export table"_Stz,   // comment about the job
  "export"_Stz,         // the prompt command
  nullptr,              // the second word
  nullptr,              // the _run task seed
  "export_req"_Stz,     // the _req task seed
  "export_beg"_Stz,     // the _rcv task seed
  "export_exe"_Stz,     // the _exe task seed
  "export_end"_Stz,     // the _end task seed
  "export_can"_Stz,     // the _can task seed
  false,                // check resources?
  false,                // detached job?
  false,                // buffer attached?
  JT_NONE,              // job trace
  
  eJob_checkCluster,    // prompt check function
  pJob_Export_pre,      // prompt pre-process function
  vJob_Export_post,     // prompt post-process function
  pJob_Export_new,      // job new function
  vJob_Export_delete,   // job delete function
  vJob_send,            // job pre-send function
  vJob_recv,            // job post-receive function
  vJob_Export_show,     // job show function
  
  // help and example
  true,
  "export <tableName> <fileName>"_Stz,
  "export SUPPLIER /tmp/supplier.csv;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Export_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_FetchRows_class;
  
  return &xJobClass_Export;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Export* Jobs::_pJob_Export(void* aAddress)
{
  Job_Export* pJob_Export = reinterpret_cast<Job_Export*>(aAddress);
  
  RASSERT_TYPE(pJob_Export,CN_JOB_EXPORT);
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Export::operator new(size_t)
{
  Job_Export* pJob_Export =
    static_cast<Job_Export*>(HeapMem::pAllocate(CN_JOB_EXPORT));
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Export::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Export::operator delete(void* aAddress)
{
  Job_Export* pJob_Export = static_cast<Job_Export*>(aAddress);
  
  HeapMem::vFree(pJob_Export);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Export::Job_Export(Interface::Session* pSession)
: Job(CN_JOB_EXPORT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  m_pTable = nullptr;
  
  m_okLock.vInitLock(LK_EXPORT);
    
  //m_oCsvFile;
  //m_oCompleter

  _checkInitialised_(CN_JOB_EXPORT);
  
  ActionSequence::vRoot(this);
  ActionSequence::vAdd(this,AN_CONSTRUCTOR,CN_JOB_EXPORT,_EG_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Export::~Job_Export(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  if (m_bWasReceived)
    m_oCsvFile.vReset();
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Export::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Export* pJob_Export = _pJob_Export(pObject);

  pJob_Export->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin export job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Export::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  ActionSequence::vAdd(this,AN_JOB_BEGIN);

  m_pTable->vMarkTable();
  
  if (! m_oCsvFile.bOpenWrite(m_sFilePath)) {
    vTrace("Failed to open file: %s\n"_Stz,m_sFilePath.aStz());
    return false;
  }
  
  ActionSequence::vAdd(this,AN_OPEN_FILE,m_sFilePath.aStz());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end export job                                                             */
/*----------------------------------------------------------------------------*/
void Job_Export::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);

  ActionSequence::vAdd(this,AN_JOB_END);
  ActionSequence::vAdd(this,AN_CLOSE_FILE,m_sFilePath.aStz());
  
  // m_oCsvFile.vClose();
  vTrace("WARNING: NOT CLOSING EXPORT FILE\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* inc no of buffers written                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Export::bIncWritten(i8 nBuffersExpected)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);

  bool bHasCompleted = m_oCompleter.bIncProcessed(nBuffersExpected);
  
  return bHasCompleted;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Export::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("export"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();

  // set the file name
  m_sFilePath = pTokenVec->pGet(2)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
void Job_Export::vWriteExportBuffer(ExportBuffer* pExportBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);

  char* aData = pExportBuffer->aDataArea();
  
  m_oCsvFile.vPrint("%s"_Stz,aData);
  m_oCsvFile.vFlush();
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Export::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  vTrace("  Job_Export %03lld: "
         "Table %s File %s \n"_Stz,
         
         m_iJobId,
         m_sObjectName.aStz(),
         m_sFilePath.aStz()
         );
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Export::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Export_pre(Interface::Session* pSession)
{
  Job_Export* pJob_Export = new Job_Export(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();

  bool bResult = pJob_Export->bPopulateJob(pTokenVec);

  if (! bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Export->m_bHasError = true;
    pJob_Export->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Export_post(Interface::Session* pSession, Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);

  ActionSequence::vAdd(pJob_Export,AN_POST_PROMPT,pJob_Export->m_bHasError);
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Export_new(Job* pArg)
{
  Job_Export* pJob_Export = new Job_Export(nullptr);
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Export_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);
  
  // now delete the job
  delete pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Export_show(Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);

  pJob_Export->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
