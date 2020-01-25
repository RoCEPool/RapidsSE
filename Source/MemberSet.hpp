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
 
    s32 m_sSetNm;
  
    eMS m_eMemberSet;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberSet(void);
   ~MemberSet(void) noexcept(false);

    MemberVar* pAddMemberVar(Stz* szVarNm);
  
    eMS  eMemberSet(void);
    void vMemberSet(eMS eMemberSet);

    Stz* szSetNm(void);
    void vSetNm(Stz* szSetNm);

    Footer* aFooter(void);
  };

  MemberSet* _pMemberSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
