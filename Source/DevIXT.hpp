/*============================================================================*/
/* DevIXT.hpp -- IXT Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevIXT : public Device
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevIXT(void);
   ~DevIXT(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  DevIXT* _pDevIXT(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
