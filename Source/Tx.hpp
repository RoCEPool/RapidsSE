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
    /* 8 */ TP_ENUMS
  };
  
  static Stz* pTP[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "EXECUTING"_Stz,
    /* 2 */ "COMMITTING"_Stz,
    /* 3 */ "COMMITTED"_Stz,
    /* 4 */ "PREPARING"_Stz,
    /* 5 */ "PREPARED"_Stz,
    /* 6 */ "ROLLINGBACK"_Stz,
    /* 7 */ "ROLLEDBACK"_Stz,
    /* 8 */ "ENUMS"_Stz
  };
  
  class Tx : public Object
  {
  private:
    Lock   m_oLock;      // lock
    i8     m_iLocalTxId; // local transaction id
    i8     m_iDistTxId;  // distributed transaction id
    eTP    m_ePhase;     // phase (executing, committing, ...)
    Atomic m_ixCounter;   // general counter

    struct {
      bool m_bDistMember; // true if part of distributed tx
    } m_xNode[C_MAX_NODES];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Tx* pTx);
    void  operator delete(void*);
    
    Tx(bool bAddToSet = true);
    Tx(i8 iDistTxId);
    ~Tx(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Tx*  pTestDistTx(i8 iDistTxId);

    static i8 iTxNo(i8 iTxId);
    static i8 iTxNode(i8 iTxId);

    i8 iLocalTxId(void);
    i8 iLocalTxNo(void);
    i8 iLocalTxNode(void);
    void vLocalTxId(i8 iTxId);

    i8 iDistTxId(void);
    i8 iDistTxNo(void);
    i8 iDistTxNode(void);
    void vDistTxId(i8 iTxId);
  
    eTP ePhase(void);
    void vPhase(eTP ePhase);
    
    void vContentDump(void);
  
    i8   iCounterDec(void);
    i8   iCounterGet(void);
    void vCounterSet(i8 iCounter);
  
    bool bLocalTx(void);
    bool bDistTx(void);
    Stz* aDecode(void);
  
    bool bDistMember(i8 i_Node);
    void vDistMember(i8 i_Node, bool bDistMember);
  
    bool bIsActive(void);
  
    Footer* aFooter(void);
  };
    
  Tx* _pTx(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
