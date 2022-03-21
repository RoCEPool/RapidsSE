/*============================================================================*/
/* SP_Op.cpp - Op sprint helper                                               */
/*============================================================================*/

#include "BASE.h"

#include "MY.hpp"

#include "StageSpec.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipework.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"

#include "SP_Op.hpp"

#include "TagConstants.h"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Op::SP_Op(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Op::SP_Op(Op* pOp, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vOp(pOp,bDescend);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Op::~SP_Op(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint schedule version of op                                              */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::sz(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 nQueuedOperands = pOp->nQueuedOperands();
  
  vInit();
  
  vCatf("Op {"
        "%p "
        "operands %lld "
        "%s"
        "}"_Stz

        ,pOp
        ,nQueuedOperands
        ,pOp->szPath()
  );
        

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::szAck(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vOp(pOp);
  
  return (Stz*) &m_caResult[0];
}
/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::szExHandler(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vOp(pOp);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint leading portion of an op trace                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::szLeading(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vInit();
  
  OpSpec* pOpSpec = pOp->pOpSpec();
  StageSpec* pStageSpec = pOpSpec->pStageSpec();
  Stream* pStream = pOp->pStream();
  Pipeline* pPipeline = pOp->pPipeline();

  Word* pEventHandler = pOp->pEventHandler();
  
  vCatf("<? pipeline %-12s "
        "[%lld.%02lld.%03lld] "
        "%-12s/%-12s "_Stz
        
        ,pPipeline  ? pPipeline->szPipelineNm() : C_NO_NAME
        
        ,pStageSpec ? pStageSpec->i_StageSpec() : -1
        ,pStream    ? pStream->i_Stream()       : -1
        ,pOpSpec    ? pOpSpec->iOpSpec()        : -1

        ,pOp->szOpNm()
        ,pStageSpec ? pStageSpec->szStageNm()   : C_NO_NAME
        
        );
  
  if (pEventHandler) {
    vCatf("%-24s #%04lld ?> ## "_Stz
        
         ,pEventHandler->szWordNm()
         ,pOp->iHandlerCount(pOp->eCurHandler())
        );
  }
  else {
    vCatf(": %-28s ?> ## "_Stz
        
         ,pOp->pCurWord()->szWordNm()
        );
  }
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint path of an op                                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::szPath(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Stream*   pStream   = pOp->pStream();
  Stage*    pStage    = pStream->pStage();
  Pipeline* pPipeline = pStage->pPipework()->pPipeline();
  
  Job*      pJob      = pPipeline->pPipelineJob();

  vInit();
  
  vCatf("%s/%s/%s/%s/%lld/%s"_Stz
        ,MY::szNodeNm()
        ,pJob->szJobNm()
        ,pPipeline->szPipelineNm()
        ,pStage->szStageNm()
        ,pStream->i_Stream()
        ,pOp->szOpNm()
        );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint schedule version of op                                              */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::szStream(Op* pOp, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vInit();
    
  i8 iNextStage = pOp->iNextStage();
  i8 iNextOp = pOp->iNextOp();
  Op* pNextOp = pOp->pNextOp();
  eYX eCrossTopology = pOp->eCrossTopology();

  if (bDescend)
    vCatf("\n      "_Stz);

  vCatf("[%03lld] Op %-16s "
        "| [%02lld.%03lld] Op %-16s "
        "x %s"_Stz
        
         ,pOp->i_Op()
         ,pOp->szOpNm()
        
         ,iNextStage
         ,iNextOp
           ,pNextOp->szOpNm()
           ,aYX[eCrossTopology]
        );

  if (XK_NONE != pOp->eSendCast())
      vCatf(" %s"_Stz,aXK[pOp->eSendCast()]);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint op                                                                  */
/*----------------------------------------------------------------------------*/
void SP_Op::vOp(Op* pOp, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vInit();

  i8 iNextStage = pOp->iNextStage();
  i8 iNextOp = pOp->iNextOp();
  Op* pNextOp = pOp->pNextOp();
  eYX eCrossTopology = pOp->eCrossTopology();

  if (bDescend)
    vCatf("\n      "_Stz);

  vCatf("[%03lld] Op %-16s "
        "| [%02lld.%03lld] Op %-16s "
        "x %s"_Stz

         ,pOp->i_Op()
         ,pOp->szOpNm()

         ,iNextStage
         ,iNextOp
           ,pNextOp->szOpNm()
           ,aYX[eCrossTopology]
        );

  if (XK_NONE != pOp->eSendCast())
      vCatf(" %s"_Stz,aXK[pOp->eSendCast()]);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Op::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
