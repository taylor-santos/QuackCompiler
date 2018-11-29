/* 
 * The built-in classes of Quack 
 * 
 */
#include <stdio.h>   
#include <stdlib.h>
#include <string.h>

#include "include/Builtins.h"


/* ==============
 * Obj 
 * ==============
 */

const class_Obj the_class_Obj;

/* Constructor */
obj_Obj new_Obj(  ) {
    obj_Obj new_thing = (obj_Obj) malloc(sizeof(struct obj_Obj_struct));
    new_thing->class = the_class_Obj;
    return new_thing; 
}

/* Obj:STR */
obj_String Obj_method_STR(obj_Obj this) {
    long addr = (long) this;
    char rep[64];
    sprintf(rep, "<Object at %ld>", addr);
    obj_String str = str_literal(rep); 
    return str;
}

/* Obj:PRINT */
obj_Nothing Obj_method_PRINT(obj_Obj this) {
     obj_String str = this->class->method_STR(this);
    fprintf(stdout, "%s", str->text);
    return lit_none;
}

/* Obj:EQUALS (Note we may want to replace this */
obj_Boolean Obj_method_EQUALS(obj_Obj this, obj_Obj other) {
    if (this == other) {
        return lit_true;
    } else {
        return lit_false;
    }
}
  

/* The Obj Class (a singleton) */
struct  class_Obj_struct  the_class_Obj_struct = {
    NULL,        /* Super */
    Obj_method_EQUALS,
    Obj_method_PRINT, 
    Obj_method_STR 
};

const class_Obj the_class_Obj = &the_class_Obj_struct; 

 
/* ================
 * String
 * ==================
 */

/* Constructor */
obj_String new_String(  ) {
    obj_String new_thing = (obj_String) malloc(sizeof(struct obj_String_struct));
    new_thing->class = the_class_String;
    new_thing->text = "";
    return new_thing; 
}

/* String:STR */
obj_String String_method_STR(obj_String this) {
    return this;
}

  
/* String:EQUALS (Note we may want to replace this */
obj_Boolean String_method_EQUALS(obj_String this, obj_Obj other) {
    obj_String other_str = (obj_String) other;
    /* But is it really? */
    if (other_str->class != the_class_String) {
        return lit_false;
    }
    if (strcmp(this->text,other_str->text) == 0) {
        return lit_true;
    } else {
        return lit_false;
    }
}

/* String:LESS */
obj_Boolean String_method_LESS(obj_String this, obj_String other) {
    if (strcmp(this->text, other->text) < 0) {
        return lit_true;
    } else {
        return lit_false;
    }
}

/* String:MORE */
obj_Boolean String_method_MORE(obj_String this, obj_String other) {
    if (strcmp(this->text, other->text) > 0) {
        return lit_true;
    } else {
        return lit_false;
    }
}

/* String:ATMOST */
obj_Boolean String_method_ATMOST(obj_String this, obj_String other) {
    if (strcmp(this->text, other->text) <= 0) {
        return lit_true;
    } else {
        return lit_false;
    }
}

/* String:ATLEAST */
obj_Boolean String_method_ATLEAST(obj_String this, obj_String other) {
    if (strcmp(this->text, other->text) >= 0) {
        return lit_true;
    } else {
        return lit_false;
    }
}

/* String:PLUS */
obj_String String_method_PLUS(obj_String this, obj_String other) {
    int new_str_len = strlen(this->text) + strlen(other->text) + 1;
    char *new_str = (char *)malloc(new_str_len);
    new_str = strcpy(new_str, this->text);
    strcat(new_str, other->text);
    
    return str_literal(new_str);
}

/* The String Class (a singleton) */
struct  class_String_struct  the_class_String_struct = {
    the_class_Obj,  /* Super */
    String_method_EQUALS,
    Obj_method_PRINT, 
    String_method_STR,
    String_method_ATLEAST,
    String_method_ATMOST,
    String_method_LESS,
    String_method_MORE,
    String_method_PLUS
};

class_String the_class_String = &the_class_String_struct; 

/* 
 * Internal use function for creating String objects
 * from char*.
 */
obj_String str_literal(char *s) {
  obj_String str = new_String(); 
  str->text = strdup(s);
  return str;
}

/* ================
 * Boolean
 * =================
 */
/* Constructor */
obj_Boolean new_Boolean(  ) {
    return lit_false;
}

/* Boolean:STR */
obj_String Boolean_method_STR(obj_Boolean this) {
  if (this == lit_true) {
    return str_literal("true");
  } else if (this == lit_false) {
    return str_literal("false");
  } else {
    return str_literal("!!!BOGUS BOOLEAN");
  }
}


