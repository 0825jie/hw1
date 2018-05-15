#include<stdio.h>
#include <stdlib.h>
#include"my402list.h"
#include "cs402.h"

int My402ListInit(My402List * my402List){

    (my402List->anchor).next = &(my402List->anchor);
    (my402List->anchor).prev = &(my402List->anchor);
     my402List->num_members = 0;
    (my402List->anchor).obj = NULL;
    return TRUE;

}

int My402ListLength(My402List * my402List){

    return my402List->num_members;

}

int My402ListEmpty(My402List * my402List){

    if(my402List->num_members <= 0) return TRUE;
    else return FALSE;

}

int My402ListAppend(My402List * my402List, void * ele){
    My402ListElem * newElem = (My402ListElem *) malloc(sizeof(My402ListElem));
    if(newElem == NULL){
        return FALSE;
    }else{
        newElem->obj = ele;
        my402List->num_members ++;
        My402ListElem * lastElem = My402ListLast(my402List);
        if(lastElem){
            newElem->prev = lastElem;
            newElem->next = &(my402List->anchor);
            (my402List->anchor).prev = newElem;
            lastElem->next = newElem;
        }else{
            newElem->next = &(my402List->anchor);
            newElem->prev = &(my402List->anchor);
            (my402List->anchor).prev = newElem;
            newElem->prev->next = newElem;

        }
        return TRUE;
    }
}

int My402ListPrepend(My402List * my402List, void * ele){
    My402ListElem * newElem = (My402ListElem *) malloc(sizeof(My402ListElem));
    if(!newElem){
       return FALSE;
    }else{
        newElem->obj = ele;
        my402List->num_members ++;
        My402ListElem * firstElem = My402ListFirst(my402List);
        if(firstElem){
            newElem->next = firstElem;
            newElem->prev = &(my402List->anchor);
            (my402List->anchor).next = newElem;
            firstElem->prev = newElem;
        }else{
            newElem->next = &(my402List->anchor);
            newElem->prev = &(my402List->anchor);
            (my402List->anchor).prev = newElem;
            newElem->prev->next = newElem;
        }
    }
    return TRUE;
}

void My402ListUnlink(My402List * my402List, My402ListElem * ele) {
    if(My402ListEmpty(my402List)) return;
    else{
        ele->prev->next = ele->next;
        ele->next->prev = ele->prev;
        ele->next = NULL;
        ele->prev = NULL;
        my402List->num_members --;
        free(ele);
    }
    return;
}

void My402ListUnlinkAll(My402List * my402List){
    if(My402ListEmpty(my402List)) return;
    else{
        My402ListElem * firstElem = My402ListFirst(my402List);
        My402ListElem * tempElem = NULL;
        for(tempElem = firstElem->next; tempElem != &(my402List->anchor); tempElem = tempElem->next){
            firstElem->next = NULL;
            firstElem->prev = NULL;
            free(firstElem);
            firstElem = tempElem;
            my402List->num_members --;
        }
        My402ListInit(my402List);
    }
    return;
}

int My402ListInsertAfter(My402List * my402List, void * newElem, My402ListElem * pendElem){
    if(pendElem == NULL){
        return My402ListAppend(my402List, newElem);
    }else{
        My402ListElem * elem = (My402ListElem *) malloc(sizeof(My402ListElem));
        if( !elem){
            return FALSE;
        }else{
            elem->obj = newElem;
            elem->next = pendElem->next;
            elem->next->prev = elem;
            pendElem->next = elem;
            elem->prev = pendElem;
            my402List->num_members ++;
        }
    }
    return TRUE;
}

int My402ListInsertBefore(My402List * my402List, void * newElem, My402ListElem * pendElem){
    if(pendElem == NULL){
        return My402ListPrepend(my402List, newElem);
    }else{
        My402ListElem * elem = (My402ListElem *) malloc(sizeof(My402ListElem));
        if(!elem){
            return FALSE;
        } else{
            elem->obj = newElem;
            elem->prev = pendElem->prev;
            elem->prev->next = elem;
            elem->next = pendElem;
            pendElem->prev = elem;
            my402List->num_members ++;
        }
    }
    return TRUE;
}

My402ListElem *My402ListFirst(My402List * my402List){
    if(my402List->num_members == 0) return NULL;
    else{
        return (my402List->anchor).next;
    }
}

My402ListElem *My402ListLast(My402List * my402List){
    if(my402List->num_members == 0) return NULL;
    else{
        return (my402List->anchor).prev;
    }
}

My402ListElem *My402ListNext(My402List * my402List, My402ListElem * elem){
    if(elem->next == &(my402List->anchor)) return NULL;
    else return elem->next;
}

My402ListElem *My402ListPrev(My402List * my402List, My402ListElem * elem){
    if(elem->prev == &(my402List->anchor) ) return NULL;
    else return elem->prev;
}

My402ListElem *My402ListFind(My402List * my402List, void * elem){
    My402ListElem * first = My402ListFirst(my402List);
    while(first){
        if(first->obj == elem) return first;
        else first = My402ListNext(my402List, first);
    }
    return NULL;
}

