#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */


/* list of sample users you can follow 
 * these users exist in server with sample
 * posted messages you can retrieve.
 * Of course if you login with multiple users to server
 * and know the userID's of these users you can follow these users
 * and retrieve their info while application is running.
 */
int sample_leaders_ids[11] = {0, 1, 2, 3, 4, 5};

/* table of logged in users
 */
int logged_in_users[10], append_logged_in_index = 0;

/* store all users ids */
int user_ids[11] = {0, 1, 2, 3, 4, 5};
int append_users_index = 6;         /* store user ids and append index */

char posted_messages[10][10];
int posted_index = 0;





