#include <linux/inet.h>
#include "user2kernel.h"

int conv_string_arr(char * dst, char __user * __user src[])
{

        // Get pointer to array
        char __user * argv_iter;
        if (get_user(argv_iter, src))
        {
                printk("conv_string_arr: error getting array pointer\n");
                return -1;
        }

        //printk("DEBUG: got array pointer %p\n", argv_iter);

        if (!argv_iter) {
                printk("conv_string_arr: got null pointer\n");
                return -1;
        }
        //printk("DEBUG: arr is not null\n");

        int arr_location = 0;
        char buf[256] = {0};
        long ret_val = 1;
        //long ret_val = strncpy_from_user(buf, (argv_iter), 256);
        while (ret_val > 0)
        {
                //printk("DEBUG: iteration %d\n", arr_location);
                //printk("DEBUG: got string %s\n", buf);
                if (get_user(argv_iter, src+arr_location))
                {
                        printk("conv_string_arr: error during array scan\n");
                        return -1;
                }

                strcat(dst, buf);
                strcat(dst," ");

                arr_location++;
                memset(buf, 0, sizeof(buf));
                ret_val = strncpy_from_user(buf, (argv_iter), 256);
        }
        return 0;
}
