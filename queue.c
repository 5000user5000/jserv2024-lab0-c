#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        // if allocation fail,return fail
        return NULL;
    }

    // point to self
    head->next = head;
    head->prev = head;

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head) {

    if (!head) // Check if the head is NULL
        return;

    struct list_head *current, *safe;  //*safe is used for saving the next pointer to avoid invalid pointer access
    element_t *entry;

    // Iterate over the queue safely to avoid invalid pointer access
    list_for_each_safe(current, safe, head) {
        // Get the container structure from the list_head pointer
        entry = list_entry(current, element_t, list);

        // Free the string memory pointed to by value
        if (entry->value)
            free(entry->value);

        // Remove the node from the list and free the element structure
        list_del(current);
        free(entry);
    }

    // Free the head node after all elements have been removed
    free(head);

}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *node = malloc(sizeof(element_t)); // allocate new node
    if (!node)
        return false;
    
    node->value = strdup(s); // copy string
    if (!node->value) {
        free(node); // if copy fail,release node
        return false;
    }

    list_add(&node->list, head); // add node at head of list
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *node = malloc(sizeof(element_t)); // allocate new node
    if (!node)
        return false;
    
    node->value = strdup(s); 
    if (!node->value) {
        free(node);
        return false;
    }
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if(!head || head->next == head)
        return NULL;
    
    // Get the first element from the queue
    element_t *entry = list_first_entry(head, element_t, list);

    // Copy the string value if sp is not NULL
    if (sp && entry->value && bufsize > 0) {
        strncpy(sp, entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0'; // Ensure null-termination
    }

    // Remove the node from the queue but do not free it
    list_del(&entry->list);

    // Return the removed element
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if(!head || head->next == head)
        return NULL;
    
    // Get the last element from the queue
    element_t *entry = list_last_entry(head, element_t, list);

    // Copy the string value if sp is not NULL
    if (sp && entry->value && bufsize > 0) {
        strncpy(sp, entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0'; // Ensure null-termination
    }

    // Remove the node from the queue but do not free it
    list_del(&entry->list);

    // Return the removed element
    return entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{   
    if(!head) return 0;

    int len = 0;
    struct list_head *li;

    list_for_each(li,head)  // linux api
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
