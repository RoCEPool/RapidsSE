/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class StageSpec : public Object
  {
  private:
  
    ePO m_eTopology;
  
    OpSpecVec* m_pOpSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(void);
   ~StageSpec(void) noexcept(false);
  
    void vAddOpSpec(OpSpec* pOpSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
