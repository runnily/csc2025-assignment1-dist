/*
 * Replace the following string of 0s with your student number
 * 180251870
 */
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#include "obj_map.h"
#include "obj_store.h"
#include "string_o.h"

/* 
 * Internal value object management functions to create, delete and 
 * provide string representation of the underlying string value.
 */ 

/* The private _object_map that maps an String object to its char* value and
 * length.
 * Do NOT change this declaration.
 */
static omap* _object_map = NULL;

/*
 * struct definition for internal representation of a string value.
 * Do NOT change this declaration.
 */
typedef struct strobj {
    int len;        /* the length of a string value */
    char* val;      /* the value of a string */
} strobj;

/*
 * Private _new_strobj function to dynamically allocate a new string value and 
 * store it in the internal object_map with the address of the String object
 * as the key to the newly allocated value. This means that there is no 
 * direct access to the value of a String, which can only be manipulated
 * and obtained using the String member functions.
 *
 * Do NOT change this function.
 */
static strobj* _new_strobj(String self, const char* value) { //constant character cannot be changed
    strobj* sobj = NULL;
    
    if (_object_map || (_object_map = create_map())) {
        int len = strnlen(value, STR_LEN_MAX);
    
        char* val = strndup(value, len); //uses malloc 
    
        if (val) {
            sobj = (strobj*) malloc(sizeof(strobj));
        
            if (sobj && set_mentry(_object_map, self, sobj)) {
                sobj->len = len;
                sobj->val = val;
            } else {
                free(val);
                free(sobj);
                sobj = NULL;
            }
        }
    }
    
    return sobj;
}

/* The string representation of type and value for saving to file.
 * Do NOT change these declarations.
 */
static const char* STR_REP_FMT = "%d:%s\n"; /* strobj->len:strobj->val */
static const char* TYPE_STR = "str";

/*
 * Private _delete_str function deletes a string object and its internal
 * object-to-value mapping, freeing all dynamically allocated memory and 
 * storage associated with the object.
 * Do NOT change this function.
 */
void _delete_str(String* as, bool check_store) {
    if (as && *as) {
        if (check_store && ostore_is_on()) {
            object_rep obj_rep = {TYPE_STR, (uintptr_t) *as, NULL};
            unlink_obj(&obj_rep);
        }
        
        strobj* sobj = delete_mentry(_object_map, *as);
        
        if (sobj) {
            free(sobj->val);
            free(sobj);
        }
        
        memset(*as, 0, sizeof(struct string));
            // 0s string memory in case reused
            
        free(*as);
        *as = NULL;
    }
}

/* 
 * Private _store_obj_rep function.
 * This function does nothing if the object store is not enabled (and 
 * returns true for success of the function). If the object store is enabled
 * the function will attempt to store a string representation of the given
 * String to the object store. The string representation is the length of 
 * the string followed by a colon followed by the string value followed by a 
 * new line (see STR_REP_FMT).
 * Do NOT change this function.
 */
static bool _store_obj_rep(String oi, strobj* sobj) {
    if (!ostore_is_on())
        return true;
        
    bool r = false;
    char* valstr = NULL;    
    (void) asprintf(&valstr, STR_REP_FMT, sobj->len, sobj->val); //uses malloc
    
    if (valstr) {
        object_rep obj_rep = { TYPE_STR, (uintptr_t) oi, valstr };
        r = store_obj(&obj_rep);
        
        free(valstr);
    }
    
    return r;
}

/* 
 * Prototype of private _char_at function for implementation of the char_at 
 * member of struct string
 */
static char _char_at(String self, int posn);            
/* 
 * Prototype of private _concat function for implementation of the concat 
 * member of struct string
 */
static String _concat(String self, String s);     
/* 
 * Prototype of private _equals function for implementation of the equals 
 * member of struct string
 */
static bool _equals(String self, String s);             
/* 
 * Prototype of private _get_value function for implementation of the get_value 
 * member of struct string
 */
static char* _get_value(String self, char* buf);
/* 
 * Prototype of private _index_of function for implementation of the index_of 
 * member of struct string
 */
