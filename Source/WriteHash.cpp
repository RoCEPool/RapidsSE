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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
