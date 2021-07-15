/*============================================================================*/
/* SP_Op.cpp - Op sprint helper                                               */
/*============================================================================*/

#include "BASE.h"

#include "StageSpec.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"

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
/* sprint leading portion                                                     */
/*----------------------------------------------------------------------------*/
void SP_Op::vLeading(Op* pOp)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vInit();
  
  OpSpec* pOpSpec = pOp->pOpSpec();
  StageSpec* pStageSpec = pOpSpec->pStageSpec();
  Stream* pStream = pOp->pStream();
  Pipeline* pPipeline = pOp->pPipeline();

  Word* pEventHandler = pOp->pEventHandler();
  
  vCatf("<? %-12s "
        "[%lld] %-12s "
        "[%02lld.%03lld] %-12s %-32s?> ## "_Stz
        
        ,pPipeline  ? pPipeline->szPipelineNm() : C_NO_NAME
        
        ,pStageSpec ? pStageSpec->i_StageSpec() : -1
        ,pStageSpec ? pStageSpec->szStageNm()   : C_NO_NAME
        
        ,pStream    ? pStream->i_Stream()       : -1
        ,pOpSpec    ? pOpSpec->iOpSpec()        : -1
        ,pOp->szOpNm()
        ,pEventHandler ? pEventHandler->szWordNm() : "(no handler)"_Stz
        );
  
//  vCatf("%12s (%10s) (N%lld) (P%02lld) (job:%p)"_Stz
//
//        ,pOp->szOpNm()
//        ,pOp->pStage()->szStageNm()
//        ,pOp->iNode()
//        ,pOp->tPtnNo()
//        ,pOp->pPipelineJob()
//
//        );

}

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
  ePX eCrossTopo = pOp->eCrossTopo();

  if (bDescend)
    vCatf("\n      "_Stz);

  vCatf("[%03lld] Op %-16s "
        "|%s [%02lld.%03lld] Op %-16s "
        "x %s"_Stz
        
         ,pOp->iOp()
         ,pOp->szOpNm()
        
         ,pOp->bAppendRight() ? "R"_Stz : "L"_Stz
         ,iNextStage
         ,iNextOp
           ,pNextOp->szOpNm()
           ,aPX[eCrossTopo]
        );

  if (XK_NONE != pOp->eSendKind())
      vCatf(" %s"_Stz,aXK[pOp->eSendKind()]);  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Op::sz(Op* pOp)
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

  vLeading(pOp);
  
  return (Stz*) &m_caResult[0];
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
