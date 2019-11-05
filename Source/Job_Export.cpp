/*============================================================================*/
/* Job_Export.cpp -- Export Table Job                                         */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Export =
{
  CN_JOB_EXPORT,        // class number
  JK_DIST2PC,           // job kind
  RS_NONE,              // response kind
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
  DL_GRAB_READ,         // ddl lock model
  PY_NORMAL,            // priority
  JF_LOWMEM,            // flags

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
  "export <tableNm> \"<fileNm>\""_Stz,
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
Job_Export::Job_Export(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
, m_okLock(LK_EXPORT,this)
, m_oCsvFile(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  m_pTable = nullptr;
      
  //m_oCsvFile;
  //m_oCompleter
  m_bAuxAckDecided = false;

  _checkInitialised_(CN_JOB_EXPORT);
  
  //ActLog::vRoot(this);
  //ActLog::vAdd(this,AN_CONSTRUCT,CN_JOB_EXPORT,_EG_CN);
  
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
  
  //ActLog::vAdd(this,AN_JOB_BEGIN);

  m_pTable->vMarkTable();
  
  if (! m_oCsvFile.bOpenWrite(szFilePath())) {
    vTrace("Failed to open file: %s\n"_Stz,szFilePath());
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end export job                                                             */
/*----------------------------------------------------------------------------*/
void Job_Export::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);

  //ActLog::vAdd(this,AN_JOB_END);
  
  m_oCsvFile.vClose();
  vTrace("CLOSING EXPORT FILE\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* inc no of buffers written                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Export::bIncWritten(i8 nBuffersExpected)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);

  bool bLastWrite = m_oWritten.bIncProcessed(nBuffersExpected);
  
  return bLastWrite;
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
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  // set the file name
  vFilePath(pTokenVec->pGet(2)->szText());

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
  
//  vTrace("  Job_Export %03lld: "
//         "Table %s File %s \n"_Stz,
//
//         m_iFocalId,
//         szObjectName(),
//         szFilePath()
//         );
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
Job* Jobs::pJob_Export_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Export* pJob_Export = new Job_Export(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();

  bool bResult = pJob_Export->bPopulateJob(pTokenVec);

  if (! bResult) {
    pJob_Export->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Export_post(Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);

  //ActLog::vAdd(pJob_Export,AN_POST_PROMPT,pJob_Export->m_bHasError);
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Export_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Export* pJob_Export = new Job_Export(aJobClass,nullptr);
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Export_delete(Job* pJob)
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
