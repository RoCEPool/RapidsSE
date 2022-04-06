
#pragma once

namespace STORE {

  class BucketInfo : public Operand {
  private:
    s32 m_sNodeName;
    s32 m_sTableName;
    i8 m_iBucketNo;
    LglPtn m_tLglPtn;

    // CrwBuffer* m_pCrwBuffer

    // must be last
    Footer m_xFooter;
  public:
    void *operator new(size_t);

    void operator delete(void *);

    BucketInfo(s32 sTblNm, LglPtn tLglPtn, i8 iBucket);

    BucketInfo(void);

    ~BucketInfo(void) noexcept(false);


    s32 sTableName(void);

    i8 iBucketNo(void);

    LglPtn tLglPtn(void);

    void vNodeName(s32 sNodeName);

    s32 sNodeName(void);

    //void vCrwBuffer(CrwBuffer* pCrwBuffer);
    //CrwBuffer* pCrwBuffer(void);

    Footer *aFooter(void);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  BucketInfo *_pBucketInfo(void *aAddr);
}
