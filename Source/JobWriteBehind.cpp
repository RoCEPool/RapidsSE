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
    m_pRowBuffer[i_Partition]   = nullptr;
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
    RASSERT( ! m_pRowBuffer[iPartition] );
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

    if ( ! m_pRowBuffer[iPartition] )
      vNewRowBuffer(iPartition);

    // get the entry buffer
    RowBuffer* pRowBuffer = m_pRowBuffer[iPartition];
    m_pRowBuffer[iPartition] = nullptr;

    i8 nWriteBehindBuffers = m_nWriteBehindBuffers[iPartition];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a write buffer job for load
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          m_pTable->aTableName(),
                                          m_eDataSource,
                                          pRowBuffer,
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
  return eInsertRow(m_oRowParts.aGet(0), m_nRowBytes);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(CacheLine* aRow_Data, i8 nRowBytes)
{
  // insert the row in a working buffer
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hash the record to a partition
    i8 iPartition = m_pTable->iRowPartition(m_oRowParts.iHash());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check for row buffer
    if (! m_pRowBuffer[iPartition]) {
      
      // create a new row buffer for this partition
      vNewRowBuffer(iPartition);
      
      if (! m_pRowBuffer[iPartition]) {
        vTrace("RowBuffer allocation failed\n"_Stz);
        return EC_LOW_MEMORY;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the entry buffer for the partition
    RowBuffer* pRowBuffer = m_pRowBuffer[iPartition];
    
    //RowWrk xRowWrk;
    
    //xRowWrk.vEmbedded(aRow_Data->aChar());
    //xRowWrk.vRowDataSize(nRowBytes);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row into working buffer
    pRowBuffer->vGrabWrite();
    CacheLine* pRow = pRowBuffer->aAppendRow(aRow_Data);
    pRowBuffer->vDropWrite();
    
    //pRowBuffer->vDump();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // appended ?
    if (pRow)
      return EC_NONE;
    
    m_pRowBuffer[iPartition] = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          m_pTable->aTableName(),
                                          m_eDataSource,
                                          pRowBuffer,
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
/* create a new row buffer                                                    */
/*----------------------------------------------------------------------------*/
void JobWriteBehind::vNewRowBuffer( i8 iPartition)
{
  m_pRowBuffer[iPartition] = new RowBuffer();
  
  m_pRowBuffer[iPartition]->vTable(m_pTable);
  m_pRowBuffer[iPartition]->vPartitionNo(iPartition);
  //m_pRowBuffer[iPartition]->vDump();
  
  m_nWriteBehindBuffers[iPartition]++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

