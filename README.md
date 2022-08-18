# PintOS

## 1. Thread (progressing)
### 1) Alarm Clock
### 2) Scheduler
    -  When a thread is added to the ready list that has a higher priority than the currently running thread, the current thread should immediately yield the processor to the new thread.
    - priority donation
### 3) Advanced Scheduler (BSD Scheduler)

## 2. User Program

## 3. VM

## 4. File System

 -------------------------------------
`pintos run alarm-multiple`
`gs201@gs201-14Z90N-VR5DK:~/Desktop/pintos/src/threads/build$` `make tests/threads/alarm-multiple.result.`
`gs201@gs201-14Z90N-VR5DK:~/Desktop/pintos/src/threads/build$` `make check`