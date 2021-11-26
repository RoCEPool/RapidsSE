/*============================================================================*/
/* Starter.hpp -- Starter                                                     */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Starter : public Object
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Starter(void);
   ~Starter(void) noexcept(false);
  
    Footer* aFooter(void);
  };

  Starter* _pStarter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
