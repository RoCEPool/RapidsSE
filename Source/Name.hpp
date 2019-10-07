/*============================================================================*/
/* Name.hpp -- Name (of object instance)                                      */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Name : public Object,
               public ListItem_t<Name>
  {
  private:
    Lock    m_okLock;
  
    s32     m_sName;    // name of the object instance
    Object* m_pObject;  // pointer to the named object
  
    Name*   m_pParent;  // parent of this named object
    Name*   m_pChild;   // child of this named object
  
    // must be last
    Footer m_xFooter;
    
  public:
    Name(Stz* szName);
   ~Name(void) noexcept(false);
  
    Stz* szName(void);
    void vName(Stz* szName);

    Object* pObject(void);
    void vObject(Object* pObject);

    Name* pParent(void);
    void vParent(Name* pParent);
  
    Name* pChild(void);
    void vChild(Name* pChild);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
