/*============================================================================*/
/* MetaClass.hpp - MetaClass                                                  */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaClass : public Object,
                    public ListItem_t<MetaClass>,
                    public List_t<MetaBlock>
  {
  public:
    MetaGraph* m_p_MetaGraph;
  
    s32 m_sClassNm;  // name of class
    i8  m_nSize;     // size of this class
    eCC m_eClass;    // kind of class

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaClass(MetaGraph* p_MetaGraph);
   ~MetaClass(void) noexcept(false);
  
    MetaBlock* pAddBlock(Stz* szBlockNm, MetaBlock* p_ParentBlock);

    Stz* szClassNm(void);
    void vClassNm(Stz* szClassNm);
  
    eCC  eMetaClass(void);
    void vMetaClass(eCC eMetaClass);

    MetaGraph* pMetaGraph(void);
  
    void vPrint(i8 i_Depth);
    void vRecPrint(i8 i_Depth);
      
    i8   nSize(void);

    Footer* aFooter(void);
  };

  MetaClass* _pMetaClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
