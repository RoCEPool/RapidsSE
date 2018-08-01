/*============================================================================*/
/* Job_ListCatalogs.cpp -- List Catalogs Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_LoadTable (
  "Job_LoadTable"_Stz,    // name of the job class
  CN_JOB_LOADTABLE,       // class enum
  "load table"_Stz,       // comment about the job
  "ld"_Stz,               // the prompt command
  nullptr,                // the second word
  "loadTable"_Stz,        // the task seed for this job
  false,                  // detached job?
  pJob_LoadTable_pre,     // prompt pre-process function
  vJob_LoadTable_post,    // prompt post-process function
  true                    // job is enabled
);

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
 */

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_LoadTable* JobsModel::_pJob_LoadTable(void* aAddress)
{
  Job_LoadTable* pJob_LoadTable = reinterpret_cast<Job_LoadTable*>(aAddress);
  
  //RASSERT( !pJob_LoadTable || CN_JOB_LOADTABLE == pJob_LoadTable->eClass());
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_LoadTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_LoadTable* pJob_LoadTable =
    static_cast<Job_LoadTable*>(Heap::pAllocate(CN_JOB_LOADTABLE));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_LoadTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::operator delete(void* aAddress)
{
  Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(aAddress);
  
  Heap::vFree(pJob_LoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_LoadTable::Job_LoadTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_LOADTABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
  m_sSeed     = mJobNote_LoadTable.pJobClass()->m_aSeed;
  //m_sFileName = ""_Stz;
  m_pLoader = nullptr;
  
  m_bCheckResources = true;
  
  _checkInitialised_(CN_JOB_LOADTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadTable::~Job_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LOADTABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LOADTABLE);
  
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pObject);

  pJob_LoadTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
      m_xFormatters.m_cFieldDelimiter = *pTokenVec->pGet(++i_Token)->sText().aChar();
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
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADTABLE,_FOOTER_);
  
                                                               //! Alka to write
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
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_LoadTable_pre(Interface::SEitf* pSEitf)
{
  Job_LoadTable* pJob_LoadTable = new Job_LoadTable(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
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
void JobsModel::vJob_LoadTable_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);
  
  if ( pJob_LoadTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_LoadTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {

    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_LoadTable->m_xNode[i_Node].m_sResponse.aChar());
    }
  }

  // now delete the job
  delete pJob_LoadTable;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
