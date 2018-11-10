/*============================================================================*/
/* Job_NetHog.cpp -- Network Hog Job                                          */
/*============================================================================*/

// Job Short: NetHog

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_NetHog =
{
  "Job_NetHog"_Stz,      // name of the job class
  CN_JOB_NETHOG,         // class enum
  JK_SYSTEM,             // job kind
  "load table"_Stz,      // comment about the job
  "ld"_Stz,              // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "netHog_req"_Stz,      // the _req task seed
  "netHog_beg"_Stz,      // the _rcv task seed
  "netHog_exe"_Stz,      // the _exe task seed
  "netHog_end"_Stz,      // the _end task seed
  "netHog_can"_Stz,      // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_NetHog_pre,       // prompt pre-process function
  vJob_NetHog_post,      // prompt post-process function
  pJob_NetHog_new,       // job new function
  vJob_NetHog_delete,    // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_NetHog_show,      // job show function
  
  // help and example
  true,
  "nethog <nHogs>"_Stz,
  "nethog 73;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_NetHog_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_NodeInfo_class;
  
  return &xJobClass_NetHog;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_NetHog* JobsModel::_pJob_NetHog(void* aAddress)
{
  Job_NetHog* pJob_NetHog = reinterpret_cast<Job_NetHog*>(aAddress);
  
  RASSERT_TYPE(pJob_NetHog,CN_JOB_NETHOG);
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_NetHog::operator new(size_t)
{
  Job_NetHog* pJob_NetHog =
    static_cast<Job_NetHog*>(HeapMem::pAllocate(CN_JOB_NETHOG));
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_NetHog::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_NetHog::operator delete(void* aAddress)
{
  Job_NetHog* pJob_NetHog = static_cast<Job_NetHog*>(aAddress);
  
  HeapMem::vFree(pJob_NetHog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_NetHog::Job_NetHog(Interface::Session* pSession)
: Job(CN_JOB_NETHOG, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  m_pTable        = nullptr;
  m_xFormatters.vReset(); // reset to defaults in attachmap
  
  m_pLoadMap      = nullptr;
  
  _checkInitialised_(CN_JOB_NETHOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_NetHog::~Job_NetHog(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  delete m_pLoadMap;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pObject);

  pJob_NetHog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the load map                                                          */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vDumpMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  vTrace("Load Map for: %s:\n"_Stz,m_sFileName.aStr());
  
  m_pLoadMap->vDumpMap();
}

/*----------------------------------------------------------------------------*/
/* find next chunk in file being loaded                                       */
/*----------------------------------------------------------------------------*/
bool Job_NetHog::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  if (! m_pLoadMap) {
    m_pLoadMap = new LoadMap;
    m_pLoadMap->vFilePath(m_sFileName);
  }
  
  bool b = m_pLoadMap->bFindChunk();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_NetHog::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("ld"_Stz));
  
  // get no of tokens in token vector
  i8 nTokens = pTokenVec->nTokens();;
  
  // there should be at least 3 tokens
  if (nTokens < 3)
    return nullptr;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // set the file name
  m_sFileName = pTokenVec->pGet(2)->aText();
  
  i8 i_Token = 3;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (i_Token < nTokens) {
    
    if (i_Token + 2 > nTokens)
       return false;
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_RECORD_TERMINATOR
    if (pTokenVec->pGet(i_Token)->bSame(C_RECORD_TERMINATOR)) {
      m_xFormatters.m_cRecordTerminator = *pTokenVec->pGet(++i_Token)->sText().aChar();
      if (m_xFormatters.m_cRecordTerminator == '0')
        m_xFormatters.m_cRecordTerminator = '\0';
    }
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_RECORD_ENDS
    else if (pTokenVec->pGet(i_Token)->bSame(C_RECORD_ENDS)) {
      char cRecordEnds = *pTokenVec->pGet(++i_Token)->sText().aChar();
      
      // is record_ends 1 character ?
      if (pTokenVec->pGet(i_Token)->iLength() == 1) {
        if (cRecordEnds == '0') {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          return false;
        }
        else if (cRecordEnds == '\n') {
          m_xFormatters.m_eRecordEnds = RE_NL;
        }
        else if (cRecordEnds == '\r') {
          m_xFormatters.m_eRecordEnds = RE_CR;
        }
      }
      // record_ends is 2 characters
      else if (pTokenVec->pGet(i_Token)->iLength() == 2) {
        
        if (pTokenVec->pGet(i_Token)->bSame("\r\n"_Stz)) {
          m_xFormatters.m_eRecordEnds = RE_CRNL;
        }
        else {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          return false;
        }
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_FIELD_DELIMITER
    else if (pTokenVec->pGet(i_Token)->bSame(C_FIELD_DELIMITER)) {
      s32 sFieldDelimiter;
      sFieldDelimiter = pTokenVec->pGet(++i_Token)->sText();
      if (sFieldDelimiter.bSame("\\t"_Stz))
        m_xFormatters.m_cFieldDelimiter = '\t';
      else
        m_xFormatters.m_cFieldDelimiter = *sFieldDelimiter.aChar();
      if (m_xFormatters.m_cFieldDelimiter == '0')
      m_xFormatters.m_cFieldDelimiter = '\0';
    }
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_ENCLOSED_BY_LEADING
    else if (pTokenVec->pGet(i_Token)->bSame(C_ENCLOSED_BY_LEADING)) {
      m_xFormatters.m_cEnclosedBy_Leading =
      *pTokenVec->pGet(++i_Token)->sText().aChar();
      if (m_xFormatters.m_cEnclosedBy_Leading == '0')
        m_xFormatters.m_cEnclosedBy_Leading = '\0';
      
      // default set trailing same as leading
      m_xFormatters.m_cEnclosedBy_Trailing =
      m_xFormatters.m_cEnclosedBy_Leading;
    }
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_ENCLOSED_BY_TRAILING
    else if (pTokenVec->pGet(i_Token)->bSame(C_ENCLOSED_BY_TRAILING)) {
      
      // if trailing is specified, leading should also have been specified
      if (m_xFormatters.m_cEnclosedBy_Leading == '\0') {
        if ( ! gpAttachMap->m_bMetadata)
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        return false;
      }
      m_xFormatters.m_cEnclosedBy_Trailing =
      *pTokenVec->pGet(++i_Token)->sText().aChar();
      if (m_xFormatters.m_cEnclosedBy_Trailing == '0')
        m_xFormatters.m_cEnclosedBy_Trailing = '\0';
    }
    //
    //    DATE_FORMAT='YYYY.MM.DD’
    //
    //
    // - - - - - - - - - - - - - - - - - - - - -
    // C_DATE_FORMAT
    else if (pTokenVec->pGet(i_Token)->bSame(C_DATE_FORMAT)) {
      // extract date format
      if ( ! bExtractDateFormat(pTokenVec->pGet(++i_Token)->sText()) ) {
        if ( ! gpAttachMap->m_bMetadata)
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        return false;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - -
    // C_ESCAPE_CHAR
    else if (pTokenVec->pGet(i_Token)->bSame(C_ESCAPE_CHAR)) {
      m_xFormatters.m_cEscapeChar = *pTokenVec->pGet(++i_Token)->sText().aChar();
      if (m_xFormatters.m_cEscapeChar == '0')
        m_xFormatters.m_cEscapeChar = '\0';
    }
    else {
      return false;
    }
    
    i_Token++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_NetHog::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_NetHog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract the date separator                                                 */
/*----------------------------------------------------------------------------*/
bool Job_NetHog::bExtractDateFormat(Stz* dateFormat)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  i8 iIndex = 0;
  
  // leading YYYY
  if (*dateFormat->aChar() == 'Y') {
    
    // expect YYYY
    if ( (*(dateFormat + iIndex++)->aChar() != 'Y') ||
        (*(dateFormat + iIndex++)->aChar() != 'Y') ||
        (*(dateFormat + iIndex++)->aChar() != 'Y') ||
        (*(dateFormat + iIndex++)->aChar() != 'Y') )
    return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_xFormatters.m_cDateSeparator = *pSeparator->aChar();
    
    // is next character 'M' ?
    if (*(dateFormat + iIndex)->aChar() == 'M') {
      
      // FORMAT should be YYYY-MM-DD
      // expect another M
      if ( (*(dateFormat + iIndex++)->aChar() != 'M') ||
          (*(dateFormat + iIndex++)->aChar() != 'M'))
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      // expect DD
      if ( (*(dateFormat + iIndex++)->aChar() != 'D') ||
          (*(dateFormat + iIndex++)->aChar() != 'D') )
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_YYYY;
      m_xFormatters.m_eDateComponent[1] = DC_MM;
      m_xFormatters.m_eDateComponent[2] = DC_DD;
      
      
    }
    else {
      // FORMAT should be YYYY-DD-MM
      // expect DD
      if ( (*(dateFormat + iIndex++)->aChar() != 'D') ||
          (*(dateFormat + iIndex++)->aChar() != 'D'))
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      // expect MM
      if ( (*(dateFormat + iIndex++)->aChar() != 'M') ||
          (*(dateFormat + iIndex++)->aChar() != 'M') )
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_YYYY;
      m_xFormatters.m_eDateComponent[1] = DC_DD;
      m_xFormatters.m_eDateComponent[2] = DC_MM;
      
    }
  }
  // leading MM
  else if (*dateFormat->aChar() == 'M') {
    
    // expect MM
    if ( (*(dateFormat + iIndex++)->aChar() != 'M') ||
        (*(dateFormat + iIndex++)->aChar() != 'M') )
    return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_xFormatters.m_cDateSeparator = *pSeparator->aChar();
    
    // is next character 'Y' ?
    if (*(dateFormat + iIndex)->aChar() == 'Y') {
      
      // FORMAT should be MM-YYYY-DD
      // expect YYY
      if ( (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') )
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      // expect DD
      if ( (*(dateFormat + iIndex++)->aChar() != 'D') ||
          (*(dateFormat + iIndex++)->aChar() != 'D') )
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_MM;
      m_xFormatters.m_eDateComponent[1] = DC_YYYY;
      m_xFormatters.m_eDateComponent[2] = DC_DD;
    }
    else {
      // FORMAT should be MM-DD-YYYY
      // expect DD
      if ( (*(dateFormat + iIndex++)->aChar() != 'D') ||
          (*(dateFormat + iIndex++)->aChar() != 'D'))
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') )
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_MM;
      m_xFormatters.m_eDateComponent[1] = DC_DD;
      m_xFormatters.m_eDateComponent[2] = DC_YYYY;
      
    }
  }
  // leading DD
  else if (*dateFormat->aChar() == 'D') {
    
    // expect DD
    if ( (*(dateFormat + iIndex++)->aChar() != 'D') ||
        (*(dateFormat + iIndex++)->aChar() != 'D') )
    return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_xFormatters.m_cDateSeparator = *pSeparator->aChar();
    
    // is next character 'M' ?
    if (*(dateFormat + iIndex)->aChar() == 'M') {
      
      // FORMAT should be DD-MM-YYYY
      // expect another M
      if ( (*(dateFormat + iIndex++)->aChar() != 'M') ||
          (*(dateFormat + iIndex++)->aChar() != 'M') )
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') )
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_DD;
      m_xFormatters.m_eDateComponent[1] = DC_MM;
      m_xFormatters.m_eDateComponent[2] = DC_YYYY;
      
    }
    else {
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') ||
          (*(dateFormat + iIndex++)->aChar() != 'Y') )
      return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->aChar() != m_xFormatters.m_cDateSeparator) )
      return false;
      
      if ( (*(dateFormat + iIndex++)->aChar() != 'M') ||
          (*(dateFormat + iIndex++)->aChar() != 'M'))
      return false;
      
      m_xFormatters.m_eDateComponent[0] = DC_DD;
      m_xFormatters.m_eDateComponent[1] = DC_YYYY;
      m_xFormatters.m_eDateComponent[2] = DC_MM;
      
    }
  }
  else
  return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_NetHog_pre(Interface::Session* pSession)
{
  Job_NetHog* pJob_NetHog = new Job_NetHog(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_NetHog->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_NetHog->m_bHasError = true;
    pJob_NetHog->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_post(Interface::Session* pSession, Job* pJob)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pJob);
  
  if ( pJob_NetHog->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_NetHog->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    i8 iRowsProcessed = 0;
    i8 iRowsInvalid   = 0;
    i8 iRowsSkipped   = 0;

    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      
      //vTrace("%s"_Stz, pJob_NetHog->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_NetHog->m_xNode[i_Node].m_ziRowsProcessed;
      iRowsInvalid   += *pJob_NetHog->m_xNode[i_Node].m_ziRowsInvalid;
      iRowsSkipped   += *pJob_NetHog->m_xNode[i_Node].m_ziRowsSkipped;
    }
    if (iRowsInvalid || iRowsSkipped)
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             iRowsProcessed,iRowsInvalid,iRowsSkipped);
    else
      vTrace("success: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
           iRowsProcessed,iRowsInvalid,iRowsSkipped);
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_NetHog_new(Job* pArg)
{
  Job_NetHog* pJob_NetHog = new Job_NetHog(nullptr);
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_delete(Interface::Session* pSession, Job* pJob)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pJob);
  
  delete pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_show(Job* pJob)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pJob);

  pJob_NetHog->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
