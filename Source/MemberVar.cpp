/*============================================================================*/
/* MemberVar.cpp - Member Variable                                            */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MemberVar,CN_MEMBERVAR)
__aNewFunction(MemberVar,CN_MEMBERVAR)
__vDeleteFunction(MemberVar,CN_MEMBERVAR)
__aFooterFunction(MemberVar)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MemberVar::MemberVar(MemberSet* p_MemberSet)
: Object(CN_MEMBERVAR)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
  
  m_p_MemberSet = p_MemberSet;
  
  // var name
  m_sVarNm = C_NO_NAME;
  m_eVar   = MV_VAR;

  // MV_VAR: simple var
  m_sTypeNm   = C_NO_NAME;
  m_nPointers = 0;
  m_bFunPtr   = false;
  m_nBits = 0;
  
  // MV_SET: nested set
  m_pNestedSet = nullptr;
  
  // arrays
  m_nElements = 0;
  
  _checkInitialised_(CN_MEMBERVAR);
  
  _postcondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MemberVar::~MemberVar(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
  
  delete m_pNestedSet;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if term set can be recogised as a member variable                     */
/*----------------------------------------------------------------------------*/
bool MemberVar::bIsMemberVar(Stz* szClassNm, EnameSet* pTerms)
{
  _static_
  
  Stz* szTerm;
  
  i8 nTerms = pTerms->nEnames();
  
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++)
    vTrace("[%lld] %s "_Stz,i_Term,pTerms->szName(i_Term)->azChar());
  vTrace("\n"_Stz);
  
  i8 i_Term = 0;
  
  szTerm = pTerms->szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (szTerm->bSame(szClassNm) &&
      pTerms->szName(i_Term)->bSame("("_Stz)) {
    return false;
  }
  
  // -- -- -- -- -- --
  // exclude destructor function
  if (szTerm->bSame("~"_Stz))
    return false;
  
  // (*) szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = pTerms->szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = pTerms->szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a member function
  szTerm = pTerms->szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = pTerms->szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a member variable
  return true;
}

/*----------------------------------------------------------------------------*/
/* get variable name from given terms                                         */
/*----------------------------------------------------------------------------*/
Stz* MemberVar::szVarNm(EnameSet* pTerms, s32& rsTypeNm,
                        i8& rnPtrs, bool& rbFunPtr, i8& rnElements, i8& rnBits)
{
  _static_
  
  rnPtrs     = 0;
  rbFunPtr   = false;
  rnElements = 0;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Stz* szTerm;
  Stz* szName = "*UNKNOWN*"_Stz;

  i8 nTerms = pTerms->nEnames();
  
  i8 i_Term = 0;
  rsTypeNm = pTerms->szName(i_Term++);

  do {
    szTerm = pTerms->szName(i_Term++);
    
    if (szTerm->bSame(":"_Stz)) {
      rnBits = pTerms->szName(i_Term)->iToI8();
      break;
    }
    
    if (szTerm->bSame("*"_Stz))
      rnPtrs++;
    else
    if (szTerm->bSame("("_Stz))
      rbFunPtr = true;
    else
      szName = szTerm;

  } while (szTerm->bSame("*"_Stz) || szTerm->bSame("("_Stz));
  
  vTrace("VarNm = %s\n"_Stz,szTerm->azChar());
  
  if (i_Term < nTerms) {
    szTerm = pTerms->szName(i_Term++);
    //RASSERT(szTerm->bSame("["_Stz))
    rnElements = 1; //!more
  }
  
  vTrace("VarNm %s Ptrs=%lld FunPtr=%lld Elms=%lld Bits=%lld\n"_Stz,
         szTerm->azChar(),
         rnPtrs,
         (i8)rbFunPtr,
         rnElements,
         rnBits);

  return szName;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of bits for a bit field                                         */
/*----------------------------------------------------------------------------*/
i8 MemberVar::nBits(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_nBits;
}

/*----------------------------------------------------------------------------*/
/* set number of bits for a bit field                                         */
/*----------------------------------------------------------------------------*/
void MemberVar::vBits(i8 nBits)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_nBits = nBits;
}

/*----------------------------------------------------------------------------*/
/* get number of array elements                                               */
/*----------------------------------------------------------------------------*/
i8 MemberVar::nElements(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_nElements;
}

/*----------------------------------------------------------------------------*/
/* set number of array elements                                               */
/*----------------------------------------------------------------------------*/
void MemberVar::vElements(i8 nElements)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_nElements = nElements;
}

/*----------------------------------------------------------------------------*/
/* is this member a function pointer?                                         */
/*----------------------------------------------------------------------------*/
bool MemberVar::bFunPtr(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_bFunPtr;
}

/*----------------------------------------------------------------------------*/
/* set is this member a function pointer?                                     */
/*----------------------------------------------------------------------------*/
void MemberVar::vFunPtr(bool bFunPtr)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_bFunPtr = bFunPtr;
}

/*----------------------------------------------------------------------------*/
/* get which member set this member var is within                             */
/*----------------------------------------------------------------------------*/
MemberSet* MemberVar::pMemberSet(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_p_MemberSet;
}

/*----------------------------------------------------------------------------*/
/* get number of type pointers                                                */
/*----------------------------------------------------------------------------*/
i8 MemberVar::nPointers(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_nPointers;
}

/*----------------------------------------------------------------------------*/
/* get number of type pointers                                                */
/*----------------------------------------------------------------------------*/
void MemberVar::vPointers(i8 nPointers)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_nPointers = nPointers;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MemberVar::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
      
  vTrace("%sMemberVar %s  Type %s Ptrs=%lld FunPtr=%lld Elms=%lld Bits=%lld\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sVarNm.azChar(),
         
         m_sTypeNm.azChar(),
         m_nPointers,
         (i8)m_bFunPtr,
         m_nElements,
         m_nBits,
         
         C_ENDARGS);

}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MemberVar::vRecPrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
  
  vPrint(i_Depth);
}

/*----------------------------------------------------------------------------*/
/* set type name                                                              */
/*----------------------------------------------------------------------------*/
void MemberVar::vTypeNm(Stz* szTypeNm)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_sTypeNm = szTypeNm;
}

/*----------------------------------------------------------------------------*/
/* get type name                                                              */
/*----------------------------------------------------------------------------*/
Stz* MemberVar::szTypeNm(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_sTypeNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* get variable name                                                          */
/*----------------------------------------------------------------------------*/
Stz* MemberVar::szVarNm(void)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  return m_sVarNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set variable name                                                          */
/*----------------------------------------------------------------------------*/
void MemberVar::vVarNm(Stz* szVarNm)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
    
  m_sVarNm = szVarNm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
