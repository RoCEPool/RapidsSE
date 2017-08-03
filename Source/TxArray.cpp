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
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_TXARRAY);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxArray::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: TxArray class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxArray::TxArray(void)
: Object(CN_TXARRAY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXARRAY,_FOOTER_);
  
  for (i8 i_Tx = 0; i_Tx < C_TX_ARRAY; i_Tx++)
    m_pTx[i_Tx] = nullptr;

  _checkInitialised_(CN_TXARRAY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TxArray::~TxArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TxArray::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_TXARRAY);
  ASSERT(pObject->eClass() == CN_TXARRAY);
  
  TxArray* pTxArray = reinterpret_cast<TxArray*>(pObject);

  i8 iTxCount = 0;
  
  for (i8 i_Tx = 0; i_Tx < C_TX_ARRAY; i_Tx++)
    if (pTxArray->m_pTx[i_Tx] != nullptr)
      iTxCount++;
  
  vTrace("tx count %lld\n"_Stz,
         iTxCount);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxArray::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXARRAY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
