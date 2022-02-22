/*============================================================================*/
/* ExprEval2.hpp -- ExprEval columns                                          */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "IntStack.hpp"
#include "eEO.h"
#include "eAG.h"
#include "ePJ.h"
#include "eEK.h"

namespace STORE
{
  class StreamDef;

  class ExprEval2
  {
  public:

    static eEC  eGenerateSelectColumns(StreamDefBase* pStreamDefIn, Stz* sExpr, u8* puSelectColumns);
    static eEC  eGenerateStreamDef    (StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, IndexDef* pIndexDef,
                                       IntStack* pAggTypeStack, eEK& eExprKind, Stz* sExpr, u8* puSelectColumns);
    static eEC  eJoinStreamDef        (StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, StreamDef* pStreamDefOut);

  private:
    static void vAppendColumnToIndexDef(IndexDef* pIndexDef, Column* pColumn);
    static void vAppendColumnToStreamDef(StreamDefBase* pStreamDefOut, Column* pColumn);
    static void vGenerateColPrefix(Stz* szColNm, Stz* szColPrefix);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
