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
  m_pStreamDef  = m_pTable->pStreamDef();
  m_pCrwHash    = pWriteHash;
  
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
    eError = eValidRecord(szRecord,nRecordBytes);
    if (! bContinueAfterError(eError,pPipelineJob))
      continue;

    // extract the row
    eEC eError = CsvCursor::eExtractNext(m_pRow,m_pStreamDef,
                                         szRecord,&m_oFormatter);
    if (! bContinueAfterError(eError,pPipelineJob))
      continue;

    // attempt to append row into crwHash
    eError = eAppendRow();
    if (! bContinueAfterError(eError,pPipelineJob))
      continue;

    AM::vIncRowsProcessed(pPipelineJob);
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
/* manage any error, indicate if processing of record is to continue          */
/*----------------------------------------------------------------------------*/
bool Loader::bContinueAfterError(eEC eError, Job* pPipelineJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  if (EC_NONE == eError)
    return true;

  if (EC_BLANKLINE == eError)
    return false;
  
  AM::vIncRowsInvalid(pPipelineJob);

  return false;
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
