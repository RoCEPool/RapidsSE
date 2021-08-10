
#pragma once

namespace STORE {

  class BucketGet : public Operand {
  private:
    s32 m_sTableName;
    i8  m_iBucketNo;

    // must be last
    Footer m_xFooter;
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    BucketGet(i8 iBucket);
    BucketGet(void);
    ~BucketGet(void) noexcept(false);

    void vTableName(s32 sTblNm) { m_sTableName = sTblNm;}
    s32 sTableName(void) { return m_sTableName;}
    i8  iBucketNo(void) { return m_iBucketNo;}

    Footer* aFooter(void);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  BucketGet *_pBucketGet(void *aAddr);
}