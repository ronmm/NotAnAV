#include <linux/inet.h>
#include <linux/module.h>
#include <linux/uaccess.h>

/* 
 * Copying data from user space to kernel:
 *
 * The memory addresses of the user space and kernel space are different, and therefore some hooks might lead to crashes
 * Receiving variables by value is safe, however when we receive pointers - we have to convert them first in order to use their data
 * 
 * Available functions to use:
 * ===========================
 *
 * 1) get_user: Useful when copying "basic" data struct such as int,long,pointer,...
 *    Example:
 *    if (get_user(dst,src) == 0))
 *        // Success
 *    else
 *        // Error
 *
 * 2) copy_from_user: Useful when copying complex data struct from user space to kernel (similar to "memcpy")
 *    Can be used to copy advanced data structs, by specifying their size
 *    Example:
 *    if (copy_from_user(dst, src, bytes_to_copy) == 0)
 *        // Success
 *    else
 *        // Error
 * 
 * 3) strncpy_from_user: Useful when copying strings from user space to kernel (similar to "strncpy")
 *    In order to copy the string, you must allocate memory (dynamic/char[]) which will store the copied string
 *    Exapmle:
 *    if (strncpy_from_user(dst, src, size_of_src) > 0)
 *        // Success
 *    else
 *        // Error
 * 
 * 4) conv_string_arr (our own customized function, source code is in user2kernel.c): 
 *    Can be used to copy "char * []" or "char **" from user space
 * 
 *    As the kernel and user space contain different address range,
 *    we have to convert the addresses twice in order to get an actual string:
 *        1. Extract "char *" pointer to the strings using "get_user"
 *        2. Use strncpy_from_user to copy the string from user space range
 *
 *    In order to iterate between all strings, we have to reuse "get_user"
 *
 *    Usage:
 *    ======
 *    dst       - pre-allocated "char*" buffer (allocated in calling function - either dynamically or as a local "char[]")
 *    src       - type of "char * []" or "char **" which is located in user space
 * 
 *    Example:
 *    if (conv_string_arr(dst, src, 1024) == 0)
 *        // Success
 *    else
 *        // Error
 */

int conv_string_arr(char * dst, char __user * __user src[]);