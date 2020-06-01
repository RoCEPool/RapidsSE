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
Loader::Loader(Table* pTable)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
    
  m_pRecordData = new RecordData(&m_oFormatter);
  
  m_pTable = pTable;
  m_pRow = new RowSingle(pTable->pStreamDef(),C_ALL);

  
  _checkInitialised_(CN_LOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pRecordData;
  
  delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
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
    eEC eError = pWriteHash->eProcessRecord(&m_oFormatter, aRecord, nRecordBytes);

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
        


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
