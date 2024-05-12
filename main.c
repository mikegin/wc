#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef uint64_t u64;
typedef int32_t b32;

int main(int argc, char ** args)
{
    b32 cFlag = 0; // output number of bytes in the file
    b32 lFlag = 0; // output number of lines in the file
    b32 wFlag = 0; // output number of words in the file
    b32 mFlag = 0; // output number of characters in the file
    char * filename = NULL;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++) {
            if (strcmp(args[i], "-c") == 0) cFlag = 1;
            else if (strcmp(args[i], "-l") == 0) lFlag = 1;
            else if (strcmp(args[i], "-w") == 0) wFlag = 1;
            else if (strcmp(args[i], "-m") == 0) mFlag = 1;
            else filename = args[i];
        }

        if (!cFlag && !lFlag && !wFlag && !mFlag)
        {
            cFlag, lFlag, wFlag = 1;
        }

        if (filename != NULL)
        {

            FILE *fp = fopen(filename, "r");
            if(fp != NULL)
            {

                char out[255] = "";
                u64 l, w, m, c = 0;
                int ch, inword = 0;

                if (cFlag) {
                    fseek(fp, 0, SEEK_END);
                    c = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    snprintf(out, sizeof(out), "%lu ", c);
                }

                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '\n') ++l;
                    if (isalpha(ch)) {
                        if (!inword) {
                            inword = 1;
                            ++w;
                        }
                    } else {
                        inword = 0;
                    }
                    ++m;
                }

                if (lFlag) snprintf(out + strlen(out), sizeof(out) - strlen(out), "%lu ", l);
                if (wFlag) snprintf(out + strlen(out), sizeof(out) - strlen(out), "%lu ", w);
                if (mFlag) snprintf(out + strlen(out), sizeof(out) - strlen(out), "%lu ", m);

                fprintf(stdout, "%s%s\n", out, filename);
            }
            else
            {
                fprintf(stderr, "Not able to open the file with name %s.\n", filename);
                return -1;
            }

            
        }
        else
        {
            fprintf(stdout, "Usage: wc [-c | -l | -w | -m ] filename");
        }

    } 
    else
    {
        fprintf(stdout, "Usage: wc [-c | -l | -w | -m ] filename");
    }

    return 0;
}