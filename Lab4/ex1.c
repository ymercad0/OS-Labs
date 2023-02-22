// Run from Minix
int do_mycall() {
  long user_num = m_in.m4_l1; // Global variable which contains the user message
  printf("Hola! El número es: %ld\n", user_num);
  return 0;
}

int mycall(long integer) {
    message m;
    m.m_type = M4; // Moodle moment
    m.m4_l1 = integer;
    _syscall(PM_PROC_NR, MYCALL, &m);
    return 0;
}