/*============================================================================*/
/* Profile.hpp - runtime profile of task                                      */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Profile : public Object
  {
  private:
    
    RU_Memory m_oRU_Memory;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Profile(void);
   ~Profile(void) noexcept(false);
    
    RU_Memory* pRU_Memory();

    Footer* aFooter(void);
  };
  
  Profile* _pProfile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
