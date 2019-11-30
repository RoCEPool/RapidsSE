/*============================================================================*/
/* LocalChain.hpp -- Locals Frame Chain                                       */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_LOCALS = 32;
  
  class LocalChain : public Object
  {
  public:
    LocalChain* m_pNext;             // nest frame in chain
    AnyVal*     m_aDiscardPoint;     // discard point (when in call stack)
    Local       m_xLocal[C_LOCALS];  // set of local values
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalChain(void);
    ~LocalChain(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    LocalChain* pNext(void);
    void      vNext(LocalChain* pLocalSet);
    
    AnyVal* aDiscardPoint(void);
    void        vDiscardPoint(AnyVal* aDiscardPoint);
    
    Local*    pLocal(i8 i_Local);
  
    // must be last
    Footer* aFooter(void);
  };
  
  LocalChain* _pLocalSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
