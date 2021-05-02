/*----------------------------*
 * Special Insertion Sort     *
 * (c) 1994, Blase B. Cindric *
 * All rights reserved, and   *
 *  jealously guarded         *
 *----------------------------*/

void special_insertion_sort(

   unsigned *list,  /* array to be sorted */
   int n,           /* no. of elements in array */
   int num_sorted) /* no. of elements already in order */

{

    int j, k;
    unsigned item_to_place;

    /*-------------------------------------------*
     *  subscripts for sorted portion of array:  *
     *     0 to (num_sorted - 1)                 *
     *  subscripts for unsorted portion:         *
     *     num_sorted to n                       *
     *-------------------------------------------*/

    for (k = num_sorted; k < n; k++) {

                /* move new item out of array */

        item_to_place = list[k];

                /* copy all values larger than new item down one
                     place in the array */

        for (j = k - 1; list[j] > item_to_place && j >= 0; j--)
            list[j+1] = list[j];

                /* place new item in its proper array position */

        list[j+1] = item_to_place;

    }  /* end of outer for loop */

}  /* end of special insertion sort */

