/*============================================================================*/
/* DevRSB.hpp -- RSB Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevRSB : public Device
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevRSB(void);
   ~DevRSB(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  DevRSB* _pDevRSB(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
