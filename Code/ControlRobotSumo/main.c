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
#include <catalina_serial4.h>

// Définition des I/O de la carte Hydre
#define PWM_IO_0 1<<0
#define PWM_IO_1 1<<2
#define CAPTEUR_SOL_0 1 << 4
#define CAPTEUR_SOL_1 1 << 5
#define CAPTEUR_SOL_2 1 << 6
#define CAPTEUR_SOL_3 1 << 7

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


    // Départ des cogs pour le contrôle de USER_DEL, DEL_0 et DEL_1
    cog_pwm0 = _coginit_C(&fct_pwm0, &stack_ud[50]);
    printf("Start fct pwm0\n\r");

    cog_pwm1 = _coginit_C(&fct_pwm1, &stack_d01[50]);
    printf("Start fct pwm1\n\r");

    fct_startsound();

    _dira(CAPTEUR_SOL_0 | CAPTEUR_SOL_1 | CAPTEUR_SOL_2 | CAPTEUR_SOL_3, 0);

    while(1)
    {

        switch(s4_rxcheck(1))
        {
            case 'q':
                if(speed0 <= 90)
                speed0 += 10;
                break;
            case 'a':
                if(speed0 >= 10)
                speed0 -= 10;
                break;
            case 'p':
                if(speed1 <= 90)
                speed1 += 10;
                break;
            case 'l':
                if(speed1 >= 10)
                speed1 -= 10;
                break;
            //default:
                // code block
        }
        s4_rxflush(1);
        /*if(speed1 < 20)
            speed1 ++;
        else
        {
            speed1 = 0;
            if(sens0)
                sens0 = 0;
            else
                sens0 = 1;
        }*/

        _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/10 sec xD
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
        if(speed0)
        {
            sens0temp = sens0;
            speed0temp = speed0;
            _outa(PWM_IO_0 << sens0temp ^ 1, 0);
            _outa(PWM_IO_0 << sens0temp, PWM_IO_0 << sens0temp); // PWM haut
            while(i < speed0temp)
            {
                //_outa(PWM_IO_0 << sens0temp, PWM_IO_0 << sens0temp); // PWM haut
                i++;
            }
            _outa(PWM_IO_0 << sens0temp, 0); // PWM bas
            while(i < 100)
            {
                //_outa(PWM_IO_0 << sens0temp, 0); // PWM bas
                i++;
            }
            i = 0;
        }
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
        if(speed1)
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
