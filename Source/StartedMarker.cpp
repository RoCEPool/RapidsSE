/*============================================================================*/
/* StartedMarker.cpp -- Started Marker operand                                */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StartedMarker* ControlData::_pStartedMarker(void* aAddr)
{
  StartedMarker* pStartedMarker = reinterpret_cast<StartedMarker*>(aAddr);
  
  return pStartedMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* StartedMarker::operator new(size_t)
{
  StartedMarker* pStartedMarker =
    static_cast<StartedMarker*>(HeapMem::pAllocate(CN_STARTEDMARKER));
  
  return pStartedMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void StartedMarker::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StartedMarker::StartedMarker(void)
: Operand(CN_STARTEDMARKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STARTEDMARKER,_FOOTER_);

  _checkInitialised_(CN_STOPMARKER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STARTEDMARKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StartedMarker::~StartedMarker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STARTEDMARKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(StartedMarker);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
