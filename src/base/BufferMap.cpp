/*============================================================================*/
/* BufferMap.cpp -- Buffer (to address) Map                                   */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,BufferMap,CN_BUFFERMAP)
__aNewFunction(BufferMap,CN_BUFFERMAP)
__vDeleteFunction(BufferMap,CN_BUFFERMAP)
__aFooterFunction(BufferMap)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferMap::BufferMap(void)
: Object(CN_BUFFERMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERMAP,_FOOTER_);
    
  _checkInitialised_(CN_BUFFERMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUFFERMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferMap::~BufferMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERMAP,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
