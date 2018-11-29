#ifndef BUILTINS_H
#define BUILTINS_H

struct class_Nothing_struct;
typedef struct class_Nothing_struct* class_Nothing; 

typedef struct obj_Nothing_struct {
    class_Nothing class;
} * obj_Nothing;

struct obj_String_struct;
typedef struct obj_String_struct* obj_String;

struct obj_Boolean_struct;
typedef struct obj_Boolean_struct* obj_Boolean;

/* ==============
 * Obj 
 * ==============
 */

struct class_Obj_struct;
typedef struct class_Obj_struct* class_Obj; 

typedef struct obj_Obj_struct {
    struct class_Obj_struct *class;
} * obj_Obj;

struct class_Obj_struct {
    void *super;
    obj_Boolean (*method_EQUALS) (obj_Obj, obj_Obj);     
    obj_Nothing (*method_PRINT) (obj_Obj);
    obj_String (*method_STR) (obj_Obj);
}; 
extern struct class_Obj_struct the_class_Obj_struct;
extern class_Obj const the_class_Obj;

/* ================
 * String
 * ==================
 */

struct class_String_struct;
typedef struct class_String_struct* class_String;

typedef struct obj_String_struct {
    class_String class;
    char *text; 
} * obj_String;

struct class_String_struct {
    /* Method table: Inherited or overridden */
    class_Obj super;  /* Super Class Pointer */
    obj_Boolean (*method_EQUALS) (obj_String, obj_Obj);      /* Overidden */
    obj_Nothing (*method_PRINT) (obj_Obj);                /* Inherited */
    obj_String (*method_STR) (obj_String);               /* Overidden */
    
    /* Method table: Introduced */
    obj_Boolean (*method_ATLEAST) (obj_String, obj_String);     /* Introduced */
    obj_Boolean (*method_ATMOST) (obj_String, obj_String);     /* Introduced */
    obj_Boolean (*method_LESS) (obj_String, obj_String);     /* Introduced */
    obj_Boolean (*method_MORE) (obj_String, obj_String);     /* Introduced */
    obj_String (*method_PLUS) (obj_String, obj_String);     /* Introduced */
};

extern class_String the_class_String;

/* Construct an object from a string literal. 
 */ 
extern obj_String str_literal(char *s);

/* ================
 * Boolean
 * =================
 */

struct class_Boolean_struct;
typedef struct class_Boolean_struct* class_Boolean; 

typedef struct obj_Boolean_struct {
    class_Boolean  class;
    int value; 
} * obj_Boolean;

struct class_Boolean_struct {
    class_Obj super;      /* Super Class Pointer */
    /* Method table: Inherited or overridden */
    obj_Boolean (*method_EQUALS) (obj_Obj, obj_Obj); /* Inherited */ 
    obj_Nothing (*method_PRINT) (obj_Obj);               /* Inherited */
    obj_String (*method_STR) (obj_Boolean);       /* Overridden */
};

extern class_Boolean the_class_Boolean; 

/* There are only two instances of Boolean, 
 * lit_true and lit_false
 * (i.e., the values of the literals true and false)
 */
extern obj_Boolean lit_false;
extern obj_Boolean lit_true;


/* ==============
 * Nothing (really just a singleton Obj)
 * ==============
 */


struct class_Nothing_struct {
    class_Obj super;  /* Super Class Pointer */
    /* Method table */
    obj_Boolean (*method_EQUALS) (obj_Obj, obj_Obj); /* Inherited */
    obj_Nothing (*method_PRINT) (obj_Obj);               /* Inherited */
    obj_String (*method_STR) (obj_Nothing);       /* Overridden */
}; 

extern class_Nothing the_class_Nothing;

/* There is a single instance of Nothing, 
 * called none
 */
extern obj_Nothing lit_none;

/* ================
 * Int
 * =================
 */

struct class_Int_struct;
typedef struct class_Int_struct* class_Int; 

typedef struct obj_Int_struct {
    class_Int  class;
    int value; 
} * obj_Int;

struct class_Int_struct {
    class_Obj super;
    /* Method table: Inherited or overridden */
    obj_Boolean (*method_EQUALS) (obj_Int, obj_Obj); /* Overridden */
    obj_Nothing (*method_PRINT) (obj_Obj);      /* Inherited */
    obj_String (*method_STR) (obj_Int);  /* Overridden */
    /* Method Table: Introduced */
    obj_Boolean (*method_ATLEAST) (obj_Int, obj_Int);   /* Introduced */
    obj_Boolean (*method_ATMOST) (obj_Int, obj_Int);   /* Introduced */
    obj_Int (*method_DIVIDE) (obj_Int, obj_Int);       /* Introduced */
    obj_Boolean (*method_LESS) (obj_Int, obj_Int);   /* Introduced */
    obj_Int (*method_MINUS) (obj_Int, obj_Int);       /* Introduced */
    obj_Boolean (*method_MORE) (obj_Int, obj_Int);   /* Introduced */
    obj_Int (*method_NEGATE) (obj_Int);               /* Introduced */
    obj_Int (*method_PLUS) (obj_Int, obj_Int);       /* Introduced */
    obj_Int (*method_TIMES) (obj_Int, obj_Int);       /* Introduced */
};


extern class_Int the_class_Int; 

/* Integer literals constructor.
 */
extern obj_Int int_literal(int n);


/* ===============================
 * Make all the methods we might 
 * inherit visible to user code 
 *================================
 */
obj_Obj new_Obj();
obj_String Obj_method_STR(obj_Obj this);
obj_Nothing Obj_method_PRINT(obj_Obj this); 
obj_Boolean Obj_method_EQUALS(obj_Obj this, obj_Obj other); 

obj_String new_String();
obj_String String_method_STR(obj_String this);
obj_String String_method_PRINT(obj_String this); 
obj_Boolean String_method_EQUALS(obj_String this, obj_Obj other); 
obj_Boolean String_method_LESS(obj_String this, obj_String other); 
obj_Boolean String_method_MORE(obj_String this, obj_String other); 
obj_Boolean String_method_ATLEAST(obj_String this, obj_String other); 
obj_Boolean String_method_ATMOST(obj_String this, obj_String other); 
obj_String String_method_PLUS(obj_String this, obj_String other); 

obj_Boolean new_Boolean();
obj_String Boolean_method_STR(obj_Boolean this); 

obj_Nothing new_Nothing();
obj_String Nothing_method_STR(obj_Nothing this);

obj_Int new_Int();
obj_String Int_method_STR(obj_Int this); 
obj_Boolean Int_method_EQUALS(obj_Int this, obj_Obj other);
obj_Boolean Int_method_LESS(obj_Int this, obj_Int other);
obj_Boolean Int_method_MORE(obj_Int this, obj_Int other);
obj_Boolean Int_method_ATLEAST(obj_Int this, obj_Int other);
obj_Boolean Int_method_ATMOST(obj_Int this, obj_Int other);
obj_Int Int_method_MINUS(obj_Int this, obj_Int other);
obj_Int Int_method_DIVIDE(obj_Int this, obj_Int other);
obj_Int Int_method_TIMES(obj_Int this, obj_Int other);
obj_Int Int_method_PLUS(obj_Int this, obj_Int other);
obj_Int Int_method_NEGATE(obj_Int this);

#endif
