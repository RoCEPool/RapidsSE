/*============================================================================*/
/* MemberVar.hpp - Member Variable                                            */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberVar : public Object,
                    public ListItem_t<MemberVar>
  {
  public:
 
    s32 m_sVarNm;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberVar(void);
   ~MemberVar(void) noexcept(false);
  
    static bool bIsMemberVar(Stz* szClassNm, EnameSet* pEnameSet);
  
    Stz* szVarNm(void);
    void vVarNm(Stz* szVarNm);

      
    Footer* aFooter(void);
  };

  MemberVar* _pMemberVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
