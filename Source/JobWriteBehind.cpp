/*============================================================================*/
/* JobWriteBehind.cpp -- Write Behind part of a Job (Ingest/Insert/Load)     */
/*============================================================================*/

// Job Short: WriteBehind

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* JobWriteBehind::operator new(size_t)
{
  vFatal("fatal: JobWriteBehind class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobWriteBehind::JobWriteBehind(void)
{
  //m_aRecord
  m_nRecordBytes = 0;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pWorkingBuffer[i_Partition]   = nullptr;
    m_nWriteBehindBuffers[i_Partition] = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobWriteBehind::~JobWriteBehind(void) noexcept(false)
{
  // Assert we don't have any dangling buffers
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    RASSERT( ! m_pWorkingBuffer[iPartition] );
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool JobWriteBehind::bFormatRow(Table* pTable, Formatters* pFormatters,
                                 Stz* aRecord, i8 nBytes, bool &bBlankLine)
{
  // format the raw data into a row
  bool bValid = Row::bFormatRow(pTable->pColumnVec(),
                                pFormatters,
                                aRecord, nBytes,
                                m_aRecord.aStz(),
                                m_nRecordBytes,
                                bBlankLine,
                                false /* allowNonPrintable */);
  
  return bValid;
}
/*----------------------------------------------------------------------------*/
/* handle dangling buffers                                                    */
/*----------------------------------------------------------------------------*/
void JobWriteBehind::vHandleDanglingBuffers(Interface::Session* pSession, Table* pTable,
                                            eDS eDataSource, i8 iParentJobId)
{
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    
    if ( ! m_pWorkingBuffer[iPartition] )
      vNewWorkingBuffer(pTable, iPartition);
    
    // get the working buffer
    WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[iPartition];
    m_pWorkingBuffer[iPartition] = nullptr;
    
    i8 nWriteBehindBuffers = m_nWriteBehindBuffers[iPartition];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a write buffer job for load
    Job_WriteBuffer::vSpawnJobWriteBuffer(
                                          pSession, pTable->aTableName(), eDataSource,
                                          pWorkingBuffer, iParentJobId, iPartition,
                                          true, nWriteBehindBuffers);
  }
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(Session* pSession, Table* pTable, eDS eDataSource, i8 iParentJobId)
{
  
  eEC eErrorCode = EC_NONE;
  
  // insert the row in a working buffer
  while (true ) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hash the record to a partition
    i8 iPartition = pTable->iRowPartition(m_aRecord);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check for working buffer
    if (! m_pWorkingBuffer[iPartition]) {
      
      // create a new working buffer for this partition
      vNewWorkingBuffer(pTable, iPartition);
      
      if (! m_pWorkingBuffer[iPartition]) {
        vTrace("EntryBuffer allocation failed\n"_Stz);
        eErrorCode = EC_LOW_BUFFERS;
        return eErrorCode;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the working buffer
    WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[iPartition];
    
    // get the tx id
    //Session* pSession = m_pLoadTable->pSession();
    //Tx* pTx = pSession->pTx();
    //i8 iDistTxId = pTx->iDistTxId();
    i8 iDistTxId = 0;
    
    RowWrk xRowWrk;
    
    xRowWrk.vTxId(iDistTxId);
    xRowWrk.vEmbedded(m_aRecord.aChar());
    xRowWrk.vRowSize(m_nRecordBytes);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row into working buffer
    bool bAppended = pWorkingBuffer->bAppendRow(xRowWrk);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // appended ?
    if (bAppended)
      return eErrorCode;
      
    m_pWorkingBuffer[iPartition] = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job 
    Job_WriteBuffer::vSpawnJobWriteBuffer(pSession, pTable->aTableName(), eDataSource,
                                          pWorkingBuffer, iParentJobId,
                                          iPartition, false, 0);
  }
  
  return eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eProcessRecord(Interface::Session* pSession,
                                     i8 iParentJobId,
                                     Table* pTable,
                                     Formatters* pFormatters,
                                     Stz* aRecord,
                                     i8 nBytes,
                                     eDS eDataSource)
{
  bool bBlankLine = false;
  
  if (nBytes > C_MAX_RECORD) {
    if ( ! gpAttachMap->m_bMetadata ) {
      vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
            aRecord,
            C_MAX_RECORD);
    }
    return EC_MAX_ROW_LENGTH;
  }

  bool bFormat = bFormatRow(pTable, pFormatters, aRecord, nBytes, bBlankLine);
  
  if (bBlankLine)
    return EC_BLANKLINE;

  if ( ! bFormat )
    return EC_INVALID_VALUE;

  // attempt to add record into a write-behind buffer
  eEC eError = eInsertRow(pSession, pTable, eDataSource, iParentJobId);

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
/* create a new working buffer                                                */
/*----------------------------------------------------------------------------*/
void JobWriteBehind::vNewWorkingBuffer(Table* pTable, i8 iPartition)
{
  m_pWorkingBuffer[iPartition] =
    new WorkingBuffer(pTable,iPartition);
  
  m_nWriteBehindBuffers[iPartition]++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

