/* ///////////////////////////////////////////////////////////////////////////

  ===============================
  rule.h - Chris Bak (23/08/2014)
  ===============================
                             
  Contains definitions for the structures necessary for rule application
  except for graphs: rules, conditions, stacks and association lists.
  
/////////////////////////////////////////////////////////////////////////// */

#ifndef INC_STRUCTURES_H
#define INC_STRUCTURES_H

#include "globals.h"
#include "graph.h"

/* The parameter list of a rule. Each variable has one of the five GP 2 types 
 * according to the rule declaration. Used in the matching algorithm to check
 * the type of a variable for label matching.
 */

typedef enum {INTEGER_VAR = 0, CHARACTER_VAR, STRING_VAR, ATOM_VAR, LIST_VAR} 
  GPType;

typedef struct VariableList {
  string variable;
  GPType type; 
  struct VariableList *next;
} VariableList;

VariableList *addVariable(VariableList *variable_list, string name, GPType type);
GPType lookupType(VariableList *variable_list, string name);
void freeVariableList(VariableList *variable_list);


/* When processing a rule's AST, two lists of index maps (one for nodes and one 
 * for edges)are maintained. They store the ID of the item, its indices in the 
 * LHS and RHS graphs, and the source and target IDs of edges. 
 *
 * The lists of index maps are used to obtain the correct source and targets 
 * when creating edges and to obtain information about edges created by the 
 * rule. */
typedef struct IndexMap {
   string id;
   int left_index;
   int right_index;
   string source_id;
   string target_id;
   struct IndexMap *next;
} IndexMap;

/* Prepends a new map with the passed information to the given list and returns
 * a pointer to the new first map in the list. */
IndexMap *addIndexMap(IndexMap *map, string id, int left_index, 
                      int right_index, string source_id, string target_id);
IndexMap *findMapFromId(IndexMap *map, string id);
/* Used to find a map for an edge with the passed source and target IDs. */
IndexMap *findMapFromSrcTgt(IndexMap *map, string source, string target);
IndexMap *removeMap(IndexMap *map, IndexMap *map_to_remove);
void freeIndexMap(IndexMap *map);

/* A simple linked list to store node indices. */
typedef struct NodeList {
   int index;
   struct NodeList *next;
} NodeList;

NodeList *addNodeItem(NodeList *node_list, int index);
void freeNodeList(NodeList *node_list);

/* A linked list of items that are preserved by the rule. It stores the 
 * indices of the item in the LHS and RHS, and a flag set to true if the rule
 * changes the item's label. */
typedef struct PreservedItem {
   int left_index;
   int right_index;
   bool label_change;
   struct PreservedItem *next;
} PreservedItem;

PreservedItem *addPreservedItem(PreservedItem *items, bool label_change,
                                int left_index, int right_index);
void freePreservedItems(PreservedItem *items);


/* A linked list of structures describing edges created by the rule. The 
 * edge's incident nodes may be preserved by the rule, in which case the
 * LHS index of the node is stored. Alternatively, the nodes could be created
 * by the rule, in which case the RHS index of the node is stored. This is
 * specified by the characters source_location and target_location. */
typedef struct NewEdgeList {
   int edge_index;
   char source_location; /* 'l' or 'r' */
   int source_index;
   char target_location; /* 'l' or 'r' */
   int target_index; 
   struct NewEdgeList *next;
} NewEdgeList;

NewEdgeList *addNewEdge(NewEdgeList *edge, int index, char source_loc, 
                        int source_index, char target_loc, char target_index);
void freeNewEdgeList(NewEdgeList *new_edge);


typedef struct Condition {
  CondExpType exp_type;		/* globals.h */
  union {
    string var; 		/* INT_CHECK, CHAR_CHECK, STRING_CHECK, 
				 * ATOM_CHECK */
    struct {
      string source; 
      string target; 
      Label *label;
    } edge_pred; 		/* EDGE_PRED */

    struct { 
      GList *left_list;
      GList *right_list; 
    } list_cmp; 		/* EQUAL, NOT_EQUAL */

    struct { 
      GList *left_exp; 
      GList *right_exp; 
    } atom_cmp; 		/* GREATER, GREATER_EQUAL, LESS, LESS_EQUAL */

    struct Condition *not_exp;  /* BOOL_NOT */

    struct { 
      struct Condition *left_exp; 
      struct Condition *right_exp; 
    } bin_exp; 			/* BOOL_OR, BOOL_AND */
  } value;
} Condition;

typedef struct Rule {
   string name; 
   VariableList *variables;
   int number_of_variables;
   Graph *lhs;
   Graph *rhs; 
   PreservedItem *preserved_nodes;
   PreservedItem *preserved_edges;
   NodeList *deleted_nodes;
   /* deleted_edges are implicit; worked out from edges not in preserved edges
    * when generating mathcing code. This is because we will need to take an 
    * edge index and search for whether it is in the deleted list (equivalently, 
    * not in the preserved items list). A search is performed either way, no 
    * point making an explicit data structure for deleted edges. */
   NodeList *added_nodes;
   NewEdgeList *added_edges;
   Condition *condition;
   struct {
      /* 1 if the rule does not change the host graph. */
      unsigned int is_predicate : 1;
      /* 1 if the rule deletes any nodes. */
      unsigned int deletes_nodes : 1;
      /* 1 if the rule is rooted. */
      unsigned int is_rooted : 1;
   } flags;
} Rule;

void printRule(Rule *rule, bool print_graphs);
void freeRule(Rule *rule);

#endif /* INC_RULE_H */
