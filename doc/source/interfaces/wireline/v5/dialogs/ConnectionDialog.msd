#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Connection Dialog"

participant "Client" as c
participant "Server" as s

activate c
activate s

c-->s: Open socket connection

c->s: ProtocolCompatibleRequest
note left of c
"""
Client indicates what version
of the wireline protocol it 
wants to use.
"""
end note

alt [protocol version is not compatible]
  s->c: ProtocolIncompatibleResponse
  destroy c
  note right of s
    "Client socket is closed"
  end note
else [protocol version is compatible]
  activate c
  s->c: ProtocolCompatibleResponse
end



note left of c
"""
Client indicates the type of tunnel
it wants. Currently only supporting
clear text tunnels.
"""
end note

c->s: ClearTextTunnelRequest

loop [while has warning]
  s->c: WarningResponse
end

alt [has error?]
  s->c: ErrorResponse
  destroy c
  note right of s
    "Client socket is closed"
  end note
 else [request is valid]
  activate c
  s->c: TunnelSupportedResponse
end



note left of c
  "Client tries to authenticate with server"
end note

c->s: PlainAuthenticationRequest

loop [while has warning]
  s->c: WarningResponse
end

alt [authentication fails?]
  s->c: ErrorResponse
  destroy c
  note right of s
    "Client socket is closed"
  end note
 else [authentication is valid]
  activate c
  s->c: AuthenticationOkResponse
end



note left of c
  "Client sends any default session properties"
end note

c->s: SessionChangeRequest

loop [while has warning]
  s->c: WarningResponse
end

alt [session property error?]
  s->c: ErrorResponse
  destroy c
  note right of s
    "Client socket is closed"
  end note
 else [request is valid]
  activate c
  s->c: SessionChangeResponse
end

note over c,s
"""
Connection dialog has completed.
Client is ready to begin executing queries.
"""
end note
