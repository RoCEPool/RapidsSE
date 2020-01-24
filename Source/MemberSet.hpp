/*============================================================================*/
/* MemberSet.hpp - Member Set                                                 */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberSet : public Object
  {
  public:
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberSet(void);
   ~MemberSet(void) noexcept(false);
      
    Footer* aFooter(void);
  };

  MemberSet* _pMemberSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
