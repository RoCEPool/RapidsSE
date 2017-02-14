/*============================================================================*/
/* IndexLevels.cpp - set of index buffers by level for an index partition     */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexLevels::operator new(size_t iSize)
{
  RASSERT(iSize);
  IndexLevels* pIndexLevels = nullptr;
  
  return pIndexLevels;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexLevels::operator delete(void* pIndexLevelsAddress)
{
  RASSERT(pIndexLevelsAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexLevels::IndexLevels(void)
: Object(CN_INDEXLEVELS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  _checkInitialised_(CN_INDEXLEVELS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexLevels::~IndexLevels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexLevels::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_INDEXLEVELS);
  ASSERT(pObject->eClass() == CN_INDEXLEVELS);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexLevels::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
