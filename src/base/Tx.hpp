/*============================================================================*/
/* Tx.hpp - Transaction                                                       */
/*============================================================================*/
#pragma once

#include "eXP.h"

namespace BASE
{    
  class Tx : public Object
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
    static void vRepair(Object* pObject, eRF eRepair);

    void vContentDump(void);
    void vOutline(s4K& sOutline);
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
