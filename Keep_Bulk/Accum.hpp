/*============================================================================*/
/* Accum.hpp -- Accumulator                                                   */
/*============================================================================*/
#pragma once

namespace MockEE
{
  class Accum : public Object
  {
  public:
    i8  m_iGroup; // which group number
    i8  m_iCount; // number of values accumulated
    i8  m_iSum;   // sum of the values in the group
    i8  m_iMin;   // minumum value in group
    i8  m_iMax;   // maximum value in group
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Accum(void);
    ~Accum(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Accum* _pAccum(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
