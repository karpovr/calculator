#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// DECLARATION

typedef struct node node;

struct node {
  int data;
  node* next;
};

void Push(node** head, int data);

node* BuildList(int length);

int Length(node* head);

void PrintList(node* head);

int Count(node* head, int searchFor);

int GetNth(node* head, int index);

void DeleteList(node** headRef);

int Pop (node** headRef);

void InsertNth(node** headRef, int index, int data);

void SortedInsert(node** headRef, node* newNode);

void InsertSort(node** headRef);

void Append(node** aRef, node** bRef);

void FrontBackSplit(node* source, node** frontRef, node** backRef);

void RemoveDuplicates(node* head);

void MoveNode(node** destRef, node** sourceRef);

void AlternatingSplit(node** source, node** aRef, node** bRef);

node* ShuffleMerge(node* a, node* b);

node* SortedMerge(node** a, node** b);

// IMPLEMENTATION

void Push(node** head, int data) {
  node* newNode = malloc(sizeof(node));
  newNode->data = data;
  newNode->next = *head;
  *head = newNode;
}

node* BuildList(int length) {
  node* head = NULL;
  while (length--) {
    Push(&head, length);
  }
  return head;
}

int Length(node* head) {
  int count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }
  return count;
}

void PrintList(node* head) {
  printf("{");
  int i;
  for (i = 0; head != NULL; i++) {
    printf( i == 0 ? "%i" : ", %i", head->data);
    head = head->next;
  }
  printf("}\n");
}

// Problems

// 1 - Count()
// Given a list and an int, return the number of times that int occurs
// in the list.

int Count(node* head, int searchFor) {
  int count = 0;
  while (head != NULL) {
    if (head->data == searchFor) count++;
    head = head->next;
  }
  return count;
}

void CountTest() {
  node* head = BuildList(3);
  int count = Count(head, 2);
  printf("\n1. Count: %i\n", count);
  DeleteList(&head);
}

// 2 - GetNth()
// Given a list and an index, return the data
// in the nth node of the list. The nodes are numbered from 0.
// Assert fails if the index is invalid (outside 0..lengh-1).

int GetNth(node* head, int index) {
  assert(index >= 0);
  while (head != NULL && index--) {
    head = head->next;
  }
  assert(index == -1);
  return head->data;
}

void GetNthTest() {
  printf("\n2. GetNth: \n");
  node* head = BuildList(3);
  int nth = GetNth(head, 2);
  printf("nth: %i\n", nth);
  DeleteList(&head);
}

// 3 - DeleteList()

void DeleteList(node** headRef) {
  node* next;
  while (*headRef != NULL) {
    next = (*headRef)->next;
    free(*headRef);
    *headRef = next;
  }
}

void DeleteListTest() {
  printf("\n3. DeleteList: \n");
  node* head = BuildList(3);
  printf("list = "); PrintList(head);
  DeleteList(&head);
  printf("deleted list = "); PrintList(head);
}

// 4 - Pop()

int Pop (node** headRef) {
  assert(*headRef != NULL);
  int data = (*headRef)->data;
  node* next = (*headRef)->next;
  free(*headRef);
  *headRef = next;
  return data;
}

void PopTest() {
  printf("\n3. Pop: \n");
  node* head = BuildList(3);
  printf("list = "); PrintList(head);
  int a = Pop(&head);
  int b = Pop(&head);
  int c = Pop(&head);
  printf("popped list = "); PrintList(head);
  int len = Length(head);
  printf("\n4. Pop: a = %i, b = %i, c = %i, len = %i\n", a, b, c, len);
  DeleteList(&head);
}

// 5 - InsertNth()
// A more general version of Push().
// Given a list, an index 'n' in the range 0..length,
// and a data element, add a new node to the list so
// that it has the given index.

