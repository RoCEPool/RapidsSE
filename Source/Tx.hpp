/*============================================================================*/
/* Tx.hpp - Transaction                                                       */
/*============================================================================*/
#pragma once

namespace Transactions
{    
  class Tx : public Focal
  {
  private:
    Lock m_okLock;       // lock
    t8   m_tPerspective; // perspective
    eXP  m_ePhase;       // phase (executing, committing, ...)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Tx* pTx);
    void  operator delete(void*);
    
    Tx(void);
   ~Tx(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vContentDump(void);
    void vFocalOutline(s4K& sOutline);
    bool bIsActive(void);
    t8   tPerspective(void);
    void vPerspective(t8 tPerspective);
    eXP  ePhase(void);
    void vPhase(eXP ePhase);
  
  private:
    Footer* aFooter(void);
  };
    
  Tx* _pTx(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