static int _index_of(String self, char c, int start);   
/* 
 * Prototype of private _length function for implementation of the length 
 * member of struct string
 */
static int _length(String self);                        
/* 
 * Prototype of private _split function for implementation of the split 
 * member of struct string
 */
static String* _split(String self, String delim); 
/* 
 * Prototype of private _substring function for implementation of the substring 
 * member of struct string
 */
static String _substring(String self, int start, int length);     

/*
 * Implementation of public functions/methods.
 * See string_o.h for specification of the following functions.
 */

/* newString: implemented, do NOT change */
String newString(const char* value) {
    if (!value) {
        errno = EINVAL;
        return NULL;
    }
    
    String self = (String) malloc(sizeof(struct string));
   
    if (self) {
        strobj* sobj = _new_strobj(self, value); //no direct access
        
        if (sobj) {
            self->concat = _concat; 
            self->char_at = _char_at;
            self->equals = _equals;
            self->get_value = _get_value;
            self->index_of = _index_of;
            self->length = _length;
            self->split = _split;
            self->substring = _substring;
        
            if (!_store_obj_rep(self, sobj))
                _delete_str(&self, false); // ostore on but storage failed
        } else {
            free(self);
            self = NULL;
        }
    }

    return self;
}

/* deleteString: implemented, do NOT change */
void deleteString(String *as) { 
    _delete_str(as, true);
}

/* printString: implemented, do NOT change */
int printString(const char* format, String s) {
    return fprintString(stdout, format, s);
}

/* fprintString: implemented, do NOT change */
int fprintString(FILE* stream, const char* format, String s) {
    strobj* sobj = (strobj*) get_mentry(_object_map, s);
    
    return sobj ? fprintf(stream, format, sobj->val) : -1;
}

/* 
 * TODO: IMPLEMENT _char_at
 * see comments to the char_at member of struct string in string_o.h for the
 * specification of this function
 */
char _char_at(String self, int posn) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self); 
    if (sobj) {
	const char *sval= sobj->val; //get value

	if ( strncmp(sval, "", 1) == 0 ) { //
		return 0;
	}
	if (posn>= 0 && posn<strnlen(sval,STR_LEN_MAX)) { //posn between boundary
		return sval[posn];
	}
	
    } 
    errno = EINVAL; 
    return 0;


}

/* 
 * TODO: IMPLEMENT _concat
 * see comments to the concat member of struct string in string_o.h for the
 * specification of this function
 */
String _concat(String self, String s) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self); 
    strobj* aobj = (strobj*) get_mentry(_object_map, s); 
    if (sobj && aobj) {
	const char *sval= sobj->val; //get value
	const char *aval= aobj->val;// get value 
	

	char buf[strnlen(sval,STR_LEN_MAX) +  strnlen(aval,STR_LEN_MAX)]; //size combined

	//added one for to indicate NUL terminator 

	strncpy(buf, sval, strnlen(sval,STR_LEN_MAX) ); //copy lhs (sval) into buffer

	buf[strnlen(sval,STR_LEN_MAX)] = '\0'; //place length here
	strncat(buf, aval, strnlen(aval,STR_LEN_MAX)); //place aval next to sval

	buf[strnlen(sval,STR_LEN_MAX)+strnlen(aval,STR_LEN_MAX)] = '\0';
	return newString(buf);
	}
    errno = EINVAL;
    return NULL;
}

/* 
 * TODO: IMPLEMENT _equals
 * see comments to the equals member of struct string in string_o.h for the
 * specification of this function
 */
bool _equals(String self, String s) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self); 
    strobj* aobj = (strobj*) get_mentry(_object_map, s);
    if (sobj && aobj) {
	const char *sval= sobj->val; //get value

	char *aval= aobj->val;// get value
	if (strncmp(sval, aval, strnlen(sval,STR_LEN_MAX)+1  ) == 0) return true;
    } 
    
    return false;	 
}

