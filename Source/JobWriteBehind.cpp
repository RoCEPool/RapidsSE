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
      vNewCrwBuffer(i_Ptn);
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
eEC JobWriteBehind::eInsertRow(Row* pRow)
{
  return eInsertRow(pRow,m_oRowParts.pNub());
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC JobWriteBehind::eInsertRow(Row* pRow, RowNub* pFromRowNub)
{
  // use row hash to get partition
  i8 i_Ptn = pTable()->iRowPartition(pRow->m_iHash);
  
  // insert the row in a working buffer
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
      m_paRowCoder_Copy[i_Ptn]->vCopyBegin(pCrwBuffer);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row into working buffer

    CrwBuffer* pCrwBuffer = m_pCrwBuffer[i_Ptn];

    //pCrwBuffer->vGrabWrite();
    bool bRowAppended = pCrwBuffer->bAppendRow(m_paRowCoder_Copy[i_Ptn],pRow,pFromRowNub);
    //pCrwBuffer->vDropWrite();
    
    //pCrwBuffer->vDump();
    
    // appended ?
    if (bRowAppended)
      return EC_NONE;
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job

    m_pCrwBuffer[i_Ptn] = nullptr;

    m_paRowCoder_Copy[i_Ptn]->vCopyEnd();
    Job_WriteBuffer::vSpawnJobWriteBuffer(m_pParentJob->m_pSession,
                                          pTable()->aTableName(),
                                          m_eDataSource,
                                          pCrwBuffer,
                                          m_pParentJob->iJobId(),
                                          i_Ptn,
                                          C_NOT_FINAL,
                                          0);
    
    // repeat, try-again
    // ...
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bValid = bEncodeRow(szRecStr, bBlankLine);
  
  if (bBlankLine)
    return EC_BLANKLINE;

  if (! bValid)
    return EC_INVALID_VALUE;

  // attempt to add row into a write-behind buffer
  eEC eError = eInsertRow(m_pRow);

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
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
void JobWriteBehind::vNewCrwBuffer( i8 i_Ptn)
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

