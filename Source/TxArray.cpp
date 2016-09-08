/*============================================================================*/
/* TxArray.cpp -- Transaction Array                                           */
/*============================================================================*/

#include "Transactions.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxArray::operator new(size_t iSize)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxArray::operator delete(void* pBufferAddress)
{
  vFatal("fatal: TxArray class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxArray::TxArray(void)
: Object(CN_TXARRAY)
{
  for (i8 iTx = 0; iTx < C_TX_ARRAY; iTx++)
    m_pTx[iTx] = nullptr;

  Memory::vCheckInitialised(this,sizeof(TxArray));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TxArray::~TxArray(void)
{
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TxArray::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TXARRAY);
  ASSERT(pObject->eClass() == CN_TXARRAY);
  
  TxArray* pTxArray = reinterpret_cast<TxArray*>(pObject);

  i8 iTxCount = 0;
  
  vTrace("%sTxArray %p: \n", pTab, pTxArray);
  for (i8 iTx = 0; iTx < C_TX_ARRAY; iTx++)
    if (pTxArray->m_pTx[iTx] != nullptr)
      iTxCount++;
  vTrace("%s  txs: %lld\n\n", pTab, iTxCount);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxArray::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/