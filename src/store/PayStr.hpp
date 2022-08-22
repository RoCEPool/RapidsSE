/*============================================================================*/
/* PayStr.hpp -- Payload String                                               */
/*============================================================================*/
#pragma once

namespace STORE
{
  class PayStrBuffer;

  class PayStr : public Object
  {
  protected:
    i8   m_jArea;   // byte length of char area/array
    char m_caCharArea[1];  // area to hold chars

  public:
    void* operator new(size_t, void* PayStrBuffer, Stz* szStz);

    PayStr(void);

  public:
    i8 jSizeOf(void);

    Footer* aFooter(void);
  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
