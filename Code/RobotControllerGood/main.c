 
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

int main(int argc, char *argv[])
{
    // Open the joystick device
    int fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Error opening joystick");
        return 1;
    }

    // Get the number of axes and buttons on the joystick
    int axes, buttons;
    int leftGaz = 0;
    int rightGaz = 0;
    int roueGauche = 0;
    int roueDroite = 0;
    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);

    // Read data from the joystick
    struct js_event e;
    while (read(fd, &e, sizeof(e)) == sizeof(e)) {
        // Process the event data
        switch (e.type & ~JS_EVENT_INIT) {
            case JS_EVENT_AXIS:
                printf("Axis %d value: %d\n", e.number, e.value);
                if(e.number == 1)
                {
                    leftGaz = e.value;
                }
                else if(e.number == 2)
                {
                    rightGaz = e.value;
                }
                break;
            case JS_EVENT_BUTTON:
                printf("Button %d value: %d\n", e.number, e.value);
                break;
        }
    
        roueGauche = ((leftGaz.x / 327.68) + 100) / 2;
        roueDroite = ((rightGaz.y / 327.68) + 100) / 2;
        printf("Axe 0 : %d\n",roueGauche);
        printf("Axe 0 : %d\n",roueDroite);
    }

    // Close the joystick device
    close(fd);

    return 0;
}
