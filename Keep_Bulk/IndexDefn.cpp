/*============================================================================*/
/* IndexDefn.cpp - index definition                                           */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexDefn* DBobjects::_pIndexDefn(void* pAddr)
{
  IndexDefn* pIndexDefn = reinterpret_cast<IndexDefn*>(pAddr);
  
  RASSERT( !pIndexDefn || CN_INDEXDEFN == pIndexDefn->eClass() );
  
  return pIndexDefn;
}
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
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexDefn::operator new(size_t iSize, IndexDefn* pIndexDefn)
{
  RASSERT(iSize);
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
IndexDefn::IndexDefn(void)
: Object(CN_INDEXDEFN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEFN,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  m_iIndexNo   = 0;
  m_sIndexName = pWord->sName();
  m_eIndexType = IT_SKIP;
  
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
