#include <stdio.h>
#include <stdlib.h> /* for malloc() and free() functions */
#define TRUE 1
#define FALSE 0

/*----------------------------------------
Define the types necessary for the binary
search tree.  The first is the treenode.
This structure includes pointers to its
left and right "children" as well as to
its "parent".  Additionally, there is the
necessary integer value of the treenode.
(This can be changed to any other type,
granted all functions are also changed
accordingly)

The other definition type is the tree itself
which need only be defined by its root which 
is of a tree node type whose parent is the 
NULL pointer
-----------------------------------------*/
typedef struct treenode {
	int value;
	struct treenode* parent;
	struct treenode* left;
	struct treenode* right;
} TreeNode;

typedef struct tree {
	TreeNode* root;
} Tree;

/*-----------------------------------------------------------
The following are all functions needed for insertion, deletion 
and sorting of a binary search tree.  The functions are in this
order: 

1. initialize_tree - set up tree for first use
2. find_node - for finding a node for a specific value, n
3. insert_node
4. tree_minimum - returns smallest element in tree
5. successor - returns a tree node's "successor" in a sorted list
6. delete_node
7. inorder - inorder "tree walk" for sorting

The code for these functions was derived from the algorithm
pseudocode in Cormen, Leiserson and Rivest's Algorithms book,
pages 244-253
--------------------------------------------------------------*/

/*-----------------------------
Initialize a new tree as empty
------------------------------*/ 
Tree * initialize_tree(){
	Tree * t;
	t = (Tree *)malloc(sizeof(Tree));
	t->root = NULL;
	return t;
}

/*------------------------------------
Initialize new node with value n and
left and right nodes set to NULL
------------------------------------*/
TreeNode * initialize_node(int n){
	TreeNode * tn;
	tn = (TreeNode *) malloc(sizeof(TreeNode));
	tn->left = NULL;
	tn->right = NULL;
	tn->parent = NULL;
	tn->value = n;
	return tn;
}

/*------------------------------------
This function finds the node associated
with a specific value, n.  This is used
for deletion and insertion purposes and 
returns NULL if the value is not in the tree
--------------------------------------*/
TreeNode * find_node(Tree * t, int n){
	int m;
	TreeNode * tn;
	tn = t->root;
	while(TRUE){
		if (tn == NULL)
			return NULL;
		else{
			m = tn->value;
			if (n < m)
				tn = tn->left;
			else if (n > m)
				tn = tn->right;
			else
				return tn;
		}
	}
}

/*------------------------------------
Insert a new integer/node into tree, 
passing the value n and creating a new
node using initialize_node(int n) and 
inserting this new node into the tree.
Note that duplicates are not allowed
by this function.  So, if find_node()
finds that the value n is already in 
the tree, the function does nothing
else.
-------------------------------------*/
void insert_node(Tree * t, int n){
	TreeNode * x, * y, *tn;
	if ((find_node(t,n)) == NULL){
		/* Allocate memory space for the new node */
		tn = initialize_node(n);
		y = NULL;
		x = t->root;
		/* Go down the tree until x is not NULL; that way,
		we can keep track of the parent of x, which is y at
		the end of each step of the loop */
		while(x != NULL){
			y = x;
			if (tn->value < x->value)
				x = x->left;
			else
				x = x->right;
		}
		/* Now that we know which node should be the
		new node's parent (y), change the link */
		tn->parent = y;
		/* if y is NULL, then tn is the first node in the tree */
		if (y==NULL)
			t->root = tn;
		/* Decide whether tn should be the left child of y
		or the right child of y */
		else if (tn->value < y->value)
			y->left = tn;
		else
			y->right = tn;
	}
}

/*------------------------------------------------
The next few functions are devoted to deleting a 
designated node of the tree, found using find_node
function
------------------------------------------------*/

/*Find the minimum value in the tree.  It only needs 
any valid node since the minimum will always be at 
the bottom left node of the tree */
TreeNode * tree_minimum(TreeNode * tn){
	while (tn->left != NULL){
		tn = tn->left;
	}
	return tn;
}


/* For any node, find its "successor", the node whose value
would be right before it in a sorted list */
TreeNode * successor(TreeNode * tn){
	TreeNode * temp_node;
	/* if tn's right child is non-empty, then tn's
	successor is merely the smallest element in tn's
	right subtree */
	if (tn->right != NULL)
		return tree_minimum(tn->right);
	/* Otherwise, march up the tree to find tn's successor.
	Actually, since the successor code will only be used when
	tn has two children (see below in delete_node function code).
	this case will never be called upon.  But, it is important
	to include in case the code is ever needed again */
	temp_node = tn->parent;
	while ((temp_node != NULL) && (temp_node->right == tn)){
		tn = temp_node;
		temp_node = temp_node->parent;
	}
	return temp_node;

}


