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
  
    struct {
      bool m_bDistMember; // true if part of distributed tx
    } m_xNode[C_MAX_NODES];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Tx* pTx);
    void  operator delete(void*);
    
    Tx(void);
    ~Tx(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Tx*  pTestDistTx(i8 iDistTxId);

    static i8 iTxNo(i8 iTxId);
    static i8 iTxNode(i8 iTxId);

    void vFocalOutline(s4K& sOutline);
    
    t8   tPerspective(void);
    void vPerspective(t8 tPerspective);

    eXP  ePhase(void);
    void vPhase(eXP ePhase);
    
    void vContentDump(void);
    
    bool bIsActive(void);
  
    Footer* aFooter(void);
  };
    
  Tx* _pTx(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
