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
    m_pCrwBuffer[i_Partition]   = nullptr;
    m_nWriteBehindBuffers[i_Partition] = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobWriteBehind::~JobWriteBehind(void) noexcept(false)
{
  // Assert we don't have any dangling buffers
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    RASSERT( ! m_pCrwBuffer[i_Ptn] );
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
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {

    if ( ! m_pCrwBuffer[i_Ptn] )
      vNewRowBuffer(i_Ptn);
    else
      m_paRowCoder_Copy[i_Ptn]->vCopyEnd();

    // get the entry buffer
    CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];
    m_pCrwBuffer[i_Ptn] = nullptr;

    i8 nWriteBehindBuffers = m_nWriteBehindBuffers[i_Ptn];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a write buffer job for load
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          pTable()->aTableName(),
                                          m_eDataSource,
                                          pCrwBuffer,
                                          m_pParentJob->iJobId(),
                                          i_Ptn,
                                          C_FINAL,
                                          nWriteBehindBuffers);
  }
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(void)
{
  return eInsertRow(m_oRowParts.pNub());
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(RowNub* pRowNub)
{
  // insert the row in a working buffer
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hash the record to a partition
    i8 i_Ptn = pTable()->iRowPartition(m_oRowParts.pNub()->iHash());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check for row buffer
    if (! m_pCrwBuffer[i_Ptn]) {
      
      // create a new row buffer for this partition
      vNewRowBuffer(i_Ptn);
      
      if (! m_pCrwBuffer[i_Ptn]) {
        vTrace("RowBuffer allocation failed\n"_Stz);
        return EC_LOW_MEMORY;
      }

      CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];
      m_paRowCoder_Copy[i_Ptn]->vCopyBegin(pCrwBuffer);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the entry buffer for the partition
    CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row into working buffer
    pCrwBuffer->vGrabWrite();
    bool bRowAppended = pCrwBuffer->bAppendRow(m_paRowCoder_Copy[i_Ptn],pRowNub);
    pCrwBuffer->vDropWrite();
    
    //pRowBuffer->vDump();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // appended ?
    if (bRowAppended)
      return EC_NONE;
    
    m_pCrwBuffer[i_Ptn] = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job
    m_paRowCoder_Copy[i_Ptn]->vCopyEnd();
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          pTable()->aTableName(),
                                          m_eDataSource,
                                          pCrwBuffer,
                                          m_pParentJob->iJobId(),
                                          i_Ptn,
                                          C_NOT_FINAL,
                                          0);
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eProcessRecord(Stz* szRecStr, i8 nBytes)
{
  bool bBlankLine = false;
  
  if (nBytes > C_MAX_RECORD) {
    if ( ! gpAttachMap->m_bMetadata ) {
      vWarn("Skipping row %.20s... - record in file > %lld bytes\n"_Stz,
            szRecStr,
            C_MAX_RECORD);
    }
    return EC_MAX_ROW_LENGTH;
  }

  bool bFormat = bEncodeRow(szRecStr, nBytes, bBlankLine);
  
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
void JobWriteBehind::vNewRowBuffer( i8 i_Ptn)
{
  m_pCrwBuffer[i_Ptn] = new CrwBuffer();
  
  m_pCrwBuffer[i_Ptn]->vTable(pTable());
  m_pCrwBuffer[i_Ptn]->vPartitionNo(i_Ptn);
  //m_pCrwBuffer[i_Ptn]->vDump();
  
  m_nWriteBehindBuffers[i_Ptn]++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

