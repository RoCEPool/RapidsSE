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
    
  Pipeline* pPipeline = pOp->pPipeline();
  
  TagSet* pOpTopoTags = pOp->pTopologyTagSet();
  
  // get the next stage
  i8 iNextStage = pOpTopoTags->iValue(C_TAG_NEXTSTAGE);
  Stage* pNextStage = pPipeline->pGet(iNextStage);
  
  // get the next op
  i8 iNextOp = pOpTopoTags->iValue(C_TAG_NEXTOP);
  Stream* pNextStream = pNextStage->pGet(0);
  Op* pNextOp = pNextStream->pGet(iNextOp);

  // get the cross topology
  ePX eCrossTopo = (ePX)(pOpTopoTags->iValue(C_TAG_CROSSTOPO));
  
  
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
