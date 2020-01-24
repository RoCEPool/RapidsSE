/*============================================================================*/
/* MemberVar.hpp - Member Variable                                            */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberVar : public Object
  {
  public:
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberVar(void);
   ~MemberVar(void) noexcept(false);
  
    static bool bIsMemberVar(EnameSet* pEnameSet);
      
    Footer* aFooter(void);
  };

  MemberVar* _pMemberVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
