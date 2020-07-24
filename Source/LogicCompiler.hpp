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
    Footer* aFooter(void);
  };
  
  LogicCompiler* _pLogicCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
