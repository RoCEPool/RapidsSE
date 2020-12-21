/*============================================================================*/
/* InfoSet.cpp -- Information Set                                             */
/*============================================================================*/

#include "DataBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* DataBuffers::_pInfoSet(void* aAddr)
{
  InfoSet* pInfoSet = reinterpret_cast<InfoSet*>(aAddr);
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* InfoSet::operator new(size_t)
{
  InfoSet* pInfoSet =
    static_cast<InfoSet*>(HeapMem::pAllocate(CN_INFOSET));
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void InfoSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(void)
: InfoSetBase(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = OI_NONE;
  _checkInitialised_(CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(StreamDef* pStreamDef)
: InfoSetBase(CN_INFOSET, pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = OI_NONE;
  
  _checkInitialised_(CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSet::~InfoSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* set operator infoset                                                       */
/*----------------------------------------------------------------------------*/
void InfoSet::vOpInfoSet(eOI eOpInfoSet) {
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = eOpInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get operator infoset                                                       */
/*----------------------------------------------------------------------------*/
eOI InfoSet::eOpInfoSet(void) {
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_eOpInfoSet;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
