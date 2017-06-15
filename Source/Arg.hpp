/*============================================================================*/
/* Arg.hpp - Set of Candidate Paths                                           */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Arg : public Object
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Arg(void);
    ~Arg(void);
  
    void vGenPaths(void);
  
    Footer* pFooter(void);
  };
  
  Arg* _pArg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
