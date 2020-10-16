/*============================================================================*/
/* HashSet.cpp -- Set of Partition Hashed Buffers with dangling support       */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,HashSet,CN_HASHSET)
__aNewFunction(HashSet,CN_HASHSET)
__vDeleteFunction(HashSet,CN_HASHSET)
__aFooterFunction(HashSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashSet::HashSet(void)
: Object(CN_HASHSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);
  

  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++)
    m_paBufferChain[i_Ptn] = nullptr;
    
  _checkInitialised_(CN_HASHSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashSet::~HashSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);

  // Assert we don't have any dangling buffers
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    RASSERT(! m_paBufferChain[i_Ptn]);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void HashSet::vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);

   // link the old with this
   pDataBuffer->vNextBuffer(m_paBufferChain[i_Ptn]);
   
   if (m_paBufferChain[i_Ptn])
     m_paBufferChain[i_Ptn]->vPrevBuffer(pDataBuffer);
   
   m_paBufferChain[i_Ptn] = pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full crwBuffer for given partition (if any)                       */
/*----------------------------------------------------------------------------*/
DataBuffer* HashSet::pBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);

  DataBuffer* pBuffer = m_paBufferChain[i_Ptn];

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full crwBuffer for given partition (if any)                       */
/*----------------------------------------------------------------------------*/
DataBuffer* HashSet::pCompletedBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);

  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pDataBuffer = (DataBuffer*)
                              (m_paBufferChain[i_Ptn]
                                 ? m_paBufferChain[i_Ptn]->pNextBuffer()
                                 : nullptr);
  while (pDataBuffer) {
    pCompletedBuffer = pDataBuffer;
    pDataBuffer = (DataBuffer*)pDataBuffer->pNextBuffer();
  }
  
  if (pCompletedBuffer) {
    DataBuffer* pPrevBuffer = (DataBuffer*)pCompletedBuffer->pPrevBuffer();
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pCompletedBuffer;
}

/*----------------------------------------------------------------------------*/
/* return any final dangling buffer                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* HashSet::pDanglingBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHSET,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_paBufferChain[i_Ptn];
  
  m_paBufferChain[i_Ptn] = nullptr;
  
  return pDataBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
