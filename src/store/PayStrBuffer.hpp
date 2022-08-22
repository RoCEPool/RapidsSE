/*============================================================================*/
/* PayStrBuffer.hpp -- Payload String Buffer                                  */
/*============================================================================*/
#pragma once

namespace STORE
{
  class PayStr;

  class PayStrBuffer : public Object
  {
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PayStrBuffer(void);
   ~PayStrBuffer(void);

  public:
    Footer* aFooter(void);
  };

  PayStrBuffer* _pPayStrBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