void InsertNth(node** headRef, int index, int data) {
  assert(index >= 0);
  node** current = headRef;
  while (*current != NULL && index--) {
    current = &(*current)->next;
  }
  assert(index <= 0);
  node* newNode = malloc(sizeof(node));
  newNode->data = data;
  newNode->next = *current;
  *current = newNode;
  return;
}

void InsertNthTest() {
  printf("\n5. InsertNth: ");
  node* head = NULL; // start with the empty list
  InsertNth(&head, 0, 13); // build {13}
  InsertNth(&head, 1, 42); // build {13, 42}
  InsertNth(&head, 1, 5); // build {13, 5, 42}
  InsertNth(&head, 3, 11); // build {13, 5, 42}
  PrintList(head);
  DeleteList(&head); // clean up after ourselves
}

// 6 - SortedInsert()

void SortedInsert(node** headRef, node* newNode) {
  int data = newNode->data;
  node** current = headRef;
  while (*current != NULL && (*current)->data <= data) {
    current = &(*current)->next;
  }
  newNode->next = *current;
  *current = newNode;
}

void SortedInsertTest() {
  node* head = NULL;
  Push(&head, 5);
  Push(&head, 4);
  Push(&head, 2);
  Push(&head, 1);
  node* node3 = malloc(sizeof(node));
  node3->data = 3;
  node* node0 = malloc(sizeof(node));
  node0->data = 0;
  node* node6 = malloc(sizeof(node));
  node6->data = 6;
  SortedInsert(&head, node3);
  SortedInsert(&head, node0);
  SortedInsert(&head, node6);
  printf("\n6. SortedInsert: ");
  PrintList(head);
  DeleteList(&head); // clean up after ourselves
}

// 7 - InsertSort()
// Given a list, change it to be in sorted order (using SortedInsert()).

void InsertSort(node** headRef) {
  node* sorted = NULL;
  node* next;
  node* current = *headRef;
  while(current != NULL) {
    next = current->next;
    SortedInsert(&sorted, current);
    current = next;
  }
  *headRef = sorted;
}

void InsertSortTest() {
  node* head = NULL;
  Push(&head, 1);
  Push(&head, 8);
  Push(&head, 5);
  printf("\n7. InsertSort:\n");
  PrintList(head);
  InsertSort(&head);
  PrintList(head);
  DeleteList(&head); // clean up after ourselves
}

// 8 - Append()
// Append 'b' onto the end of 'a', and then set 'b' to NULL.

void Append(node** aRef, node** bRef) {
  node** current = aRef;
  while(*current != NULL) {
    current = &(*current)->next;
  }
  *current = *bRef;
  *bRef = NULL;
}

void AppendTest() {
  printf("\n8. Append: \n");

  node* a = NULL;
  Push(&a, 2);
  Push(&a, 1);
  printf("a: ");
  PrintList(a);

  node* b = NULL;
  Push(&b, 4);
  Push(&b, 3);
  printf("b: ");
  PrintList(b);

  Append(&a, &b);

  printf("a+b: ");
  PrintList(a);
  printf("b: ");
  PrintList(b);
}

// 9 — FrontBackSplit()
// Split the nodes of the given list into front and back halves,
// and return the two lists using the reference parameters.
// If the length is odd, the extra node should go in the front list.

void FrontBackSplit(node* source, node** frontRef, node** backRef) {
  assert(source != NULL);
  int index = 0;
  node* current = source;
  node* prevBack = source;
  *frontRef = source;
  *backRef = source;
  while (current != NULL) {
    if (index++ % 2) {
      prevBack = (*backRef);
      *backRef = (*backRef)->next;
    }
    current = current->next;
  }
  if(index == 1) {
    *frontRef = NULL;
  }
  prevBack->next = NULL;
}

void FrontBackSplitTest() {
  node* head = BuildList(5);
  node* front = NULL;
  node* back = NULL;
  printf("\n9. FrontBackSplit: \n");
  printf("full: \n");
  PrintList(head);

  FrontBackSplit(head, &front, &back);

  printf("front: \n");
  PrintList(front);

  printf("back: \n");
  PrintList(back);
}

