#include "thoughts.h"
#include "display.h"
#include "buttons.h"
#include "esp_random.h"

static bool active = false;
static const char *current_text = "";

/* ---------- 3x5 Font, skaliert ---------- */

static const uint8_t *font_rows(char c)
{
    static const uint8_t A[5]={2,5,7,5,5}, B[5]={6,5,6,5,6}, C[5]={3,4,4,4,3};
    static const uint8_t D[5]={6,5,5,5,6}, E[5]={7,4,6,4,7}, F[5]={7,4,6,4,4};
    static const uint8_t G[5]={3,4,5,5,3}, H[5]={5,5,7,5,5}, I[5]={7,2,2,2,7};
    static const uint8_t J[5]={1,1,1,5,2}, K[5]={5,5,6,5,5}, L[5]={4,4,4,4,7};
    static const uint8_t M[5]={5,7,7,5,5}, N[5]={5,7,7,7,5}, O[5]={2,5,5,5,2};
    static const uint8_t P[5]={6,5,6,4,4}, R[5]={6,5,6,5,5}, S[5]={3,4,2,1,6};
    static const uint8_t T[5]={7,2,2,2,2}, U[5]={5,5,5,5,7}, V[5]={5,5,5,5,2};
    static const uint8_t W[5]={5,5,7,7,5}, X[5]={5,5,2,5,5}, Y[5]={5,5,2,2,2};
    static const uint8_t Z[5]={7,1,2,4,7};

    static const uint8_t N0[5]={7,5,5,5,7}, N1[5]={2,6,2,2,7}, N2[5]={7,1,7,4,7};
    static const uint8_t N3[5]={7,1,7,1,7}, N4[5]={5,5,7,1,1}, N5[5]={7,4,7,1,7};

    static const uint8_t SPACE[5]={0,0,0,0,0};
    static const uint8_t DOT[5]={0,0,0,0,2};
    static const uint8_t EXCL[5]={2,2,2,0,2};
    static const uint8_t QMARK[5]={6,1,2,0,2};
    static const uint8_t COLON[5]={0,2,0,2,0};

    if (c >= 'a' && c <= 'z') c -= 32;

    switch(c)
    {
        case 'A': return A; case 'B': return B; case 'C': return C;
        case 'D': return D; case 'E': return E; case 'F': return F;
        case 'G': return G; case 'H': return H; case 'I': return I;
        case 'J': return J; case 'K': return K; case 'L': return L;
        case 'M': return M; case 'N': return N; case 'O': return O;
        case 'P': return P; case 'R': return R; case 'S': return S;
        case 'T': return T; case 'U': return U; case 'V': return V;
        case 'W': return W; case 'X': return X; case 'Y': return Y;
        case 'Z': return Z;

        case '0': return N0; case '1': return N1; case '2': return N2;
        case '3': return N3; case '4': return N4; case '5': return N5;

        case '.': return DOT;
        case '!': return EXCL;
        case '?': return QMARK;
        case ':': return COLON;

        default: return SPACE;
    }
}

static void draw_char_big(int x, int y, char c)
{
    const uint8_t *rows = font_rows(c);

    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (rows[row] & (1 << (2 - col)))
            {
                fill_rect(x + col * 2,
                          y + row * 2,
                          2,
                          2);
            }
        }
    }
}

static void draw_text_big(int x, int y, const char *text)
{
    int start_x = x;

    while (*text)
    {
        if (*text == '\n')
        {
            y += 13;
            x = start_x;
        }
        else
        {
            draw_char_big(x, y, *text);
            x += 8;
        }

        text++;
    }
}

/* ---------- Texte ---------- */

static const char *water_texts[] =
{
    "WASSER IST\nEIN GERUECHT.",
    "DIE SAHARA\nHAT MITLEID.",
    "WURZELN\nBESCHWEREN SICH."
};

static const char *light_texts[] =
{
    "IST DAS HIER\nEINE HOEHLE?",
    "PHOTOSYNTHESE\nPAUSIERT.",
    "DIE SONNE\nHAT WOHL FREI."
};

static const char *happiness_texts[] =
{
    "ETWAS LIEBE\nWAERE NETT.",
    "HALLO!\nWIE GEHT ES DIR?",
    "STREICHELN\nERWUENSCHT"
};

static const char *critical_texts[] =
{
    "SENSOR SAGT\nNEIN.",
    "ICH WEIGERE\nMICH ZU REDEN.",
    "DU HAST DAS\nNICHT IM GRIFF."
};

static const char *judge_texts[] =
{
    "INTERESSANTE\nPFLEGE.",
    "ICH BEOBACHTE\nDICH.",
    "2 VON 5\nBLAETTERN.",
    "FUEHLE DICH\nBEOBACHTET"
};

static const char *lovely_texts[] =
{
    "DU BIST\nAKZEPTABEL.",
    "BLAETTER\nSIND ERFREUT.",
    "MEHR DAVON."
};

static const char *happy_texts[] =
{
    "ALLES IM\nGRUENEN BEREICH.",
    "KEINE\nBESCHWERDEN.",
    "SEHR\nPFLANZLICH."
};

static const char *fun_texts[] =
{
    "BANANEN SIND\nBEEREN.",
    "KAKTEEN SIND\nCOOL.",
    "SONNENLICHT\nIST ESSEN."
};

static const char *dead_texts[] =
{
    "RIP.\nDANK SCHLECHTER \nPFLEGE",
    "ICH WAR\nNOCH SO JUNG.",
    "DIE GIESSKANNE\nWAR DIREKT DA.",
    "TODESURSACHE:\nMANAGEMENT."
};

static const char *pick_random(const char **list, int count)
{
    return list[esp_random() % count];
}

static const char *pick_text(plant_t *plant)
{
    switch (plant->state)
    {
        case PLANT_DEAD:
            return pick_random(dead_texts, 4);

        case PLANT_JUDGE:
            return pick_random(judge_texts, 4);

        case PLANT_THIRSTY:
            return pick_random(happy_texts, 3);

        case PLANT_SAD:
            return pick_random(light_texts, 3);

        case PLANT_LOVELY:
            return pick_random(lovely_texts, 3);

        case PLANT_HAPPY:
        default:
            if ((esp_random() % 4) == 0)
            {
                return pick_random(fun_texts, 3);
            }

            return pick_random(happiness_texts, 3);
    }
}

/* ---------- Public ---------- */

void thoughts_start(plant_t *plant)
{
    active = true;
    current_text = pick_text(plant);
}

bool thoughts_is_active(void)
{
    return active;
}

void thoughts_update(plant_t *plant)
{
    (void)plant;

    button_event_t event = button_event();

    if (event == BUTTON_CLICK || event == BUTTON_LONG_PRESS)
    {
        active = false;
        return;
    }

    display_clear();
    draw_text_big(4, 18, current_text);
    display_update();
}