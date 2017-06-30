/*============================================================================*/
/* Accum.hpp -- Accumulator                                                   */
/*============================================================================*/
#pragma once

namespace MockEE
{
  class GroupBy : public Object
  {
  public:
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    GroupBy(void);
    ~GroupBy(void);
  
    Footer* aFooter(void);
  };
  
  GroupBy* _pGroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
