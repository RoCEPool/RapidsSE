/*============================================================================*/
/* Tx.hpp - Transaction                                                       */
/*============================================================================*/
#pragma once

namespace Transactions
{    
  enum eTP // Transaction Phase
  {
    /* 0 */ TP_NONE=0,
    /* 1 */ TP_EXECUTING,
    /* 2 */ TP_COMMITTING,
    /* 3 */ TP_COMMITTED,
    /* 4 */ TP_PREPARING,
    /* 5 */ TP_PREPARED,
    /* 6 */ TP_ROLLINGBACK,
    /* 7 */ TP_ROLLEDBACK,
    /* 8 */ TP_FREE,
    /* 9 */ TP_ENUMS
  };
  
  static Str* pTP[]
  {
    /* 0 */ "FREE"_Str,
    /* 1 */ "ALLOCATED"_Str,
    /* 2 */ "EXECUTING"_Str,
    /* 3 */ "COMMITTING"_Str,
    /* 4 */ "COMMITTED"_Str,
    /* 5 */ "PREPARING"_Str,
    /* 6 */ "PREPARED"_Str,
    /* 7 */ "ROLLINGBACK"_Str,
    /* 8 */ "ROLLEDBACK"_Str,
    /* 9 */ "ENUMS"_Str
  };
  
  class Tx : public Object,
             public CollectionNode_t<Tx>
  {
  private:
    Lock  m_xLock;    // lock
    i8    m_iTxSlot;  // transaction slot (0..C_TX_ARRAY-1)
    i8    m_iTxId;    // transaction id
    eTP   m_ePhase;   // phase (executing, committing, ...)

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Tx* pTx);
    void  operator delete(void*);
    
    Tx(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    i8 iTxSlot(void);
    void vTxSlot(i8 iTxSlot);

    i8 iTxId(void);
    void vTxId(i8 iTxId);

    eTP ePhase(void);
    void vPhase(eTP ePhase);
    
    void vContentDump(void);
    
    Footer* pFooter(void);
  };
    
  Tx* _pTx(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
