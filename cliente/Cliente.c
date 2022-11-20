/*-------------------------------------------------------------------------
|
|								   C Module
|
|--------------------------------------------------------------------------
|
| Filename   : Cliente.c
| Description: Archivo fuente que contiene la implementación de funciones 
|	       	   utilizadas en un programa cliente que realiza peticiones, 
|			   relacionadas al préstamo de libros, a un servidor que 
|			   gestione dichas peticiones.
|
|--------------------------------------------------------------------------
|
|				Copyright (c) 2022, Universidad del Cauca
|			   				All rights reserved
|
|------------------------------------------------------------------------*/
#include "gestionUsuarios.h"
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>

int mesesDias[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
void clear_newlines(void);
int esAlfanumerica(char *cadena);

/**
 * Funcion que va despues de cada scanf en el que se lea numeros.
 * Sirve para que no se salte los fgets.
 */
void clear_newlines(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
/**
 * Valida si una cadena es alfanumérica.
 * @param cadena Cadena a evaluar.
 * @return 1 si la cadena es alfanumérica, 0 en caso contrario.
*/
int esAlfanumerica(char *cadena)
{
	int i;
	int tieneLetra = 0;
	int tieneNumero = 0;
	for (i = 0; i < strlen(cadena); i++)
	{
		char letra = cadena[i];
		if (isalpha(letra))
		{
			tieneLetra = 1;
		}
		if (isdigit(letra))
		{
			tieneNumero = 1;
		}
	}
	return tieneLetra && tieneNumero;
}

void
gestion_biblioteca_1(char *host)
{
	CLIENT *clnt;
	bool_t  *rta_abrir_sesion;
	datos_credencial  abrir_sesion_arg;
	bool_t  *rta_registrar_usuario;
	datos_usuario  registrar_usuario_arg;
	bool_t  *rta_registrar_prestamo;
	datos_prestamo  registrar_prestamo_arg;
	datos_usuario  *rta_consultar_usuario;
	char * consultar_usuario_arg;
	char * *rta_consultar_libros;
	char * consultar_libros_arg;
	datos_prestamo  *rta_consultar_prestamo;
	char * consultar_prestamo_arg;
	bool_t  *rta_realizar_devolucion;
	datos_prestamo  realizar_devolucion_arg;
	datos_multa  *rta_consultar_multa;
	char * consultar_multa_arg;
	int  *rta_consultar_multas_usuario;
	char * consultar_multas_usuario_arg;
	char * *rta_imprimir_multas;
	char *imprimir_multas_arg;
	bool_t  *rta_cancelar_multa;
	char * cancelar_multa_arg;
	char * *rta_es_dia_devolucion;
	datos_credencial  es_dia_devolucion_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, gestion_biblioteca, gestion_biblioteca_version, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	int op=0, op1=0, op2=0, dia_actual, mes_actual, anio_actual, hay_multas=0;
    char* admin_username = "ingesis";
    char* admin_password = "ingesis";
	char op_devolucion;
    do
	{
        char username[MAXDAT];
        char password[MAXDAT];
		printf("\n======== Menú  Inicio =========\n");
		printf("* 1.-Abrir sesión.            *\n");
		printf("* 2.-Salir.                   *\n");
		printf("===============================\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &op);
        clear_newlines();
        switch(op)
		{
			case 1:
            
			printf("\n======== Abrir  sesión ========\n");
			printf("Ingrese el nombre de usuario: ");
			scanf("%s", &username);
            printf("Ingrese la contraseña del usuario: ");
			scanf("%s", &password);
            strcpy(abrir_sesion_arg.usuario, username);
            strcpy(abrir_sesion_arg.clave, password);
            
            rta_abrir_sesion = abrirsesion_1(&abrir_sesion_arg, clnt);
			if (rta_abrir_sesion == (bool_t *) NULL) {
				clnt_perror (clnt, "call ABRIR SESION failed");
			}
            
            if((strcmp(username,admin_username)==0) && (strcmp(password, admin_password)==0))
			{
                do
				{
                    int es_valido=1;
                    printf("\n========= Menú Admin ==========\n");
                    printf("* 1.-Registrar usuario        *\n");
                    printf("* 2.-Consultar usuario        *\n");
					printf("* 3.-Consultar préstamo       *\n");
					printf("* 4.-Consultar multas         *\n");
					printf("* 5.-Pagar multa              *\n");
                    printf("* 6.-Salir                    *\n");
                    printf("===============================\n");
                    printf("Seleccione una opcion: ");
                    scanf("%d", &op1);
                    clear_newlines();
                    switch(op1)
					{
                        case 1:
                            printf("\n===== Registrar  Usuario  =====");
                            printf("\nIngrese el nombre completo: ");
                            fgets(registrar_usuario_arg.nombreCompleto, MAXNOM, stdin);
                            registrar_usuario_arg.nombreCompleto[strlen(registrar_usuario_arg.nombreCompleto)-1]=' ';//Quita el \n
                            if(esAlfanumerica(registrar_usuario_arg.nombreCompleto)){
                                es_valido = 0;
                            }
                            printf("\n1.-Estudiante\n");
                            printf("2.-Profesor\n");
                            printf("Escoja un valor para la ocupación: ");
                            scanf("%d", &op2);
                            clear_newlines();
                            if(op2<1 || op2>2){
                                es_valido=0;
                            }else if(op2==1){
                                strcpy(registrar_usuario_arg.ocupacion, "Estudiante");
                            }else if(op2==2){
                                strcpy(registrar_usuario_arg.ocupacion, "Profesor");
                            }
                            int flag1=0, flag2=0;
                            do
							{
                                printf("\nIngrese el usuario: ");
                                scanf("%s", &registrar_usuario_arg.usuario);
								if(strlen(registrar_usuario_arg.usuario)<8){
                                    printf("** El usuario debe tener mínimo 8 caracteres alfanuméricos.**");
                                }else{
                                    flag1=1;
                                }
                            }while(flag1==0);
                            do
							{
                                printf("\nIngrese la contraseña: ");
                                scanf("%s", &registrar_usuario_arg.clave);
								if(strlen(registrar_usuario_arg.clave)<8){
                                    printf("** La contraseña debe tener mínimo 8 caracteres alfanuméricos.**");
                                }else{
                                    flag2=1;
                                }
                            }while(flag2==0);
                            
                            if(es_valido==1)
							{
                                rta_registrar_usuario = registrarusuario_1(&registrar_usuario_arg, clnt);
								if (rta_registrar_usuario == (bool_t *) NULL) {
									clnt_perror (clnt, "call REGISTRAR USUARIO failed");
								}else if(*rta_registrar_usuario==TRUE){
                                    printf("\n**Usuario registrado exitósamente**");
                                }else if(*rta_registrar_usuario==FALSE){
                                    printf("\n**Usuario NO registrado, se alcanzó la cantidad máxima de usuarios a registrar**\n");
                                }
                            }else{
                                printf("\n**Usuario NO registrado, error en el formato de los datos ingresados**\n");
                            }
                            
                        break;
                        
                        case 2:
  
                            printf("\n===== Consultar  Usuario =====\n");
							char user_consultar[MAXDAT];
                            printf("\nIngrese el usuario: ");
                            scanf("%s", &user_consultar);
							consultar_usuario_arg = user_consultar;
                            rta_consultar_usuario = consultarusuario_1(&consultar_usuario_arg, clnt);
							if (rta_consultar_usuario == (datos_usuario *) NULL) {
								clnt_perror (clnt, "call CONSULTAR USUARIO failed");
							}else if(strcmp(rta_consultar_usuario->nombreCompleto," ")==0){
                                printf("\n**Usuario NO encontrado**");
                            }else {
                                printf("\nNombre completo: %s\n", (*rta_consultar_usuario).nombreCompleto);
                                printf("Ocupación: %s\n", (*rta_consultar_usuario).ocupacion);
                                printf("Usuario: %s\n", (*rta_consultar_usuario).usuario);
                            }
                            
                        break;

						case 3:
  
                            printf("\n===== Consultar  Pŕestamo =====\n");
							char id_libro_prestamo[MAXID];
							printf("\nIngrese el id del libro: ");
							scanf("%s",&id_libro_prestamo);
							consultar_prestamo_arg = id_libro_prestamo;
							
							rta_consultar_prestamo = consultarprestamo_1(&consultar_prestamo_arg, clnt);
							if (rta_consultar_prestamo == (datos_prestamo *) NULL) {
								clnt_perror (clnt, "call CONSULTAR PRESTAMO failed");
							}else if(strcmp(rta_consultar_prestamo->idLibro, " ")==0){
								printf("\n**Préstamo NO encontrado**");
							}else{
								printf("\nUsuario: %s",rta_consultar_prestamo->usuario);
								printf("\nFecha préstamo: %d/%d/%d",rta_consultar_prestamo->diaPrestamo, rta_consultar_prestamo->mesPrestamo, rta_consultar_prestamo->anioPrestamo);
								printf("\nFecha de plazo máximo de devolución: %d/%d/%d",rta_consultar_prestamo->diaPlazoMax,rta_consultar_prestamo->mesPlazoMax, rta_consultar_prestamo->anioPlazoMax);
								
								printf("\nDesea realizar la devolución?[S/n]: ");
								scanf(" %c",&op_devolucion);
								getchar();
								if(op_devolucion=='S')
								{
									printf("Ingrese el día de devolución: ");
									scanf("%d",&rta_consultar_prestamo->diaDevolucion);
									clear_newlines();
									printf("Ingrese el mes de devolución: ");
									scanf("%d",&rta_consultar_prestamo->mesDevolucion);
									clear_newlines();
									printf("Ingrese el año de devolución: ");
									scanf("%d",&rta_consultar_prestamo->anioDevolucion);
									clear_newlines();

									realizar_devolucion_arg = *rta_consultar_prestamo;
									rta_realizar_devolucion = realizardevolucion_1(&realizar_devolucion_arg, clnt);
									if (rta_realizar_devolucion == (bool_t *) NULL) {
										clnt_perror (clnt, "call failed");
									}else if(*rta_realizar_devolucion==TRUE){
										printf("\n**Devolución realizada exitósamente**\n");
									}else if(*rta_realizar_devolucion==FALSE){
										printf("\n**NO se pudo realizar la devolución**\n");
									}
								}
							}
						break;

						case 4:
							
							printf("\n===== Consultar  Multas =====\n");
							hay_multas = 0;
							rta_imprimir_multas = imprimirmultas_1((void*)&imprimir_multas_arg, clnt);
							if (rta_imprimir_multas == (char **) NULL) {
								clnt_perror (clnt, "call failed");
							}
							if (strcmp(*rta_imprimir_multas, "")==0)
							{
								printf("\nNO hay multas registradas");
							}
							else{
								hay_multas=1;
								printf("\n.=============================.");
								printf("\n| ID multa, Usuario, ID libro |");
								printf("\n'============================='\n");
								printf("%s", *rta_imprimir_multas);
							}
							
							if (hay_multas==1)
							{
								char id_multa[MAXID];
								printf("\nIngrese el id de la multa: ");
								scanf("%s",&id_multa);
								consultar_multa_arg = id_multa;

								rta_consultar_multa = consultarmulta_1(&consultar_multa_arg, clnt);
								if (rta_consultar_multa == (datos_multa *) NULL) {
									clnt_perror (clnt, "call failed");
								}
								else if(strcmp(rta_consultar_multa->idLibro, "")==0)
								{
									printf("\n**Multa NO encontrada**\n");
								}
								else
								{
									printf("\nID Multa: %s", rta_consultar_multa->idMulta);
									printf("\nEl usuario: %s", rta_consultar_multa->usuario);
									printf("\nSolicitó el libro con ID: %s", rta_consultar_multa->idLibro);
									printf("\nFecha de préstamo: %d - %d - %d", rta_consultar_multa->diaPrestamo, rta_consultar_multa->mesPrestamo, rta_consultar_multa->anioPrestamo);
									printf("\nFecha de devolución: %d - %d - %d", rta_consultar_multa->diaDevolucion, rta_consultar_multa->mesDevolucion, rta_consultar_multa->anioDevolucion);
									printf("\nEntregó el libro con %d días de retraso.", rta_consultar_multa->diasRetraso);
									printf("\nTotal a pagar: %d\n", rta_consultar_multa->valorMulta);
								}
							}
							
						break;

						case 5:
							
							printf("\n====== Pagar  Multa ======\n");
							char id_multa_cancelar[MAXID];
							printf("\nIngrese el id de la multa a cancelar: ");
							scanf("%s",&id_multa_cancelar);
							cancelar_multa_arg = id_multa_cancelar;

							rta_cancelar_multa = cancelarmulta_1(&cancelar_multa_arg, clnt);
							if (rta_cancelar_multa == (bool_t *) NULL) {
								clnt_perror (clnt, "call failed");
							}else if(*rta_cancelar_multa==TRUE){
								printf("\n**Multa cancelada exitósamente**\n");
							}else if(*rta_cancelar_multa==FALSE){
								printf("\n**NO se pudo cancelar la Multa**\n");
							}
						break;

                    }//fin switch case de Menu Admin
                }while(op1 != 6);
                
            }else if(*rta_abrir_sesion==TRUE){
				printf("Día de ingreso: ");
                scanf("%d", &abrir_sesion_arg.diaIngreso);
				clear_newlines();
				printf("Mes de ingreso: ");
                scanf("%d", &abrir_sesion_arg.mesIngreso);
				clear_newlines();
				printf("Año de ingreso: ");
                scanf("%d", &abrir_sesion_arg.anioIngreso);
				clear_newlines();
				
				dia_actual = abrir_sesion_arg.diaIngreso;
				mes_actual = abrir_sesion_arg.mesIngreso;
				anio_actual = abrir_sesion_arg.anioIngreso;
				
				es_dia_devolucion_arg = abrir_sesion_arg;
				rta_es_dia_devolucion = esdiadevolucion_1(&es_dia_devolucion_arg, clnt);
				if (rta_es_dia_devolucion == (char **) NULL) {
					clnt_perror (clnt, "call failed");
				}
				else if(strcmp(*rta_es_dia_devolucion,"")!=0)
				{
					printf("\n%s",*rta_es_dia_devolucion);
				}
				
				do
				{
                    printf("\n======== Menú Usuario =========\n");
                    printf("* 1.-Buscar libros            *\n");
					printf("* 2.-Solicitar Préstamo       *\n");
                    printf("* 3.-Salir                    *\n");
                    printf("===============================\n");
                    printf("Seleccione una opcion: ");
                    scanf("%d", &op);
                    clear_newlines();
					switch (op)
					{
					case 1:
						
						printf("\n===== Buscar  libros =====\n");
						printf("\n*Área de conocimiento [Medicina, Calculo, Biologia]\n");
                        printf("*Editorial [Elsevier, McGraw Hill]\n");
						printf("*Autor [R.L. Drake, Kenneth Saladin, Dennis Zill, Sylvia S. Mader, Bruce Carlson]\n");
						char busqueda_libros[MAXNOM];
						printf("\nIngrese el área de conocimiento, editorial o nombre del autor del libro: ");
						fgets(busqueda_libros, MAXNOM, stdin);
						consultar_libros_arg = busqueda_libros;						
						rta_consultar_libros = consultarlibros_1(&consultar_libros_arg, clnt);
						if (rta_consultar_libros == (char **) NULL) {
							clnt_perror (clnt, "call CONSULTAR LIBROS failed");
						}else if(strcmp(*rta_consultar_libros," ")==0){
                            printf("\n**Libro NO encontrado**");
                        }else {
							printf("\n.====================================.");
							printf("\n| ID, Área, Título, Autor, Editorial |");
							printf("\n'===================================='\n");
							printf("%s\n",*rta_consultar_libros);
                        }
						break;
					
					case 2:

						printf("\n===== Solicitar  Préstamo =====\n");
						int puede_solicitar = 1;
						consultar_multas_usuario_arg = username;
						rta_consultar_multas_usuario = consultarmultasusuario_1(&consultar_multas_usuario_arg, clnt);
						if (rta_consultar_multas_usuario == (int *) NULL) {
							clnt_perror (clnt, "call failed");
						}
						else if ((*rta_consultar_multas_usuario>0) && (*rta_consultar_multas_usuario<=20000))
						{
							printf("\n%s tiene %d multas\n", username, *rta_consultar_multas_usuario);
						}
						else if(*rta_consultar_multas_usuario>20000)
						{
							printf("\nEl usuario %s tiene una multa mayor a 20000\nNO es posible solicitar el préstamo.\n", username);
							puede_solicitar = 0;
						}

						if (puede_solicitar==1)
						{
							printf("\nIngrese el id del Libro:");
							scanf("%s", &registrar_prestamo_arg.idLibro);
							strcpy(registrar_prestamo_arg.usuario, username);
							registrar_prestamo_arg.diaPrestamo = dia_actual;
							registrar_prestamo_arg.mesPrestamo = mes_actual;
							registrar_prestamo_arg.anioPrestamo = anio_actual;
							int dia_devolucion, mes_devolucion, anio_devolucion;
							int num_dias_mes = mesesDias[mes_actual-1];
							if((dia_actual+7) > num_dias_mes)
							{
								if(mes_actual==12)
								{
									mes_devolucion = 1;
									anio_devolucion = anio_actual + 1;
								}
								else
								{
									mes_devolucion = mes_actual + 1;
									anio_devolucion = anio_actual;
								}
								dia_devolucion = (dia_actual + 7) - num_dias_mes;
							}
							else
							{
								dia_devolucion = dia_actual + 7;
								mes_devolucion = mes_actual;
								anio_devolucion = anio_actual;
							}
							registrar_prestamo_arg.diaPlazoMax = dia_devolucion;
							registrar_prestamo_arg.mesPlazoMax = mes_devolucion;
							registrar_prestamo_arg.anioPlazoMax = anio_devolucion;

							rta_registrar_prestamo = registrarprestamo_1(&registrar_prestamo_arg, clnt);
							if (rta_registrar_prestamo == (bool_t *) NULL) {
								clnt_perror (clnt, "call REGISTRAR PRESTAMO failed");
							}else if(*rta_registrar_prestamo==TRUE){
								printf("\n**Préstamo registrado exitósamente**\n");
							}else if(*rta_registrar_prestamo==FALSE){
								printf("\n**Préstamo NO registrado**\n");
							}
						}
						
						break;
					}	
						
                }while(op != 3);   
                
            }else if(*rta_abrir_sesion==FALSE){
                printf("\n**El usuario %s no está autorizado para ingresar al sistema.**\n", username);
                printf("\n**Verificar que el usuario y la clave sean las correctas.**\n");
            }
			break;//fin case 1 de Menu Inicio

			case 2:
                printf("\nExit...\n");
			break;
			
		}//fin switch case de Menu Inicio
        
	}while(op != 2);//fin do while principal

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	gestion_biblioteca_1 (host);
exit (0);
}
