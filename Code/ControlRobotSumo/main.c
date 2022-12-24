<<<<<<< HEAD
 
@@ -7,178 +7,153 @@
  * main.c - main program
  *
  */
 
 #include <stdio.h>
-#include <stdlib.h>
-#include <string.h>
 #include <catalina_cog.h>
 #include <catalina_serial4.h>
 
-// DÃ©finition des I/O de la carte Hydre
 #define PWM_IO_0 1<<0
 #define PWM_IO_1 1<<2
 #define CAPTEUR_SOL_0 1 << 4
 #define CAPTEUR_SOL_1 1 << 5
 #define CAPTEUR_SOL_2 1 << 6
 #define CAPTEUR_SOL_3 1 << 7
 
-void fct_pwm0(void);    // Prototype de la fonction qui contrÃ´le le PWM 0
-void fct_pwm1(void);    // Prototype de la fonction qui contrÃ´le le PWM 1
 
 void fct_startsound(void); // Prototype de la fonction pour le bruit au startup
 
 //int no_lock = -1; // Variable pour le mutex
 int speed0 = 0;
-int dir0 = 0;
 int speed1 = 0;
-int dir1 = 0;
 
-// Point d'entrÃ©e du programme
+// Point d'entrÃ¯Â¿Â½e du programme
 void main(int argc, char *argv[])
 {
-    char rxData[50] = "";
-    char rxLast = 0;
-    int testspeed0 = 0;
-    int testspeed1 = 0;
-    int testsens0 = 0;
-    int testsens1 = 0;
-    int i = 0;
-
-    unsigned long stack_pwm0[50];  // Stack pour le cog qui exÃ©cutera fct_pwm0
-    unsigned long stack_pwm1[50]; // Stack pour le cog qui exÃ©cutera fct_pwm1
-    int cog_pwm0 = -1;    // No associÃ© au cog qui exÃ©cutera fct_pwm0
-    int cog_pwm1 = -1;   // No associÃ© au cog qui exÃ©cutera fct_pwm1
 
 
-    // DÃ©part des cogs pour le contrÃ´le de pwm0 et pwm1
-    cog_pwm0 = _coginit_C(&fct_pwm0, &stack_pwm0[50]);
 
-    cog_pwm1 = _coginit_C(&fct_pwm1, &stack_pwm1[50]);
     printf("Start fct pwm1\n\r");
 
     fct_startsound();
 
     _dira(CAPTEUR_SOL_0 | CAPTEUR_SOL_1 | CAPTEUR_SOL_2 | CAPTEUR_SOL_3, 0);
 
-    speed0 = 30;
-
     while(1)
     {
         switch(s4_rxcheck(1))
         {
-            case 'a':
-                i = 0;
-                rxLast = '0';
-                testspeed0 = 0;
-                for(i = 0; i < 3; i++)
-                {
-                    rxLast = s4_rxcheck(1);
-                    testspeed0 = (testspeed0 * 10) + (rxLast - '0');
-                }
-                speed0 = testspeed0;
                 break;
-            case 'b':
-                i = 0;
-                rxLast = '0';
-                testspeed1 = 0;
-                for(i = 0; i < 3; i++)
-                {
-                    rxLast = s4_rxcheck(1);
-                    testspeed1 = (testspeed1 * 10) + (rxLast - '0');
-                }
-                speed1 = testspeed1;
                 break;
-            case 'c':
-                rxLast = s4_rxcheck(1);
-                dir0 = (rxLast - '0');
                 break;
-            case 'd':
-                rxLast = s4_rxcheck(1);
-                dir1 = (rxLast - '0');
             //default:
                 // code block
         }
         s4_rxflush(1);
         /*if(speed1 < 20)
             speed1 ++;
         else
         {
             speed1 = 0;
-            if(dir0)
-                dir0 = 0;
             else
-                dir0 = 1;
         }*/
 
-        _waitcnt(_clockfreq()/10+_cnt()); // DÃ©lai 1/10 sec xD
     }
 }
 
