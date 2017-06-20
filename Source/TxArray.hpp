/*============================================================================*/
/* TxArray.hpp -- Transaction Array                                           */
/*============================================================================*/
#pragma once

namespace Transactions
{
  const static i8 C_TX_ARRAY = 16;
  
  class TxArray : public Object
  {
    friend class TxMap;
    
  private:
    
    Tx* m_pTx[C_TX_ARRAY];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxArray(void);
    ~TxArray(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
