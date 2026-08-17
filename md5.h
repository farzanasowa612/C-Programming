#ifndef MD5_H
#define MD5_H

/* Hashes the given string and writes a 32-character hex hash + '\0'
   into output_buffer (must be at least 33 bytes). */
void generateMD5(const char *string, char *output_buffer);

#endif
