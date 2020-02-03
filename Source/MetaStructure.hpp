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

    s32            m_sStructureNm;  // name of this structure
    eMS            m_eStructure;    // kind of structure
    eCP            m_ePart;         // c++ part
  
    EnameSet       m_oTplArgs;      // set of template arguments
  
    MetaInherits*  m_pMetaInherits;

    i8  m_nSize;         // rolled up size of this structure

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaStructure(Stz* szStructureNm, eMS eStructure, eCP ePart,
                  MetaGraph* p_MetaGraph, MetaStructure* p_ParentStructure);
   ~MetaStructure(void) noexcept(false);

    MetaMember* pAddMember(Stz* szMemberNm);

    MetaInherits* pInherits(void);

    eMS  eStructure(void);
    void vStructure(eMS eStructure);

    MetaStructure* p_ParentStructure(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    Stz* szStructureNm(void);
    void vStructureNm(Stz* szStructureNm);

    EnameSet* pTplArgs(void);
    void vTplArgs(EnameSet* pTplArgs);
  
    Footer* aFooter(void);
  };

  MetaStructure* _pMetaStructure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
