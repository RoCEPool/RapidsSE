/*============================================================================*/
/* MemberSet.hpp - Member Set                                                 */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberSet : public Object,
                    public ListItem_t<MemberSet>,
                    public List_t<MemberVar>
  {
  public:
 
    MetaClass* m_p_MetaClass;    // the _p_ sequence means not owned by
    MemberSet* m_p_ParentMbrSet; // parent member set

    s32 m_sSetNm; // name of this set
    i8  m_nSize;  // rolled up size of this set
    eMS m_eSet;   // kind of set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberSet(MetaClass* p_MetaClass, MemberSet* p_ParentMbrSet);
   ~MemberSet(void) noexcept(false);

    MemberVar* pAddMemberVar(Stz* szVarNm);
  
    eMS  eMemberSet(void);
    MemberSet* pParentMbrSet(void);
    void vMemberSet(eMS eMemberSet);

    void vPrint(i8 i_Tab);
    void vRecPrint(i8 i_Depth);
  
    Stz* szSetNm(void);
    void vSetNm(Stz* szSetNm);

    i8   nSize(void);
  
    Footer* aFooter(void);
  };

  MemberSet* _pMemberSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
