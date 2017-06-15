/*============================================================================*/
/* Predicte.hpp - RDP Predicate                                               */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Predicte : public Object
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Predicte(void);
    ~Predicte(void);
  
    void vGenPaths(void);
  
    Footer* pFooter(void);
  };
  
  Predicte* _pPredicte(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
