/*============================================================================*/
/* LogicCompiler.hpp -- Logic Compiler                                        */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_LOGIC_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class LogicCompiler : public Object
  {
  private:
  
    char  m_caSource[C_LOGIC_SOURCE];
  
    char* m_azSrcCrs;
  
    void* m_aTccState;

  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicCompiler(void);
   ~LogicCompiler(void) noexcept(false);

    i8 iCall(void);
    i8 iCompile(i8 nCodes, eEO eaCode[]);
  
  private:
  
    void vAcquire(void);
    void vBegin(void);
    bool bCompile(void);
    void vEnd(void);
    void vRelease(void);
    void vSymbols(void);

    void v_preliminary(void);
    void v_concluding(void);
    void v_EO_INTEGER (i8& ri_Code, eEO eaCode[]);
    void v_EO_ADD     (i8& ri_Code, eEO eaCode[]);
    void v_EO_PRINT   (i8& ri_Code, eEO eaCode[]);
    void v_EO_RETURN  (i8& ri_Code, eEO eaCode[]);
  
    i8 iToSource(i8 nCodes, eEO eaCode[]);
  
    Footer* aFooter(void);
  };
  
  LogicCompiler* _pLogicCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
