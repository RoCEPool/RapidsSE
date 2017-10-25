/*============================================================================*/
/* TxRef.hpp -- Transaction Reference                                         */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxRef : public Object
  {
  private:
    Tx* m_pTx;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TxRef* pTxRef);
    void  operator delete(void*);
    
    static void vFinalize(eCN eClass, Object* pObject);

    TxRef(void);
    ~TxRef(void);
    
    i8   iTxId(void);
  
    bool bBeginTx(void);
    bool bCommitTx(void);
    void vDump(void);
    bool bEndTx(void);
    bool bRollbackTx(void);
    
    Footer* aFooter(void);
  };
  
  TxRef* _pTxRef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
