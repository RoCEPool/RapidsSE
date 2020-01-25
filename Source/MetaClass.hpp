/*============================================================================*/
/* MetaClass.hpp - MetaClass                                                  */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaClass : public Object,
                    public ListItem_t<MetaClass>,
                    public List_t<MemberSet>
  {
  public:
    MetaGraph* m_p_MetaGraph;
  
    s32 m_sClassNm;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaClass(MetaGraph* p_MetaGraph);
   ~MetaClass(void) noexcept(false);
  
    MemberSet* pAddMemberSet(Stz* szSetNm);

    Stz* szClassNm(void);
    void vClassNm(Stz* szClassNm);
  
    MetaGraph* pMetaGraph(void);
  
    void vPrint(i8 i_Depth);
    void vRecPrint(i8 i_Depth);
      
    Footer* aFooter(void);
  };

  MetaClass* _pMetaClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
