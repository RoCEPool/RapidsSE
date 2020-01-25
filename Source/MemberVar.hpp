/*============================================================================*/
/* MemberVar.hpp - Member Variable                                            */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MemberVar : public Object,
                    public ListItem_t<MemberVar>
  {
  public:
    MemberSet* m_p_MemberSet; // the _p_ sequence means not owned by
  
    // var name
    s32  m_sVarNm;    // name of the member variable
    eMV  m_eVar;      // kind of var
  
    // MV_VAR: simple var
    s32  m_sTypeNm;   // type name (such as Footer)
    i8   m_nPointers; // number of asterisks after the type name
    bool m_bFunPtr;   // is this variable a function pointer?
    i8   m_nBits;     // no of bits if bit field;
    
    // MV_SET: nested set
    MemberSet* m_pNestedSet; // nested set

    // arrays
    i8   m_nElements; // how many array elements (0 means not an array)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MemberVar(MemberSet* p_MemberSet);
   ~MemberVar(void) noexcept(false);
  
    static bool bIsMemberVar(Stz* szClassNm, EnameSet* pTerms);
    static Stz* szVarNm(EnameSet* pTerms, s32& rsTypeNm,
                        i8& rnPtrs, bool& rbFunPtr, i8& rnElements, i8& rnBits);

    i8   nBits(void);
    void vBits(i8 nBits);
    
    i8   nElements(void);
    void vElements(i8 nElements);
  
    bool bFunPtr(void);
    void vFunPtr(bool bFunPtr);
  
    MemberSet* pMemberSet(void);
  
    MemberSet* pNestedSet(void);
    void vNestedSet(MemberSet* pNestedSet);

    i8   nPointers(void);
    void vPointers(i8 nPointers);
  
    void vPrint(i8 i_Tab);
    void vRecPrint(i8 i_Depth);
  
    Stz* szTypeNm(void);
    void vTypeNm(Stz* szVarNm);
  
    Stz* szVarNm(void);
    void vVarNm(Stz* szVarNm);

      
    Footer* aFooter(void);
  };

  MemberVar* _pMemberVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
