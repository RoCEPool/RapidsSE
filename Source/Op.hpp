/*============================================================================*/
/* Op.hpp - RDP Op                                                            */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Op : public Object
  {
  private:
    eRO m_eOpKind; // what kind of operator
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Op(void);
    Op(eRO eOpKind);
    ~Op(void);
  
    eRO eOpKind(void);
    void vOpKind(eRO eOpKind);
  
    Footer* pFooter(void);
  };
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
