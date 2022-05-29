/*============================================================================*/
/* Marker.cpp -- Marker                                                        */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"
#include "MarkerMgr.hpp"

#include "eCF.h"
#include "Job.hpp"
#include "AM.hpp"

ClassFn __Marker_fn (CN_MARKER,CF_INSPECT,(fpCLASSFN*)Marker::bInspect,-1);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Marker* STORE::_pMarker(void* aAddr)
{
  Marker* pMarker = reinterpret_cast<Marker*>(aAddr);
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Marker::operator new(size_t, eMG eMarker)
{
  RASSERT(MG_CHAIN == eMarker || MG_MANAGER == eMarker);
  
  Marker* pMarker =
    static_cast<Marker*>(Heaps::pAllocate(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from job marker chain                                             */
/*----------------------------------------------------------------------------*/
void* Marker::operator new(size_t, Job* pJob)
{
  RASSERT(pJob);
  
  Marker* pMarker = pJob->pNewMarker();
  
  if (! pMarker)
    pMarker =  static_cast<Marker*>(Heaps::pAllocate(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Marker::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Marker::Marker(eOM eOpMarker)
: Operand(CN_MARKER)
{
  _precondition_(_MAGIC_,CN_MARKER);
  
  m_eOpMarker = eOpMarker;
  m_eMarker   = MG_ORDINARY;
  m_pNextItem = nullptr;

  _checkInitialised_(CN_MARKER);

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
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Marker::vPostRecv(void* aMarker)
{
  _static_
  
  //Marker* pMarker = _pMarker(aMarker);
}


/*----------------------------------------------------------------------------*/
/* inspect the marker                                                         */
/*----------------------------------------------------------------------------*/
bool Marker::bInspect(Object* pObject)
{
  _static_
  
  Marker* pMarker = _pMarker(pObject);
  
  vTrace("Marker { %s } %p @%lld\n"_Stz,
         aOM[pMarker->m_eOpMarker],
         pMarker,
         pMarker->m_xFooter.iWhenAllocated());

  return true;
}

/*----------------------------------------------------------------------------*/
/* get marker category                                                        */
/*----------------------------------------------------------------------------*/
eMG Marker::eMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eMarker;
}

/*----------------------------------------------------------------------------*/
/* set marker category                                                        */
/*----------------------------------------------------------------------------*/
void Marker::vMarker(eMG eMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eMarker = eMarker;
}

/*----------------------------------------------------------------------------*/
/* get the next chain item                                                    */
/*----------------------------------------------------------------------------*/
Marker* Marker::pNextItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pNextItem;
}

/*----------------------------------------------------------------------------*/
/* set the next chain item                                                    */
/*----------------------------------------------------------------------------*/
void Marker::vNextItem(Marker* pNextItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pNextItem = pNextItem;
}

/*----------------------------------------------------------------------------*/
/* get the Op marker type                                                     */
/*----------------------------------------------------------------------------*/
eOM Marker::eOpMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eOpMarker;
}

/*----------------------------------------------------------------------------*/
/* set the Op marker type                                                     */
/*----------------------------------------------------------------------------*/
void Marker::vOpMarker(eOM eOpMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eOpMarker = eOpMarker;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
