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




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
