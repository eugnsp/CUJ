#include <stdio.h>

/* TREE STRUCT: GENERIC BINARY TREE NODE */
typedef struct treenode {
  struct treenode *left;
  struct treenode *right;
  /* Data is unimportant ...*/
} TREENODE;

/* STATIC VARIABLES */
static int       list_num;
static TREENODE  list_base;
static TREENODE *list_tail;
static TREENODE *root;
static int       left;

/*list_num  IS SIMPLY A COUNTER OF HOW MANY ITEMS HAVE 
            BEEN ADDED TO THE LIST.
  list_base IS USED AS A DUMMY NODE AT THE HEAD OF THE 
            LIST TO PREVENT TESTING FOR A NULL ROOT 
            EVERY TIME A NODE IS ADDED TO THE LIST.
  list_tail IS USED AS A POINTER TO THE LAST NODE IN 
            THE LINKED LIST.
  root      IS USED AS A POINTER TO THE NODE AT THE 
            HEAD OF THE LIST.
  left      IS A BOOLEAN TO KEEP TRACK OF WHICH WAY WE 
            ARE GOING IN THE TREE SO THAT WE DIVIDE 
            UNEVEN LISTS CORRECTLY. ALTHOUGH IT WOULD 
            BE MORE CLEAR TO USE A PARAMETER, THE 
            ROUTINE IS FINISHED WITH left BEFORE IT 
            RECURSES (IT'S NO LONGER USING IT) AND 
            MAKING IT STATIC REDUCES THE STACK SIZE 
            REQUIRED FOR THE RECURSION. */

/* Function prototypes */
       void     *optree    (void *);
static void      list_build(TREENODE *);
static TREENODE *form_tree (int);

/* OPTIMIZE A TREE */
void *optree(void *opt_root)
{
  if (opt_root == NULL) return(NULL);

  list_tail = &list_base;
  list_num  = 0;

  list_build(opt_root);

  root = list_base.right;

  left = 0;
  return((void *) form_tree(list_num));
}

/* FORM AN OPTIMIZED TREE FROM LIST */
TREENODE *form_tree(int num)
{
  int middle;
  TREENODE *ptr;

  middle = (num >> 1);   /* (num / 2) */

  /* SPECIAL 5-NODE CASE */
  if(num == 5) middle++;

  /* LEAN BRANCH TO CENTER OF TREE */
  if(left) middle = num - middle - 1; 

  /* REMOVE LEFT SUBTREE FROM LIST */
  left = 1;
  ptr = (middle > 0) ? form_tree(middle) : NULL;
  root->left = ptr;

  /* REMOVE THIS NODE FROM LIST */
  ptr = root;
  root = root->right;

  /* REMOVE RIGHT SUBTREE FROM LIST */
  left = 0;
  middle = num - middle - 1;
  ptr->right = (middle > 0) ? form_tree(middle) : NULL;
  return ptr;
}

/* FLATTEN TREE INTO LINKED LIST */
void list_build(TREENODE *node)
{
  if(node->left)  list_build(node->left);

  list_tail->right = node;
  list_tail = node;
  list_num++;

  if(node->right) list_build(node->right);
}
