/*============================================================================*/
/* Loader.cpp -- Loader                                                       */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader* Load::_pLoader(void* aAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(aAddress);
  
  RASSERT_TYPE(pLoader,CN_LOADER);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t)
{
  Loader* pLoader =
    static_cast<Loader*>(HeapMem::pAllocate(CN_LOADER));
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* aAddress)
{
  Loader* pLoader = static_cast<Loader*>(aAddress);
  
  HeapMem::vFree(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(Table* pTable)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pWriteHash = new WriteHash(pTable);
  
  m_pRecordData = new RecordData(&m_oFormatter);
  
  _checkInitialised_(CN_LOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pWriteHash;
  delete m_pRecordData;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* read records                                                               */
/*----------------------------------------------------------------------------*/
bool Loader::bReadRecords(WriteHash* pWriteHash, Job* pPipelineJob,
                          Stz* aDataArea, i8 nDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  Stz* aRecord;
  i8 nRecordBytes;
  
  m_pRecordData->vInit(aDataArea, nDataBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the chunk
  for (;;) {

    if (bTerminate())
      return false;

    // fetch next record from data area
    bool bHaveRecord = m_pRecordData->bNextRecord(&aRecord,nRecordBytes);
    if (! bHaveRecord)
      return true;

    // process that record into write behind
    eEC eError = m_pWriteHash->eProcessRecord(&m_oFormatter, aRecord, nRecordBytes);

    if (EC_NONE == eError) {
      //vTrace(aRecord);
      AM::vIncRowsProcessed(pPipelineJob);
      continue;
    }

    if (EC_BLANKLINE == eError)
      continue;

    if (EC_INVALID_VALUE == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      //++m_pJob_LoadTable->m_ixErrors;
      continue;
    }

    if (EC_MAX_ROW_LENGTH == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      //++m_pJob_LoadTable->m_ixErrors;
      continue;
    }

    return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
//void Job_SubLoader::vEnd(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
//
//  if (gpBufferPool->bLowMemory())
//    vTrace("Job_SubLoader #%lld -- ending with low memory\n"_Stz,
//           m_iFocalId);
//
//  // handle any dangling buffers
//  vHandleDanglingBuffers();
//
//  // add invalid rows into parent load table job
//  i8 iRowsInvalid = this->iRowsInvalid();
//  m_pJob_LoadTable->vAddRowsInvalid(iRowsInvalid);
//
//  // add processed rows into parent load table job
//  i8 iRowsProcessed = this->iRowsProcessed();
//  //! REMOVE THIS TRACE
//  //vTrace("iRowsProcessed %lld [RJH REMOVE]\n"_Stz,iRowsProcessed);
//  m_pJob_LoadTable->vAddRowsProcessed(iRowsProcessed);
//
//  // close the file being loaded
//  m_pLoadChunk->vCloseFile();
//}

/*----------------------------------------------------------------------------*/
/* set the formatter from the tags in the ename set                           */
/*----------------------------------------------------------------------------*/
bool Loader::bSetFormat(TagSet* pFmtTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  s32 sTagValue;
  i8 iTagValue;
  i8 i_Tag;

  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_TERMINATOR
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_RECORD_TERMINATOR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    m_oFormatter.m_cRecordTerminator = *sTagValue.azChar();
    if (m_oFormatter.m_cRecordTerminator == '0')
      m_oFormatter.m_cRecordTerminator = '\0';

    m_oFormatter.m_bDefault = false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_ENDS
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_RECORD_ENDS);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    char cRecordEnds = *sTagValue.azChar();
    
    // is record_ends 1 character ?
    if (sTagValue.nLength() == 1) {
      if (cRecordEnds == '0') {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
      else if (cRecordEnds == '\n') {
        m_oFormatter.m_eRecordEnd = RE_NL;
      }
      else if (cRecordEnds == '\r') {
        m_oFormatter.m_bDefault = false;
        m_oFormatter.m_eRecordEnd = RE_CR;
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    // record_ends is 2 characters
    else if (sTagValue.nLength() == 2) {
      
      if (sTagValue.bSame("\r\n"_Stz)) {
        m_oFormatter.m_bDefault = false;
        m_oFormatter.m_eRecordEnd = RE_CRNL;
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
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_FIELD_DELIMITER);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    if (sTagValue.bSame("\\t"_Stz))
      m_oFormatter.m_cFieldDelimiter = '\t';
    else
      m_oFormatter.m_cFieldDelimiter = *sTagValue.azChar();
    if (m_oFormatter.m_cFieldDelimiter == '0')
      m_oFormatter.m_cFieldDelimiter = '\0';
    m_oFormatter.m_bDefault = false;
  }

  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_LEADING
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_LEADING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    m_oFormatter.m_cEnclosedBy_Leading = *sTagValue.azChar();
    if (m_oFormatter.m_cEnclosedBy_Leading == '0')
      m_oFormatter.m_cEnclosedBy_Leading = '\0';
    
    m_oFormatter.m_bDefault = false;
    
    // default set trailing same as leading
    m_oFormatter.m_cEnclosedBy_Trailing =
    m_oFormatter.m_cEnclosedBy_Leading;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_TRAILING
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_TRAILING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    // if trailing is specified, leading should also have been specified
    if (m_oFormatter.m_cEnclosedBy_Trailing == '\0') {
      if (! AM::bMetadata())
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
    m_oFormatter.m_cEnclosedBy_Trailing = *sTagValue.azChar();
    if (m_oFormatter.m_cEnclosedBy_Trailing == '0')
      m_oFormatter.m_cEnclosedBy_Trailing = '\0';
    
    m_oFormatter.m_bDefault = false;
  }
  //
  //    DATE_FORMAT='YYYY.MM.DD’
  //
  //
  // - - - - - - - - - - - - - - - - - - - - -
  // C_DATE_FORMAT
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_DATE_FORMAT);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    // extract date format
    if ( ! bExtractDateFormat(sTagValue.szStz()) ) {
      if ( ! AM::bMetadata())
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
    m_oFormatter.m_bDefault = false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ESCAPE_CHAR
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_ESCAPE_CHAR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pFmtTagSet->szValue(i_Tag);
    m_oFormatter.m_cEscapeChar = *sTagValue.azChar();
    if (m_oFormatter.m_cEscapeChar == '0')
      m_oFormatter.m_cEscapeChar = '\0';
    
    m_oFormatter.m_bDefault = false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_MAX_ERRORS
  i_Tag = pFmtTagSet->i_Find(C_TAG_LD_MAX_ERRORS);
  if (i_Tag != C_UNKNOWN) {
    iTagValue = pFmtTagSet->iValue(i_Tag);
    
    m_oFormatter.m_nMaxErrors = iTagValue;
    
    m_oFormatter.m_bDefault = false;
  }
        
  return true;
}

/*----------------------------------------------------------------------------*/
/* terminate loading early?                                                   */
/*----------------------------------------------------------------------------*/
bool Loader::bTerminate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

//  if (m_oFormatters.m_nMaxErrors &&
//      *m_pJob_LoadTable->m_ixErrors >= m_oFormatters.m_nMaxErrors) {
//    vTrace("Loader -- terminating with max errors %lld %lld\n"_Stz,
//           *m_pJob_LoadTable->m_ixErrors,m_oFormatters.m_nMaxErrors);
//    return true;
//  }
//
//  if (gpBufferPool->bLowMemory()) {
//    vTrace("Job_SubLoader #%lld -- terminating with low memory\n"_Stz,
//           m_iFocalId);
//    return true;
//  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* terminate loading early?                                                   */
/*----------------------------------------------------------------------------*/
WriteHash* Loader::pWriteHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_pWriteHash;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
        
/*----------------------------------------------------------------------------*/
/* extract the date separator                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bExtractDateFormat(Stz* dateFormat)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  i8 iIndex = 0;
  
  // leading YYYY
  if (*dateFormat->azChar() == 'Y') {
    
    // expect YYYY
    if ( (*(dateFormat + iIndex++)->azChar() != 'Y') ||
         (*(dateFormat + iIndex++)->azChar() != 'Y') ||
         (*(dateFormat + iIndex++)->azChar() != 'Y') ||
         (*(dateFormat + iIndex++)->azChar() != 'Y') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_oFormatter.m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'M' ?
    if (*(dateFormat + iIndex)->azChar() == 'M') {
      
      // FORMAT should be YYYY-MM-DD
      // expect another M
      if ( (*(dateFormat + iIndex++)->azChar() != 'M') ||
           (*(dateFormat + iIndex++)->azChar() != 'M'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      // expect DD
      if ( (*(dateFormat + iIndex++)->azChar() != 'D') ||
           (*(dateFormat + iIndex++)->azChar() != 'D') )
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_YYYY;
      m_oFormatter.m_eDateComponent[1] = DC_MM;
      m_oFormatter.m_eDateComponent[2] = DC_DD;
      
      
    }
    else {
      // FORMAT should be YYYY-DD-MM
      // expect DD
      if ( (*(dateFormat + iIndex++)->azChar() != 'D') ||
           (*(dateFormat + iIndex++)->azChar() != 'D'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      // expect MM
      if ( (*(dateFormat + iIndex++)->azChar() != 'M') ||
           (*(dateFormat + iIndex++)->azChar() != 'M') )
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_YYYY;
      m_oFormatter.m_eDateComponent[1] = DC_DD;
      m_oFormatter.m_eDateComponent[2] = DC_MM;
      
    }
  }
  // leading MM
  else if (*dateFormat->azChar() == 'M') {
    
    // expect MM
    if ( (*(dateFormat + iIndex++)->azChar() != 'M') ||
        (*(dateFormat + iIndex++)->azChar() != 'M') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_oFormatter.m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'Y' ?
    if (*(dateFormat + iIndex)->azChar() == 'Y') {
      
      // FORMAT should be MM-YYYY-DD
      // expect YYY
      if ( (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      // expect DD
      if ( (*(dateFormat + iIndex++)->azChar() != 'D') ||
          (*(dateFormat + iIndex++)->azChar() != 'D') )
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_MM;
      m_oFormatter.m_eDateComponent[1] = DC_YYYY;
      m_oFormatter.m_eDateComponent[2] = DC_DD;
    }
    else {
      // FORMAT should be MM-DD-YYYY
      // expect DD
      if ( (*(dateFormat + iIndex++)->azChar() != 'D') ||
          (*(dateFormat + iIndex++)->azChar() != 'D'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') ||
          (*(dateFormat + iIndex++)->azChar() != 'Y') )
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_MM;
      m_oFormatter.m_eDateComponent[1] = DC_DD;
      m_oFormatter.m_eDateComponent[2] = DC_YYYY;
      
    }
  }
  // leading DD
  else if (*dateFormat->azChar() == 'D') {
    
    // expect DD
    if ( (*(dateFormat + iIndex++)->azChar() != 'D') ||
         (*(dateFormat + iIndex++)->azChar() != 'D') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + iIndex++;
    m_oFormatter.m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'M' ?
    if (*(dateFormat + iIndex)->azChar() == 'M') {
      
      // FORMAT should be DD-MM-YYYY
      // expect another M
      if ( (*(dateFormat + iIndex++)->azChar() != 'M') ||
           (*(dateFormat + iIndex++)->azChar() != 'M') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') )
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_DD;
      m_oFormatter.m_eDateComponent[1] = DC_MM;
      m_oFormatter.m_eDateComponent[2] = DC_YYYY;
      
    }
    else {
      
      // expect YYYY
      if ( (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') ||
           (*(dateFormat + iIndex++)->azChar() != 'Y') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + iIndex++)->azChar() != m_oFormatter.m_cDateSeparator) )
        return false;
      
      if ( (*(dateFormat + iIndex++)->azChar() != 'M') ||
           (*(dateFormat + iIndex++)->azChar() != 'M'))
        return false;
      
      m_oFormatter.m_eDateComponent[0] = DC_DD;
      m_oFormatter.m_eDateComponent[1] = DC_YYYY;
      m_oFormatter.m_eDateComponent[2] = DC_MM;
      
    }
  }
  else
    return false;
  
  return true;
}
        
/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Loader)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
