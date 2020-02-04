/*============================================================================*/
/* MetaType.hpp - Meta Type (definition of class, struct, union)              */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaType : public Object,
                   public ListItem_t<MetaType>,
                   public List_t<MetaMember>
  {
  public:
 
    MetaType*     m_p_OuterType;   // outer/containing type

    s32           m_sTypeNm;       // name of this type
    eMS           m_eStructure;    // kind of structure
    eCP           m_eCppPart;      // c++ part
  
    EnameSet*     m_pTplParams;    // set of template parameters for this type
  
    MetaInherits* m_pMetaInherits;

    i8            m_nSize;         // rolled up size of this structure

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaType(Stz* szTypeNm, eMS eStructure, eCP eCppPart,
             MetaType* p_OuterType);
   ~MetaType(void) noexcept(false);

    MetaMember* pAddMember(Stz* szMemberNm);

    MetaInherit* pAddInherit(Stz* szBaseClassNm);

    MetaInherits* pInherits(void);

    eMS  eStructure(void);
    void vStructure(eMS eStructure);

    MetaType* p_OuterType(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    EnameSet* pTplParams(void);
    void vTplParams(EnameSet* pTplParams);
  
    Stz* szTypeNm(void);
  
    Footer* aFooter(void);
  };

  MetaType* _pMetaType(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
