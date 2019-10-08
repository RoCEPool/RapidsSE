/*============================================================================*/
/* Article.cpp -- Article                                                   */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Article::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ARTICLE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Article::operator delete(void* pAddress)
{
  Object* pObject = reinterpret_cast<Article*>(pAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Article::Article(Stz* szName)
: Object(CN_ARTICLE)
, m_okLock(LK_NAME,this)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_sName   = szName;
  m_eKind   = NK_NONE;
  m_pObject = nullptr;
  
  m_pCategory = nullptr;
  m_pParent   = nullptr;
  m_pChild    = nullptr;
  
  // m_ixRefCount

  _checkInitialised_(CN_ARTICLE);
  
  _postcondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Article::~Article(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get category                                                               */
/*----------------------------------------------------------------------------*/
Article* Article::pCategory(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_pCategory;
}

/*----------------------------------------------------------------------------*/
/* set category                                                               */
/*----------------------------------------------------------------------------*/
void Article::vCategory(Article* pCategory)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_pCategory = pCategory;
}

/*----------------------------------------------------------------------------*/
/* get child of article                                                       */
/*----------------------------------------------------------------------------*/
Article* Article::pChild(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/* set child article                                                          */
/*----------------------------------------------------------------------------*/
void Article::vChild(Article* pChild)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/* drop lock for read                                                         */
/*----------------------------------------------------------------------------*/
void Article::vDropRead(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Article::vDropWrite(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for read                                                         */
/*----------------------------------------------------------------------------*/
void Article::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Article::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);

  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get kind of article                                                        */
/*----------------------------------------------------------------------------*/
eNK Article::eKind(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* set kind of article                                                        */
/*----------------------------------------------------------------------------*/
void Article::vKind(eNK eKind)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_eKind = eKind;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Article::szName(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void Article::vName(Stz* szName)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_sName = szName;
}

/*----------------------------------------------------------------------------*/
/* get object                                                                 */
/*----------------------------------------------------------------------------*/
Object* Article::pObject(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_pObject;
}

/*----------------------------------------------------------------------------*/
/* set object                                                                 */
/*----------------------------------------------------------------------------*/
void Article::vObject(Object* pObject)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_pObject = pObject;
}

/*----------------------------------------------------------------------------*/
/* get parent article                                                         */
/*----------------------------------------------------------------------------*/
Article* Article::pParent(void)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/* set parent article                                                         */
/*----------------------------------------------------------------------------*/
void Article::vParent(Article* pParent)
{
  _precondition_(_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  m_pParent = pParent;
}

/*----------------------------------------------------------------------------*/
/* decrement references to this article                                       */
/*----------------------------------------------------------------------------*/
void Article::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  u8 iRef = --m_ixRefCount;
  
  //vTrace("ARTICLE: %s --ref : %lld\n"_Stz,m_sArticle.s(),iRef);
}

/*----------------------------------------------------------------------------*/
/* increment references to this article                                       */
/*----------------------------------------------------------------------------*/
void Article::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  u8 iRef = ++m_ixRefCount;
  
  //vTrace("ARTICLE: %s ++ref : %lld\n"_Stz,m_sArticle.s(),iRef);
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this article                            */
/*----------------------------------------------------------------------------*/
bool Article::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARTICLE,_FOOTER_);
  
  u8 iRef = *m_ixRefCount;
  
  return iRef ? true : false;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Article);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
