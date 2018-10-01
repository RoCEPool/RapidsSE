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
    Compiler(void);
    ~Compiler(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
