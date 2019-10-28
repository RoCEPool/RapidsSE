/*============================================================================*/
/* ReqTuple.cpp -- Request Tuple                                              */
/*============================================================================*/

#include "Focals.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ReqTuple::operator new(size_t)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: ReqTuple class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
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
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ReqTuple::~ReqTuple(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void ReqTuple::vSet(ReqTuple* aReqTuple)
{
  if (aReqTuple) {
    m_i_Origin   = aReqTuple->m_i_Origin;
    m_i_Session  = aReqTuple->m_i_Session;
    m_iReqId     = aReqTuple->m_iReqId;
    m_iForId     = aReqTuple->m_iForId;
    m_i_Session  = aReqTuple->m_i_Session;

    m_iDetail    = aReqTuple->m_iDetail;
    m_sPurpose   = aReqTuple->m_sPurpose;
    
    return;
  }
  
  m_i_Origin   = AM::bAttached() && AM::pGrid() ? AM::iThisNodeNo() : C_UNKNOWN;
  m_i_Session  = C_UNKNOWN;
  m_iReqId     = C_UNKNOWN;
  m_iForId     = C_UNKNOWN;

  m_iDetail    = 0;
  
  //m_sPurpose   = "?"_Stz;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
