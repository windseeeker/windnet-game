#ifndef __SPIN_LOCK_H__
#define __SPIN_LOCK_H__

#define spinlock_init(x) { x = 0; }
#define spinlock_lock(x) while (__sync_lock_test_and_set(&x->lock, 1)) {}
#define spinlock_unlock(x) __sync_lock_release(&x->lock)

#endif
