/*============================================================================*/
/* TagSet.hpp -- ename-value set                                              */
/*============================================================================*/
#pragma once

#include "eTN.h"
#include "SegmentedArrayNL_t.hpp"

namespace BASE
{
  static const i8 C_TAG_SET = 256;
  
  struct Tag
  {
    i8  m_i_Name{};
    i8  m_i_Value{};
    eTN m_eToken;

    void vInit(void) {
      m_i_Name  = C_UNKNOWN;
      m_i_Value = C_UNKNOWN;
      m_eToken  = TN_NONE;
    }

    Tag() {
      vInit();
    }

    Tag(i8 iName, i8 iValue, eTN eToken) {
      m_i_Name = iName;
      m_i_Value = iValue;
      m_eToken = eToken;
    }

    Tag(i8 iName, Stz* szValue, eTN eToken) {
      m_i_Name = iName;
      m_i_Value = (i8) szValue;
      m_eToken = eToken;
    }
  };

  typedef SegmentedArrayNL_t<CN_TAGARRAY, Tag, (C_SIZE_TINY_OBJECT - (6 * C_BYTES_PER_WORD)) / sizeof(Tag)> TagArray;
  typedef SegmentedArrayIter<CN_TAGARRAY, Tag, (C_SIZE_TINY_OBJECT - (6 * C_BYTES_PER_WORD)) / sizeof(Tag)> TagArrayIter;

  class TagSet : public Object
  {
  public:
    TagArray m_oTagArray;
    b32K m_xyStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    TagSet(void);
    TagSet(const TagSet& oOther);
   ~TagSet() noexcept(false);
  
    static void vRepair(Object* pObject);
    static bool bSerialise(TagSet* pTagSet, Bytes* pBuf);
    static TagSet* pDeserialise(Bytes* ayTagSet);

    void vAppend(Stz* szName);
    void vAppend(Stz* szName, i8 iValue);
    void vAppend(Stz* szName, Object* pObject);
    void vAppend(Stz* szName, Stz* szValue);
  
    void vConcat(String* pCmd);
    void vConcat(String* pCmd, i8 i_Tag);
    void vContentDump(void);
    void vCopy(TagSet* pSrcTagSet);
  
    void vDelete(Stz* szName);
    void vDumpSet(bool bCarriageReturns=false);
    void vDumpSetDetailed(void);
    TagSet* pDuplicate(void);
  
    i8   i_Find(Stz* szName);
    Tag* pFind(Stz* szName);
  
    bool bHasName(Stz* szName);
  
    void vInsert(Stz* szName);
    void vInsert(Stz* szName, i8 iValue);
    void vInsert(Stz* szName, Object* pValue);
    void vInsert(Stz* szName, Stz* szValue);
    bool bIsDecodable(i8 i_Tag);
    bool bIsDecodable(Tag* pTag);
    bool bIsInteger(i8 i_Tag);
    bool bIsInteger(Tag* pTag);
    bool bIsObject(i8 i_Tag);
    bool bIsObject(Tag* pTag);
    bool bIsString(i8 i_Tag);
    bool bIsString(Tag* pTag);
    i8   nItems(void);
  
    Stz* szName(i8 i_Tag);
    void vNamesString(String* pString);

    void vReplace(Stz* szName, i8 iValue);
    void vReplace(Stz* szName, Object* pObject);
    void vReplace(Stz* szName, Stz* szValue);
    void vReset(void);

    void vSprint(String* psArgs,
                 bool bParenth=true, bool bNames=false, bool bDecode=false);
    bool bSame(TagSet* pTagSet_arg);

    Tag* pTag(i8 i_Tag);
    eTN  eToken(i8 i_Tag);
    void vToString(String* psStr, bool bValueOnly=false);

    i8   iValue(i8 i_Tag);
    i8   iValue(Stz* szName);
    Object* pValue(i8 i_Tag);
    Object* pValue(Stz* szName);
    Stz* szValue(i8 i_Tag);
    Stz* szValueDecoded(i8 i_Tag);
    Stz* szValueDecoded(Tag* pTag);
    Stz* szValue(Stz* szName);

  private:
    i8   i_InsertPoint(Stz* szName);
    Stz* szName(Tag* pTag);
    i8   iValue(Tag* pTag);
    Object* pValue(Tag* pTag);
    Stz* szValue(Tag* pTag);

    Footer* aFooter(void);
  };
  
  TagSet* _pTagSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
