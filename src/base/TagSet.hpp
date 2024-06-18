/*============================================================================*/
/* TagSet.hpp -- ename-value set                                              */
/*============================================================================*/
#pragma once

#include "eTN.h"
#include "SegmentedArrayNL_t.hpp"
#include "Lock.hpp"

namespace PRIMITIVES { class PackedStringBuf; }

namespace BASE
{
  struct Tag
  {
    Stz* m_szName{};
    union {
      i8      m_iValue{};
      Stz*    m_szValue;
      Object* m_pValue;
    };
    eTN m_eToken;

    void vInit(void) {
      m_szName  = nullptr;
      m_iValue = C_UNKNOWN;
      m_eToken  = TN_NONE;
    }

    Tag() {
      vInit();
    }

    Tag(Stz* szName, i8 iValue, eTN eToken) {
      m_szName = szName;
      m_iValue = iValue;
      m_eToken = eToken;
    }

    Tag(Stz* szName, Stz* szValue, eTN eToken) {
      m_szName = szName;
      m_szValue = szValue;
      m_eToken = eToken;
    }
  };

  typedef SegmentedArrayNL_t<CN_TAGARRAY, Tag, (C_SIZE_TINY_OBJECT - sizeof(Lock) - ( (6+4) * C_BYTES_PER_WORD)) / sizeof(Tag)> TagArray;
  typedef SegmentedArrayIter<CN_TAGARRAY, Tag, (C_SIZE_TINY_OBJECT - sizeof(Lock) - ( (6+4) * C_BYTES_PER_WORD)) / sizeof(Tag)> TagArrayIter;

  // Note: The size calculation above tries to maximise the size of the segmented array by
  // taking into account member variables in SegmentedArrayNL_t, as well as the size of
  // TagSet itself. This allows us to fit TagSet and TagArray into the same single 1KB chunk.
  // The SegmentedArrayNL_t class itself has members worth 6 words and the TagSet class has
  // members worth 4 words (header, m_pStrings, footer (2)).


  class TagSet : public Object
  {
  public:
    Lock              m_oLock;
    TagArray          m_oTagArray;
    PackedStringBuf*  m_pStrings;
  
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
    bool bIsInteger(i8 i_Tag);
    bool bIsObject(i8 i_Tag);
    bool bIsString(i8 i_Tag);
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

    eTN  eToken(i8 i_Tag);
    void vToString(String* psStr, bool bValueOnly=false);

    i8   iValue(i8 i_Tag);
    i8   iValue(Stz* szName);
    Object* pValue(i8 i_Tag);
    Object* pValue(Stz* szName);
    Stz* szValue(i8 i_Tag);
    Stz* szValueDecoded(i8 i_Tag);
    Stz* szValue(Stz* szName);

  private:
    i8   i_FindNL(Stz* szName);
    Tag* pFindNL(Stz* szName);
    bool bHasNameNL(Stz* szName);
    i8   i_InsertPoint(Stz* szName);
    void vInsertNL(Stz* szName);
    void vInsertNL(Stz* szName, i8 iValue);
    void vInsertNL(Stz* szName, Object* pValue);
    void vInsertNL(Stz* szName, Stz* szValue);
    bool bIsDecodable(Tag* pTag);
    bool bIsInteger(Tag* pTag);
    bool bIsObject(Tag* pTag);
    i8   nItemsNL(void);
    Stz* szName(Tag* pTag);
    void vReplaceNL(Stz* szName, i8 iValue);
    void vReplaceNL(Stz* szName, Object* pObject);
    void vReplaceNL(Stz* szName, Stz* szValue);
    bool bIsString(Tag* pTag);
    i8   iValue(Tag* pTag);
    Object* pValue(Tag* pTag);
    Stz* szValue(Tag* pTag);
    Stz* szValueDecoded(Tag* pTag);

    Footer* aFooter(void);
  };
  
  TagSet* _pTagSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
