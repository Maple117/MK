/*****************************************************************************
 * DESCRIPTION:         This file is part of the Contiki operating system.
                        The linked list library provides a set of functions for
                        manipulating linked lists.
                        A linked list is made up of elements where the first element
                        must be a pointer. This pointer is used by the linked list library
                        to form lists of the elements.

                        Lists are declared with the LIST() macro. The declaration specifies
                        the name of the list that later is used with all list functions.

                        Lists can be manipulated by inserting or removing elements from
                        either sides of the list (list_push(), list_add(), list_pop(),
                        list_chop()). A specified element can also be removed from inside a
                        list with list_remove(). The head and tail of a list can be
                        extracted using list_head() and list_tail(), respecitively.

 ****************************************************************************
 *
 * This software is owned by Versalinks and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Versalinks products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". Versalinks MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. Versalinks SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 ****************************************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cpluplus
extern "C" {
#endif

#define LIST_CONCAT2(s1, s2) s1##s2
#define LIST_CONCAT(s1, s2) LIST_CONCAT2(s1, s2)

/**
 * Declare a linked list.
 *
 * This macro declares a linked list with the specified \c type. The
 * type \b must be a structure (\c struct) with its first element
 * being a pointer. This pointer is used by the linked list library to
 * form the linked lists.
 *
 * \param name The name of the list.
 */
#define LIST(name) \
         static void *LIST_CONCAT(name,_list) = NULL; \
         static list_t name = (list_t)&LIST_CONCAT(name,_list)

/**
 * The linked list type.
 *
 */
typedef void ** list_t;

void   list_init(list_t list);
void * list_head(list_t list);
void * list_tail(list_t list);
void * list_pop (list_t list);
void   list_push(list_t list, void *item);

void * list_chop(list_t list);

void   list_add(list_t list, void *item);
void   list_remove(list_t list, void *item);

int    list_length(list_t list);

void   list_copy(list_t dest, list_t src);

void   list_insert(list_t list, void *previtem, void *newitem);

#ifndef NULL
  #define  NULL   0x00u
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LIST_H__ */

