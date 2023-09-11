#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Termination Dialog"

participant "Client" as c
participant "Server" as s

activate c
activate s

c->s: DisconnectionRequest

note over s
  """
  Server cleans up and 
  closes connection to client
  """
end note

s-->c: close socket
destroy c

note over (:---:) c, s
  "Dialog ends"
end note