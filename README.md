# csc2025-assignment1-dist
CSC2025 Assignment 1 - C programming, abstraction and use of system library functions

Due: 15:00 on Monday 18 November 2019

Maximum mark: 10

This assignment is worth 10% of the overall module mark, 50% of the coursework component.

Contents

Introduction
Aim
Learning/skills outcomes
What you are given
What you have to do 
5.1 Integer - an overflow detecting integer 
5.2 String - a safe string 
5.3 Object store - persistent storage for objects 
5.4 How to work - programming to test
C/POSIX library functions to investigate
Breakdown of marks
Deadline for submission and what to submit
1. Introduction

In this assignment you will complete three libraries that implement higher level abstractions of Integer and String "objects" and a simple object store that persists objects to file during program execution.

You will see how abstraction and indirection can be used to enforce safer programmer practice and provide additional facilities, in particular:

to prevent positive and negative overflow of arithmetic operations,
to keep operations on strings within length bounds,
to transparently provide additional facilities such as persistent object storage, and
to provide data encapsulation and immutability by using an object map to prevent direct access to the values of objects.
You will also see how a lower level language such as C can be used to provide higher level abstractions similar to those found in object oriented languages such as Java and Python and, thereby, gain more insight into those abstractions, their implementation and their relationship to lower levels. The use of an object map, for example, is one mechanism in a higher level programming environment to prevent direct access to data in memory.

You will also gain practice in C programming and, for example, in the use of:

C library functions for string manipulation,
the POSIX system interface to dynamic allocation and deallocation of memory
the POSIX system interface to the file system, and
C programming features such as structs, pointers and pointers to functions.
Important notes

The coursework specification includes:
this document
the integer, string and obj_store source code provided and function specifications in comments in header files
the suite of tests for each library that also demonstrate usage of libraries
the example apps for additional example usage of the libraries
the README.txt file that provides additional instructions, advice and rules for this assignment
You are expected to familiarise yourself with all this material. Advice and instructions, including things you must do, may be in this document or in comments in source code (.c or .h files) or in the README.txt file. Ignoring such advice and instruction may lead to loss of marks.
In addition, there is an implementation of a hashmap/table (in obj_map.h and obj_map.c) for object to value mappings, there are shell scripts to run tests, and there is a Makefile for compilation of code (see README.txt).
There are references to the "user" in the materials provided. This means a programmer (possibly yourself) who uses/calls a library function in their program. The library function may be from a system library, may be one of the functions provided for you in the integer, string_o, obj_map or obj_store libraries or may be one of the functions that you have to implement. In all cases, the user of the function is the programmer including a call to the function in their code (as opposed to an end user of an application).
You are completing the implementation of libraries not an application program. The example apps and tests are the applications that use those libraries. There is no need to write any other application programs.
Library functions do not produce print out. There should not be any printf or perror or other such statements in the implementations you submit.
A good starting point is to compile and run the test application scripts that you are given as part of this assignment - see instructions in README.txt.
You can complete this assignment on any POSIX-compliant system. For example, on your Minix VM, on Mac OSX or on Linux. We will test your solution using Minix 3.2.1. Therefore, whichever system you choose for development, you should at least test your solution on Minix.
2. Aim

The aim of this assignment is for you to:

practice C programming
practice programming to test
use the C library and POSIX system interface
analyse and modify program code
gain insight into definition of abstractions and their implementation
3. Learning/skills outcomes

C programming skills
Programming using a system interface
Implementing a specified interface
Programming to test
Code comprehension skills - understanding, analysing, using and modifying code provided by others
Formulate problems and identify suitable approaches to solving them
Cognitive/intellectual skills and self management (see graduate skills framework)
4. What you are given

You are given initial source code for your solution in the zip archive csc2025-assignment1-dist.zip. It contains the following directories and files:

    csc2025-assignment1-dist/
        Makefile                - the compilation script
        README-test-output.txt  - example successful output from tests
        README.txt              - additional information and instructions
        integer.c               - implementation of Integer
        integer.h               - declaration/interface of Integer
        integer_tests.sh        - script to run Integer tests
        obj_map.c               - implementation of the map of objects to their values
        obj_map.h               - declaration/interface of the object map
        obj_map_tests.sh        - script to run object map tests
        obj_store.c             - implementation of object store
        obj_store.h             - declaration/interface of object store
        obj_store_tests.sh      - script to run object store tests
        string_o.c              - implementation of String
        string_o.h              - declaration/interface of String
        string_tests.sh         - script to run String tests
        test_src/               - test source directory 
             integer_app.c      - application with example usage of Integer
             obj_map_app.c      - application with example usage of object map
             string_app.c       - application with example usage of String         
             strtest_lib.c      - library source for String tests
             strtest_lib.h      - library header for String tests 
             test_integer.c     - Integer test program
             test_lib.c         - library source for tests
             test_lib.h         - library header for tests
             test_obj_map.c     - object map test program
             test_obj_store.c   - object store test program
             test_string.c      - String test program
The above source code is the starting point for your solution and also forms part of the coursework specification, particularly the function specifications in integer.h, string_o.h and obj_store.h. The files you edit are hightlighted.

