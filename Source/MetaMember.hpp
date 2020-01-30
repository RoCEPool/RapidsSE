/*============================================================================*/
/* MetaMember.hpp - Meta Block Member                                         */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaMember : public Object,
                     public ListItem_t<MetaMember>
  {
  public:
    MetaBlock* m_p_ParentBlock; // tparent meta block
  
    // var name
    s32  m_sMemberNm; // name of the member variable
    eMV  m_eVar;      // kind of var
    i8   m_nSize;     // size of this member

    // MV_VAR: simple var
    s32  m_sTypeNm;   // type name (such as Footer)
    i8   m_nPointers; // number of asterisks after the type name
    bool m_bFunPtr;   // is this variable a function pointer?
    i8   m_nBits;     // no of bits if bit field;
    
    // MV_SET: nested set
    MetaBlock* m_pNestedBlock; // nested set

    // arrays
    i8   m_nElements; // how many array elements (0 means not an array)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaMember(Stz* szMemberNm, MetaBlock* p_ParentBlock);
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
  
    MetaBlock* p_ParentBlock(void);
    eMV  eMetaMember(void);
    MetaBlock* pNestedBlock(void);
    void vNestedBlock(MetaBlock* pNestedBlock);

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
