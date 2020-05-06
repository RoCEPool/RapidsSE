/*============================================================================*/
/* Profile.hpp - runtime profile of task                                      */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Profile : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Profile(void);
   ~Profile(void) noexcept(false);
    
    Footer* aFooter(void);
  };
  
  Profile* _pProfile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
