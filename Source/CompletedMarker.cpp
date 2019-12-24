/*============================================================================*/
/* CompletedMarker.cpp -- Stop Marker operand                                      */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CompletedMarker* ControlData::_pCompletedMarker(void* aAddr)
{
  CompletedMarker* pCompletedMarker = reinterpret_cast<CompletedMarker*>(aAddr);
  
  return pCompletedMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* CompletedMarker::operator new(size_t)
{
  CompletedMarker* pCompletedMarker =
    static_cast<CompletedMarker*>(HeapMem::pAllocate(CN_COMPLETEDMARKER));
  
  return pCompletedMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void CompletedMarker::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CompletedMarker::CompletedMarker(void)
: Operand(CN_COMPLETEDMARKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETEDMARKER,_FOOTER_);

  _checkInitialised_(CN_COMPLETEDMARKER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPLETEDMARKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CompletedMarker::~CompletedMarker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETEDMARKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(CompletedMarker);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
