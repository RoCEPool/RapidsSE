/*============================================================================*/
/* Job_LoadTable.cpp -- Load Table Job                                        */
/*============================================================================*/

// Job Short: LoadTable

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/SF1/lineitem.csv" ;
 
 */

JobClass xJobClass_LoadTable =
{
  "Job_LoadTable"_Stz,    // name of the job class
  CN_JOB_LOADTABLE,       // class enum
  JK_DIST2PC,             // job kind
  "load table"_Stz,       // comment about the job
  "ld"_Stz,               // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "loadTable_req"_Stz,    // the _req task seed
  "loadTable_beg"_Stz,    // the _rcv task seed
  "loadTable_exe"_Stz,    // the _exe task seed
  "loadTable_end"_Stz,    // the _end task seed
  "loadTable_can"_Stz,    // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_LoadTable_pre,     // prompt pre-process function
  vJob_LoadTable_post,    // prompt post-process function
  pJob_LoadTable_new,     // job new function
  vJob_LoadTable_delete,  // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_LoadTable_show,    // job show function
  
  // help and example
  true,
  "ld <tableName> \"<fileName>\""_Stz,
  "ld supplier \"/tpch/supplier.tbl\" ;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_LoadTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_NetHog_class;
  
  return &xJobClass_LoadTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_LoadTable* JobModel::_pJob_LoadTable(void* aAddress)
{
  Job_LoadTable* pJob_LoadTable = reinterpret_cast<Job_LoadTable*>(aAddress);
  
  RASSERT_TYPE(pJob_LoadTable,CN_JOB_LOADTABLE);
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_LoadTable::operator new(size_t)
{
  Job_LoadTable* pJob_LoadTable =
    static_cast<Job_LoadTable*>(HeapMem::pAllocate(CN_JOB_LOADTABLE));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_LoadTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::operator delete(void* aAddress)
{
  Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(aAddress);
  
  HeapMem::vFree(pJob_LoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_LoadTable::Job_LoadTable(Interface::Session* pSession)
: Job(CN_JOB_LOADTABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  m_pTable        = nullptr;
  m_xFormatters.vReset(); // reset to defaults in attachmap
  
  m_pLoadMap      = nullptr;
  
  _checkInitialised_(CN_JOB_LOADTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadTable::~Job_LoadTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  delete m_pLoadMap;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pObject);

  pJob_LoadTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin loading                                                             */
/*----------------------------------------------------------------------------*/
bool Job_LoadTable::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  m_pLoadMap = new LoadMap(m_sFileName);
  
  bool bOpened = m_pLoadMap->bOpenFile();
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* find next chunk in file being loaded                                       */
/*----------------------------------------------------------------------------*/
bool Job_LoadTable::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  bool bCallAgain = m_pLoadMap->bMapChunk();
  
  return bCallAgain;
}

/*----------------------------------------------------------------------------*/
/* dump the load map                                                          */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vDumpMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  vTrace("Load Map for: %s:\n"_Stz,m_sFileName.aStz());
  
  m_pLoadMap->vDumpMap();
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  m_pLoadMap->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_LoadTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
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
          m_xFormatters.m_eRecordEnd = RE_NL;
        }
        else if (cRecordEnds == '\r') {
          m_xFormatters.m_eRecordEnd = RE_CR;
        }
      }
      // record_ends is 2 characters
      else if (pTokenVec->pGet(i_Token)->iLength() == 2) {
        
        if (pTokenVec->pGet(i_Token)->bSame("\r\n"_Stz)) {
          m_xFormatters.m_eRecordEnd = RE_CRNL;
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
Table* Job_LoadTable::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_LoadTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract the date separator                                                 */
/*----------------------------------------------------------------------------*/
bool Job_LoadTable::bExtractDateFormat(Stz* dateFormat)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
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
Job* JobModel::pJob_LoadTable_pre(Interface::Session* pSession)
{
  Job_LoadTable* pJob_LoadTable = new Job_LoadTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_LoadTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_LoadTable->m_bHasError = true;
    pJob_LoadTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_LoadTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);
  
  if ( pJob_LoadTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_LoadTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    i8 iRowsProcessed = 0;
    i8 iRowsInvalid   = 0;
    i8 iRowsSkipped   = 0;

    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      
      //vTrace("%s"_Stz, pJob_LoadTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsProcessed;
      iRowsInvalid   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsInvalid;
      iRowsSkipped   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsSkipped;
    }
    if (iRowsInvalid || iRowsSkipped)
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
    else
      vTrace("success: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
           iRowsProcessed-iRowsSkipped,iRowsInvalid,iRowsSkipped);
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_LoadTable_new(Job* pArg)
{
  Job_LoadTable* pJob_LoadTable = new Job_LoadTable(nullptr);
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_LoadTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);
  
  delete pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_LoadTable_show(Job* pJob)
{
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);

  pJob_LoadTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
