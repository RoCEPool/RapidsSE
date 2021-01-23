/*============================================================================*/
/* Result.cpp -- Result                                                       */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "StreamSet.hpp"
#include "Session.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

#include "Grid.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Output.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Result,CN_RESULT)
__aNewFunction(Result,CN_RESULT)
__vDeleteFunction(Result,CN_RESULT)
__aFooterFunction(Result)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Result::Result(void)
: Object(CN_RESULT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  //m_xJobResponse
  m_bJobHasError       = false;
  
  m_pOriginResponseVec = nullptr;
  m_bPartHasError      = false;
    
  _checkInitialised_(CN_RESULT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Result::~Result(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  if ( m_pOriginResponseVec)
    delete m_pOriginResponseVec;
}

/*----------------------------------------------------------------------------*/
/* -- Static ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode operand into class and marker                                       */
/*----------------------------------------------------------------------------*/
void Result::vDecode(Operand* pOperand, eCN& eClassNo, eOM& eOpMarker)
{
  _static_
  
  eClassNo = pOperand->eGetClassNo();
  
  if (CN_MARKER == eClassNo) {
    Marker* pMarker = _pMarker(pOperand);
    
    eOpMarker = pMarker->eOpMarker();
  }
  else {
    eOpMarker = OM_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
void Result::vPrint(Session* pSession, eRS eResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  // first check for job errors
  if (m_bJobHasError != EC_NONE) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_xJobResponse.m_eErrorCode]);
    return;
  }
  
  switch (eResponse) {
    
    case RS_NONE             : /* do nothing */                  break;
    case RS_LOCAL_RESPONSE   : vPrintLocalResponse(pSession);    break;
    case RS_DIS2PC_RESPONSE  : vPrintDist2pcResponse(pSession);  break;
    
    default:
      vTrace("Invalid response : %s\n"_Stz,aRS[eResponse]);
  }
}

/*----------------------------------------------------------------------------*/
/* print dist2pc response                                                     */
/*----------------------------------------------------------------------------*/
void Result::vPrintDist2pcResponse(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  i8 iResponses = m_pOriginResponseVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Errors
  if (m_bPartHasError) {
    vTrace("error: "_Stz);
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("node%lld: %s; "_Stz,
             m_pOriginResponseVec->pGet(i_Node)->m_i_Node,
             aEC[m_pOriginResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    vTrace("\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Responses
  
  if (*pSession->m_pOutput->m_ixRowsProcessed ||
      *pSession->m_pOutput->m_ixRowsInvalid   ||
      *pSession->m_pOutput->m_ixRowsSkipped ) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // success / error
    if (*pSession->m_pOutput->m_ixRowsInvalid   ||
        *pSession->m_pOutput->m_ixRowsSkipped )
      vTrace("error: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             *pSession->m_pOutput->m_ixRowsProcessed -
               *pSession->m_pOutput->m_ixRowsSkipped,
             *pSession->m_pOutput->m_ixRowsInvalid,
             *pSession->m_pOutput->m_ixRowsSkipped);
    else
      vTrace("\nsuccess: %lld rows loaded (%lld invalid rows %lld skipped rows)\n"_Stz,
             *pSession->m_pOutput->m_ixRowsProcessed -
               *pSession->m_pOutput->m_ixRowsSkipped,
             *pSession->m_pOutput->m_ixRowsInvalid,
             *pSession->m_pOutput->m_ixRowsSkipped);
  }
  else {
    vTrace("\nsuccess: %lld rows fetched\n"_Stz,
           *pSession->m_pOutput->m_ixRowsProcessed);
  }

  pSession->m_pOutput->m_ixRowsProcessed = 0;
  pSession->m_pOutput->m_ixRowsInvalid   = 0;
  pSession->m_pOutput->m_ixRowsSkipped   = 0;
}

/*----------------------------------------------------------------------------*/
/* print job response                                                         */
/*----------------------------------------------------------------------------*/
void Result::vPrintLocalResponse(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Errors should already be handled
  ASSERT( ! m_bJobHasError );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // success
  vTrace("\nsuccess: %lld rows affected\n"_Stz,
         *pSession->m_pOutput->m_ixRowsProcessed);
  
  pSession->m_pOutput->m_ixRowsProcessed = 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
