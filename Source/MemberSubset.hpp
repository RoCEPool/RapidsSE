/*============================================================================*/
/* MemberSubset.hpp - Member Subset                                           */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberSubset : public Object
  {
  public:
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberSubset(void);
   ~MemberSubset(void) noexcept(false);
      
    Footer* aFooter(void);
  };

  MemberSubset* _pMemberSubset(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
