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

    void vFocalOutline(s4K& sOutline);
    
    t8   tPerspective(void);
    void vPerspective(t8 tPerspective);

    eXP  ePhase(void);
    void vPhase(eXP ePhase);
    
    void vContentDump(void);
    
    bool bIsActive(void);
  
  private:
    Footer* aFooter(void);
  };
    
  Tx* _pTx(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
