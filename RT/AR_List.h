#ifndef AR_LIST_H
#define AR_LIST_H

namespace AR_List_Util
{
    
// loosing my mind so I had to make my own list
template <typename T> 
struct AR_List{
    AR_List* next;
    T* data;
    AR_List* prev;
};

    template <typename T>
    void Append(AR_List<T> *lst, AR_List<T> *obj)
    {
        lst->next->prev = obj->prev; //assumes, that the Previous object of the list is the end of the object list (or itself)
        obj->prev->next = lst->next;

        lst->next = obj;
        obj->prev = lst;

    }
}

#endif