-// Fonction qui contrÃ´le le PWM 0
+// Fonction qui contrÃ¯Â¿Â½le le PWM 0
 void fct_pwm0(void)
 {
     int i = 0;
-    int dir0temp = 0;
     int speed0temp = 0;
-    // P27 en sortie et Ã  l'Ã©tat bas
     _dira(PWM_IO_0, PWM_IO_0);
     _dira(PWM_IO_0 << 1, PWM_IO_0 << 1);
 
     while(1)
     {
         if(speed0)
         {
-            dir0temp = dir0;
             speed0temp = speed0;
-            _outa(PWM_IO_0 << dir0temp ^ 1, 0);
-            _outa(PWM_IO_0 << dir0temp, PWM_IO_0 << dir0temp); // PWM haut
             while(i < speed0temp)
             {
-                //_outa(PWM_IO_0 << dir0temp, PWM_IO_0 << dir0temp); // PWM haut
                 i++;
             }
-            _outa(PWM_IO_0 << dir0temp, 0); // PWM bas
             while(i < 100)
             {
-                //_outa(PWM_IO_0 << dir0temp, 0); // PWM bas
                 i++;
             }
             i = 0;
         }
     }
 }
 
-// Fonction qui contrÃ´le le PWM 1
+// Fonction qui contrÃ¯Â¿Â½le le PWM 1
 void fct_pwm1(void)
 {
     int i = 0;
-    int dir1temp = 0;
     int speed1temp = 0;
-    // P27 en sortie et Ã  l'Ã©tat bas
     _dira(PWM_IO_1, PWM_IO_1);
     _dira(PWM_IO_1 << 1, PWM_IO_1 << 1);
 
     while(1)
     {
         if(speed1)
         {
-            dir1temp = dir1;
+            sens1temp = sens1;
             speed1temp = speed1;
-            _outa(PWM_IO_1 << dir1temp ^ 1, 0);
-            _outa(PWM_IO_1 << dir1temp, PWM_IO_1 << dir1temp); // PWM haut
             while(i < speed1temp)
             {
-                //_outa(PWM_IO_1 << dir1temp, PWM_IO_1 << dir1temp); // PWM haut
                 i++;
             }
-            _outa(PWM_IO_1 << dir1temp, 0); // PWM bas
+            _outa(PWM_IO_1 << sens1temp, 0); // PWM bas
             while(i < 100)
             {
-                //_outa(PWM_IO_1 << dir1temp, 0); // PWM bas
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
-        _waitcnt(_clockfreq()/2+_cnt()); // DÃ©lai 1/4 sec xD
         speed0 = speed1 = 0;
-        _waitcnt(_clockfreq()/10+_cnt()); // DÃ©lai 1/4 sec xD
         speed0 = speed1 = 10;
-        _waitcnt(_clockfreq()/10+_cnt()); // DÃ©lai 1/4 sec xD

         speed0 = speed1 = 0;
-        _waitcnt(_clockfreq()/10+_cnt()); // DÃ©lai 1/4 sec xD

     }
     speed0 = speed1 = 10;
-    _waitcnt(_clockfreq()+_cnt()); // DÃ©lai 1 sec xD
     speed0 = speed1 = 0;
 }

=======
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
#include <stdlib.h>
#include <string.h>
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
int dir0 = 0;
int speed1 = 0;
int dir1 = 0;

// Point d'entrée du programme
void main(int argc, char *argv[])
{
    char rxData[50] = "";
    char rxLast = 0;
    int testspeed0 = 0;
    int testspeed1 = 0;
    int testsens0 = 0;
    int testsens1 = 0;
    int i = 0;

    unsigned long stack_pwm0[50];  // Stack pour le cog qui exécutera fct_pwm0
    unsigned long stack_pwm1[50]; // Stack pour le cog qui exécutera fct_pwm1
    int cog_pwm0 = -1;    // No associé au cog qui exécutera fct_pwm0
    int cog_pwm1 = -1;   // No associé au cog qui exécutera fct_pwm1


    // Départ des cogs pour le contrôle de pwm0 et pwm1
    cog_pwm0 = _coginit_C(&fct_pwm0, &stack_pwm0[50]);
    printf("Start fct pwm0\n\r");

    cog_pwm1 = _coginit_C(&fct_pwm1, &stack_pwm1[50]);
    printf("Start fct pwm1\n\r");

    fct_startsound();

    _dira(CAPTEUR_SOL_0 | CAPTEUR_SOL_1 | CAPTEUR_SOL_2 | CAPTEUR_SOL_3, 0);

    speed0 = 30;

    while(1)
    {
        switch(s4_rxcheck(1))
        {
            case 'a':
                i = 0;
                rxLast = '0';
                testspeed0 = 0;
                for(i = 0; i < 3; i++)
                {
                    rxLast = s4_rxcheck(1);
                    testspeed0 = (testspeed0 * 10) + (rxLast - '0');
                }
                speed0 = testspeed0;
                break;
            case 'b':
                i = 0;
                rxLast = '0';
                testspeed1 = 0;
                for(i = 0; i < 3; i++)
                {
                    rxLast = s4_rxcheck(1);
                    testspeed1 = (testspeed1 * 10) + (rxLast - '0');
                }
                speed1 = testspeed1;
                break;
            case 'c':
                rxLast = s4_rxcheck(1);
                dir0 = (rxLast - '0');
                break;
            case 'd':
                rxLast = s4_rxcheck(1);
                dir1 = (rxLast - '0');
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
            if(dir0)
                dir0 = 0;
            else
                dir0 = 1;
        }*/

        _waitcnt(_clockfreq()/10+_cnt()); // Délai 1/10 sec xD
    }
}

// Fonction qui contrôle le PWM 0
void fct_pwm0(void)
{
    int i = 0;
    int dir0temp = 0;
    int speed0temp = 0;
    // P27 en sortie et à l'état bas
    _dira(PWM_IO_0, PWM_IO_0);
    _dira(PWM_IO_0 << 1, PWM_IO_0 << 1);

    while(1)
    {
        if(speed0)
        {
            dir0temp = dir0;
            speed0temp = speed0;
            _outa(PWM_IO_0 << dir0temp ^ 1, 0);
            _outa(PWM_IO_0 << dir0temp, PWM_IO_0 << dir0temp); // PWM haut
            while(i < speed0temp)
            {
                //_outa(PWM_IO_0 << dir0temp, PWM_IO_0 << dir0temp); // PWM haut
                i++;
            }
            _outa(PWM_IO_0 << dir0temp, 0); // PWM bas
            while(i < 100)
            {
                //_outa(PWM_IO_0 << dir0temp, 0); // PWM bas
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
    int dir1temp = 0;
    int speed1temp = 0;
    // P27 en sortie et à l'état bas
    _dira(PWM_IO_1, PWM_IO_1);
    _dira(PWM_IO_1 << 1, PWM_IO_1 << 1);

    while(1)
    {
        if(speed1)
        {
            dir1temp = dir1;
            speed1temp = speed1;
            _outa(PWM_IO_1 << dir1temp ^ 1, 0);
            _outa(PWM_IO_1 << dir1temp, PWM_IO_1 << dir1temp); // PWM haut
            while(i < speed1temp)
            {
                //_outa(PWM_IO_1 << dir1temp, PWM_IO_1 << dir1temp); // PWM haut
                i++;
            }
            _outa(PWM_IO_1 << dir1temp, 0); // PWM bas
            while(i < 100)
            {
                //_outa(PWM_IO_1 << dir1temp, 0); // PWM bas
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
>>>>>>> 5a49a8412c0c84b36c599643ca22b7436c4cf675