/* The Boolean Class (a singleton) */
struct  class_Boolean_struct  the_class_Boolean_struct = {
    the_class_Obj,  /* Super */
    Obj_method_EQUALS,
    Obj_method_PRINT, 
    Boolean_method_STR
};

class_Boolean the_class_Boolean = &the_class_Boolean_struct; 
  
/* 
 * These are the only two objects of type Boolean that 
 * should ever exist.
 */ 
struct obj_Boolean_struct lit_false_struct =
    { &the_class_Boolean_struct, 0 };
obj_Boolean lit_false = &lit_false_struct;
struct obj_Boolean_struct lit_true_struct =
    { &the_class_Boolean_struct, 1 };
obj_Boolean lit_true = &lit_true_struct;

/* ==============
 * Nothing (really just a singleton Obj)
 * ==============
 */

/*  Constructor */
obj_Nothing new_Nothing(  ) {
  return lit_none; 
}

/* Nothing:STR */
obj_String Nothing_method_STR(obj_Nothing this) {
    return str_literal("<nothing>");
}

/* The Nothing Class (a singleton) */
struct  class_Nothing_struct  the_class_Nothing_struct = {
    the_class_Obj,  /* Super */
    Obj_method_EQUALS,
    Obj_method_PRINT, 
    Nothing_method_STR 
};

class_Nothing the_class_Nothing = &the_class_Nothing_struct; 
  
/* 
 * This is the only instance of class Nothing that 
 * should ever exist
 */ 
struct obj_Nothing_struct none_struct =
  { &the_class_Nothing_struct };
obj_Nothing lit_none = &none_struct; 

/* ================
 * Int
 * =================
 */

/* Constructor */
obj_Int new_Int(  ) {
    obj_Int new_thing = (obj_Int)
        malloc(sizeof(struct obj_Int_struct));
    new_thing->class = the_class_Int;
    new_thing->value = 0;          
    return new_thing; 
}

/* Int:STR */
obj_String Int_method_STR(obj_Int this) {
    char rep[16];
    sprintf(rep, "%d", this->value);
    return str_literal(rep); 
}

/* Int:EQUALS */
obj_Boolean Int_method_EQUALS(obj_Int this, obj_Obj other) {
    obj_Int other_int = (obj_Int) other; 
    /* But is it? */
    if (other_int->class != this->class) {
        return lit_false;
    }
    if (this->value != other_int->value) {
        return lit_false;
    }
    return lit_true;
}

/* Inherit Obj:PRINT, which will call Int:STR */

/* LESS (new method) */ 
obj_Boolean Int_method_LESS(obj_Int this, obj_Int other) {
    if (this->value < other->value) {
        return lit_true;
    }
    return lit_false;
}

/* MORE (new method) */
obj_Boolean Int_method_MORE(obj_Int this, obj_Int other) {
    if (this->value > other->value) {
        return lit_true;
    }
    return lit_false;
}

/* ATLEAST (new method) */
obj_Boolean Int_method_ATLEAST(obj_Int this, obj_Int other) {
    if (this->value >= other->value) {
        return lit_true;
    }
    return lit_false;
}

/* ATMOST (new method) */
obj_Boolean Int_method_ATMOST(obj_Int this, obj_Int other) {
     if (this->value <= other->value) {
        return lit_true;
    }
    return lit_false;
}

/* PLUS (new method) */
obj_Int Int_method_PLUS(obj_Int this, obj_Int other) {
    return int_literal(this->value + other->value);
}

/* MINUS (new method) */
obj_Int Int_method_MINUS(obj_Int this, obj_Int other) {
    return int_literal(this->value - other->value);
}

/* TIMES (new method) */
obj_Int Int_method_TIMES(obj_Int this, obj_Int other) {
    return int_literal(this->value * other->value);
}

/* DIVIDE (new method) */
obj_Int Int_method_DIVIDE(obj_Int this, obj_Int other) {
    return int_literal(this->value / other->value);
}

/* NEGATE (new method) */
obj_Int Int_method_NEGATE(obj_Int this) {
    return int_literal(-(this->value));
}

/* The Int Class (a singleton) */
struct  class_Int_struct  the_class_Int_struct = {
    the_class_Obj,  /* Super */
    Int_method_EQUALS,
    Obj_method_PRINT, 
    Int_method_STR, 
    Int_method_ATLEAST,
    Int_method_ATMOST,
    Int_method_DIVIDE,
    Int_method_LESS,
    Int_method_MINUS,
    Int_method_MORE,
    Int_method_NEGATE,
    Int_method_PLUS,
    Int_method_TIMES
};

class_Int the_class_Int = &the_class_Int_struct; 
  
/* Integer literals constructor
 */
obj_Int int_literal(int n) {
    obj_Int boxed = new_Int();
    boxed->value = n;
    return boxed;
}

