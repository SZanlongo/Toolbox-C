// DESCRIPTION:
// We modified the clook_add_request from a FIFO to an ascending ordered list.  

/*
 * elevator clook
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct clook_data {
	struct list_head queue;
//we added this to keep track of what was the last sector that was dispatched
//sector_t dispatched;	//this should no longer be used
//it was used in our original (flawed) solution
};

//for reference
/*
 * this is our original printk:
 * printk("[CLOOK] add %c %ld\n", rq_data_dir((rq)), (unsigned long)rq->sector);
 * the problem with this is that we are not sure how to read from rq_data_dir((rq))
 * 
 * this has since been replaced with:
 * printk("[CLOOK] add <%p> <%p>\n", rq->bio->bi_rw, rq->bio->bi_sector);
 */

static void clook_merged_requests(struct request_queue *q, struct request *rq,
		struct request *next) {
	list_del_init(&next->queuelist);
}

static int clook_dispatch(struct request_queue *q, int force) {
	struct clook_data *ld = q->elevator->elevator_data;

	if (!list_empty(&ld->queue)) {
		struct request *rq;
		rq = list_entry(ld->queue.next, struct request, queuelist);
		//no longer needed
		//nd->last_dispatched = rq->sector;		
		char direction;
		if (rq_data_dir(rq) == READ)
			direction = 'R';
		else
			direction = 'W';
		printk("[CLOOK] dsp %c %ld\n", direction,
				(unsigned long) rq->bio->bi_sector);

		list_del_init(&rq->queuelist);
		elv_dispatch_add_tail(q, rq);
		return 1;
	}
	return 0;
}

static struct request *
clook_latter_request(struct request_queue *q, struct request *rq) {
	struct clook_data *cd = q->elevator->elevator_data;

	if (rq->queuelist.next == &cd->queue)
		return NULL;
return list_entry(rq->queuelist.next, struct request, queuelist);
}

static void clook_add_request(struct request_queue *q, struct request *rq) {
struct clook_data *cd = q->elevator->elevator_data;

//added the printk message here
char direction;
if (rq_data_dir(rq) == READ)
	direction = 'R';
else
	direction = 'W';
printk("[CLOOK] add %c %ld\n", direction, (unsigned long) rq->bio->bi_sector);

//list is empty, so no need to do anything
//add the request, and exit
if (list_empty(&cd->queue)) {
	list_add_tail(&rq->queuelist, &cd->queue);
	return;
}

//this should be the first place in the queue
struct request
*rQueue = list_entry(cd->queue.next, struct request, queuelist);

//now that we've covered a list of length, 0, what about length 1?
//if less than the only item, then add at the beginning
if (blk_rq_pos(rq) < blk_rq_pos(rQueue)) {
	list_add(&rq->queuelist, &cd->queue);
	return;
}

//our original implementation
/*
 //check if the linked-list is empty
 //if yes, just add to end and stop
 if (list_empty(&cd->queue)) {
 list_add_tail(&rq->queuelist, &cd->queue);
 return;
 } else {
 //otherwise, iterate through the linked-list
 for (ptr = cd->queue.next; ptr != &cd->queue; ptr = ptr->next) {
 //keep going until we get to a node that is smaller than the one we are inserting
 entry = list_entry(ptr, struct request, &cd->queuelist);
 if (entry->rqSec >= cd->dispatched || rqSec > entry->rqSec) {
 //if the inserting node is larger than the node we reached, then switch them
 list_add_tail(&rq->queuelist, ptr->next);
 return;
 }
 }
 //close the linked-list
 list_add_tail(&rq->queuelist, ptr->next);
 }
 */

//if list is >0, and its not length 1
//check if there are more entries
if (clook_latter_request(q, rQueue) == NULL) {
	//no more, so add request
	list_add(&rq->queuelist, &rQueue->queuelist);
	return;
}

//if we get this far, then we know:
//list length >0
//list length >1, and we cannot put it there
//so we need to traverse the linked list
while (clook_latter_request(q, rQueue) != NULL) {
	//as long as the current request is bigger than the current one, loop
	struct request *secReq = clook_latter_request(q, rQueue);
	//if smaller than current item, then add in that position
	if ((blk_rq_pos(rQueue) < blk_rq_pos(rq))
			&& (blk_rq_pos(rq) < blk_rq_pos(secReq))) {
		list_add(&rq->queuelist, &rQueue->queuelist);
		break;
	} else {
		//not smaller, so loop by going to next
		rQueue = secReq;
	}
}
}

static struct request *
clook_former_request(struct request_queue *q, struct request *rq) {
struct clook_data *cd = q->elevator->elevator_data;

if (rq->queuelist.prev == &cd->queue)
	return NULL;
return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static int clook_init_queue(struct request_queue *q) {
struct clook_data *cd;

cd = kmalloc_node(sizeof(*cd), GFP_KERNEL, q->node);
if (!cd)
return NULL;
INIT_LIST_HEAD(&cd->queue);
return cd;
}

static void clook_exit_queue(struct elevator_queue *e) {
struct clook_data *cd = e->elevator_data;

BUG_ON(!list_empty(&cd->queue));
kfree(cd);
}

static struct elevator_type elevator_clook = { .ops = { .elevator_merge_req_fn =
clook_merged_requests, .elevator_dispatch_fn = clook_dispatch,
.elevator_add_req_fn = clook_add_request, .elevator_former_req_fn =
clook_former_request, .elevator_latter_req_fn = clook_latter_request,
.elevator_init_fn = clook_init_queue, .elevator_exit_fn = clook_exit_queue, },
.elevator_name = "clook", .elevator_owner = THIS_MODULE, };

static int __init clook_init(void)
{
elv_register(&elevator_clook);
return 0;
}

static void __exit clook_exit(void)
{
elv_unregister(&elevator_clook);
}

module_init( clook_init);
module_exit( clook_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("C-Look IO scheduler");
