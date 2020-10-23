Alberto Maiocco
CS4760 Project 3
10/22/2020

Feel like I was really close this time but just could't get it finished...

Anyways, the thing compiles and send messages. Towards the end there I was having major issues with resource starvation,
and I don't think I protected the critical section well enough.

At any rate, compile with make as usual. Make clean gets rid of objects/executables.

-h will show usage.

User sleeps for the given duration before returning control and sending the message back. Was that correct?

Log file contains when children are terminated. Its supposed to contain when they are created but I think
there are conflicts on writing to the file that I didn't protect well enough.

I left in a console output of children getting messages to show that message passing is occuring and so
you can tell me where I went wront.

Qapla'!
