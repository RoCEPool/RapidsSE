/*============================================================================*/
/* LocalSet.hpp -- Local Frame                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_REGISTERS = 32;
  
  class LocalSet : public Object
  {
  public:
    LocalSet* m_pNext;                // nest frame in chain
    void*     m_pStackPtr;            // discard point (when in call stack)
    Local     m_xLocal[C_REGISTERS];  // set of registers
    
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
    
    void*     pStackPtr(void);
    void      vStackPtr(void* pStackPtr);
    
    Local* pLocal(i8 iReg);
    
    Footer* aFooter(void);
  };
  
  LocalSet* _pLocalSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
