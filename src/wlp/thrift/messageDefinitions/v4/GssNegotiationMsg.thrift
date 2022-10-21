
namespace java com.rapidsdata.wirelineprotocol.messages.v4

// Sent from the client to the server and from the server to the client with opaque
// GSS-API token information while the security mechanisms on each end negotiate.
//
// Direction: client --> server
//            and
//            server --> client

struct GssNegotiationMsg {

  // The opaque GSS-API token
  1: required binary token;

}
