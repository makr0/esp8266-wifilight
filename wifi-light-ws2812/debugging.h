
#ifdef DEBUG
 #define debug_print(x, ...) Serial.print (x, ## __VA_ARGS__)
 #define debug_println(x, ...) Serial.println (x, ## __VA_ARGS__)
#else
 #define debug_print(x, ...)
 #define debug_println(x, ...)
#endif
