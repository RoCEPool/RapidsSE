/*============================================================================*/
/* Tx.hpp - Transaction                                                       */
/*============================================================================*/
#pragma once

namespace Transactions
{    
  class Tx : public Focal
  {
  private:
    Lock   m_okLock;       // lock
    t8     m_tPerspective; // perspective
    eXP    m_ePhase;       // phase (executing, committing, ...)
  
    i8     m_iLocalTxId;   // local transaction id
    i8     m_iDistTxId;    // distributed transaction id
    Atomic m_ixCounter;    // general counter

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

    void vFocalOutline(s4K& sOutline);
  
    i8 iLocalTxId(void);
    i8 iLocalTxNo(void);
    i8 iLocalTxNode(void);
    void vLocalTxId(i8 iTxId);

    i8 iDistTxId(void);
    i8 iDistTxNo(void);
    i8 iDistTxNode(void);
    void vDistTxId(i8 iTxId);
  
    t8   tPerspective(void);
  
    eXP  ePhase(void);
    void vPhase(eXP ePhase);
    
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
