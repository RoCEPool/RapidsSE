/*============================================================================*/
/* User.hpp - User                                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class User : public Object
  {
  private:
    s32   sName;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, User* pUser);
    void  operator delete(void*);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
