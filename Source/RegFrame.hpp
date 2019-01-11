/*============================================================================*/
/* RegFrame.hpp -- Register Frame                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_REGISTERS = 32;
  
  class RegFrame : public Object
  {
  public:
    RegFrame* m_pNext;              // nest frame in chain
    void*     m_pStackPtr;          // discard point (when in call stack)
    Register  m_xRegister[C_REGISTERS]; // set of registers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RegFrame(void);
    ~RegFrame(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    RegFrame* pNext(void);
    void      vNext(RegFrame* pRegFrame);
    
    void*     pStackPtr(void);
    void      vStackPtr(void* pStackPtr);
    
    Register* pRegister(i8 iReg);
    
    Footer* aFooter(void);
  };
  
  RegFrame* _pRegFrame(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
