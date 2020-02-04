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
    MetaType* m_p_WithinType; // which meta type this type is within
  
    // member name
    s32  m_sMemberNm;      // name of the member 
    eMM  m_eMetaMember;    // kind of meta member
    i8   m_nSize;          // size of this member

    // type specifier
    s32       m_sTypeNm;   // type name (such as i8, Footer, List_t<..>)
    MetaType* m_pType;     // the type of this member
    EnameSet* m_pTplArgs;  // opt. with these template arguments
    i8        m_nPointers; // number of asterisks after the type name
    bool      m_bFunPtr;   // is this member a function pointer?
      
    // nested complex type
    MetaType* m_p_NestedStructure; // nested structure

    // fields and arrays
    i8   m_nBits;     // no of bits if bit field;
    i8   m_nElements; // how many array elements (0 means not an array)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaMember(Stz* szMemberNm, MetaType* p_MetaType);
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
  
    MetaType* p_WithinType(void);
  
    eMM  eMetaMember(void);
  
    MetaType* p_NestedStructure(void);
    void vNestedStructure(MetaType* p_NestedStructure);

    i8   nPointers(void);
    void vPointers(i8 nPointers);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    EnameSet* pTplArgs(void);
    void vTplArgs(EnameSet* pTplArgs);

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
