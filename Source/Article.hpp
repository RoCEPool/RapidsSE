/*============================================================================*/
/* Article.hpp -- Article                                                     */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Article : public Object,
                   public ListItem_t<Article>,
                   public ListNL_t__<Article>
  {
  private:
    Lock    m_okLock;
  
    s32      m_sName;      // name of the Article
    Object*  m_pObject;    // pointer to the named object
    eNK      m_eKind;      // kind of article

    Article* m_pCategory;  // category of this Article
    Article* m_pParent;    // parent of this Article
    Article* m_pChild;     // child of this Article
  
    Atomic   m_ixRefCount; // reference count
  
    pARTFUN* m_fpArtFun;   // article sp function

  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Article(Stz* szName, Object* pObject, pARTFUN* fpArtFun);
   ~Article(void) noexcept(false);
  
    pARTFUN* fpArtFun(void);

    Article* pCategory(void);
    void  vCategory(Article* pCategory);
  
    Article* pChild(void);
    void  vChild(Article* pChild);
  
    void  vDropRead(void);
    void  vDropWrite(void);
  
    void  vDumpRecursively(i8 iDepth);
  
    Article* pFind(Stz* szName);
  
    void  vGrabRead(void);
    void  vGrabWrite(void);

    eNK   eKind(void);
    void  vKind(eNK eKind);
  
    Stz*  szName(void);
    void  vName(Stz* szName);

    Object* pObject(void);
    void    vObject(Object* pObject);

    Article* pParent(void);
    void vParent(Article* pParent);
  
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
