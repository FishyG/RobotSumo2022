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

// Définition des I/O de la carte Hydre
#define USER_DEL 1<<27
#define DEL_0 1<<24
#define DEL_1 1<<25
#define PWM_IO_0 1<<0
#define PWM_IO_1 1<<2
#define DEL_CHASER 0xff
#define CP_FLIPFLOP 1<<26
#define CLR  12

#define user_sw1 1<<8  // Pour l'état de la switch user 1
#define user_sw2 1<<10  // Pour l'état de la switch user 2

void fct_pwm0(void);    // Prototype de la fonction qui contrôle le PWM 0
void fct_pwm1(void);    // Prototype de la fonction qui contrôle le PWM 1

void fct_startsound(void); // Prototype de la fonction pour le bruit au startup

//int no_lock = -1; // Variable pour le mutex
int speed0 = 0;
int sens0 = 0;
int speed1 = 0;
int sens1 = 0;

// Point d'entrée du programme
void main(int argc, char *argv[])
{
    unsigned long stack_ud[50];  // Stack pour le cog qui exécutera fct_userdel
    unsigned long stack_d01[50]; // Stack pour le cog qui exécutera fct_del01
    int cog_pwm0 = -1;    // No associé au cog qui exécutera fct_userdel
    int cog_pwm1 = -1;   // No associé au cog qui exécutera fct_del01
    int sw1_state = 0;  // Etat de la switch 1
    int sw2_state = 0;  // Etat de la switch 2


    // Départ des cogs pour le contrôle de USER_DEL, DEL_0 et DEL_1
    cog_pwm0 = _coginit_C(&fct_pwm0, &stack_ud[50]);
    printf("Start fct pwm0\n\r");

    cog_pwm1 = _coginit_C(&fct_pwm1, &stack_d01[50]);
    printf("Start fct pwm1\n\r");

    fct_startsound();

    while(1)
    {
        //sw1_state = (_ina() & user_sw1);    // Lecture de l'état de la switch 1
        //sw2_state = (_ina() & user_sw2);    // Lecture de l'état de la switch 2

        speed0 = 30;
        while(1)
        {
            if(speed1 < 20)
                speed1 ++;
            else
            {
                speed1 = 0;
                if(sens0)
                    sens0 = 0;
                else
                    sens0 = 1;
            }

            _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/10 sec xD
        }
    }
}

// Fonction qui contrôle le PWM 0
void fct_pwm0(void)
{
    int i = 0;
    int sens0temp = 0;
    int speed0temp = 0;
    // P27 en sortie et à l'état bas
    _dira(PWM_IO_0, PWM_IO_0);
    _dira(PWM_IO_0 << 1, PWM_IO_0 << 1);

    while(1)
    {
        sens0temp = sens0;
        speed0temp = speed0;

        _outa(PWM_IO_0 << sens0temp ^ 1, 0);

        while(i < speed0temp)
        {
            _outa(PWM_IO_0 << sens0temp, PWM_IO_0 << sens0temp); // PWM haut
            i++;
        }
        while(i < 100)
        {
            _outa(PWM_IO_0 << sens0temp, 0); // PWM bas
            i++;
        }
        i = 0;
    }
}

// Fonction qui contrôle le PWM 1
void fct_pwm1(void)
{
    int i = 0;
    int sens1temp = 0;
    int speed1temp = 0;
    // P27 en sortie et à l'état bas
    _dira(PWM_IO_1, PWM_IO_1);
    _dira(PWM_IO_1 << 1, PWM_IO_1 << 1);

    while(1)
    {
        sens1temp = sens1;
        speed1temp = speed1;

        _outa(PWM_IO_1 << sens1temp ^ 1, 0);

        _outa(PWM_IO_1 << sens1temp, PWM_IO_1 << sens1temp); // PWM haut
        while(i < speed1temp)
        {
            //_outa(PWM_IO_1 << sens1temp, PWM_IO_1 << sens1temp); // PWM haut
            i++;
        }
        _outa(PWM_IO_1 << sens1temp, 0); // PWM bas
        while(i < 100)
        {
            //_outa(PWM_IO_1 << sens1temp, 0); // PWM bas
            i++;
        }
        i = 0;
    }
}

void fct_startsound(void)
{
    int i = 0;
    for(i = 0; i < 3; i ++)
    {
        speed0 = speed1 = 10;
        _waitcnt(_clockfreq()/2+_cnt()); // Délai 1/4 sec xD
        speed0 = speed1 = 0;
        _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/4 sec xD
        speed0 = speed1 = 10;
        _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/4 sec xD
        speed0 = speed1 = 0;
        _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/4 sec xD
    }
    speed0 = speed1 = 10;
    _waitcnt(_clockfreq()+_cnt()); // Délai 1 sec xD
    speed0 = speed1 = 0;
}
