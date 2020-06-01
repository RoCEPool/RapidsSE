/*============================================================================*/
/* WriteBehindH.cpp -- Write Behind  (Ingest/Insert/Load)                     */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,WriteHash,CN_WRITEHASH)
__aNewFunction(WriteHash,CN_WRITEHASH)
__vDeleteFunction(WriteHash,CN_WRITEHASH)
__aFooterFunction(WriteHash)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteHash::WriteHash(Table* pTable)
: Object(CN_WRITEHASH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);
  
  m_pTable = pTable;
  m_pRow = new RowSingle(pTable->pStreamDef(),C_ALL);
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pCrwBuffer[i_Partition]     = nullptr;
    m_paRowCompactor[i_Partition] = nullptr;
  }
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;
  
  _checkInitialised_(CN_WRITEHASH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteHash::~WriteHash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  delete m_pRow;
  
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
/* return the ptn crwBuffer                                                   */
/*----------------------------------------------------------------------------*/
CrwBuffer* WriteHash::pCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);
  return m_pCrwBuffer[iPtn];
}

/*----------------------------------------------------------------------------*/
/* set the ptn crwBuffer                                                      */
/*----------------------------------------------------------------------------*/
void WriteHash::vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  m_pCrwBuffer[iPtn] = pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full ptn crwBuffer if any                                         */
/*----------------------------------------------------------------------------*/
CrwBuffer* WriteHash::pFullCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  CrwBuffer* pCrwBuffer = m_pCrwBuffer[iPtn] ? m_pCrwBuffer[iPtn]->pNextBuffer() : nullptr;
  CrwBuffer* pFirstCrwBuffer = nullptr;
  
  while( pCrwBuffer) {
    pFirstCrwBuffer = pCrwBuffer;
    pCrwBuffer = pCrwBuffer->pNextBuffer();
  }
  
  if (pFirstCrwBuffer) {
    CrwBuffer* pPrevBuffer = pFirstCrwBuffer->pPrevBuffer();
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pFirstCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* process record                                                             */
/*----------------------------------------------------------------------------*/
eEC WriteHash::eProcessRecord(Format* pFormatter, Stz* szRecStr, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

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
  
  bool bValid = bExtractRow(pFormatter, szRecStr, bBlankLine);
  
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
/* append row into working buffer                                             */
/*----------------------------------------------------------------------------*/
eEC WriteHash::eAppendRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  CrwBuffer* pCrwBuffer = nullptr;
  
  // use row hash to get partition
  i8 i_Ptn = m_pTable->iToPartition(m_pRow->m_iHash);
  
  // append the row into a working buffer
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for row buffer
  
  if (! m_pCrwBuffer[i_Ptn]) {
    
    // create a new crw buffer for this partition
    pCrwBuffer = pNewCrwBuffer(i_Ptn);
    
    if (! pCrwBuffer) {
      vTrace("CrwBuffer allocation failed\n"_Stz);
      return EC_LOW_MEMORY;
    }

  }
  else {
    pCrwBuffer = m_pCrwBuffer[i_Ptn];
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
  pCrwBuffer = pNewCrwBuffer(i_Ptn);
  
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
bool WriteHash::bExtractRow(Format* pFormatter, Stz* szRecStr, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  StreamDef* pStreamDef = m_pTable->pStreamDef();

  m_pRow->vResetValues();
  
  char* acRecCrs = szRecStr->azChar();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = CsvCursor::bExtractNext(m_pRow,pStreamDef,
                                         &acRecCrs,pFormatter,
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
CrwBuffer* WriteHash::pNewCrwBuffer( i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEHASH,_FOOTER_);

  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();;
  
  // link the old with this
  pCrwBuffer->vNextBuffer(m_pCrwBuffer[i_Ptn]);
  
  if (m_pCrwBuffer[i_Ptn])
    m_pCrwBuffer[i_Ptn]->vPrevBuffer(pCrwBuffer);
  
  m_pCrwBuffer[i_Ptn] = pCrwBuffer;
  
  // set table and ptn no in it
  pCrwBuffer->vTable(m_pTable);
  pCrwBuffer->vPartitionNo(i_Ptn);
  
  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[i_Ptn], true);
  
  //m_pCrwBuffer[i_Ptn]->vDump();
  
  return pCrwBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
