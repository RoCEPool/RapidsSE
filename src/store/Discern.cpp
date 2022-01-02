/*============================================================================*/
/* Discern.cpp --                                                               */
/*============================================================================*/

#include "Discern.hpp"

#include "Pipeline.hpp"
#include "Stage.hpp"
#include "Stream.hpp"
#include "Op.hpp"

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Discern::nRdCsvChunk(Pipeline* pPipeline)
{
  _static_
  
  i8 nSumQueued = 0;
  
  i8 nStages = pPipeline->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pPipeline->pGet(i_Stage);
    
    i8 nStreams = pStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);
      
      i8 nOps = pStream->nItems();
      for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
        Op* pOp = pStream->pGet(i_Op);

        Stz* szOpNm = pOp->szOpNm();
        
        if (   szOpNm->bSame("rdCsvChunk"_Stz)
            || szOpNm->bSame("insert"_Stz)
            )
          continue;
        
        i8 nQueued = pOp->nQueuedOperands();
        nSumQueued += nQueued;
//        vTrace("%s %lld %lld\n"_Stz,szOpNm,nQueued,nSumQueued);
      }
    }
  }

  return nSumQueued;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
