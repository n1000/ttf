/*
 * Copyright (c) 2009,2022 Nathaniel Houghton <nathan@brainwerk.org>
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose with or without fee is hereby granted, provided that
 * the above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define AMP 1000
#define NUM_ROW 9
#define NUM_COL 5

struct options {
    double duration;
    double low_freq;
    double high_freq;
    double freq_gap;
    double char_gap;
    double f_s;
};

void output_char(struct options *, char);
void output_gap(struct options *opt);
void usage(void);

static uint8_t char_map[][NUM_ROW][NUM_COL] = {
    {
      { 0, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 }
    },
    {
      { 1, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 0 },
      { 1, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 0 }
    },
    {
      { 0, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 1, 1 },
      { 0, 1, 1, 1, 0 }
    },
    {
      { 1, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 0 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 0, 1, 1, 1, 1 },
      { 0, 0, 0, 1, 0 },
      { 0, 0, 0, 1, 0 },
      { 0, 0, 0, 1, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 1, 0 },
      { 0, 1, 1, 0, 0 }
    },
    {
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 0 },
      { 1, 1, 1, 0, 0 },
      { 1, 1, 1, 0, 0 },
      { 1, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 0, 1 }
    },
    {
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 1, 0, 0, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 0, 1 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 0, 0 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 1 },
      { 0, 0, 0, 1, 0 },
      { 0, 0, 0, 0, 1 }
    },
    {
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 0, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 0, 1 }
    },
    {
      { 0, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 1, 1, 1, 0 },
      { 0, 1, 1, 1, 1 },
      { 0, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 0 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 1, 1, 0 },
      { 0, 1, 1, 1, 0 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 0, 1, 0 },
      { 0, 1, 0, 1, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 0, 0, 1 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 0, 1, 0 },
      { 0, 1, 1, 1, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 1, 1, 1, 0 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 0, 0, 1 }
    },
    {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 0, 1, 0 },
      { 0, 1, 1, 1, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 }
    },
    {
      { 1, 1, 1, 1, 1 },
      { 0, 0, 0, 1, 1 },
      { 0, 0, 0, 1, 1 },
      { 0, 0, 1, 1, 0 },
      { 0, 1, 1, 0, 0 },
      { 0, 1, 0, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 1, 1, 1, 1 }
    },
    {
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    }
};

int
main(int argc, char **argv)
{
    struct options opt;
    char *str;
    int ch;

    opt.f_s = 44100;
    opt.low_freq = 400;
    opt.high_freq = 1100;
    opt.char_gap = 0.1;
    opt.duration = 0.4;

    while ((ch = getopt(argc, argv, "l:h:r:d:g:")) != -1) {
        switch (ch) {
        case 'l':
            opt.low_freq = atof(optarg);
            break;
        case 'h':
            opt.high_freq = atof(optarg);
            break;
        case 'r':
            opt.f_s = atof(optarg);
            break;
        case 'd':
            opt.duration = atof(optarg);
            break;
        case 'g':
            opt.char_gap = atof(optarg);
            break;
        default:
            usage();
            /* NOTREACHED */
        }
    }
    argc -= optind;
    argv += optind;

    if (!*argv)
        usage();

    str = *argv;

    opt.freq_gap = (opt.high_freq - opt.low_freq) / NUM_ROW;

    while (*str) {
        output_char(&opt, *str);
        output_gap(&opt);
        ++str;
    }

    return 0;
}

void
output_gap(struct options *opt)
{
    int n;

    for (n = 0; n < opt->f_s * opt->char_gap; ++n) {
        int16_t val = 0;

        size_t r = fwrite(&val, sizeof(val), 1, stdout);
        if (r != 1) {
            fprintf(stderr, "error: failed to write to disk :(\n");
            exit(1);
        }
    }
}


void
output_char(struct options *opt, char c)
{
    uint8_t ind;
    int n;
    int16_t val;
    double freq;
    int cur_col;
    int i;

    c = tolower(c);

    if (c >= 'a' && c <= 'z') {
        ind = c - 'a';
    } else {
        /* Remap to "space" character for all other characters */
        ind = 'z' - 'a' + 1;
    }

    for (n = 0; n < opt->f_s * opt->duration; ++n) {
        cur_col = round(n / (opt->f_s * opt->duration /
            (NUM_COL - 1)));

        val = 0;

        for (i = NUM_ROW - 1; i >= 0; --i) {
            if (char_map[ind][i][cur_col] == 0) {
                continue;
            }

            freq = opt->high_freq - (opt->freq_gap * i);

            val += AMP * cos(2  * M_PI * freq * n /
                opt->f_s);
        }

        size_t r = fwrite(&val, sizeof(val), 1, stdout);
        if (r != 1) {
            fprintf(stderr, "error: failed to write to disk :(\n");
            exit(1);
        }
    }
}

void
usage(void)
{
    extern char *__progname;

    fprintf(stderr, "usage: %s [-l low_freq (Hz)] "
        "[-h high_freq (Hz)] [-r sampling_rate "
        "(Hz)] [-d character_duration (sec)] [-g "
        "character gap (sec)] string\n", __progname);
    exit(1);
}
