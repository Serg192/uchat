#include "../inc/libmx.h"

t_list *mx_sort_list(t_list *lst, bool(*cmp)(void*, void*)){
   if(lst == NULL)
      return NULL;

   for(t_list *i = lst; i; i = i->next){
       for(t_list *j = lst; j->next; j = j->next){
           if(cmp(j->data, j->next->data)){
               void* t = j->next->data;
               j->next->data = j->data;
               j->data = t;
           }
       }
   }

   return lst;
}


