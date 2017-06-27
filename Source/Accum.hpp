/*============================================================================*/
/* Accum.hpp -- Accumulator                                                   */
/*============================================================================*/
#pragma once

namespace MockEE
{
  class Accum : public Object
  {
  public:
    i8  iGroupNo; // which group number
    i8  iCount;   // number of values accumulated
    i8  iSum;     // sum of the values in the group
    i8  iMin;     // minumum value in group
    i8  iMax;     // maximum value in group
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Accum(void);
    ~Accum(void);
  
    Footer* aFooter(void);
  };
  
  Accum* _pAccum(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
