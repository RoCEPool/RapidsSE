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
 
    MetaType*     m_p_OuterType;   // (opt) this type is within outer/containing type

    s32           m_sTypeNm;       // name of this type
    eMT           m_eMetaType;     // kind of type
    eCP           m_eCppPart;      // c++ part
  
    TagSet*     m_pTplParams;    // template parameters of this type
  
    MetaInherits* m_pMetaInherits; // set of inherited types

    i8            m_nSize;         // rolled up size of this structure

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaType(Stz* szTypeNm, eMT eMetaType, eCP eCppPart,
             MetaType* p_OuterType);
   ~MetaType(void) noexcept(false);

    MetaMember* pAddMember(Stz* szMemberNm);

    MetaInherit* pAddInherit(Stz* szBaseClassNm);

    MetaInherits* pInherits(void);

    eMT eMetaType(void);

    MetaType* p_OuterType(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    TagSet* pTplParams(void);
    void vTplParams(TagSet* pTplParams);
  
    Stz* szTypeNm(void);
  
    Footer* aFooter(void);
  };

  MetaType* _pMetaType(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
