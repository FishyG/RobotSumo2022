/*
 * Laboratoire 2a
 * Hiver 2022, DEPSO
 *
 * Jessy Grimard-Maheu
 *
 * main.c - main program
 *
 */

#include <stdio.h>
#include <catalina_cog.h>

// D�finition des I/O de la carte Hydre
#define USER_DEL 1<<27
#define DEL_0 1<<24
#define DEL_1 1<<25
#define PWN_IO_1 1<<0
#define DEL_CHASER 0xff
#define CP_FLIPFLOP 1<<26
#define CLR  12

#define user_sw1 1<<8  // Pour l'�tat de la switch user 1
#define user_sw2 1<<10  // Pour l'�tat de la switch user 2
void fct_userdel(void); // Prototype de la fonction qui contr�le la USER_DEL

void fct_del01(void); // Prototype de la fonction qui contr�le les DEL 0 et 1

void fct_sequenceur(void); // Prototype de la fonction du sequenceur

//int no_lock = -1; // Variable pour le mutex
int delOn = 0;
int speed = 50;

// Point d'entr�e du programme
void main(int argc, char *argv[])
{
    unsigned long stack_ud[50];  // Stack pour le cog qui ex�cutera fct_userdel
    unsigned long stack_d01[50]; // Stack pour le cog qui ex�cutera fct_del01
    int cog_ud = -1;    // No associ� au cog qui ex�cutera fct_userdel
    int cog_d01 = -1;   // No associ� au cog qui ex�cutera fct_del01
    int sw1_state = 0;  // Etat de la switch 1
    int sw2_state = 0;  // Etat de la switch 2


    // D�part des cogs pour le contr�le de USER_DEL, DEL_0 et DEL_1
    cog_ud = _coginit_C(&fct_userdel, &stack_ud[50]);
    printf("Start fct USER_DEL\n\r");

    cog_d01 = _coginit_C(&fct_del01, &stack_d01[50]);
    printf("Start fct DEL_0/DEL_1\n\r");

    while(1)
    {
        sw1_state = (_ina() & user_sw1);    // Lecture de l'�tat de la switch 1
        sw2_state = (_ina() & user_sw2);    // Lecture de l'�tat de la switch 2

        _waitcnt(_clockfreq()/10+_cnt()); // D�lai 1/10 sec xD

        if(speed < 100)
        speed ++;
        else
        speed = 0;

        if(sw1_state == 0)
        {
            while (sw1_state == 0)  // D�bounce
            {
                sw1_state = (_ina() & user_sw1);
                _waitcnt(_clockfreq()/4+_cnt()); // D�lai 1/4 sec (pour le debounce)
            }

            printf("Button USER_SW1 pressed\n\r");
        }
        else if(sw2_state == 0)
        {
            while (sw2_state == 0)  // Debounce
            {
                sw2_state = (_ina() & user_sw2);
                _waitcnt(_clockfreq()/4+_cnt()); // D�lai 1/4 sec (pour le debounce)
            }
            printf("Button USER_SW2 pressed\n\r");


            if (cog_d01 == -1)   // Part le cog s'il est en arr�t
            {
                cog_d01 = _coginit_C(&fct_del01, &stack_d01[50]);
                printf("Start fct DEL_01\n\r");
            }
            else    // Sinon stop le cog
            {
                _cogstop(cog_d01);
                cog_d01 = -1;
                printf("Stop fct DEL_01\n\r");
            }
        }
    }
}

// Fonction qui contr�le la USER_DEL
void fct_userdel(void)
{
    int i = 0;
    // P27 en sortie et � l'�tat bas
    _dira(USER_DEL, USER_DEL);
    _outa(USER_DEL, 0);
    _dira(PWN_IO_1, PWN_IO_1);
    _outa(PWN_IO_1, 0);

    while(1)
    {
        if(speed >= 100)
        {
            _outa(USER_DEL, USER_DEL); // DEL � ON
        }
        else if(speed <= 0)
        {
            _outa(USER_DEL, 0); // DEL � OFF
        }
        else
        {
            while(i < speed)
            {
                _outa(USER_DEL, USER_DEL); // DEL � ON
                _outa(PWN_IO_1, PWN_IO_1); // PWM haut
                i++;
            }
            while(i < 100)
            {
                _outa(USER_DEL, 0); // DEL � OFF
                _outa(PWN_IO_1, 0); // PWM bas
                i++;
            }
        }
        i = 0;
    }
}

// Fonction qui contr�le les DEL 0 et 1
void fct_del01(void)
{
    // P24 et P25 en sortie et � l'�tat bas
    _dira(DEL_0|DEL_1, DEL_0|DEL_1);
    _outa(DEL_0|DEL_1, 0);
    _waitcnt(_clockfreq()/5+_cnt()); // D�lai 1/5 sec.

    while(1)
    {
        _outa(DEL_0|DEL_1, DEL_0);  // DEL_0 � ON et DEL_1 � OFF
        _waitcnt(_clockfreq()/8+_cnt()); // D�lai 1/8 sec.
        _outa(DEL_0|DEL_1, DEL_1); // DEL_0 � OFF et DEL_1 � ON
        _waitcnt(_clockfreq()/8+_cnt()); // D�lai 1/8 sec.
    }
}
