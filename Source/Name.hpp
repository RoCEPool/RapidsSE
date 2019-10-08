/*============================================================================*/
/* Name.hpp -- Name (of object instance)                                      */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Name : public Object,
               public ListItem_t<Name>,
               public ListNL_t__<Name>
  {
  private:
    Lock    m_okLock;
  
    s32     m_sName;     // name of the object instance
    eNK     m_eKind;     // kind of name
    Object* m_pObject;   // pointer to the named object
  
    Name*   m_pCategory; // category of this name
    Name*   m_pParent;   // parent of this name
    Name*   m_pChild;    // child of this name
  
    Atomic   m_ixRefCount; // reference count
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Name(Stz* szName);
   ~Name(void) noexcept(false);
  
    Name* pCategory(void);
    void  vCategory(Name* pCategory);
  
    Name* pChild(void);
    void  vChild(Name* pChild);
  
    void  vDropRead(void);
    void  vDropWrite(void);
  
    void  vGrabRead(void);
    void  vGrabWrite(void);

    eNK   eKind(void);
    void  vKind(eNK eKind);
  
    Stz*  szName(void);
    void  vName(Stz* szName);

    Object* pObject(void);
    void    vObject(Object* pObject);

    Name* pParent(void);
    void vParent(Name* pParent);
  
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
