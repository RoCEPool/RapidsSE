/*============================================================================*/
/* LocalSet.hpp -- Local Set                                                  */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_LOCALS = 32;
  
  class LocalSet : public Object
  {
  public:
    LocalSet* m_pNext;             // nest frame in chain
    Value*    m_aDiscardPoint;     // discard point (when in call stack)
    Local     m_xLocal[C_LOCALS];  // set of local values
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalSet(void);
    ~LocalSet(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    LocalSet* pNext(void);
    void      vNext(LocalSet* pLocalSet);
    
    Value*    aDiscardPoint(void);
    void      vDiscardPoint(Value* aDiscardPoint);
    
    Local*    pLocal(i8 i_Local);
  
    // must be last
    Footer* aFooter(void);
  };
  
  LocalSet* _pLocalSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
