/*============================================================================*/
/* DevRLU.hpp -- RLU Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevRLU : public Device
  {
  public:
  
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevRLU(void);
   ~DevRLU(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  DevRLU* _pDevRLU(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
