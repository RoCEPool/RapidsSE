/*============================================================================*/
/* Index.cpp - Table                                                          */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t iSize)
{
  RASSERT(iSize);
  Index* pIndex = nullptr;
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Index::operator delete(void* pIndexAddress)
{
  RASSERT(pIndexAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(const char* pIndexName)
: Object(CN_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_sIndexName = pIndexName;
  
  _class_(CN_INDEX);
  
  _checkInitialised_(CN_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Index::~Index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

i8    m_iIndexNo;
s32   m_sIndexName;
eIT   m_eIndexType;

/*----------------------------------------------------------------------------*/
/* dump index                                                                 */
/*----------------------------------------------------------------------------*/
void Index::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_INDEX);
  ASSERT(pObject->eClass() == CN_INDEX);
  
  Index* pIndex = reinterpret_cast<Index*>(pObject);
  
  vTrace("%sIndex %p:\n", pTab, pIndex);
  vTrace("%s  number %lld\n", pTab, pIndex->m_iIndexNo);
  vTrace("%s  name %s\n", pTab, pIndex->m_sIndexName.a());
  vTrace("%s  type %p\n\n", pTab, pIT[pIndex->m_eIndexType]);
}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
char* Index::pIndexName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_sIndexName.a();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Index::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/