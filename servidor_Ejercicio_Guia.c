#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket (Escucha, el servidor espera una conexión)
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr)); // Inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// Asocia el socket a cualquiera de las IP de la máquina. 
	// htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Escucharemos en el port 9050
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	if (listen(sock_listen, 3) < 0) // Hasta 3 peticiones pueden estar en espera
		printf("Error en el listen");
	
	int i;
	// Atenderemos solo 5 peticiones
	for(;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL); // Esperar la conexión hasta recibir
		printf ("He recibido conexión\n");
		// sock_conn es el socket que usaremos para este cliente
		
		//Bucle de atención al cliente
		int terminar=0;
		while (terminar==0)
		{
			// Ahora recibimos su nombre, que dejamos en buff peticion
			ret=read(sock_conn,peticion,sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que añadirle la marca de fin de string 
			// Para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos el nombre en la consola
			
			printf ("Se ha conectado: %s\n",peticion);
			
			// Saber que nos piden, que petición
			char *p = strtok(peticion, "/"); // Coge el buff y corta por donde hay una barra
			int codigo =  atoi (p);
			char nombre[20];
			
			if (codigo!=0)
			{
				p = strtok( NULL, "/");
				strcpy (nombre, p); // Copiar nombre
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo==0)
				terminar=1;
			
			else if (codigo ==1) // Piden la longitud del nombre
				sprintf (respuesta,"%d",strlen(nombre));
			
			else if (codigo==2)
				// Quieren saber si el nombre es bonito
				if((nombre[0]=='M') || (nombre[0]=='S'))
					strcpy (respuesta,"SI");
				else
					strcpy (respuesta,"NO");
			
			else if (codigo==3) // Decir si es alto
			{
				p = strtok(NULL, "/"); // Coge el buff y corta por donde hay una barra
				float altura =  atof (p);
				if (altura > 1.70)
					sprintf(respuesta,"%s: eres alto",nombre);
				else
					sprintf(respuesta,"%s: eres bajo",nombre);
			}
			
			else if (codigo==4) // Dime si mi nombre es palíndromo
			{
				int i;
				for (i=0;nombre[i]!='\0';i++) {
					nombre[i]=toupper(nombre[i]);
				}
				int x; int y; int z;
				x=strlen(nombre);
				x=x-1;
				for(y=0,z=x;y<=x/2;y++,z--)
				{
					if (nombre[y]==nombre[z])
						strcpy (respuesta,"SI");
					else
						strcpy (respuesta,"NO");
				}
			}
			
			else if (codigo==5) // Dame mi nombre en mayúsculas
			{
				int i;
				for (i=0;nombre[i]!='\0';i++) {
					nombre[i]=toupper(nombre[i]);
				}
				sprintf(respuesta,"%s",nombre);
			}
			
			if (codigo!=0)
			{
				printf ("Respuesta: %s\n", respuesta);
				// Y lo enviamos
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}
		// Se acabó el servicio para este cliente
		close(sock_conn); 
	}
}
