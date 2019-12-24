/*============================================================================*/
/* StoppedMarker.cpp -- Stop Marker operand                                      */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StoppedMarker* ControlData::_pStoppedMarker(void* aAddr)
{
  StoppedMarker* pStoppedMarker = reinterpret_cast<StoppedMarker*>(aAddr);
  
  return pStoppedMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* StoppedMarker::operator new(size_t)
{
  StoppedMarker* pStoppedMarker =
    static_cast<StoppedMarker*>(HeapMem::pAllocate(CN_STOPPEDMARKER));
  
  return pStoppedMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void StoppedMarker::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StoppedMarker::StoppedMarker(void)
: Operand(CN_STOPPEDMARKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STOPPEDMARKER,_FOOTER_);

  _checkInitialised_(CN_STOPPEDMARKER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STOPPEDMARKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StoppedMarker::~StoppedMarker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STOPPEDMARKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(StoppedMarker);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
