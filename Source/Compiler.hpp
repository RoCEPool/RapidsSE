/*============================================================================*/
/* Compiler.hpp -- Sen Word Compiler                                          */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Compiler : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Compiler(void);
    ~Compiler(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
