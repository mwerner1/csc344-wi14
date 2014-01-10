//
//  soundjumble.c
//  project1
//
//  Created by Matthew Werner on 1/8/14.
//  Copyright (c) 2014 Cal Poly. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sox.h"

#define SONG_LENGTH 30

/* This function returns a buffer that is large enough to store entire 30 
   second wav file */
sox_sample_t *createBuffer(sox_format_t *file)
{
    return (sox_sample_t *) malloc((sizeof(sox_sample_t)) *
                                   (SONG_LENGTH * file->signal.rate *
                                    file->signal.channels));
}

/* Generate random number between 0 (inclusive) and n-1 */
int randNum(int n)
{
    int lim, rnd;
    
    lim = RAND_MAX - RAND_MAX % n;
    
    do {
        rnd = rand();
    } while (rnd >= lim);
    
    return rnd % n;
}

/* Swap numbers in array around */
void shuffleArr(unsigned int *array, int n)
{
    int i, j, tmp;
    
    for (i=n-1; i>0; i--)
    {
        j = randNum(i + 1);
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

int main(int argc, const char * argv[])
{
    static sox_format_t *in, *out;
    sox_sample_t *buffer;
    size_t samples;
    unsigned int timeChunks[SONG_LENGTH];
    int i;
    
    /* Verify that a sound filename is given */
    if (argc != 3)
    {
        fprintf(stderr, "Incorrect number of commandline args!\n");
        return -1;
    }
    
    /* Always have to initialize Sox first.  Check to make sure init 
       is successful */
    if (sox_init() != SOX_SUCCESS)
    {
        fprintf(stderr, "Error initializing Sox!\n");
        return -1;
    }
    
    /* Open the input file for reading */
    in = sox_open_read(argv[1], NULL, NULL, NULL);
    
    /* Exit failure if open unsuccessful */
    if (in == NULL)
    {
        fprintf(stderr, "Error opening %s!\n", argv[1]);
        return -1;
    }
    
    /* Open the output file for writing */
    out = sox_open_write(argv[2], &in->signal, NULL, NULL, NULL, NULL);
    
    /* Exit failure if open unsuccessful */
    if (out == NULL)
    {
        fprintf(stderr, "Error opening %s!\n", argv[1]);
        return -1;
    }
    
    /* The number of samples */
    samples = SONG_LENGTH * in->signal.rate * in->signal.channels;
    
    buffer = createBuffer(in);
    
    /* Read from infile and output error if number of samples
       read is incorrect */
    if (sox_read(in, buffer, samples) != samples)
    {
        fprintf(stderr, "Incorrect number of samples read!\n");
        return -1;
    }
    
    /* Initialize time chunk array */
    for (i=0; i<SONG_LENGTH; i++)
    {
        timeChunks[i] = (unsigned int)i;
    }
    
    /* Set seed */
    srand(time(NULL));
    
    /* Mix order of array of 1 second time chunks */
    shuffleArr(timeChunks, SONG_LENGTH);
    
    for (i=0; i<SONG_LENGTH; i++)
    {
        sox_write(out,
                  buffer + (timeChunks[i] * (((unsigned int)(in->signal.rate) * in->signal.channels))),
                  in->signal.rate * in->signal.channels);
    }
    
    free(buffer);
    
    /* Don't forget to close files and quit */
    sox_close(in);
    sox_close(out);
    
    sox_quit();
    
    return 0;
}

