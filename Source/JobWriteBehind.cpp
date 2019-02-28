/*============================================================================*/
/* JobWriteBehind.cpp -- Write Behind part of a Job (Ingest/Insert/Load)     */
/*============================================================================*/

// Job Short: WriteBehind

#include "Jobs.h"
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
JobWriteBehind::JobWriteBehind(eDS eDataSource)
{
  m_pParentJob = nullptr;
  m_eDataSource = eDataSource;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pEntryBuffer[i_Partition]   = nullptr;
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
    RASSERT( ! m_pEntryBuffer[iPartition] );
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* handle dangling buffers                                                    */
/*----------------------------------------------------------------------------*/
void JobWriteBehind::vHandleDanglingBuffers(void)
{
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    
    if ( ! m_pEntryBuffer[iPartition] )
      vNewEntryBuffer(iPartition);
    
    // get the entry buffer
    EntryBuffer* pEntryBuffer = m_pEntryBuffer[iPartition];
    m_pEntryBuffer[iPartition] = nullptr;
    
    i8 nWriteBehindBuffers = m_nWriteBehindBuffers[iPartition];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a write buffer job for load
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          m_pTable->aTableName(),
                                          m_eDataSource,
                                          pEntryBuffer,
                                          m_pParentJob->iJobId(),
                                          iPartition,
                                          C_FINAL,
                                          nWriteBehindBuffers);
  }
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(void)
{
  return eInsertRow(m_aRow, m_nRowBytes);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(Stz* aRow_Data, i8 nRowBytes)
{
  // insert the row in a working buffer
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hash the record to a partition
    i8 iPartition = m_pTable->iRowPartition(aRow_Data);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check for working buffer
    if (! m_pEntryBuffer[iPartition]) {
      
      // create a new working buffer for this partition
      vNewEntryBuffer(iPartition);
      
      if (! m_pEntryBuffer[iPartition]) {
        vTrace("EntryBuffer allocation failed\n"_Stz);
        return EC_LOW_BUFFERS;
       }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the entry buffer for the partition
    EntryBuffer* pEntryBuffer = m_pEntryBuffer[iPartition];
        
    RowWrk xRowWrk;
    
    xRowWrk.vEmbedded(aRow_Data->aChar());
    xRowWrk.vRowDataSize(nRowBytes);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row into working buffer
    pEntryBuffer->vGrabWrite();
    Item* aItem = pEntryBuffer->aAppendRow(xRowWrk);
    pEntryBuffer->vDropWrite();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // appended ?
    if (aItem)
      return EC_NONE;
      
    m_pEntryBuffer[iPartition] = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stall if ready queue is building up
    while (gpAttachMap->m_pQueues->m_pReadyQueue->nTasks() > 1000)
      Pthread::vSleepMilli(1,CN_JOB_WRITEBUFFER);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job 
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          m_pTable->aTableName(),
                                          m_eDataSource,
                                          pEntryBuffer,
                                          m_pParentJob->iJobId(),
                                          iPartition,
                                          C_NOT_FINAL,
                                          0);
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eProcessRecord(Stz* aRecord,
                                   i8 nBytes)
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

  bool bFormat = bFormatRow(aRecord, nBytes, bBlankLine);
  
  if (bBlankLine)
    return EC_BLANKLINE;

  if ( ! bFormat )
    return EC_INVALID_VALUE;

  // attempt to add record into a write-behind buffer
  eEC eError = eInsertRow();

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
void JobWriteBehind::vNewEntryBuffer( i8 iPartition)
{
  m_pEntryBuffer[iPartition] = new EntryBuffer();
  
  m_pEntryBuffer[iPartition]->vIndex(m_pTable->pHiddenIndex());
  m_pEntryBuffer[iPartition]->vHeightNo(0);
  m_pEntryBuffer[iPartition]->vPartitionNo(iPartition);

  m_nWriteBehindBuffers[iPartition]++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

