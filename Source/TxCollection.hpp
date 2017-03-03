/*============================================================================*/
/* gpTxCollection.hpp -- Transaction Collection                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxCollection : public Object,
                       public Collection_t<Tx>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, TxCollection* pTxCollection);
    void  operator delete(void*);
    
    TxCollection(void);
    ~TxCollection(void);
    
    Tx*   pAllocTx(void);
    void  vFreeTx(Tx* pTx);
    
    Footer* pFooter(void);
  };
  
  TxCollection* _pTxCollection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
