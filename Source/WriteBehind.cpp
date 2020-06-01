/*============================================================================*/
/* WriteBehind.cpp -- Write Behind  (Ingest/Insert/Load)                      */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
WriteBehind* CompactBuffers::_pWriteBehind(void* aAddr)
{
  WriteBehind* pWriteBehind = reinterpret_cast<WriteBehind*>(aAddr);
  
  return pWriteBehind;
}

/*----------------------------------------------------------------------------*/
/* allocate a new writeBehind from heap                                       */
/*----------------------------------------------------------------------------*/
void* WriteBehind::operator new(size_t)
{
  WriteBehind* pWriteBehind =
  static_cast<WriteBehind*>(HeapMem::pAllocate(CN_WRITEBEHIND));
  
  return pWriteBehind;
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* free a writeBehind back to the heap                                        */
/*----------------------------------------------------------------------------*/
void WriteBehind::operator delete(void* aAddress)
{
  WriteBehind* pWriteBehind = static_cast<WriteBehind*>(aAddress);
  
  HeapMem::vFree(pWriteBehind);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteBehind::WriteBehind(Table* pTable)
: Object(CN_WRITEBEHIND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pTable = pTable;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pCrwBuffer[i_Partition]     = nullptr;
    m_paRowCompactor[i_Partition] = nullptr;
  }
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;
  
  _checkInitialised_(CN_WRITEBEHIND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteBehind::~WriteBehind(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
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
eEC WriteBehind::eAppendRow(RowSingle* pRow, CrwBuffer** ppCrwBuffer, i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
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
    bool bWasAppended = pCrwBuffer->bAppendRow(pRow);
    //pCrwBuffer->vDropWrite();
    //pCrwBuffer->vDump();
    
    // appended ?
    if (bWasAppended)
      return EC_NONE;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn a write buffer job
    
    m_pCrwBuffer[i_Ptn] = nullptr;
    pCrwBuffer->vAppendClose();
    
    // return the full buffer
    *ppCrwBuffer = pCrwBuffer;
    
    // repeat, try-again
    // ...
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* return the ptn crwBuffer                                                   */
/*----------------------------------------------------------------------------*/
CrwBuffer* WriteBehind::pCrwBuffer(i8 iPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  return m_pCrwBuffer[iPtn];
}

/*----------------------------------------------------------------------------*/
/* set the ptn crwBuffer                                                      */
/*----------------------------------------------------------------------------*/
void WriteBehind::vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pCrwBuffer[iPtn] = pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
void WriteBehind::vNewCrwBuffer( i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pCrwBuffer[i_Ptn] = new CrwBuffer();
  
  m_pCrwBuffer[i_Ptn]->vTable(m_pTable);
  m_pCrwBuffer[i_Ptn]->vPartitionNo(i_Ptn);
  //m_pCrwBuffer[i_Ptn]->vDump();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(WriteBehind)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