// 10 - RemoveDuplicates()
// Remove duplicates from a sorted list.

void RemoveDuplicates(node* head) {
  node* next;
  while (head != NULL) {
    next = head->next;
    if (next != NULL && head->data == next->data) {
      head->next = next->next;
      free(next);
    }
    head = head->next;
  }
}

void RemoveDuplicatesTest() {
  printf("\n10. RemoveDuplicates: \n");
  node* head = BuildList(5);

  node* node2;
  Push(&node2, 2);
  SortedInsert(&head, node2);

  node* node3;
  Push(&node3, 3);
  SortedInsert(&head, node3);

  printf("Duplicates: ");
  PrintList(head);

  RemoveDuplicates(head);

  printf("Unique: ");
  PrintList(head);
}

// 11 — MoveNode()
/*
 Take the node from the front of the source, and move it to
 the front of the dest.
 It is an error to call this with the source list empty.
*/
void MoveNode(node** destRef, node** sourceRef) {
  assert(*sourceRef != NULL);
  node* tmp = *sourceRef;
  *sourceRef = (*sourceRef)->next;
  tmp->next = *destRef;
  *destRef = tmp;
}

void MoveNodeTest() {
  printf("\n11. MoveNode: \n");
  node* a = BuildList(3);
  node* b = BuildList(3);
  PrintList(a);
  PrintList(b);

  MoveNode(&a, &b);
  MoveNode(&a, &b);
  MoveNode(&a, &b);

  PrintList(a);
  PrintList(b);
}

// 12 — AlternatingSplit()
// Given the source list, split its nodes into two shorter lists.
// If we number the elements 0, 1, 2, ... then all the even elements
// should go in the first list, and all the odd elements in the second.
// The elements in the new lists may be in any order.

void AlternatingSplit(node** source, node** aRef, node** bRef) {
  int index = 0;
  while (*source != NULL) {
    if (index++ % 2 == 0) {
      MoveNode(aRef, source);
    } else {
      MoveNode(bRef, source);
    }
  }
}

void AlternatingSplitTest() {
  printf("\n12. AlternatingSplit: \n");
  node* head = BuildList(3);
  node* a = NULL;
  node* b = NULL;
  PrintList(head);
  AlternatingSplit(&head, &a, &b);
  PrintList(head);
  PrintList(a);
  PrintList(b);
}

// 13 - ShuffleMerge()
// Merge the nodes of the two lists into a single list taking a node
// alternately from each list, and return the new list.

node* ShuffleMerge(node* a, node* b) {
  node* merged = NULL;
  node** lastPtr = &merged;
  while ( a != NULL || b != NULL) {
    if (a) {
      MoveNode(lastPtr, &a);
      lastPtr = &(*lastPtr)->next;
    }
    if (b) {
      MoveNode(lastPtr, &b);
      lastPtr = &(*lastPtr)->next;
    }
  }
  return merged;
}

void ShuffleMergeTest() {
  printf("\n13. ShuffleMerge: \n");
  node* a = BuildList(3);
  node* b = BuildList(5);
  printf("a = "); PrintList(a);
  printf("b = "); PrintList(b);
  node* merged = ShuffleMerge(a, b);
  printf("merged = "); PrintList(merged);
}

// 14 — SortedMerge()
// Takes two lists sorted in increasing order, and
// splices their nodes together to make one big
// sorted list which is returned.
node* SortedMerge(node** a, node** b) {
  node* head = NULL;
  node** lastPtr = &head;
  while (*a != NULL && *b != NULL) {
    if ((*a)->data < (*b)->data) {
      MoveNode(lastPtr, a);
    } else {
      MoveNode(lastPtr, b);
    }
    lastPtr = &(*lastPtr)->next;
  }
  if (*a == NULL) Append(lastPtr, b);
  if (*b == NULL) Append(lastPtr, a);
  return head;
}

