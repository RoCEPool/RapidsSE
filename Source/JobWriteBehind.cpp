/*============================================================================*/
/* JobWriteBehind.cpp -- Write Behind part of a Job (Ingest/Insert/Load)     */
/*============================================================================*/

// Job Short: WriteBehind

#include "JobModel.h"
#include "StorageEngine.h"

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
                                m_aRecord.aStr(),
                                m_nRecordBytes,
                                bBlankLine,
                                false /* allowNonPrintable */);
  
  return bValid;
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
bool JobWriteBehind::bProcessRecord(Interface::Session* pSession,
                                     i8 iParentJobId,
                                     Table* pTable,
                                     Formatters* pFormatters,
                                     Stz* aRecord,
                                     i8 nBytes)
{
  bool bBlankLine = false;
  
  bool bFormat = bFormatRow(pTable, pFormatters, aRecord, nBytes, bBlankLine);
  RASSERT( ! bBlankLine );
  
  // attempt to add record into a write-behind buffer
  eEC eErrorCode = eInsertRow(pSession, pTable, DS_INGEST, iParentJobId);
  
  // was there an error ?
  if (eErrorCode != EC_NONE) {
    vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eErrorCode]);
    return false;
  }

  return true;
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

