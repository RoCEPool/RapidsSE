/*============================================================================*/
/* SubLoader.cpp -- Subordinate Parallel Table LoadTable                      */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SubLoader* Utilities::_pLoaderSub(void* aAddr)
{
  SubLoader* pSubLoader = reinterpret_cast<SubLoader*>(aAddr);
  
  RASSERT( !pSubLoader || CN_SUBLOADER == pSubLoader->eClass() );
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = HeapMem::pAllocate(CN_SUBLOADER);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize, SubLoader* pSubLoader)
{
  RASSERT(iSize);
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void SubLoader::operator delete(void* pLoaderAddress)
{
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pLoaderAddress);
  
  HeapMem::vFree(pLoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubLoader::SubLoader(Job_SubLoader* pJob_SubLoader)
: Object(CN_SUBLOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);

  m_pJob_SubLoader = pJob_SubLoader;

  _checkInitialised_(CN_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubLoader::~SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void SubLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SUBLOADER);
  RASSERT(pObject->eClass() == CN_SUBLOADER);
  
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get loader                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader* SubLoader::pJob_SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SubLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
