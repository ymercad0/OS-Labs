/*===========================================================================*
  *				pick_proc				     *
  *===========================================================================*/
static struct proc *
pick_proc ()
{
//  /* Decide who to run now.  A new process is selected and returned.
//   * When a billable process is selected, record it in 'bill_ptr', so that the
//   * clock task can tell who to bill for system time.
//   *
//   * This function always uses the run queues of the local cpu!
//   */
//   register struct proc *rp;                  /* process to run */
//   struct proc **rdy_head;
//   int q;                             /* iterate over queues */

//   /* Check each of the scheduling queues for ready processes. The number of
//     * queues is defined in proc.h, and priorities are set in the task table.
//     * If there are no processes ready to run, return NULL.
//     */
//   rdy_head = get_cpulocal_var(run_q_head);
//   for (q=0; q < NR_SCHED_QUEUES; q++) {
//      if(!(rp = rdy_head[q])) {
//              TRACE(VF_PICKPROC, printf("cpu %d queue %d empty\n", cpuid, q););
//              continue;
//      }
  struct proc *rp;
  rp = rdy_head[q];
    /*************** your lottery scheduling algorithm below ***************/
    struct proc *head = rdy_head[q];
    struct proc *tail = rdy_tail[q];
	if (cont_wins == 2) {
		cont_wins = 0;
		rp = tail; // a ticket has won more than 3 times, set the current winner to the tail
	}

	else{
		int winning_tks = rand(); // randomly pick a winner
		int sum = 0;
		while (sum < winning_tks){
			sum += rp->tickets;
			if (sum < winning_tks) {
				if(rp == tail) rp = head; // wrap around the queue
				else rp = rp->p_nextready;
			}
		}
	}

	if(rp->tickets > 10){
		cont_wins += 1;  // a ticket won once
	}
	total_tckts -= rp->tickets;
    /*************** your lottery scheduling algorithm  above ***************/

//      assert(proc_is_runnable(rp));
//      if (priv(rp)->s_flags & BILLABLE)
//              get_cpulocal_var(bill_ptr) = rp; /* bill for system time */
  return rp;
 }