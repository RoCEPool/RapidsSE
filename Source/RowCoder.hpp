/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowCoder
  {
  public:
    static i8   nDataBytes(eMC eMetaCode);
    static void vDteFromEdt(Word* aWord, i8 iEra);
    static void vDteIntoEdt(Dtm wDte, Dtm& wEdt_, i8& iEra_);

    static TailLine* aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel);

    static bool bIsFit(eMC eMetaCode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