/* Now, actually do the deleting of the node,
using the successor and tree_minimum functions */
void delete_node(Tree * t, int n){
	TreeNode * tn, * rep_tn; /* rep_tn is tn's replacement */
	/* Despite the fact that I follow Cormen, Leiserson
	and Rivest here, I have made the code more explicit
	and readable.  There are essentially three cases:
	1. tn has no children in which case we simply extract it and
	set tn->parent to NULL.
	2. tn has one child, simply remove tn and make a link
	between tn's parent and child.
	3.  tn has two children, so we find its successor, replace
	tn's value with the successor's value and then "repair"
	the tree.  More notes are offered on this below.
	Additionally, however, check to see if tn is the root of the
	tree since we must do things slightly differently if so
	during each step of going throught the tree. */

	/* First, find if n is actually in the tree */
	tn = find_node(t,n);
	if (tn == NULL)
		puts("Integer Not in tree.  Try again.");
	else{
		/* Case 1: tn has no children */
		if ((tn->left == NULL)&&(tn->right == NULL)){
			if (tn->parent == NULL)/*tn is t->root */
				t->root = NULL;
			else{
				if (tn == (tn->parent)->right)
					(tn->parent)->right = NULL;
				else
					(tn->parent)->left = NULL;
			}
			free(tn);
		}
		/* Case 2:  tn has one child.  Check to see which
		child it has (right or left) */
		/* tn has a right child only */
		else if ((tn->left == NULL)&&(tn->right != NULL)){
			if (tn->parent == NULL){ /*tn is t->root */
				t->root = tn->right;
				(tn->right)->parent = NULL;
			}
			else{
				if (tn == (tn->parent)->left)
					(tn->parent)->left = tn->right;
				else
					(tn->parent)->right = tn->right;
				(tn->right)->parent = tn->parent;
			}
			free(tn);
		}
		/* tn has a left child only */
		else if ((tn->right == NULL)&&(tn->left != NULL)){
			if (tn->parent == NULL){ /*tn is t->root */
				t->root = tn->left;
				(tn->left)->parent = NULL;
			}
			else{
				if (tn == (tn->parent)->right)
					(tn->parent)->right = tn->left;
				else
					(tn->parent)->left = tn->left;
				(tn->left)->parent = tn->parent;
			}
			free(tn);
		}

		/* Case 3:  This is where things get the most complicated:
		If tn has two children, we need to find	its successor and 
		place the successor's value	into tn's value and then free up 
		its successor, correcting all pointers surrounding
		tn's successor.  Note that since we never look at tn->parent,
		there is no need to consider special cases if tn is the
		root of the tree, t.*/
		else{
			rep_tn = successor(tn);
			/* Again, we have multiple cases here.  rep_tn could
			have no children or one child (see below)
			So, if tn's successor has no children, do the following:*/
			if ((rep_tn->left == NULL)&&(rep_tn->right == NULL)){
				if (rep_tn == (rep_tn->parent)->right){
					(rep_tn->parent)->right = NULL;
					tn->value = rep_tn->value;
					free(rep_tn);
				}
				else if (rep_tn == (rep_tn->parent)->left){
					(rep_tn->parent)->left = NULL;
					tn->value = rep_tn->value;
					free(rep_tn);
				}
			}
			/* Otherwise, tn's successor may have a child.  In fact,
			tn's successor can have only one child.  This is due to 
			the fact that in a binary search tree, if a node has two
			children, its successor has no left child.  This is 
			something which was shown in Fundamental Algorithms and
			is discussed in Cormen, Leiserson and Rivest's Algorithms
			book, page 251.  I will not prove it here, taking it as
			fact
			*/
			else if ((rep_tn->right != NULL)&&(rep_tn->left == NULL)){
				if ((rep_tn->parent)->left == rep_tn){
					(rep_tn->parent)->left = rep_tn->right;
					(rep_tn->right)->parent = rep_tn->parent;
				}
				else if ((rep_tn->parent)->right == rep_tn){
					(rep_tn->parent)->right = rep_tn->right;
					(rep_tn->right)->parent = rep_tn->parent;
				}
				tn->value = rep_tn->value;
				free(rep_tn);
			}
		}
	}
}
/* End of the delete node section */


/*------------------------------------------
Use Inorder Tree Walk to sort integers in
increasing order and print them to the screen 
--------------------------------------------*/

void inorder(TreeNode * tn){
	if (tn != NULL){
		inorder(tn->left);
		printf("%d  ",tn->value);
		inorder(tn->right);
	}
}


/*--------------------------------------------------------
The next two functions are for use in my main() function
which tests all binary search tree functions above
---------------------------------------------------------*/

/* getchoice() sets up the menu and returns the user's choice
as an integer */
int getchoice(){
	int ch;
	int getfirst();

	puts("Enter letter of your choice:");
	puts("a. Insert value         b. Delete value");
	puts("c. Sort values          q. Quit");
	ch = getfirst();
	while ((ch < 'a' || ch > 'c') && ch != 'q'){
		puts("Please enter a, b, c or q");
		ch = getfirst();
	}
	return ch;
}

/* getfirst() is used in getchoice() to destroy the '\n' 
character */
int getfirst(){
	int ch;
	ch = getchar();
	while (getchar() != '\n')
		continue;
	return ch;
}


/* According to user's choice, insert, delete or sort */
void  main(int argc, char ** argv) { 
	Tree * t;
	int n;
	int choice;

	t = initialize_tree();

	while ((choice = getchoice()) != 'q'){
		switch (choice){
			case 'a':  puts("Type integer to insert:");
				scanf("%d", &n);
				insert_node(t,n);
				while (getchar() != '\n')
					continue;
				break;
			case 'b':
				puts("Type integer to delete:");
				scanf("%d", &n);
				delete_node(t,n);
				while (getchar() !='\n')
					continue;
				break;
			case 'c':
				puts("");
				inorder(t->root);
				puts("");
				break;
		}
	}
}
