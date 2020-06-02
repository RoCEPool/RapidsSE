/*============================================================================*/
/* Loader.cpp -- Loader                                                       */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,Loader,CN_LOADER)
__aNewFunction(Loader,CN_LOADER)
__vDeleteFunction(Loader,CN_LOADER)
__aFooterFunction(Loader)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(Table* pTable, CrwHash* pWriteHash)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
    
  m_pTable      = pTable;
  m_pCrwHash  = pWriteHash;
  
  m_pLoadCursor = new LoadCursor(&m_oFormatter);
  m_pRow        = new RowSingle(pTable->pStreamDef(),C_ALL);

  // m_oFormatter

  _checkInitialised_(CN_LOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pLoadCursor;
  
  delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan csv records (in read chunk) and convert to crw in crwHash             */
/*----------------------------------------------------------------------------*/
bool Loader::bCsvToCrw(Stz* szCsvArea, i8 nCsvBytes, Job* pPipelineJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  Stz* szRecord;
  i8   nRecordBytes;
  eEC  eError;
  
  m_pLoadCursor->vInit(szCsvArea,nCsvBytes);
  
  // process the records from the chunk
  
  for (;;) {

    // test if this loader needs to terminate early
    if (bTerminateEarly())
      return false;

    // advance to next record in the csv data area
    if (! m_pLoadCursor->bNextRecord(&szRecord,nRecordBytes))
      return true;
    
    // test record length etc
    if (EC_NONE != eValidRecord(szRecord,nRecordBytes)) {
      AM::vIncRowsInvalid(pPipelineJob);
      continue;
    }

    // process that csv record into crwHash
    eError = eProcessRecord(szRecord,nRecordBytes);

    if (EC_NONE == eError) {
      AM::vIncRowsProcessed(pPipelineJob);
      continue;
    }

    if (EC_BLANKLINE == eError) {
      continue;
    }
    
    // we inc errors here to detect error limit as early as possible
    if (EC_INVALID_VALUE == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      continue;
    }

    if (EC_MAX_ROW_LENGTH == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      continue;
    }

    // some other error?
    return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* set format                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bSetFormat(TagSet* pFmtTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_oFormatter.bSetFormat(pFmtTagSet);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
        
/*----------------------------------------------------------------------------*/
/* append row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC Loader::eAppendRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  CrwBuffer* pCrwBuffer = nullptr;
  
  // use row hash to get partition
  i8 i_Ptn = m_pTable->iToPartition(m_pRow->m_iHash);
  
  // append the row into a working buffer
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for row buffer
  
  if (! m_pCrwHash->m_pCrwBuffer[i_Ptn]) {
    
    // create a new crw buffer for this partition
    pCrwBuffer = m_pCrwHash->pNewCrwBuffer(i_Ptn);
    
    if (! pCrwBuffer) {
      vTrace("CrwBuffer allocation failed\n"_Stz);
      return EC_LOW_MEMORY;
    }

  }
  else {
    pCrwBuffer = m_pCrwHash->m_pCrwBuffer[i_Ptn];
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append row into working buffer
  
  //pCrwBuffer->vGrabWrite();
  bool bWasAppended = pCrwBuffer->bAppendRow(m_pRow);
  //pCrwBuffer->vDropWrite();
  
  // appended ?
  if (bWasAppended)
    return EC_NONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the buffer for append
  
  pCrwBuffer->vAppendClose();
  
  // create a new crw buffer for this partition linking to the old
  pCrwBuffer = m_pCrwHash->pNewCrwBuffer(i_Ptn);
  
  if (! pCrwBuffer) {
    vTrace("CrwBuffer allocation failed\n"_Stz);
    return EC_LOW_MEMORY;
  }
  
  // reattempt insrt. Should work
  bWasAppended = pCrwBuffer->bAppendRow(m_pRow);
  ASSERT(bWasAppended);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* Extract row (with hash) from given record string                           */
/*----------------------------------------------------------------------------*/
eEC Loader::eExtractRow(Stz* szRecord, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  StreamDef* pStreamDef = m_pTable->pStreamDef();

  m_pRow->vResetValues();
  
  char* acRecCrs = szRecord->azChar();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eEC eError = CsvCursor::eExtractNext(m_pRow,pStreamDef,
                                         &acRecCrs,&m_oFormatter,
                                         bBlankLine);
//  if (! bResult)
//    return EC_INVALID_VALUE;
//
//  if (bBlankLine)
//    return EC_BLANKLINE;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  //SP_Row spRow;
//  //vTrace("%s\n"_Stz,spRow.sz(m_pRow));
//
//  return EC_NONE;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* process csv record into crwHash                                            */
/*----------------------------------------------------------------------------*/
eEC Loader::eProcessRecord(Stz* szRecord, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  bool bBlankLine = false;
  
  eEC eError = eExtractRow(szRecord,bBlankLine);
  
  if (bBlankLine)
    return EC_BLANKLINE;
  
  if (EC_NONE != eError)
    return eError;
  
  // attempt to append row into a write-behind buffer
  eError = eAppendRow();
  
  // was there an error ?
  if (eError != EC_NONE) {
    vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eError]);
    return eError;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* terminate loading early?                                                   */
/*----------------------------------------------------------------------------*/
bool Loader::bTerminateEarly(void)
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
/* coarse validation of the record (length etc)                               */
/*----------------------------------------------------------------------------*/
eEC Loader::eValidRecord(Stz* szRecord, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  if (nBytes > C_MAX_RECORD) {
    if (! AM::bMetadata()) {
      vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
            szRecord,
            C_MAX_RECORD);
    }
    return EC_MAX_ROW_LENGTH;
  }
    
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
