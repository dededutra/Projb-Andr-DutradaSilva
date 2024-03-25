#include <stdio.h>

#define amax 2047
#define levmax 3
#define cxmax 200

typedef enum {
    LIT, OPR, LOD, STO, CAL, INT, JMP, JPC
} fct;

typedef struct {
    fct f;
    int l;
    int a;
} instruction;

instruction code[cxmax + 1];

void somaQuadrados1a100 ()
{
    code[0].f = INT;
    code[0].l = 0;
    code[0].a = 5;

    code[1].f = LIT;
    code[1].l = 0;
    code[1].a = 1;

    code[2].f = STO;
    code[2].l = 0;
    code[2].a = 4;

    code[3].f = LOD;
    code[3].l = 0;
    code[3].a = 4;

    code[4].f = STO;
    code[4].l = 0;
    code[4].a = 3;

    code[5].f = LOD;
    code[5].l = 0;
    code[5].a = 4;

    code[6].f = LIT;
    code[6].l = 0;
    code[6].a = 1;

    code[7].f = OPR;
    code[7].l = 0;
    code[7].a = 2;

    code[8].f = STO;
    code[8].l = 0;
    code[8].a = 4;

    code[9].f = LOD;
    code[9].l = 0;
    code[9].a = 4;

    code[10].f = LOD;
    code[10].l = 0;
    code[10].a = 4;

    code[11].f = OPR;
    code[11].l = 0;
    code[11].a = 4;

    code[12].f = LOD;
    code[12].l = 0;
    code[12].a = 3;

    code[13].f = OPR;
    code[13].l = 0;
    code[13].a = 2;

    code[14].f = STO;
    code[14].l = 0;
    code[14].a = 3;

    code[15].f = LIT;
    code[15].l = 0;
    code[15].a = 100;

    code[16].f = LOD;
    code[16].l = 0;
    code[16].a = 4;

    code[17].f = OPR;
    code[17].l = 0;
    code[17].a = 3;

    code[18].f = JPC;
    code[18].l = 0;
    code[18].a = 20;

    code[19].f = JMP;
    code[19].l = 0;
    code[19].a = 5;

    code[20].f = OPR;
    code[20].l = 0;
    code[20].a = 0;
}


int base(int l, int b, int s[]) {
    int b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l--;
    }
    return b1;
}

void interpret() {
    const int stacksize = 500;
    int p, b, t;
    instruction i;
    int s[stacksize];

    t = 0; b = 1; p = 0;
    s[1] = 0; s[2] = 0; s[3] = 0;
    printf(" p  |(i.f   i.l   i.a)|(p   b   t)| s[00]        s[01]        s[02]        s[03]        s[04]        s[05]        s[06]        s[07]       s[08]        s[09]        s[10]       \n");
    do {
        i = code[p]; p++;
        printf(" %2d",p);
        switch (i.f) {
            case LIT:
                printf("   LIT   %d    %.4d",i.l,i.a);
                t++;
                s[t] = i.a;
                break;
            case OPR:
                printf("   OPR   %d    %.4d",i.l,i.a);
                switch (i.a) {
                    case 0:
                        t = b - 1;
                        p = s[t + 3];
                        b = s[t + 2];
                        break;
                    case 1:
                        s[t] = -s[t];
                        break;
                    case 2:
                        t--;
                        s[t] += s[t + 1];
                        break;
                    case 3:
                        t--;
                        s[t] -= s[t + 1];
                        break;
                    case 4:
                        t--;
                        s[t] *= s[t + 1];
                        break;
                    case 5:
                        t--;
                        s[t] /= s[t + 1];
                        break;
                    case 8:
                        t--;
                        s[t] = (s[t] == s[t + 1]) ? 1 : 0;
                        break;
                    case 9:
                        t--;
                        s[t] = (s[t] != s[t + 1]) ? 1 : 0;
                        break;
                    case 10:
                        t--;
                        s[t] = (s[t] < s[t + 1]) ? 1 : 0;
                        break;
                    case 11:
                        t--;
                        s[t] = (s[t] >= s[t + 1]) ? 1 : 0;
                        break;
                    case 12:
                        t--;
                        s[t] = (s[t] > s[t + 1]) ? 1 : 0;
                        break;
                    case 13:
                        t--;
                        s[t] = (s[t] <= s[t + 1]) ? 1 : 0;
                        break;
                }
                break;
            case LOD:
                printf("   LOD   %d    %.4d",i.l,i.a);
                t++;
                s[t] = s[base(i.l, b, s) + i.a];
                break;
            case STO:
                printf("   STO   %d    %.4d",i.l,i.a);
                s[base(i.l, b, s) + i.a] = s[t];
                t--;
                break;
            case CAL:
                printf("   CAL   %d    %.4d",i.l,i.a);
                s[t + 1] = base(i.l, b, s);
                s[t + 2] = b;
                s[t + 3] = p;
                b = t + 1;
                p = i.a;
                break;
            case INT:
                printf("   INT   %d    %.4d",i.l,i.a);
                t += i.a;
                break;
            case JMP:
                printf("   JMP   %d    %.4d",i.l,i.a);
                p = i.a;
                break;
            case JPC:
                printf("   JPC   %d    %.4d",i.l,i.a);
                if (s[t] == 0)
                    p = i.a;
                t--;
                break;
        }
        printf("  %2d  %2d  %2d   ",p,b,t);
    for (int k = 0;k<(t+1);k++) printf("%.9d    ",s[k]);
    printf("\n");
    } while (p != 0);
        printf("\n");
}

int main() {
    somaQuadrados1a100();
    interpret();
    return 0;
}