5. What you have to do

You will modify functions in integer.c, string_o.c and obj_store.c. The functions must be implemented according to:

their specification in integer.h, string_o.h and obj_store.h, respectively, and
the information in this coursework specification, and
the information in README.txt.
The source files integer.c, string_o.c and obj_store.c include TODO comments to indicate which parts you are expected to change. In some cases, there are also hints in the files to help you implement your solution.

Do not modify any files or functions other than those specified.

5.1 Integer - an overflow detecting integer (2 marks)

The integer library provides the abstraction of an Integer with detection of positive or negative overflow of a set of common arithmetic operations. It also demonstrates encapsulation and immutability of the internal data value of an Integer through the indirection of an object hashmap.

All integers are dynamically allocated. The result of any arithmetic operation is an Integer pointer to a new instance of the struct integer defined in integer.h (or NULL if the operation fails). One of the reasons for failure is integer overflow. That is, the result of an operation would be a real positive value greater than the maximum representable value or a real negative value less than the minimum representable value.

Integer objects are not modified by the arithmetic operations. That is, instances are immutable. Each operation creates a new instance. Values to operate on in the functions you implement are retrieved from the integer object map (e.g see implementations of _get_value and _multiply).

You must provide the add, subtract, divide and modulo operations of the overflow detecting integer. You do this by completing the implementation of the following functions in integer.c:

    Integer _add(Integer self, Integer i);
    Integer _subtract(Integer self, Integer i);
    Integer _divide(Integer self, Integer i);
    Integer _modulo(Integer self, Integer i);
according to their specification in integer.h and taking account of any additional information and instructions in this specification, in README.txt and in integer.c.

The function signatures are part of their specification and must not be changed. Functions for creating a new integer, deleting an integer, multiplication, and getting the int value have already been implemented and must not be changed. Helper functions that are useful for your implementation of _add, _subtract, _divide and _modulo are also provided.

Example usage of Integer

    #include "stdio.h"
    #include "integer.h"
    
    int main(void) {
        Integer i = newInteger(1);      // creates new integer struct value 1
        Integer j = newInteger(2);      // creates new integer struct value 1
        
        Integer k = i->add(i, j);
        
        printf("%d + %d = %d\n", i->get_value(i), j->get_value(j), k->get_value(k));
                                        // prints 1 + 2 = 3
                                        
        /* deallocate integers */
        deleteInteger(&i);
        deleteInteger(&j);
        deleteInteger(&k);    
    }
The preceding example assumes that calls to newInteger and add all succeed and return a pointer to a new integer struct (Integer). See integer_app.c and test_integer.c for other examples, including what happens on integer overflow.

5.2 String - a safe string (6 marks)

The string_o library provides the abstraction of a String with safe operations that are checked for length bounds (e.g. to prevent indexing out of the valid range) and that have a maximum length (STR_LEN_MAX) to prevent the creation of arbitrarily long strings. Imposing these constraints can be useful because, for example, unsafe use of strings is a common cause of memory access errors and can be exploited in buffer overflow attacks. The library also demonstrates encapsulation and immutability of the internal data value of a String through the indirection of an object hashmap.

All Strings are dynamically allocated. The result of an operation that returns a String pointer is a new instance of the struct string defined in string_o.h (or NULL if the operation fails). Other operations access the String's internal C-string.

String objects are not modified by the operations. That is, instances are immutable. Values to operate on in the functions you implement are retrieved from the string object map (e.g. see implementations of _get_value and _length).

You must provide the following operations of a safe String:

    String _concat(String self, String s);
    char _char_at(String self, int posn);  
    bool _equals(String self, String s);
    int _index_of(String self, char c, int start);
    String* _split(String self, String delim);
    String _substring(String self, int start, int length);
according to their specification in string_o.h and taking account of any additional information and instructions in this specification, in README.txt and in string_o.c.

The function signatures are part of their specification and must not be changed.

Example usage of String

    #include "stdio.h"
    #include "string_o.h"
    
    int main(void) {
        String s = newString("hello");      /* create new string structs with 
        String t = newString(", ");          * internal C-string values:
        String u = newString("goodbye");     * "hello", ", " and  "goodbye"
                                             *
        String v = s->concat(s, t);          * v will have value "hello, " 
        String w = v->concat(v, u);          * w will have value 
                                             * "hello, goodbye"
                                             */       
        printf("length of s: %d\n", s->length(s));          // prints 5
        
        printf("2nd char in s: %c\n", s->char_at(s, 1));    // prints 'e'
        
        int i = u->index_of(u, 'o', 0);     /* i will be 1, the position of 
                                             * first 'o' in "goodbye"
                                             */
        int j = u->index_of(u, 'o', 7);     /* j will be -1 and errno will be
                                             * EINVAL because 7 is out of 
                                             * valid range
                                             */
                                        
        /* deallocate strings */
        deleteString(&s);
        deleteString(&t);
        deleteString(&u);
        deleteString(&v);
        deleteString(&w);
    }
The preceding example assumes that calls to newString and concat succeed and return a pointer to a new string struct (String). See string_app.c and test_string.c for other examples.

