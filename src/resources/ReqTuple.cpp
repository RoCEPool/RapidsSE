/*============================================================================*/
/* ReqTuple.cpp -- Request Tuple                                              */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ReqTuple::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: ReqTuple class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ReqTuple::ReqTuple(void)
{
  m_i_Origin   = AM::bAttached() && AM::pGrid() ? AM::iThisNodeNo() : C_UNKNOWN;
  m_i_Session  = C_UNKNOWN;
  m_iReqNo     = C_UNKNOWN;
  m_iForId     = C_UNKNOWN;

  m_iDetail    = 0;
  
  m_sPurpose   = "?"_Stz;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ReqTuple::ReqTuple(ReqTuple* aReqTuple)
{
  vSet(aReqTuple);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ReqTuple::ReqTuple(Stz* szService, bool bFlag)
{
  vSet(nullptr);
  
  m_sPurpose = szService;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ReqTuple::ReqTuple(Stz* szService, i8 iSessionId, bool bFlag)
{
  vSet(nullptr);
  
  m_i_Session = iSessionId;
  m_sPurpose  = szService;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ReqTuple::~ReqTuple(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void ReqTuple::vContentDump(void)
{
  vTrace("\n"_Stz);
  vTrace("ReqTuple\n"_Stz);
  vTrace("--------\n"_Stz);
  
  vTrace("%p: m_sPurpose=%s\n"
         "m_i_Origin=%lld m_i_Session=%lld m_iReqNo=%lld m_iForId=%lld\n"_Stz,

         this,
         m_sPurpose.azChar(),
         
         m_i_Origin,
         m_i_Session,
         m_iReqNo,
         m_iForId,
         
         C_VARGS_END
         );

}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void ReqTuple::vSet(ReqTuple* aReqTuple)
{
  if (aReqTuple) {
    m_i_Origin   = aReqTuple->m_i_Origin;
    m_i_Session  = aReqTuple->m_i_Session;
    m_iReqNo     = aReqTuple->m_iReqNo;
    m_iForId     = aReqTuple->m_iForId;
    m_i_Session  = aReqTuple->m_i_Session;

    m_iDetail    = aReqTuple->m_iDetail;
    m_sPurpose   = aReqTuple->m_sPurpose;
    
    return;
  }
  
  m_i_Origin   = AM::bAttached() && AM::pGrid() ? AM::iThisNodeNo() : C_UNKNOWN;
  m_i_Session  = C_UNKNOWN;
  m_iReqNo     = C_UNKNOWN;
  m_iForId     = C_UNKNOWN;

  m_iDetail    = 0;
  
  //m_sPurpose   = "?"_Stz;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
