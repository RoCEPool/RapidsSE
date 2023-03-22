/*============================================================================*/
/* ExprEval_v2.hpp -- ExprEval Version 2                                      */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "IntStack.hpp"
#include "eAG.h"
#include "ePE.h"
#include "eEK.h"

namespace STORE
{
  class StreamDef;

  class ExprEval_v2
  {
  public:

    static eEC  eGenerateSelectColumns(StreamDefBase* pStreamDefIn, Stz* sExpr, u8* puSelectColumns);
    static eEC  eGenerateStreamDef    (StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, IndexDef* pIndexDef,
                                       IntStack* pPreExpStack, Stz* sExpr, u8* puSelectColumns);
    static eEC  eJoinStreamDef        (StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, StreamDef* pStreamDefOut);

  private:
    static void vAppendColumnToIndexDef(IndexDef* pIndexDef, Column* pColumn);
    static void vAppendColumnToStreamDef(StreamDefBase* pStreamDefOut, Column* pColumn);
    static void vGenerateColPrefix(StreamDefBase* pStreamDef, Stz* szColPrefix);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
