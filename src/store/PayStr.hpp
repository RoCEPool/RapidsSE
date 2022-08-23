/*============================================================================*/
/* PayStr.hpp -- Payload String                                               */
/*============================================================================*/
#pragma once

namespace STORE
{
  class PayStrBuffer;

  class PayStr : public Object
  {
  friend class PayStrBuffer;

  protected:
    i8   m_jArea;   // byte length of char area/array
    char m_caCharArea[1];  // area to hold chars

  public:
    static i8 jFullSize(i8 jSizeStz);

    void* operator new(size_t, PayStrBuffer* pPayStrBuffer, Stz* szStz);

    PayStr(void);

    i8 jSizeOf(void);
    Stz* szStz(void);

  public:

    Footer* aFooter(void);
  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
