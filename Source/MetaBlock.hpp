/*============================================================================*/
/* MetaBlock.hpp - Meta Block                                                 */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaBlock : public Object,
                    public ListItem_t<MetaBlock>,
                    public List_t<MemberVar>
  {
  public:
 
    MetaClass* m_p_MetaClass;    // the _p_ sequence means not owned by
    MetaBlock* m_p_ParentMbrSet; // parent member set

    s32 m_sSetNm; // name of this set
    i8  m_nSize;  // rolled up size of this set
    eMS m_eSet;   // kind of set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaBlock(MetaClass* p_MetaClass, MetaBlock* p_ParentMbrSet);
   ~MetaBlock(void) noexcept(false);

    MemberVar* pAddMemberVar(Stz* szVarNm);
  
    eMS  eMemberSet(void);
    MetaBlock* pParentMbrSet(void);
    void vMemberSet(eMS eMemberSet);

    void vPrint(i8 i_Tab);
    void vRecPrint(i8 i_Depth);
  
    Stz* szSetNm(void);
    void vSetNm(Stz* szSetNm);

    i8   nSize(void);
  
    Footer* aFooter(void);
  };

  MetaBlock* _pMetaBlock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
