/*============================================================================*/
/* TagSet.hpp -- ename-value set                                              */
/*============================================================================*/
#pragma once

#include "eTN.h"

namespace BASE
{
  static const i8 C_TAG_SET = 256;
  
  struct Tag
  {
    i8  m_i_Name;
    i8  m_i_Value;
    eTN m_eToken;
  };

  class TagSet : public Object
  {
  public:
  
    i8   m_nItems;
    Tag  m_xaTag[C_TAG_SET];
  
    b32K m_xyStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    TagSet(void);
   ~TagSet() noexcept(false);
  
    void vAppend(Stz* szName);
    void vAppend(Stz* szName, i8 iValue);
    void vAppend(Stz* szName, Object* pObject);
    void vAppend(Stz* szName, Stz* szValue);
    void vConcat(Stz* szCmd);
    void vConcat(Stz* szCmd, i8 i_Tag);
    void vContentDump(void);
    void vCopy(TagSet* pSrcTagSet);
    void vDelete(Stz* szName);
    void vDumpSet(bool bCarriageReturns=false);
    void vDumpSetDetailed(void);
    i8   i_Find(Stz* szName);
    bool bHasName(Stz* szName);
    void vInsert(Stz* szName);
    void vInsert(Stz* szName, i8 iValue);
    void vInsert(Stz* szName, Object* pValue);
    void vInsert(Stz* szName, Stz* szValue);
    bool bIsDecodable(i8 i_Tag);
    bool bIsInteger(i8 i_Tag);
    bool bIsString(i8 i_Tag);
    i8   nItems(void);
    Stz* szName(i8 i_Tag);
    void vNamesString(String* pString);
    void vReplace(Stz* szName, i8 iValue);
    void vReplace(Stz* szName, Object* pObject);
    void vReplace(Stz* szName, Stz* szValue);
    void vReset(void);
    void vSprint(Stz* sArgs,
                 bool bParenth=true, bool bNames=false, bool bDecode=false);
    eTN  eToken(i8 i_Tag);
    i8   iValue(i8 i_Tag);
    i8   iValue(Stz* szName);
    Object* pValue(Stz* szName);
    Stz* szValue(i8 i_Tag);
    Stz* szValueDecoded(i8 i_Tag);
    Stz* szValue(Stz* szName);

  private:
    i8   i_InsertPoint(Stz* szName);
    void vShiftDown(i8 i_From);

    Footer* aFooter(void);
  };
  
  TagSet* _pTagSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
