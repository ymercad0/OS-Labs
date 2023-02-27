#define CURRENT 1
#define PARENT  2

/* System call */
int my_getpid() {
  // m_in is a global variable which contains the user message and other attributes
  // Get the current PID from the process array. mproc has 1 slot per process
  long user_option = m_in.m4_l1;
  if(user_option == CURRENT)    // who_p contains caller's process slot
    return mproc[who_p].mp_pid; // gets current process ID
  // Get parent's PID
  return mproc[mp->mp_parent].mp_pid;
}

/* User library */
int mygetpid(long whichOne) {
    message m;
    m.m_type = M4;
    m.m4_l1 = whichOne;
    return _syscall(PM_PROC_NR, MYCALL, &m);
}

