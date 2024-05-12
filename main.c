#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t b32;

typedef float f32;
typedef double f64;

int int_to_str_size(int num)
{
    return (int)((ceil(log10(num))+1)*sizeof(char));
}

int main(int argc, char ** args)
{
    b32 cFlag = 0; // output number of bytes in the file
    b32 lFlag = 0; // output number of lines in the file
    b32 wFlag = 0; // output number of words in the file
    b32 mFlag = 0; // output number of characters in the file
    char * filename;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++) {
            if (strcmp(args[i], "-c") == 0)
            {
                cFlag = 1;
            }
            else if (strcmp(args[i], "-l") == 0)
            {
                lFlag = 1;
            }
            else if (strcmp(args[i], "-w") == 0)
            {
                wFlag = 1;
            }
            else if (strcmp(args[i], "-m") == 0)
            {
                mFlag = 1;
            }
            else
            {
                filename = args[i];
            }
        }

        if (cFlag & lFlag & wFlag & mFlag == 0)
        {
            cFlag = 1;
            lFlag = 1;
            wFlag = 1;
        }

        if (filename != NULL)
        {

            FILE *fp = fopen(filename, "r");
            if(fp != NULL)
            {
                char out[255];
                if (cFlag == 1)
                {
                    fseek(fp, 0L, SEEK_END);
                    size_t sz = ftell(fp);
                    fseek(fp, 0L, SEEK_SET);

                    u64 cOutLen = int_to_str_size(sz);
                    char cOut[cOutLen];
                    snprintf(cOut, cOutLen, "%lu", (u64)sz);
                    strcat(out, cOut);
                    strcat(out, " ");
                }

                u64 l = 0;
                u64 w = 0;
                u64 m = 0;

                u64 wStart = 0;
                while(!feof(fp))
                {
                    u64 c = fgetc(fp);
                    printf("%c\n", c);

                    if (c == '\n')
                    {
                        l++;
                    }

                    if(isalpha(c) || c == '\'')
                    {
                        if (wStart == 0)
                        {
                            wStart = 1;
                            w++;
                        }
                    }
                    else
                    {
                        wStart = 0;
                    }

                    m++;
                }

                if (lFlag == 1)
                {
                    u64 lOutLen = int_to_str_size(l);
                    char lOut[lOutLen];
                    snprintf(lOut, lOutLen, "%lu", l);
                    strcat(out, lOut);
                    strcat(out, " ");
                }

                if (wFlag == 1)
                {
                    u64 wOutLen = int_to_str_size(w);
                    char wOut[wOutLen];
                    snprintf(wOut, wOutLen, "%lu", w);
                    strcat(out, wOut);
                    strcat(out, " ");
                }

                if (mFlag == 1)
                {
                    u64 mOutLen = int_to_str_size(m);
                    char mOut[mOutLen];
                    snprintf(mOut, mOutLen, "%lu", m);
                    strcat(out, mOut);
                    strcat(out, " ");
                }




                fprintf(stdout, "%s%s\n", out, filename);
            }
            else
            {
                fprintf(stderr, "Not able to open the file with name %s.", filename);
                return -1;
            }

            
        }
        else
        {
            fprintf(stdout, "Usage: mwc [-c] filename");
        }

    } 
    else
    {
        fprintf(stdout, "Usage: mwc [-c] filename");
    }

    return 0;
}