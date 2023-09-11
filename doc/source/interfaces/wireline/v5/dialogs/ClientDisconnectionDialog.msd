#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Client Disconnection Dialog"

participant "Client" as c
participant "Server" as s

activate c
activate s

note over c,s
  """
  Client has an existing 
  connection to the server
  """
end note

s->c: DisconnectNotification

s-->c: close socket
destroy c


note over (:---:) c, s
  "Dialog ends"
end note