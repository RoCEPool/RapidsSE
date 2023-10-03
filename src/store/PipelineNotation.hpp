/*============================================================================*/
/* PipelineNotation.hpp -- Pipeline Notation Helper                           */
/*============================================================================*/
#pragma once
#include "BASE.h"
namespace STORE
{
  class PipelineNotation
  {
  public:
    static void vPipelineNotation(Stz* szPipelineStatement);
    static void vPipelineFromFile(Stz* szFilename, String* psDstStr);
    static void vPipelineToFile(Stz* pFilename, Stz* sDstStr);
    static void vPrintPipeline(TokenVec* pTknVec);
    static void vShorten(TokenVec* pTknVec);
    static void vStages(TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
