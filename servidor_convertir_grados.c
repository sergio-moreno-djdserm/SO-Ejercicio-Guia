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
	serv_adr.sin_port = htons(9080);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	if (listen(sock_listen, 3) < 0) // Hasta 3 peticiones pueden estar en espera
		printf("Error en el listen");
	
	int i;
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
			char tempe_aux[20];
			float fahrenheit;
			float celsius;
			
			if (codigo!=0)
			{
				p = strtok( NULL, "/");
				strcpy(tempe_aux,p);
				printf("Temperatura a convertir: %s\n",tempe_aux);
			}
			
			if (codigo==0)
				terminar=1;
			
			else if (codigo==1) // Piden la conversion de Celsius a Fahrenheit
			{
				float temperatura = atof(tempe_aux);
				printf("Temperatura %f\n",temperatura);
				fahrenheit = ((temperatura*1.8)+32);
				sprintf(respuesta,"%f",fahrenheit);
			}
			
			else if (codigo==2) // Piden la conversión de Fahrenheit a Celsius
			{
				float temperatura = atof(tempe_aux);
				celsius = ((temperatura-32)/1.8);
				sprintf(respuesta,"%f",celsius);
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
