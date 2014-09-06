// DESCRIPTION:
// We modified the clook_add_request from a FIFO to an ascending ordered list. 


Test Results:
Looking at the dmesg results, we can see that the sector order is in sorted, ascending order. The read write order appears to still be in FIFO, although the execution of these is still in an ordered format.

Sources:


https://code.oregonstate.edu/svn/cs411g5/linux-2.6.34.7-clook_iosched/block/clook-iosched.c
With the reference above, we saw an implementation of the dispatch file that helped us understand the function of this method.

http://www.emerham.com/media/Class/CS/CS%20411/proj3/team04-proj03-clook/clook-iosched.c
The emerham link showed a method of printing the current direction and sector that is being written or read from.

http://www.makelinux.net/ldd3/chp-11-sect-5
The link above showed a way of sorting a linked list in descending order. This was very similar to the algorithm that we came up with for sorting, and so we used it to verify our sorting, and to learn many of the linked list methods available.


http://code.google.com/p/cs411-look-iosched/source/browse/branches/linux-2.6.33.7-project4/linux-2.6.33.7/block/look-iosched.c?spec=svn9&r=9
The code hosted on google helped us with the issue we faced of sorting the linked list. We had attempted to sort the entire list, but learned from this code that it is much easier to write a program that deals first with an empty list, or a list of length 1. Then we can move on to testing the whole list.
