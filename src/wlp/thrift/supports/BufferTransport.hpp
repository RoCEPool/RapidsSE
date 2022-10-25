/*============================================================================*/
/* BufferTransport.hpp - A Thrift transport that reads & writes from a buffer */
/*============================================================================*/

#pragma once

#include "thrift/transport/TTransport.h"
#include "BaseTypes_tcc.h"
#include "MessageTypes_types.h"
#include "WlpHeader.hpp"


namespace wlp
{
  class BufferTransport : public apache::thrift::transport::TTransport
  {
  private:
    char* m_pcBuf;
    u8    m_iLen;
    u8    m_iNext;

  public:
    BufferTransport(char* pcBuf, u8 iLen);
    ~BufferTransport() override;

    WlpHeader* pAllocHeader(com::rapidsdata::wirelineprotocol::messages::v5::MessageTypes::type eType);
    i8 iBytesWritten();
    i8 iBytesRead();
    i8 iBytesLeft();

    bool isOpen() const override;
    bool peek() override;
    void open() override;
    void close() override;

    uint32_t read_virt(uint8_t* uint8, uint32_t uint32) override;
    void write_virt(const uint8_t* uint8, uint32_t uint32) override;

    const uint8_t* borrow_virt(uint8_t* uint8, uint32_t* uint32) override;
    void consume_virt(uint32_t uint32) override;
  };
}