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

    INIT_LIST_HEAD(head); // 使用 Linux API 初始化鏈結串列頭節點
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
    if (!head || !s)
        return false;
    
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
    if (!head || !s)
        return false;
    
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
    if (!head || list_empty(head))
        return false;
    
    struct list_head *slow = head->next, *fast = head->next;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    element_t *entry = list_entry(slow, element_t, list);
    list_del(&entry->list);

    if (entry->value)
        free(entry->value);
    free(entry);

    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *current, *safe;
    element_t *entry, *next_entry;

    list_for_each_safe(current, safe, head) {
        entry = list_entry(current, element_t, list);

        if (safe != head) {
            next_entry = list_entry(safe, element_t, list);
            if (strcmp(entry->value, next_entry->value) == 0) {
                list_del(&entry->list);
                free(entry->value);
                free(entry);
            }
        }
    }

    return true;
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *node1, *node2, *safe;

    list_for_each_safe(node1, safe, head) {
        if (node1->next == head)
            break;

        node2 = node1->next;
        list_move(node1, node2); // 移動 node1 到 node2 後面
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {
    if (!head || list_empty(head))
        return;

    struct list_head *node, *safe;
    list_for_each_safe(node, safe, head) {
        list_move(node, head); // move to head
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || k <= 1)
        return;

    struct list_head *start = head->next, *end, *safe;
    int count;

    while (start != head) {
        struct list_head *group_head = start;
        count = 1;
        end = start;

        // 找到 k 節點或到達鏈結尾部
        while (count < k && end->next != head) {
            end = end->next;
            count++;
        }

        if (count < k)
            break;

        // 翻轉當前 k 節點
        safe = end->next;
        struct list_head *cur = group_head, *tmp;
        while (cur != safe) {
            tmp = cur->next;
            list_move(cur, end);
            cur = tmp;
        }

        start = safe;
    }
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Compare function for list_sort */
static int compare_element(const struct list_head *a, const struct list_head *b, bool descend)
{
    element_t *e1 = list_entry(a, element_t, list);
    element_t *e2 = list_entry(b, element_t, list);
    return descend ? strcmp(e2->value, e1->value) : strcmp(e1->value, e2->value);
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {
    if (!head || list_empty(head))
        return;

    struct list_head *i, *j, *min_max;
    for (i = head->next; i != head; i = i->next) {
        min_max = i;
        for (j = i->next; j != head; j = j->next) {
            if (compare_element(min_max, j, descend) > 0)
                min_max = j;
        }
        if (min_max != i) {
            list_move(min_max, i->prev);
        }
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *current = head->prev, *safe;
    char *max_value = list_entry(current, element_t, list)->value;
    // int removed_count = 0;

    list_for_each_safe(current, safe, head) {
        element_t *entry = list_entry(current, element_t, list);
        if (strcmp(entry->value, max_value) > 0) {
            max_value = entry->value;
        } else {
            list_del(&entry->list);
            free(entry->value);
            free(entry);
            // removed_count++;
        }
    }
    // return removed_count;
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
     if (!head || list_empty(head))
        return 0;

    struct list_head *current = head->prev, *safe;
    char *min_value = list_entry(current, element_t, list)->value;
    // int removed_count = 0;

    list_for_each_safe(current, safe, head) {
        element_t *entry = list_entry(current, element_t, list);
        if (strcmp(entry->value, min_value) < 0) {
            min_value = entry->value;
        } else {
            list_del(&entry->list);
            free(entry->value);
            free(entry);
            // removed_count++;
        }
    }

    // return removed_count;
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
      if (!head || list_empty(head))
        return 0;

    struct list_head *queue, *safe;
    struct list_head result;
    INIT_LIST_HEAD(&result);

    // 合併所有鏈結
    list_for_each_safe(queue, safe, head) {
        struct list_head *node, *temp;
        list_for_each_safe(node, temp, queue) {
            list_del(node);
            list_add_tail(node, &result);
        }
    }

    // 排序合併後的鏈結
    q_sort(&result, descend);

    // 將結果鏈結接回 head
    list_splice(&result, head);

    // return q_size(head);
    
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}