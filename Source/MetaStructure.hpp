/*============================================================================*/
/* MetaStructure.hpp - Meta Structure (definition of class, struct, union)    */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaStructure : public Object,
                        public ListItem_t<MetaStructure>,
                        public List_t<MetaMember>
  {
  public:
 
    MetaGraph*     m_p_MetaGraph;        // the _p_ sequence means not owned by
    MetaStructure* m_p_ParentStructure;  // parent structure

    s32 m_sBlockNm;  // name of this meta block
    eMB m_eBlock;    // kind of meta block

    i8  m_nSize;     // rolled up size of this block

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaStructure(Stz* szBlockNm, eMB eBlock,
                  MetaGraph* p_MetaGraph, MetaStructure* p_ParentStructure);
   ~MetaStructure(void) noexcept(false);

    MetaMember* pAddMember(Stz* szMemberNm);
  
    eMB  eBlock(void);
    void vBlock(eMB eBlock);

    MetaStructure* p_ParentStructure(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    Stz* szBlockNm(void);
    void vBlockNm(Stz* szBlockNm);

    i8   nSize(void);
  
    Footer* aFooter(void);
  };

  MetaStructure* _pMetaStructure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
