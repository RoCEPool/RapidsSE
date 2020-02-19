/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Procedure
  {
  public:
    static void  vAddProcedure(Stz* szProcNm, Stz* szProcText);
    static Word* pFindProcedure(Stz* szProcNm, DicStack* pDicStack);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
