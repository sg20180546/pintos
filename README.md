# PintOS

## 1. Thread (clear at 9/2)
### 1) Alarm Clock

 <img src = "https://user-images.githubusercontent.com/81512075/186709398-5358a3f6-97f0-4d59-b4ce-08bb008c3270.png" width="700" height="270">
 
![image](https://user-images.githubusercontent.com/81512075/186709495-cb2404d7-e8d9-4ba3-8db9-5acf0da6b30d.png)

- 1 tick = 10 ms = 0.01s
- if timer_sleep, insert into `struct list sleep_list`
- Each timer interrupt, test all elem in sleep list and get thread wake up.
### 2) Scheduler
-  When a thread is added to the ready list that has a higher priority than the currently running thread, the current thread should immediately yield the processor to the new thread.
- nested priority donation
### 3) Advanced Scheduler : Multi Level Feedback Queue Scheduler (mlfqs)
- integer : `priority`, `ready_threads nice`
- fixed point : `recent_cpu`, `load_avg` 
- `priority` = `PRI_MAX` - (`recent_cpu` / 4) - (`nice` * 2)
- `recent_cpu` = (2*`load_avg`)/(2*`load_avg` + 1) * `recent_cpu` + `nice`
- `load_avg` = (59/60) * `load_avg` + (1/60) * `ready_threads`
- Each Timer interrupt (10 ms, 1 tick)
- Every 40 ms (4 tick)
- Every 1 second(1 tick * TIME_FREQ)

<img src = "https://user-images.githubusercontent.com/81512075/188171945-e76f513b-709a-455a-964b-1f20129e4eef.png" width="500" height="800">
- disable thread_set_priority(int new_priority)
- disable priority donation
- little issue in mlfqs-load-avg

## 2. User Program (clear at 10/2)
### 1) process_wait
- process wait list in child, which is running
- process wait elem in parent, which is waiting for the process exit
- parent blocked and inserted to wait list of running ps
- if running process exit, update exit status for waiting processes and unblock those.
### 2) argument passing by stack (executing)
![image](https://user-images.githubusercontent.com/81512075/196643562-83a90000-bdc9-45d0-8832-60d976dd9a0f.png)

- use intr_frame
- *esp == system call number
- *(esp+i)== system call argument
### 3) system call handler
- Register handler
 
 <img src = "https://user-images.githubusercontent.com/81512075/196631762-e12d0af8-6009-4d0b-90ff-45f475ba17b2.png" width="600" height="100">

- Handling system call
 
 ![image](https://user-images.githubusercontent.com/81512075/196632021-a0b98f89-8ada-4335-a777-51bb73e38059.png)

## 3. VM

## 4. File System

 -------------------------------------
#### command
0. set gcc older version
`sudo update-alternatives --config gcc`
1. threads
`pintos run alarm-multiple`
`gs201@gs201-14Z90N-VR5DK:~/Desktop/pintos/src/threads/build$` `make tests/threads/alarm-multiple.result.`
`gs201@gs201-14Z90N-VR5DK:~/Desktop/pintos/src/threads/build$` `make check`
2. userprog
`pintos-mkdisk filesys.dsk --filesys-size=2`
`pintos -f -q`
`pintos -p ../../examples/echo -a echo -- -q`
`pintos -q run 'echo x'`
`pintos --filesys-size=2 –p ../../examples/echo –a echo -- -f –q run ‘echo x’`

