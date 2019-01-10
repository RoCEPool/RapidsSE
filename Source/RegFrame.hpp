/*============================================================================*/
/* RegFrame.hpp -- Register Frame                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_VALREGS = 32;
  
  class RegFrame : public Object
  {
  public:
    s32       m_sName;              // name of the reg frame
    RegFrame* m_pNext;              // nest frame in stack / list
    void*     m_pStackPtr;          // discard point (when in call stack)
    Dic*      m_pOpDic;             // operator dic associated with this frame
    Register  m_xRegister[C_VALREGS]; // set of registers
    
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
    
    s32&      sName(void);
    void      vName(s32& sName);

    void*     pStackPtr(void);
    void      vStackPtr(void* pStackPtr);

    Dic*      pOpDic(void);
    void      vOpDic(Dic* pOpDic);
    
    Register* pRegister(i8 iReg);
    
    Footer* aFooter(void);
  };
  
  RegFrame* _pRegFrame(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
