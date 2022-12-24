/*
 * main.c - main program
 */

#include <stdio.h>
#include <catalina_cog.h>

// Définition des I/O de la carte Hydre
#define MOT_1F 1<<0
#define MOT_1B 1<<1
#define MOT_2F 1<<2
#define MOT_2B 1<<3
#define CAPTEUR_SOL 1 << 4
#define CAPTEUR_SOL2 1 << 5
#define CAPTEUR_SOL3 1 << 6
#define CAPTEUR_SOL4 1 << 7

#define user_sw1 1<<8  // Pour l'état de la switch user 1
#define user_sw2 1<<10  // Pour l'état de la switch user 2
void fct_foward1(void); // Prototype de la fonction qui contrôle sens mot
void fct_foward2(void);
void fct_back1(void);
void fct_back2(void);

void fct_sequenceur(void); // Prototype de la fonction du sequenceur

//int no_lock = -1; // Variable pour le mutex
int delOn = 0;
int speed = 50;


// Point d'entrée du programme
void main(int argc, char *argv[])
{
    unsigned long stack_ud[50];  // Stack pour le cog qui exécutera fct_userdel
    unsigned long stack_pwm2[50]; // Stack pour le cog qui exécutera fct_del01
    int cog_ud = -1;    // No associé au cog qui exécutera fct_userdel
    int cog_pwm2 = -1;   // No associé au cog qui exécutera fct_del01


    // Départ des cogs pour le contrôle de USER_DEL, DEL_0 et DEL_1
    //cog_ud = _coginit_C(&fct_userdel, &stack_ud[50]);
    //cog_pwm2 = _coginit_C(&fct_pwm2, &stack_pwm2[50]);

    //cog_d01 = _coginit_C(&fct_del01, &stack_d01[50]);
    //printf("Start fct DEL_0/DEL_1\n\r");

    _dira(CAPTEUR_SOL, 0);
    _dira(CAPTEUR_SOL2, 0);
    _dira(CAPTEUR_SOL3, 0);
    _dira(CAPTEUR_SOL, 0);
     cog_ud = _coginit_C(&fct_foward1, &stack_ud[50]);
     cog_pwm2 = _coginit_C(&fct_foward2, &stack_pwm2[50]);

    while(1)
    {

        if(!(_ina()& CAPTEUR_SOL))  //Capteur de SOI boy
        {
            if(cog_ud == -1)
            {
                //cog_ud = _coginit_C(&fct_foward1, &stack_ud[50]);
                //cog_pwm2 = _coginit_C(&fct_pwm2, &stack_pwm2[50]);
            }
        }
        else if((_ina()& CAPTEUR_SOL))
        {
            if(cog_ud != -1)
            {
                _cogstop(cog_ud );
                cog_ud = -1;
                cog_ud = _coginit_C(&fct_foward1, &stack_ud[50]);

            }
            if(cog_pwm2 != -1)
            {
                _cogstop(cog_pwm2);
                cog_pwm2 = -1;
                cog_pwm2 = _coginit_C(&fct_foward2, &stack_pwm2[50]);
            }
        }
        if(!(_ina()& CAPTEUR_SOL2))  //Capteur de SOI boy
        {
            if(cog_ud == -1)
            {
                //cog_ud = _coginit_C(&fct_foward1, &stack_ud[50]);
                //cog_pwm2 = _coginit_C(&fct_pwm2, &stack_pwm2[50]);
            }
        }
        else if((_ina()& CAPTEUR_SOL2))
        {
            if(cog_ud != -1)
            {
                _cogstop(cog_ud );
                cog_ud = -1;
                cog_ud = _coginit_C(&fct_foward1, &stack_ud[50]);
            }
            if(cog_pwm2 != -1)
            {
                _cogstop(cog_pwm2);
                cog_pwm2 = -1;
                cog_pwm2 = _coginit_C(&fct_foward2, &stack_pwm2[50]);
            }
        }

        if(!(_ina()& CAPTEUR_SOL3))  //Capteur de SOI boy
        {
            if(cog_ud == -1)
            {
            }
        }
        else if((_ina()& CAPTEUR_SOL3))
        {
            if(cog_ud != -1)
            {
                _cogstop(cog_ud );
                cog_ud = -1;
                cog_ud = _coginit_C(&fct_back1, &stack_ud[50]);
            }
            if(cog_pwm2 != -1)
            {
                _cogstop(cog_pwm2);
                cog_pwm2 = -1;
                cog_pwm2 = _coginit_C(&fct_back2, &stack_pwm2[50]);
            }
        }

        if(!(_ina()& CAPTEUR_SOL4))  //Capteur de SOI boy
        {
            if(cog_ud == -1)
            {
            }
        }
        else if((_ina()& CAPTEUR_SOL4))
        {
            if(cog_ud != -1)
            {
                _cogstop(cog_ud );
                cog_ud = -1;
                cog_ud = _coginit_C(&fct_back1, &stack_ud[50]);
            }
            if(cog_pwm2 != -1)
            {
                _cogstop(cog_pwm2);
                cog_pwm2 = -1;
                cog_pwm2 = _coginit_C(&fct_back2, &stack_pwm2[50]);            }
        }

        /*if(!(_ina()& CAPTEUR_SOL2))  //Capteur de SOI boy
        {
            if(cog_pwm2 == -1)
            {
                //cog_pwm2 = _coginit_C(&fct_foward2, &stack_pwm2[50]);
            }
        }
        else if((_ina()& CAPTEUR_SOL2))
        {
            if(cog_pwm2 != -1)
            {
                _cogstop(cog_pwm2 );
                cog_pwm2 = -1;
                cog_pwm2 = _coginit_C(&fct_back2, &stack_pwm2[50]);
            }
        }*/
    }
}

// Fonction qui contrôle la USER_DEL
void fct_foward1(void)
{
    int i = 0;
    // P27 en sortie et à l'état bas
    _dira(MOT_1F, MOT_1F);
    _outa(MOT_1F, 0);

    while(1)
    {
            while(i < speed)
            {
                _outa(MOT_1F, MOT_1F); // PWM haut
                i++;
            }
            while(i < 100)
            {
                _outa(MOT_1F, 0); // PWM bas
                i++;
            }

        i = 0;
    }
}

// Fonction qui contrôle la USER_DEL
void fct_foward2(void)
{
    int i = 0;
    // P27 en sortie et à l'état bas
    _dira(MOT_2F, MOT_2F);
    _outa(MOT_2F, 0);

    while(1)
    {
            while(i < speed)
            {
                _outa(MOT_2F, MOT_2F); // PWM haut
                i++;
            }
            while(i < 100)
            {
                _outa(MOT_2F, 0); // PWM bas
                i++;
            }

        i = 0;
    }
}

void fct_back1(void)
{
    int i = 0;
    // P27 en sortie et à l'état bas
    _dira(MOT_1B, MOT_1B);
    _outa(MOT_1B, 0);

    while(1)
    {
            while(i < speed)
            {
                _outa(MOT_1B, MOT_1B); // PWM haut
                i++;
            }
            while(i < 100)
            {
                _outa(MOT_1B, 0); // PWM bas
                i++;
            }

        i = 0;
    }
}

void fct_back2(void)
{
    int i = 0;
    // P27 en sortie et à l'état bas
    _dira(MOT_2B, MOT_2B);
    _outa(MOT_2B, 0);

    while(1)
    {
            while(i < speed)
            {
                _outa(MOT_2B, MOT_2B); // PWM haut
                i++;
            }
            while(i < 100)
            {
                _outa(MOT_2B, 0); // PWM bas
                i++;
            }

        i = 0;
    }
}

