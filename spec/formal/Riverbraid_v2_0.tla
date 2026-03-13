------------------------------ MODULE Riverbraid_v2_0 ------------------------------
EXTENDS Naturals, Sequences
CONSTANTS PetalCount, Version, ProtocolID
VARIABLES sequence, petals
Petal == [ hash : Seq(Int), status : {0,1}, reserved : Seq(Int) ]
TypeInvariant == 
    /\ sequence \in Nat 
    /\ Len(petals) = PetalCount
ReservedZeroInvariant == \A i \in DOMAIN petals : \A j \in 1..31 : petals[i].reserved[j] = 0
Init == sequence = 0 /\ petals = [i \in 1..PetalCount |-> [hash |-> <<0>>, status |-> 0, reserved |-> <<0>>]]
Next == sequence' = sequence + 1 /\ UNCHANGED petals
Spec == Init /\ [][Next]_<<sequence, petals>>
=============================================================================
