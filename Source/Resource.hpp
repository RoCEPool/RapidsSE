/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public Focal
  {
  protected:
    ReqTuple m_xReqTuple;
  
    Atomic m_ixAct;
  
  public:
    Resource(eCN eClass, ReqTuple* aReqTuple);
   ~Resource(void) noexcept(false);
  
    void vFocalOutline(s4K& sOutline);
    i8   iResId(void);
  
    ReqTuple* aReqTuple(void);
    i8 iAct(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
