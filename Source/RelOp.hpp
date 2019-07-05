/*============================================================================*/
/* RelOp.hpp - Relational Operator                                            */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class RelOp : public Object
  {
  private:
    eRO m_eRelOp; // what kind of relational operator
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
  
    static eRO eDecode(Stz* aOpText);
  
    RelOp(void);
    RelOp(eRO eRelOp);
    ~RelOp(void) noexcept(false);
  
    eRO eRelOp(void);
    void vRelOp(eRO eRelOp);
  
    Footer* aFooter(void);
  };
  
  RelOp* _pRelOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
