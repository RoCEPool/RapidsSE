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

    s32 m_sStructureNm;  // name of this structure
    eMB m_eStructure;    // kind of structure

    i8  m_nSize;         // rolled up size of this structure

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaStructure(Stz* szStructureNm, eMB eStructure,
                  MetaGraph* p_MetaGraph, MetaStructure* p_ParentStructure);
   ~MetaStructure(void) noexcept(false);

    MetaMember* pAddMember(Stz* szMemberNm);
  
    eMB  eStructure(void);
    void vStructure(eMB eStructure);

    MetaStructure* p_ParentStructure(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    Stz* szStructureNm(void);
    void vStructureNm(Stz* szStructureNm);

    i8   nSize(void);
  
    Footer* aFooter(void);
  };

  MetaStructure* _pMetaStructure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
