PRIORITY BASED SCHEDULING REPORT:

---> Implemented the ps and set_priority system call.
---> All processes are given initial priority of 3.


$ ps
Name     Pid     State           Priority 
init     1       SLEEPING                3 
sh       2       SLEEPING                3 
ps       4       RUNNING                 3 


$ hackbench 3 &

---> Created 3 processes with priorities in descending order.

$ Parent 6 creating child 7
Parent 6 creating child 8
Parent 6 creating child 9
Child 8 created
Child 9 created

$ Total time taken = 353 ticks
Child 7 created
Total time taken = 359 ticks

$ Total time taken = 3515 ticks
Total time taken = 3515 ticks
zombie!

---> Total ticks = 3515 ticks

ROUND ROBIN SCHEDULING REPORT:

$ hackbench 3 &
$ Parent 5 creating child 6
Parent 5 creating child 7
Child 7 cChild 6 created
reated
Parent 5 creating child 8
Child 8 created

$ Total time taken = 375 ticks

$ Total time taken = 810 ticks

$ Total time taken = 1525 ticks
Total time taken = 1526 ticks
zombie!

---> Total ticks = 1526 ticks
