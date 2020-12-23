/*============================================================================*/
/* Marker.cpp -- Marker                                                        */
/*============================================================================*/

#include "ControlData.h"

#include "eCF.h"

ClassFn __Marker_fn (CN_MARKER,CF_INSPECT,(pCLASSFN*)Marker::bInspect,-1);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Marker* ControlData::_pMarker(void* aAddr)
{
  Marker* pMarker = reinterpret_cast<Marker*>(aAddr);
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Marker::operator new(size_t)
{
  Marker* pMarker =
  static_cast<Marker*>(HeapMem::pAllocate(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Marker::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Marker::Marker(void)
: Operand(CN_MARKER)
{
  _precondition_(_MAGIC_,CN_MARKER);
  
  m_eOpMarker = OM_NONE;
  
  //vTrace("new Marker() %p\n"_Stz,this);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Marker::Marker(eOM eOpMarker)
: Operand(CN_MARKER)
{
  _precondition_(_MAGIC_,CN_MARKER);
  
  m_eOpMarker = eOpMarker;
//  vTrace("new Marker(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpMarker],this,m_xFooter.iWhenAllocated());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Marker::~Marker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
//  vTrace("delete Marker(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpMarker],this,m_xFooter.iWhenAllocated());
}

/*----------------------------------------------------------------------------*/
/* inspect the operator                                                       */
/*----------------------------------------------------------------------------*/
bool Marker::bInspect(Object* pObject)
{
  _static_
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* get the Op marker type                                                     */
/*----------------------------------------------------------------------------*/
eOM Marker::eOpMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eOpMarker;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
