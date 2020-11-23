/*============================================================================*/
/* KeyBuilder.hpp -- Key Builder                                              */
/*============================================================================*/
#pragma once

namespace Indexing
{
  class KeyBuilder
  {
  public:
    static void vAddElement(eKE eKeyKind, EntWrk* pEntWrk, Column* pCol,
                            Stz* azSrc, Format* pFmt);
    static void vAddElement_gen(EntWrk* pEntWrk, Column* pCol,
                                Stz* azSrc, Format* pFmt);
    static void vAddElement_int(EntWrk* pEntWrk, Column* pCol,
                                Stz* azSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
