/*============================================================================*/
/* SP_Op.cpp - Op sprint helper                                               */
/*============================================================================*/

#include "BASE.h"

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
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_Op::vOp(Op* pOp, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vInit();
  
  s256 sArgs;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  TagSet* pOpTopoTags = pOp->pTopologyTagSet();
  pOpTopoTags->vSprint(sArgs);
  
  // get the next stage
  i8 iNextStage = pOpTopoTags->iValue(C_TAG_NEXTSTAGE);
  Stage* pNextStage = pPipeline->pGet(iNextStage);
  
  // get the next op
  i8 iNextOp = pOpTopoTags->iValue(C_TAG_NEXTOP);
  Stream* pNextStream = pNextStage->pGet(0);
  Op* pNextOp = pNextStream->pGet(iNextOp);

  // get the cross topology
  ePX eCrossTopo = (ePX)(pOpTopoTags->iValue(C_TAG_CROSSTOPO));
  
  // get the left EOS markers for consume op
  //! ALKA: right EOS markers ???
  TagSet* pOpTags = pOp->pOpTagSet();
  i8 iSends =  pOpTags->iValue(C_TAG_SENDS);
  i8 iLeftMarkers = iSends ? iSends : 1;
  
  if (bDescend)
    vCatf("\n      "_Stz);

  vCatf("[%03lld] "
        "Op %12s%-20s "
        "(N%lld,P%02lld) "
        "Markers(L%lld R0) "
        "| [%02lld.%03lld] Op %-10s %s %s %s"_Stz,
        
         pOp->iOp(),
        
         pOp->szOpNm(),
         sArgs.szStz(),
        
         pOp->iNode(),
         pOp->tPtnNo(),

         iLeftMarkers,
        
         iNextStage,
         iNextOp,
         pNextOp->szOpNm(),
         pOp->bAppendRight() ? "appendRight"_Stz : "appendLeft "_Stz,
         aPX[eCrossTopo],
         aXK[pOp->eSendKind()]
        );

  
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