5.3 Object store - persistent storage for objects (2 marks)

The object store library provides operations to persist objects to file during program execution. This could be useful, for example, to enable tracing of the state of an executing application or to recover application state after failure. The primary purpose of the library in this assignment is to demonstrate that once we have abstractions such as Integer and String, the indirection required to implement those abstractions allows us to transparently provide additional useful facilities. This is similar to the work "behind the scenes" that Java libraries and the JVM can do once we have the abstractions of classes and objects.

The implementations of newInteger and deleteInteger in integer.c and newString and deleteString in string_o.c show how object store operations can be used to persist the significant information about an object to an object store file at creation and delete/unlink the file from the store at deletion. Object files are stored in type sub-directories of a directory called ostore. An object's file name is the hexadecimal value of the object's memory address plus the ".txt" extension. The content of the file is a value string representation of the object's state.

The type name, id (memory address) and value string (valstr) of an object are the fields of an object_rep struct that is passed to the object store's storage function.

You must provide the object store's storage operation. You do this by completing the implementation of the following function in obj_store.c:

    bool store_obj(object_rep* obj_rep);
according to its specification in obj_store.h and taking account of any additional information and instructions in this specification, in README.txt and in store_obj.c.

The function signature is part of its specification and must not be changed. All other operations of the object store have already been implemented and must not be changed.

Example usage of the object store

You can see the use of object store functions in integer.c, string_o.c and in test_obj_store.c. Use in an application to switch on object storage would be as follows.

    #include "stdio.h"
    #include "integer.h"
    #include "obj_store.h"
    
    int main(void) {
        enable_ostore();             // switch on the object store
        
        Integer i = newInteger(1);  // create new integer i and persist to file
        Integer j = newInteger(2);  // create new integer j and persist to file
        
        ...
        ...
                                        
        deleteInteger(&i);          // delete i and its file
        deleteInteger(&j);          // delete j and its file
    }
The preceding example assumes that calls to newInteger succeed and return a new integer struct. As part of successful object creation, if the object store is enabled, the new integer's state is written to a file. See test_obj_store.c for other examples. See obj_store.h for the object store interface/functions.

5.4 How to work - programming to test

You should program to the tests provided. README.txt gives you advice on how to do this and includes instructions on compiling and running test programs. The file also gives an example of output on test failure. README-test-output.txt gives the full expected output for successful completion of tests.

6. C/POSIX library functions to investigate

You should investigate use of the following system library functions for your solution.

 	man section	header files
filesystem	 	 
open	2	fcntl.h, sys/types.h
close, unlink, write	2	unistd.h
mkdir, stat, ISDIR	2	sys/stat.h (but look at object store helper functions)
memory allocation	 	 
malloc, calloc, free	3	stdlib.h
string manipulation	 	 
strlen, strnlen, strcat, strncat
strcmp, strncmp, strcpy, strncpy, strsep, strtok	3	string.h
The fact that a library function is listed above is not an instruction to use it. It is a suggestion that you should investigate its use and decide whether you need to use it in your solution. You should consider its use in the context of your understanding of the code base that has been given to you.

There may be others library functions that you need and/or decide to use.

Note: README.txt specifies additional rules for the use of library functions to implement the String and object store libraries. You must follow the additional rules in README.txt.

Read the man pages to work out how to use a system library function.

Most of the system library man pages are accessible within Minix. You can list a man page for a function using the man section number and the name of the function. For example, type the command:

    # man 3 malloc
at the Minix terminal prompt to see the man page for malloc.

You can also access Minix man pages on the Web at: http://man.minix3.org/cgi-bin/man.cgi?manpath=Minix+3.2.1.

You will also find example usage of many of the above system library functions in the source code for tests and in the implementation of existing functions of the integer, string and object store libraries that you have been given.

7. Breakdown of marks

The indicative breakdown of marks for each part of the assignment is as follows:

Integer - 2 marks
String - 6 marks
Object store - 2 marks
For full marks your solution must:

pass all normal case and error case unit tests in test_integer.c, test_string.c and test_obj_store.c, and
comply with all instructions and rules given in this specification and related material (including in relevant .h files, .c files and README.txt), and
demonstrate good programming practice particularly with respect to non-functional aspects (not overcomplicating solutions, guarding against memory leaks, safe use of library functions, programming defensively with respect to parameters to functions you implement etc.)
8. Deadline for submission and what to submit

Your coursework must be submitted to the NESS system by 15:00 on Monday 18 November 2019. The deadline is rigorously imposed by NESS. Work that is a few seconds beyond the submission deadline will be flagged as late.

You must submit the following three files:

integer.c
obj_store.c
string_o.c
Submit a tgz or zip archive of a directory called csc2025-assignment1 that contains the three files. That is, the archive should have the following contents:

    csc2025-assignment1/
        integer.c
        obj_store.c
        string_o.c
Do not submit any other files. Anything other than the three files listed above will be ignored. Submitting your files in the csc2025-assignment1 directory means that they can be tested automatically by our markers.

Each file should start with the following comment:

/*
 * Replace the following string of 0s with your student number
 * 000000000
 */
where, as instructed, you replace 000000000 with your student number.


