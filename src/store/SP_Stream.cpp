/*============================================================================*/
/* SP_Stream.cpp - Stream sprint helper                                       */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StageSpec.hpp"

#include "SP_Stream.hpp"
#include "SP_Op.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Stream::SP_Stream(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Stream::SP_Stream(Stream* pStream, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
    
  vStream(pStream,bDescend);

  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Stream::~SP_Stream(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_Stream::vStream(Stream* pStream, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StageSpec* pStageSpec = pStream->pStageSpec();
  
  if (bDescend)
    vCatf("\n    "_Stz);

  if (pStageSpec->bNodeTopo()) {
    vCatf("[%02lld] Node-Stream %s (N%lld) "_Stz
          ,pStream->i_Stream()
          ,pStream->szStreamNm()
          ,pStream->iNodeId()
          );
  }
  else {
    vCatf("[%02lld] Partition-Stream %s (N%lld,S%02lld) "_Stz
          ,pStream->i_Stream()
          ,pStream->szStreamNm()
          ,pStream->iNodeId()
          ,pStream->i_Stream()
          );
  }
  
  if (bDescend) {
    for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++ ) {
      
      Op* pOp = pStream->pGet(i_Op);
      SP_Op spOp;
      vCatf("%s"_Stz,spOp.szStream(pOp,bDescend));
    }
  }

}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Stream::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Stream::sz(Stream* pStream)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vStream(pStream);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Stream::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
