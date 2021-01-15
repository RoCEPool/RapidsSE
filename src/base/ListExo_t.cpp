/*============================================================================*/
/* ListExo_t.cpp -- List of Items (with exo-nodes)                            */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ListExo_ABSTRACT,CN_CLASSPLACEHOLDER)
__aNewFunction(ListExo_ABSTRACT,CN_CLASSPLACEHOLDER)
__vDeleteFunction(ListExo_ABSTRACT,CN_CLASSPLACEHOLDER)
__aFooterFunction(ListExo_ABSTRACT)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ListExo_ABSTRACT::ListExo_ABSTRACT(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
   m_pHeadNode_abstract = nullptr;
  m_pTailNode_abstract = nullptr;
  
  // _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ListExo_ABSTRACT::~ListExo_ABSTRACT(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- public: -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get head node                                                              */
/*----------------------------------------------------------------------------*/
Object* ListExo_ABSTRACT::pHeadNode_abstract(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pHeadNode_abstract;
}

/*----------------------------------------------------------------------------*/
/* get tail node                                                              */
/*----------------------------------------------------------------------------*/
Object* ListExo_ABSTRACT::pTailNode_abstract(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pTailNode_abstract;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
