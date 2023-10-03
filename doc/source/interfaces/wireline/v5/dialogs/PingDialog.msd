#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Ping Dialog"

participant "Client" as c
participant "Server" as s

activate c
activate s


c->s: PingRequest
s->c: PingResponse

note over (:---:) c, s
  "Dialog ends"
end note