void enqueue(
  register struct proc *rp	/* this process is now runnable */
)
{
/* Add 'rp' to one of the queues of runnable processes.  This function is
 * responsible for inserting a process into one of the scheduling queues.
 * The mechanism is implemented here.   The actual scheduling policy is
 * defined in sched() and pick_proc().
 *
 * This function can be used x-cpu as it always uses the queues of the cpu the
 * process is assigned to.
 */
  int q = rp->p_priority;	 		/* scheduling queue to use */
  struct proc **rdy_head, **rdy_tail;

  assert(proc_is_runnable(rp));
  assert(q >= 0);
  rdy_head = get_cpu_var(rp->p_cpu, run_q_head);
  rdy_tail = get_cpu_var(rp->p_cpu, run_q_tail);

  if(rp->tickets > 0) total_tckts = total_tckts + (rp->tickets);

 /********* Your enqueue modification should be below ************/
  /* Now add the process to the queue. */
  if (!rdy_head[q]) {		/* add to empty queue */
      rdy_head[q] = rdy_tail[q] = rp; 		/* create a new queue */
      rp->p_nextready = NULL;		/* mark new end */
  }

  else { /* Implementing tickets in descending order */
	struct proc *start = rdy_head[q];
	struct proc *end = rdy_tail[q];
	//Case that needs to be added to the head
	if(rp->tickets > start->tickets){
		rp->p_nextready = start;
		start = rdy_head[q] = rp;
	}

	// Needs to be added to the tail
	else if(rp->tickets < end->tickets){
		end->p_nextready = rp;
		rp->p_nextready = NULL;
		end = rdy_tail[q] = rp;
	}

	//Find an empty position to insert (depending on tickets value)
	else{
		struct proc *curr = start;
		while(rp->tickets < curr->p_nextready->tickets){
			curr = curr->p_nextready;
		}
		rp->p_nextready = curr->p_nextready;
		curr->p_nextready = rp;
	  }
  }
 /********* Your enqueue modification should be above ************/

  if (cpuid == rp->p_cpu) {
	  /*
	   * enqueueing a process with a higher priority than the current one,
	   * it gets preempted. The current process must be preemptible. Testing
	   * the priority also makes sure that a process does not preempt itself
	   */
	  struct proc * p;
	  p = get_cpulocal_var(proc_ptr);
	  assert(p);
	  if((p->p_priority > rp->p_priority) &&
			  (priv(p)->s_flags & PREEMPTIBLE))
		  RTS_SET(p, RTS_PREEMPTED); /* calls dequeue() */
  }
#ifdef CONFIG_SMP
  /*
   * if the process was enqueued on a different cpu and the cpu is idle, i.e.
   * the time is off, we need to wake up that cpu and let it schedule this new
   * process
   */
  else if (get_cpu_var(rp->p_cpu, cpu_is_idle)) {
	  smp_schedule(rp->p_cpu);
  }
#endif

  /* Make note of when this process was added to queue */
  read_tsc_64(&(get_cpulocal_var(proc_ptr)->p_accounting.enter_queue));


#if DEBUG_SANITYCHECKS
  assert(runqueues_ok_local());
#endif
}