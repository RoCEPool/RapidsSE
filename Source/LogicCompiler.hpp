/*============================================================================*/
/* LogicCompiler.hpp -- Logic Compiler                                        */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class LogicCompiler : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicCompiler(void);
   ~LogicCompiler(void) noexcept(false);

    void vCompile(i8 nCodes, eEO eaCode[]);
  
  private:
  
    void v_EO_INTEGER (i8& ri_Code, eEO eaCode[]);
    void v_EO_ADD     (i8& ri_Code, eEO eaCode[]);
    void v_EO_PRINT   (i8& ri_Code, eEO eaCode[]);
    void v_EO_RETURN  (i8& ri_Code, eEO eaCode[]);
  
    void vBegin(void);
    void vEnd(void);
    void vRelease(void);

    Footer* aFooter(void);
  };
  
  LogicCompiler* _pLogicCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
