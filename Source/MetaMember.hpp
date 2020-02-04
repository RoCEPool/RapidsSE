/*============================================================================*/
/* MetaMember.hpp - Member Variable of a Meta Structure                       */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaMember : public Object,
                     public ListItem_t<MetaMember>
  {
  public:
    MetaStructure* m_p_MetaStructure; // which meta structure
  
    // var name
    s32  m_sMemberNm;   // name of the member variable
    eMM  m_eMetaMember; // kind of meta member
    i8   m_nSize;       // size of this member

    // MM_VAR: simple var
    s32  m_sTypeNm;     // type name (such as i8, Footer, List_t<..>)
    i8   m_nPointers;   // number of asterisks after the type name
    bool m_bFunPtr;     // is this member a function pointer?
    i8   m_nBits;       // no of bits if bit field;
    
    // MM_SET: nested set
    MetaStructure* m_p_NestedStructure; // nested structure

    // arrays
    i8   m_nElements; // how many array elements (0 means not an array)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaMember(Stz* szMemberNm, MetaStructure* p_MetaStructure);
   ~MetaMember(void) noexcept(false);
  
    static bool bIsMemberVar(Stz* szClassNm, EnameSet* pTerms);
    static Stz* szMemberNm(EnameSet* pTerms, Stz*& rszTypeNm,
                           i8& rnPtrs, bool& rbFunPtr,
                           i8& rnElements, i8& rnBits);

    i8   nBits(void);
    void vBits(i8 nBits);
    
    i8   nElements(void);
    void vElements(i8 nElements);
  
    bool bFunPtr(void);
    void vFunPtr(bool bFunPtr);
  
    MetaStructure* p_MetaStructure(void);
  
    eMM  eMetaMember(void);
  
    MetaStructure* p_NestedStructure(void);
    void vNestedStructure(MetaStructure* p_NestedStructure);

    i8   nPointers(void);
    void vPointers(i8 nPointers);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    Stz* szTypeNm(void);
    void vTypeNm(Stz* szTypeNm);
  
    Stz* szMemberNm(void);

    Footer* aFooter(void);
  };

  MetaMember* _pMetaMember(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
