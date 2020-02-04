/*============================================================================*/
/* MetaMember.cpp - Member Variable of a Meta Structure                       */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaMember,CN_METAMEMBER)
__aNewFunction(MetaMember,CN_METAMEMBER)
__vDeleteFunction(MetaMember,CN_METAMEMBER)
__aFooterFunction(MetaMember)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaMember::MetaMember(Stz* szMemberNm, MetaType* p_MetaType)
: Object(CN_METAMEMBER)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
  
  m_p_MetaType = p_MetaType;
  
  // var name
  m_sMemberNm   = szMemberNm;
  m_eMetaMember = MM_SIMPLE;
  m_nSize       = 0;

  // MM_SIMPLE: simple var
  m_sTypeNm   = C_NO_NAME;
  m_pTplArgs  = nullptr;
  m_nPointers = 0;
  m_bFunPtr   = false;
  m_nBits     = 0;
  
  // MM_STRUCTURE: nested structure
  m_p_NestedStructure = nullptr;
  
  // arrays
  m_nElements = 0;
  
  _checkInitialised_(CN_METAMEMBER);
  
  _postcondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaMember::~MetaMember(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if term set can be recogised as a member variable                     */
/*----------------------------------------------------------------------------*/
bool MetaMember::bIsMemberVar(Stz* szClassNm, EnameSet* pTerms)
{
  _static_
  
  Stz* szTerm;
  
  i8 nTerms = pTerms->nEnames();
  
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
    vTrace("[%lld] %s "_Stz,i_Term,pTerms->szName(i_Term)->azChar());
    if (pTerms->szName(i_Term)->bSame("operator"_Stz))
      return false;
  }
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
  
  // nb. szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = pTerms->szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = pTerms->szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a function pointer
  if (szTerm->bSame("("_Stz)) {
    szTerm = pTerms->szName(i_Term++);
    
    if (szTerm->bSame("*"_Stz))
  
      return true;
  }
  
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
/* get member name from given terms                                           */
/*----------------------------------------------------------------------------*/
Stz* MetaMember::szMemberNm(EnameSet* pTerms, Stz*& rszTypeNm,
                            i8& rnPtrs, bool& rbFunPtr,
                            i8& rnElements, i8& rnBits)
{
  _static_
  
  rnPtrs     = 0;
  rbFunPtr   = false;
  rnElements = 0;
  rnBits     = 0;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Stz* szTerm;
  Stz* szName = "*UNKNOWN*"_Stz;

  i8 nTerms = pTerms->nEnames();
  
  i8 i_Term = 0;
  rszTypeNm = pTerms->szName(i_Term++);

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
i8 MetaMember::nBits(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_nBits;
}

/*----------------------------------------------------------------------------*/
/* set number of bits for a bit field                                         */
/*----------------------------------------------------------------------------*/
void MetaMember::vBits(i8 nBits)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_nBits = nBits;
}

/*----------------------------------------------------------------------------*/
/* get number of array elements                                               */
/*----------------------------------------------------------------------------*/
i8 MetaMember::nElements(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_nElements;
}

/*----------------------------------------------------------------------------*/
/* set number of array elements                                               */
/*----------------------------------------------------------------------------*/
void MetaMember::vElements(i8 nElements)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_nElements = nElements;
}

/*----------------------------------------------------------------------------*/
/* is this member a function pointer?                                         */
/*----------------------------------------------------------------------------*/
bool MetaMember::bFunPtr(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_bFunPtr;
}

/*----------------------------------------------------------------------------*/
/* set is this member a function pointer?                                     */
/*----------------------------------------------------------------------------*/
void MetaMember::vFunPtr(bool bFunPtr)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_bFunPtr = bFunPtr;
}

/*----------------------------------------------------------------------------*/
/* get which member set this member var is within                             */
/*----------------------------------------------------------------------------*/
MetaType* MetaMember::p_MetaType(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_p_MetaType;
}

/*----------------------------------------------------------------------------*/
/* get member kind                                                            */
/*----------------------------------------------------------------------------*/
eMM MetaMember::eMetaMember(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_eMetaMember;
}

/*----------------------------------------------------------------------------*/
/* get the nested structure                                                   */
/*----------------------------------------------------------------------------*/
MetaType* MetaMember::p_NestedStructure(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_p_NestedStructure;
}

/*----------------------------------------------------------------------------*/
/* set the nested set                                                         */
/*----------------------------------------------------------------------------*/
void MetaMember::vNestedStructure(MetaType* p_NestedStructure)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_p_NestedStructure = p_NestedStructure;
}

/*----------------------------------------------------------------------------*/
/* get number of type pointers                                                */
/*----------------------------------------------------------------------------*/
i8 MetaMember::nPointers(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_nPointers;
}

/*----------------------------------------------------------------------------*/
/* get number of type pointers                                                */
/*----------------------------------------------------------------------------*/
void MetaMember::vPointers(i8 nPointers)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_nPointers = nPointers;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaMember::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
     
  if (MM_SIMPLE == m_eMetaMember)
    vTrace("%sMember %-16s %lldB\n"_Stz,
           
           Stz::szTab(i_Tab)->azChar(),
           m_sMemberNm.azChar(),
           m_nSize,
           
           C_ENDARGS);
  else
    vTrace("%sMember %-16s %lldB : Type=%s Ptrs=%lld FunPtr=%lld Elms=%lld Bits=%lld\n"_Stz,
         
           Stz::szTab(i_Tab)->azChar(),
           m_sMemberNm.azChar(),
           m_nSize,
           
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
void MetaMember::vRecursivePrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
  
  vPrint(i_Depth);
  
  if (MM_STRUCTURE == m_eMetaMember)
    m_p_NestedStructure->vRecursivePrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get rolled-up size of this member                                          */
/*----------------------------------------------------------------------------*/
i8 MetaMember::nSize(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_nSize;
}

/*----------------------------------------------------------------------------*/
/* get the optional template arguments                                        */
/*----------------------------------------------------------------------------*/
EnameSet* MetaMember::pTplArgs(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
  
  return m_pTplArgs;
}

/*----------------------------------------------------------------------------*/
/* set template arguments                                                     */
/*----------------------------------------------------------------------------*/
void MetaMember::vTplArgs(EnameSet* pTplArgs)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
  
  if (!m_pTplArgs)
    m_pTplArgs = new EnameSet;
  
  m_pTplArgs->vCopy(pTplArgs);
}

/*----------------------------------------------------------------------------*/
/* set type name                                                              */
/*----------------------------------------------------------------------------*/
void MetaMember::vTypeNm(Stz* szTypeNm)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_sTypeNm = szTypeNm;
}

/*----------------------------------------------------------------------------*/
/* get type name                                                              */
/*----------------------------------------------------------------------------*/
Stz* MetaMember::szTypeNm(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_sTypeNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* get variable name                                                          */
/*----------------------------------------------------------------------------*/
Stz* MetaMember::szMemberNm(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_sMemberNm.szStz();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
