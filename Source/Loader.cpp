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
Loader::Loader(Table* pTable, WriteHash* pWriteHash)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
    
  m_pTable      = pTable;
  m_pWriteHash  = pWriteHash;
  
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
/* read records                                                               */
/*----------------------------------------------------------------------------*/
bool Loader::bReadRecords(Job* pPipelineJob, Stz* szCsvData, i8 nCsvBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  Stz* szRecord;
  i8 nRecordBytes;
  
  m_pLoadCursor->vInit(szCsvData,nCsvBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the chunk
  for (;;) {

    if (bTerminate())
      return false;

    // fetch next record from data area
    bool bHaveRecord = m_pLoadCursor->bNextRecord(&szRecord,nRecordBytes);
    if (! bHaveRecord)
      return true;

    // process that record into write behind
    eEC eError = eProcessRecord(szRecord,nRecordBytes);

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
/* set format                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bSetFormat(TagSet* pFmtTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_oFormatter.bSetFormat(pFmtTagSet);
  
  return b;
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
  
  if (! m_pWriteHash->m_pCrwBuffer[i_Ptn]) {
    
    // create a new crw buffer for this partition
    pCrwBuffer = m_pWriteHash->pNewCrwBuffer(i_Ptn);
    
    if (! pCrwBuffer) {
      vTrace("CrwBuffer allocation failed\n"_Stz);
      return EC_LOW_MEMORY;
    }

  }
  else {
    pCrwBuffer = m_pWriteHash->m_pCrwBuffer[i_Ptn];
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
  pCrwBuffer = m_pWriteHash->pNewCrwBuffer(i_Ptn);
  
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
bool Loader::bExtractRow(Stz* szRecord, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  StreamDef* pStreamDef = m_pTable->pStreamDef();

  m_pRow->vResetValues();
  
  char* acRecCrs = szRecord->azChar();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = CsvCursor::bExtractNext(m_pRow,pStreamDef,
                                         &acRecCrs,&m_oFormatter,
                                         bBlankLine);
  if (! bResult)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //SP_Row spRow;
  //vTrace("%s\n"_Stz,spRow.sz(m_pRow));
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC Loader::eProcessRecord(Stz* szRecord, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  bool bBlankLine = false;
  
  if (nBytes > C_MAX_RECORD) {
    if (! AM::bMetadata()) {
      vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
            szRecord,
            C_MAX_RECORD);
    }
    return EC_MAX_ROW_LENGTH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bValid = bExtractRow(szRecord, bBlankLine);
  
  if (bBlankLine)
    return EC_BLANKLINE;
  
  if (! bValid)
    return EC_INVALID_VALUE;
  
  // attempt to append row into a write-behind buffer
  eEC eError = eAppendRow();
  
  // was there an error ?
  if (eError != EC_NONE) {
    vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eError]);
    return eError;
  }
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
