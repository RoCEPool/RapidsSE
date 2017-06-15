/*============================================================================*/
/* Op.hpp - RDP Op                                                            */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Op : public Object
  {
  private:
    eRO m_eOperator; // what kind of operator
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Op(void);
    ~Op(void);
  
    void vGenPaths(void);
  
    Footer* pFooter(void);
  };
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