void SortedMergeTest() {
  printf("\n14. SortedMerge: \n");
  node* a = BuildList(5);
  printf("a = "); PrintList(a);
  node* b = BuildList(7);
  printf("b = "); PrintList(b);
  node* merged = SortedMerge(&a, &b);
  printf("merged = "); PrintList(merged);
}


// 15 — MergeSort()

void MergeSort(node** headRef) {
  if (*headRef == NULL || (*headRef)->next == NULL) return;
  node* a;
  node* b;
  FrontBackSplit(*headRef, &a, &b);
  MergeSort(&a);
  MergeSort(&b);
  *headRef = SortedMerge(&a, &b);
}

void MergeSortTest() {
  printf("\n15. MergeSort: \n");
  node* head = NULL;
  Push(&head, 12);
  Push(&head, 912);
  Push(&head, 2);
  Push(&head, 1);
  Push(&head, 10);
  Push(&head, 8);
  Push(&head, 13);
  //node* head = BuildList(1000);
  printf("original = "); PrintList(head);
  MergeSort(&head);
  printf("sorted = "); PrintList(head);
}

// 16 — SortedIntersect()
// Compute a new sorted list that represents the intersection
// of the two given sorted lists.

node* SortedIntersect(node* a, node* b) {
  node* head = NULL;
  node** tail = &head;
  while (a != NULL && b != NULL) {
    if (a->data < b->data) {
      a = a->next;
    } else if (a->data > b->data) {
      b = b->next;
    } else {
      Push(tail, a->data);
      tail = &(*tail)->next;
      a = a->next;
      b = b->next;
    }
  }
  return head;
}

void SortedIntersectTest() {
  printf("\n16. SortedIntersect: \n");
  node* a = NULL;
  Push(&a, 6);
  Push(&a, 5);
  Push(&a, 4);
  Push(&a, 3);
  Push(&a, 2);
  Push(&a, 1);
  printf("a = "); PrintList(a);

  node* b = NULL;
  Push(&b, 9);
  Push(&b, 8);
  Push(&b, 7);
  Push(&b, 6);
  Push(&b, 5);
  Push(&b, 4);
  printf("b = "); PrintList(b);

  node* intersected = SortedIntersect(a, b);
  printf("intersected = "); PrintList(intersected);
}

// 17 — Reverse()
void Reverse(node** headRef) {
  node* rev = NULL;
  while (*headRef != NULL) {
    MoveNode(&rev, headRef);
  }
  *headRef = rev;
}

void ReverseTest() {
  printf("\n17. Reverse: \n");
  node* list = BuildList(10);
  printf("list = "); PrintList(list);
  Reverse(&list);
  printf("reversed = "); PrintList(list);
  DeleteList(&list); // clean up after ourselves
}

// 18 — RecursiveReverse()
// Recursively reverses the given linked list by changing its .next
// pointers and its head pointer in one pass of the list.
void RecursiveReverse(node** headRef) {
  if (*headRef == NULL) { return; }
  node* first = *headRef;
  node* rest = first->next;
  if (rest == NULL) { return; }
  RecursiveReverse(&rest);
  first->next->next = first;
  first->next = NULL;
  *headRef = rest;
}

void RecursiveReverseTest() {
  printf("\n18. RecursiveReverse: \n");
  node* list = BuildList(10);
  printf("list = "); PrintList(list);
  RecursiveReverse(&list);
  printf("reversed = "); PrintList(list);
  DeleteList(&list); // clean up after ourselves
}


// Main

int main() {

  CountTest();
  GetNthTest();
  DeleteListTest();
  PopTest();
  InsertNthTest();
  SortedInsertTest();
  InsertSortTest();
  AppendTest();
  FrontBackSplitTest();
  RemoveDuplicatesTest();
  MoveNodeTest();
  AlternatingSplitTest();
  ShuffleMergeTest();
  SortedMergeTest();
  MergeSortTest();
  SortedIntersectTest();
  ReverseTest();
  RecursiveReverseTest();

  return 0;
}
