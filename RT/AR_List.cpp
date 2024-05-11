#include "AR_List.h"

using namespace AR_List_Util;

template <typename T> 
void Append(AR_List<T> *lst, AR_List<T> *obj)
{
    lst->next->prev = obj->prev; //assumes, that the Previous object of the list is the end of the object list (or itself)
    obj->prev->next = lst->next;

    lst->next = obj;
    obj->prev = lst;

};
