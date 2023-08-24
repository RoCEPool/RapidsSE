/*============================================================================*/
/* PayStrBuffer.hpp -- Payload String Buffer                                  */
/*============================================================================*/
#pragma once

namespace STORE
{
  static const i8 C_WSP_PAYSTRBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;

  class PayStr;

  class PayStrBuffer : public Object
  {
    PayStrBuffer* m_pFrom;
    PayStrBuffer* m_pNext;

    Wsp_t<C_WSP_PAYSTRBUFFER> m_xWsp;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PayStrBuffer(void);
   ~PayStrBuffer(void) noexcept(false);

  public:
    byte* aAllot(Stz* szStz, i8* piSizeAllocated);

    PayStrBuffer* pFrom(void);
    void vFrom(PayStrBuffer* pFrom);

    PayStrBuffer* pNext(void);
    void vNext(PayStrBuffer* pNext);

    Footer* aFooter(void);

  private:

    byte* aAllot(i8 jSizeStz);

  };

  PayStrBuffer* _pPayStrBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
