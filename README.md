# hw1

Doubly-linked Circular List in C


[BC: sentence fixed and two figures updated 1/8/2018]  A traditional doubly-linked list of length 4 looks like the following:
Image of a Traditional Doubley-linked List
A corresponding My402List would look like the following:
Image of a My402List
The functions you need to implement has the following meaning (note that, for readability, all the functions are missing "My402List" before the name, and all of them are missing (My402List*) as the first argument as compare to what's in the actual header file, "my402list.h"):
int Length()
Returns the number of elements in the list.
int Empty()
Returns TRUE if the list is empty. Returns FALSE otherwise.
int Append(void *obj)
If list is empty, just add obj to the list. Otherwise, add obj after Last(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
int Prepend(void *obj)
If list is empty, just add obj to the list. Otherwise, add obj before First(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
void Unlink(My402ListElem *elem)
Unlink and delete elem from the list. Please do not delete the object pointed to by elem and do not check if elem is on the list.
void UnlinkAll()
Unlink and delete all elements from the list and make the list empty. Please do not delete the objects pointed to by the list elements.
int InsertBefore(void *obj, My402ListElem *elem)
Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
int InsertAfter(void *obj, My402ListElem *elem)
Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
My402ListElem *First()
Returns the first list element or NULL if the list is empty.
My402ListElem *Last()
Returns the last list element or NULL if the list is empty.
My402ListElem *Next(My402ListElem *elem)
Returns elem->next or NULL if elem is the last item on the list. Please do not check if elem is on the list.
My402ListElem *Prev(My402ListElem *elem)
Returns elem->prev or NULL if elem is the first item on the list. Please do not check if elem is on the list.
My402ListElem *Find(void *obj)
Returns the list element elem such that elem->obj == obj. Returns NULL if no such element can be found.
int Init()
Initialize the list into an empty list. Returns TRUE if all is well and returns FALSE if there is an error initializing the list.
