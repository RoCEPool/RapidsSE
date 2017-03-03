/*============================================================================*/
/* RegFrame.hpp -- Register Frame                                             */
/*============================================================================*/
#pragma once

namespace Dictionaries
{
  static const i8 C_VALREGS = 16;
  
  class RegFrame : public Buffer
  {
  public:
    s32       m_sName;              // name of the reg frame
    RegFrame* m_pNext;              // nest frame in stack / list
    void*     m_pStackPtr;          // discard point (when in call stack)
    Dic*      m_pOpDic;             // operator dic associated with this frame
    ValReg    m_xValReg[C_VALREGS]; // set of registers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RegFrame(void);
    ~RegFrame(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    RegFrame* pNext(void);
    void      vNext(RegFrame* pRegFrame);
    
    s32&      sName(void);
    void      vName(s32& sName);

    void*     pStackPtr(void);
    void      vStackPtr(void* pStackPtr);

    Dic*      pOpDic(void);
    void      vOpDic(Dic* pOpDic);
    
    ValReg* pValReg(i8 iReg);
    
    Footer* pFooter(void);
  };
  
  RegFrame* _pRegFrame(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
