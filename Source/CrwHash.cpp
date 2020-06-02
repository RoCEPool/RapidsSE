/*============================================================================*/
/* CrwHash.cpp -- Compact Row Hash                                            */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,CrwHash,CN_CRWHASH)
__aNewFunction(CrwHash,CN_CRWHASH)
__vDeleteFunction(CrwHash,CN_CRWHASH)
__aFooterFunction(CrwHash)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrwHash::CrwHash(Table* pTable)
: Object(CN_CRWHASH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);
  
  m_pTable = pTable;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pCrwBuffer[i_Partition]     = nullptr;
    m_paRowCompactor[i_Partition] = nullptr;
  }
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;
  
  _checkInitialised_(CN_CRWHASH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwHash::~CrwHash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

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
eEC CrwHash::eAppendRow(RowSingle* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

  CrwBuffer* pCrwBuffer = nullptr;
  
  // use row hash to get partition
  i8 i_Ptn = m_pTable->iToPartition(pRow->m_iHash);
  
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
  bool bWasAppended = pCrwBuffer->bAppendRow(pRow);
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
  bWasAppended = pCrwBuffer->bAppendRow(pRow);
  ASSERT(bWasAppended);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* return the ptn crwBuffer                                                   */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwHash::pCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);
  return m_pCrwBuffer[iPtn];
}

/*----------------------------------------------------------------------------*/
/* set the ptn crwBuffer                                                      */
/*----------------------------------------------------------------------------*/
void CrwHash::vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

  m_pCrwBuffer[iPtn] = pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full ptn crwBuffer if any                                         */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwHash::pFullCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwHash::pNewCrwBuffer( i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

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