/* _get_value: implemented, do NOT change */
char* _get_value(String self, char* buf)  {
    strobj* sobj = (strobj*) get_mentry(_object_map, self); //get value
    
    if (!sobj) //if value is null
        return NULL; //then return null
        
    if (buf) { //if they provided a buffer value
	//sobj is point to val sobs is point to length function
        (void) strncpy(buf, sobj->val, sobj->len); //copy up the length val character into buffer (make void)
        buf[sobj->len] = '\0'; //added null terminator to the buffer to show  its a string
    } else {
        buf = strndup(sobj->val, sobj->len); //else create a buffer
    }
    
    return buf;
}

/* 
 * TODO: IMPLEMENT _index_of
 * see comments to the index_of member of struct string in string_o.h for the
 * specification of this function
 */
int _index_of(String self, char c, int start) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self);
    if (sobj)  { //check if not null and within valid
	const char *sval= sobj->val; //get value
	

	if (start>=0 && start<strnlen(sval,STR_LEN_MAX) ) { //check of start

		for (int i = start; i<strnlen(sval,STR_LEN_MAX); i++) { //loop from start position
			if (c == sval[i]) { //if char is equal return I
				return i;

			}
		} 
		return -1; //when we didn't find value in for loop return -1
	}

    }
    errno =  EINVAL;
    return -2;
}

/* _length: implemented, do NOT change */
int _length(String self) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self);
    
    return sobj ? sobj->len : -1;
}

/* 
 * TODO: IMPLEMENT _split
 * see comments to the split member of struct string in string_o.h for the
 * specification of this function
 */
String* _split(String self, String delim) {
     strobj* sobj = (strobj*) get_mentry(_object_map, self); //pointing to struct in map
     strobj* dobj = (strobj*) get_mentry(_object_map, delim);
	
     
     if (sobj && dobj) {

	const char *sval=  sobj->val; //get val of string
	char *ref = strndup(sval , STR_LEN_MAX); //duplicate so value would not change
	const char *dval=  dobj->val; //get value of delimiter
	
       	
	String *entries = (String*) calloc(strnlen(ref, STR_LEN_MAX)+2, sizeof(String)); //Assigns the max amount of string

        /*....FAILED ALLOCATION...*/
	if (entries == NULL) { 
		errno = EINVAL;
        	return NULL;
   	} 
	
	const char *found = NULL;
	
	int i = 0; 
        /*loop around the tokens in found until it reaches null*/
        while( (found = strsep(&ref,dval)) != NULL ) {
		entries[i] = newString(found);
		i+=1;
	}
	

	free(ref);
	return entries;

     }
     errno = EINVAL; 
     return NULL;

}

/* 
 * TODO: IMPLEMENT _substring
 * see comments to the substring member of struct string in string_o.h for the
 * specification of this function
 */
String _substring(String self, int start, int length) {
    strobj* sobj = (strobj*) get_mentry(_object_map, self);
    if (sobj) {
	const char *sval=  sobj->val; //get val of string

	int lenRef = strnlen(sval, STR_LEN_MAX);
	if ( (start >= 0 && start <= lenRef) && (length>= 0 &&  length<=lenRef-start) ) {
		
		char buf[STR_LEN_MAX]; //creates buffer
		
		
		if (length == 0) { //when length is 0 just return empty string
			buf[0] = '\0';
			return newString(buf);
		}

		for (int i = 0; i<length; i++) { //creates substring
			buf[i] = sval[i+start];
		
		} 
		
		
		char str[STR_LEN_MAX]; //creates a new string 
		strncpy(str, buf , length); //copy only valid values
		str[length] = '\0'; //add the NUL-Terminator
		return newString(str);
	
	} 
	
    } 
    errno = EINVAL;
    return NULL;
}

/*  
 * Access to string value to simplify tests - this would be removed in 
 * production release of String. Do NOT call this function in any of the 
 * other functions of this file.
 * Do NOT change.
 */
const char* _test_string_val(String s) {
    strobj* sobj = (strobj*) get_mentry(_object_map, s);
    
    return sobj ? sobj->val : NULL;
}

