/*============================================================================*/
/* IndexDefn.cpp - index definition                                           */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexDefn::operator new(size_t iSize)
{
  RASSERT(iSize);
  IndexDefn* pIndexDefn = nullptr;
  
  return pIndexDefn;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexDefn::operator delete(void* pIndexDefnAddress)
{
  RASSERT(pIndexDefnAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexDefn::IndexDefn(const char* pIndexName)
: Object(CN_INDEXDEFN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN,_FOOTER_);
  
  m_sIndexName = pIndexName;
  
  _class_(CN_INDEXDEFN);
  
  _checkInitialised_(CN_INDEXDEFN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexDefn::~IndexDefn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN,_FOOTER_);
}

i8    m_iIndexDefnNo;
s32   m_sIndexDefnName;
eIT   m_eIndexDefnType;

/*----------------------------------------------------------------------------*/
/* dump index                                                                 */
/*----------------------------------------------------------------------------*/
void IndexDefn::vDump(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_INDEXDEFN);
  ASSERT(pObject->eClass() == CN_INDEXDEFN);
  
  IndexDefn* pIndexDefn = reinterpret_cast<IndexDefn*>(pObject);
  
  vTrace("no %lld name %s type %p\n",
         pIndexDefn->m_iIndexNo,
         pIndexDefn->m_sIndexName.aStr(),
         pIT[pIndexDefn->m_eIndexType]);
}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
char* IndexDefn::pIndexName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN,_FOOTER_);
  
  return m_sIndexDefnName.aStr();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexDefn::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
