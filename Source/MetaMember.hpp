/*============================================================================*/
/* MetaMember.hpp - Member of a Meta Type                                     */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaMember : public Object,
                     public ListItem_t<MetaMember>
  {
  public:
    MetaType* m_p_WithinType; // this member is within which meta type
  
    // member name
    s32  m_sMemberNm;      // name of the member 
    eMM  m_eMetaMember;    // kind of meta member
    i8   m_nSize;          // size of this member

    // type specifier
    s32       m_sTypeNm;    // name of specified type (such as i8, Footer, List_t<..>)
    MetaType* m_p_MetaType; // the type of this member
    TagSet* m_pTplArgs;   //   opt. specified with these template arguments
    i8        m_nAsterisks; // number of asterisks after the specified type
    bool      m_bFunPtr;    // is this member a function pointer?

    // fields and arrays
    i8   m_nBits;     // no of bits (if bit field)
    i8   m_nElements; // how many array elements (0 means not an array)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaMember(Stz* szMemberNm, MetaType* p_WithinType);
   ~MetaMember(void) noexcept(false);
  
    static bool bIsMemberVar(Stz* szClassNm, TagSet* pTerms);
    static Stz* szMemberNm(TagSet* pTerms, Stz*& rszTypeNm,
                           i8& rnAsterisks, bool& rbFunPtr,
                           i8& rnElements, i8& rnBits);

    i8   nAsterisks(void);
    void vAsterisks(i8 nAsterisks);
  
    i8   nBits(void);
    void vBits(i8 nBits);
    
    i8   nElements(void);
    void vElements(i8 nElements);
  
    bool bFunPtr(void);
    void vFunPtr(bool bFunPtr);
   
    Stz* szMemberNm(void);

    eMM  eMetaMember(void);
  
    MetaType* p_MetaType(void);
    void vMetaType(MetaType* p_MetaType);

  
    void vPrint(i8 i_Tab);
  
    void vRecursivePrint(i8 i_Depth);
  
    i8   nSize(void);
  
    TagSet* pTplArgs(void);
    void vTplArgs(TagSet* pTplArgs);

    Stz* szTypeNm(void);
    void vTypeNm(Stz* szTypeNm);

    MetaType* p_WithinType(void);
  
    Footer* aFooter(void);
  };

  MetaMember* _pMetaMember(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
