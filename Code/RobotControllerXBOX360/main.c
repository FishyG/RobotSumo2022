#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#define MAX 80
#define PORT 8869
#define IPADDR "192.168.2.26"//"10.240.3.172"
#define SA struct sockaddr


void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[3])
{
    size_t axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}

int main(int argc, char *argv[])
{
	int sockfd, connfd;
	char buff[MAX];
	struct sockaddr_in servaddr, cli;
	const char *device;
    int js;
    int count = 0;
    int roueGauche = 0;
    int roueDroite = 0;
    char commandeToSend[4];
    struct js_event event;
    struct axis_state axes[3] = {0};
    size_t axis;

    
    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IPADDR);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	//func(sockfd);
	/* This loop will exit if the controller is unplugged. */
    bool sendGauche = 0;
    bool sendDroite = 0;
    bool needToSend = 0;
    
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                break;
            case JS_EVENT_AXIS:
                axis = get_axis_state(&event, axes);
                if (axis < 3)
                {
                    roueGauche = ((axes[1].x / 327.68) + 100) /2;
                    roueDroite = ((axes[2].y / 327.68) + 100) /2;
                	printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
                }
                if(axis == 1)
                {
                    sprintf(commandeToSend, "a%03d", roueGauche);	// Commande qui sera envoyé
                    needToSend = 1;	// Les valeurs on changer donc on doit envoyer les données
                }
                if(axis == 2)
                {
                    sprintf(commandeToSend, "b%03d", roueDroite);	// Commande qui sera envoyé
                    needToSend = 1;	// Les valeurs on changer donc on doit envoyer les données
                }
               break;
            default:
                /* Ignore init events. */
                break;
        }
        
        printf("Roue Gauche : %6d \n", roueGauche);
        printf("Roue Droite : %6d \n", roueDroite);
        
	// Si les valeurs on changer on envoie le data
        if(needToSend)
        {
            printf("\nSending : %s", commandeToSend);
            write(sockfd, commandeToSend, sizeof(commandeToSend));
            bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff));
            usleep(100000);
        }
        
        
        //usleep(100000);
        fflush(stdout);
    }

    close(js);
	// close the socket
	close(sockfd);
    return 0;
}

