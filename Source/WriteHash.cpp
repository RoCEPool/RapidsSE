/*============================================================================*/
/* WriteBehindH.cpp -- Write Behind  (Ingest/Insert/Load)                     */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
WriteHash* Load::_pWriteHash(void* aAddr)
{
  WriteHash* pWriteHash = reinterpret_cast<WriteHash*>(aAddr);
  
  return pWriteHash;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteHash::WriteHash(eCN eClassNo, Table* pTable)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_pTable = pTable;
  m_pRow   = new Row(pTable->pStreamDef(),C_ALL);
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pCrwBuffer[i_Partition]     = nullptr;
    m_paRowCompactor[i_Partition] = nullptr;
  }
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;
  
  _checkInitialised_(CN_WRITEBEHIND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteHash::~WriteHash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // Assert we don't have any dangling buffers
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    delete m_paRowCompactor[i_Ptn];
    RASSERT( ! m_pCrwBuffer[i_Ptn] );    
  }
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC WriteHash::eAppendRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // use row hash to get partition
  i8 i_Ptn = m_pTable->iToPartition(m_pRow->m_iHash);
  
  // append the row into a working buffer
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check for row buffer
    
    if (! m_pCrwBuffer[i_Ptn]) {
      
      // create a new crw buffer for this partition
      vNewCrwBuffer(i_Ptn);
      
      if (! m_pCrwBuffer[i_Ptn]) {
        vTrace("CrwBuffer allocation failed\n"_Stz);
        return EC_LOW_MEMORY;
      }
      
      CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];
      pCrwBuffer->vAppendOpen(m_paRowCompactor[i_Ptn], true);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append row into working buffer
    
    CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];
    
    //pCrwBuffer->vGrabWrite();
    bool bWasAppended = pCrwBuffer->bAppendRow(m_pRow);
    //pCrwBuffer->vDropWrite();
    //pCrwBuffer->vDump();
    
    // appended ?
    if (bWasAppended)
      return EC_NONE;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job
    
    m_pCrwBuffer[i_Ptn] = nullptr;
    pCrwBuffer->vAppendClose();
    
//    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->pSession(),
//                                          m_pTable->szTableNm(),
//                                          m_eDataSource,
//                                          pCrwBuffer,
//                                          m_pParentJob->iJobId(),
//                                          i_Ptn,
//                                          C_NOT_FINAL,
//                                          0);
    
    // repeat, try-again
    // ...
  }
  
  return EC_NONE;
}
/*----------------------------------------------------------------------------*/
/* return the ptn crwBuffer                                                   */
/*----------------------------------------------------------------------------*/
CrwBuffer* WriteHash::pCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_pCrwBuffer[iPtn];
}

/*----------------------------------------------------------------------------*/
/* set the ptn crwBuffer                                                      */
/*----------------------------------------------------------------------------*/
void WriteHash::vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pCrwBuffer[iPtn] = pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC WriteHash::eProcessRecord(Stz* szRecStr, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bBlankLine = false;
  
  if (nBytes > C_MAX_RECORD) {
    if (! AM::bMetadata()) {
      vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
            szRecStr,
            C_MAX_RECORD);
    }
    return EC_MAX_ROW_LENGTH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bValid = bExtractRow(szRecStr, bBlankLine);
  
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

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract row (with hash) from given record string                           */
/*----------------------------------------------------------------------------*/
bool WriteHash::bExtractRow(Stz* szRecStr, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();

  m_pRow->vResetValues();
  
  char* acRecCrs = szRecStr->azChar();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = CsvCursor::bExtractNext(m_pRow,pStreamDef,
                                         &acRecCrs,&m_oFormatters,
                                         bBlankLine);
  if (! bResult)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //SP_Row spRow;
  //vTrace("%s\n"_Stz,spRow.sz(m_pRow));
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
void WriteHash::vNewCrwBuffer( i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pCrwBuffer[i_Ptn] = new CrwBuffer();
  
  m_pCrwBuffer[i_Ptn]->vTable(m_pTable);
  m_pCrwBuffer[i_Ptn]->vPartitionNo(i_Ptn);
  //m_pCrwBuffer[i_Ptn]->vDump();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

