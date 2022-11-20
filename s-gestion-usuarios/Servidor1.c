/*-------------------------------------------------------------------------
|
|								   C Module
|
|--------------------------------------------------------------------------
|
| Filename   : Servidor1.c
| Description: Archivo fuente que contiene la implementación de funciones 
|	       	   utilizadas en un programa servidor, de una biblioteca,   
|			   que recibe y gestiona peticiones de un cliente.
|
|--------------------------------------------------------------------------
|
|				Copyright (c) 2022, Universidad del Cauca
|			   				All rights reserved
|
|------------------------------------------------------------------------*/
#include "gestionUsuarios.h"
#include "gestionNotificaciones.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
/* Arreglo de una estructura de libros, contiene la información de los libros disponibles */
struct datos_libro libros[5] = {{"1001","Medicina","Anatomía Básica","R.L. Drake","Elsevier"},
							{"1002","Medicina","Anatomía y fisiología","Kenneth Saladin","McGraw Hill"},
							{"1003","Calculo","Calculo Diferencial","Dennis Zill","McGraw Hill"},
							{"1004","Biologia","Biología","Sylvia S. Mader","McGraw Hill"},
							{"1005","Biologia","Biología del desarrollo","Bruce Carlson","Elsevier"}};
/* Cadena con los libros filtrados por área de conocimiento */
char area_conocimiento[100];
/* Cadena con los libros filtrados por editorial */
char editorial[100];
/* Cadena con los libros filtrados por autor */
char libro_autor[100];
/* Cadena con la información retornada en la funcion esdiadevolucion_1_svc */
char info_dev[100];
/* Cadena que contiene el id, usuario e id del libro asociados a cada multa*/
char info[100];
/* Número de multas de un usuario */
int numMultas;
/* Arreglo que tiene cuantos días tiene cada mes */
int mesDias[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* Arreglo de tipo datos_usuario para almacenar hasta 5 usuarios en el servidor */
datos_usuario vectorDeUsuarios[5];

int consultaVector_username(char* username);
int consultaVector_credenciales(datos_credencial *credenciales);
/* Cantidad de usuarios registrados */
int posUsuario=0;
/* Arreglo de tipo datos_prestamo para almacenar hasta 5 préstamos en el servidor */
datos_prestamo vectorDePrestamos[5];
int consultaVector_prestamos(char* id_libro);
int consultaVector_prestamos_usuario(char* p_usuario);
/* Cantidad de préstamos registrados */
int posPrestamo=0;
/* Arreglo de tipo datos_multa para almacenar hasta 10 multas en el servidor */
datos_multa vectorDeMultas[10];
int calcularMulta(datos_fecha fecha_plazo_max, datos_fecha fecha_devolucion);
int consultaVector_multas(char* p_id_multa);
/* Cantidad de multas registradas */
int posMulta=0;

int contarAniosBisiestos(datos_fecha d);
int numDias(datos_fecha fecha1, datos_fecha fecha2);

/**
 * Verifica si un usuario está autorizado para ingresar al sistema y envía una 
 * notificación a otro servidor de seguimiento de usuarios.
 * @param argp Struct con la informacion de inicio de sesión.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si el usuario puede ingresar, FALSE en caso contrario.
*/
bool_t *
abrirsesion_1_svc(datos_credencial *argp, struct svc_req *rqstp)
{
	static bool_t  result;
    char * host = "127.0.0.1";
	CLIENT *clnt;
	bool_t  *rta_enviar_notificacion_acceso;
	char * enviar_notificacion_acceso_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, gestion_notificaciones, gestion_notificaciones_version, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	printf("\n --------Invocando a Abrir Sesion\n");
    char* nombre_completo =  malloc(sizeof(char)*30);
    char* ocupacion = malloc(sizeof(char)*20);
    if(strcmp(argp->usuario, "ingesis")==0 && strcmp(argp->clave, "ingesis")==0){
		enviar_notificacion_acceso_arg = "Bienvenido ingesis";
    }else{
        int pos = consultaVector_credenciales(argp);
        
        if(pos!=-1){
            strcpy(nombre_completo, vectorDeUsuarios[pos].nombreCompleto);
            strcpy(ocupacion, vectorDeUsuarios[pos].ocupacion);
            char mensaje[200];
            strcpy(mensaje," ");
            strcat(mensaje, "El personal ");
            strcat(mensaje, nombre_completo);
            strcat(mensaje, " y ocupacion ");
            strcat(mensaje, ocupacion);
            strcat(mensaje, " está autorizado para ingresar al sistema.");
            enviar_notificacion_acceso_arg = mensaje;
        }else{
            enviar_notificacion_acceso_arg = "";
        }
    }
    
	rta_enviar_notificacion_acceso = enviarnotificacionacceso_1(&enviar_notificacion_acceso_arg, clnt);
	if (rta_enviar_notificacion_acceso == (bool_t *) NULL) {
		clnt_perror (clnt, "call ENVIAR NOTIFICACION failed");
	}else if(*rta_enviar_notificacion_acceso==TRUE){
        result = TRUE;
    } else{
        result = FALSE;
    }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
    
	return &result;
}

/**
 * Registra un usuario en el servidor.
 * @param argp Struct con la información del usuario a registrar.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si se pudo registrar el usuario, FALSE en caso contrario.
*/
bool_t *
registrarusuario_1_svc(datos_usuario *argp, struct svc_req *rqstp)
{
	static bool_t  result;

	printf("\n --------Invocando a Registrar Usuario\n");
	if (posUsuario<5)
	{
		vectorDeUsuarios[posUsuario]=*argp;
		result = TRUE;
		posUsuario++;
		printf("\n --------Usuario Registrado\n");
	}
	else
    {
		printf("\n --------Cantidad maxima de registros alcanzada\n");
		result=FALSE;
	}

	return &result;
}

/**
 * Registra un préstamo en el servidor.
 * @param argp Struct con la información del préstamo a registrar.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si se pudo registra el préstamo, FALSE en caso contrario.
*/
bool_t *
registrarprestamo_1_svc(datos_prestamo *argp, struct svc_req *rqstp)
{
	static bool_t  result;

	printf("\n --------Invocando a Registrar Prestamo\n");
	if (posPrestamo<5)
	{
		vectorDePrestamos[posPrestamo]=*argp;
		result = TRUE;
		posPrestamo++;
		printf("\n --------Prestamo Registrado\n");
	}
	else
    {
		printf("\n --------Cantidad maxima de registros alcanzada\n");
		result=FALSE;
	}

	return &result;
}

/**
 * Consulta un usuario registrado en el servidor.
 * @param argp Nombre del usuario.
 * @param rqstp Solicitud de servicio.
 * @return Struct con la información del usuario.
*/
datos_usuario *
consultarusuario_1_svc(char **argp, struct svc_req *rqstp)
{
	static datos_usuario  result;

	strcpy(result.nombreCompleto, " ");
    int pos = consultaVector_username(*argp);
    printf("\n --------Invocando a Consultar Usuario\n");
    if (pos!=-1)
	{
        result = vectorDeUsuarios[pos];
        printf("\n --------Usuario Encontrado\n");
    }

	return &result;
}

/**
 * Consulta los libros disponibles por área de conocimiento, editorial o autor.
 * @param argp Filtro de búsqueda.
 * @param rqstp Solicitud de servicio.
 * @return Cadena con la información de los libros que contienen el parámetro de búsqueda.
*/
char **
consultarlibros_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	printf("\n --------Invocando a Consultar Libros\n");    
	int tam = strlen(*argp) - 1;
    if ((strncmp(*argp,"Medicina", tam)==0) || (strncmp(*argp,"Calculo", tam)==0) || (strncmp(*argp,"Biologia", tam)==0))
	{
		strcpy(area_conocimiento,"");
		for (int i = 0; i < 5; i++)
		{
			if(strncmp(libros[i].areaConocimiento,*argp, tam)==0)
			{
				strcat(area_conocimiento, libros[i].id);
				strcat(area_conocimiento, ", ");
				strcat(area_conocimiento, libros[i].areaConocimiento);
				strcat(area_conocimiento, ", ");
				strcat(area_conocimiento, libros[i].nombre);
				strcat(area_conocimiento, ", ");
				strcat(area_conocimiento, libros[i].autor);
				strcat(area_conocimiento, ", ");
				strcat(area_conocimiento, libros[i].editorial);
				strcat(area_conocimiento, "\n");
			}
		}
		result = area_conocimiento;
    }
	else if ((strncmp(*argp,"Elsevier", tam)==0) || (strncmp(*argp,"McGraw Hill", tam)==0))
	{
		strcpy(editorial,"");
		for (int i = 0; i < 5; i++)
		{
			if(strncmp(libros[i].editorial,*argp, tam)==0)
			{
				strcat(editorial, libros[i].id);
				strcat(editorial, ", ");
				strcat(editorial, libros[i].areaConocimiento);
				strcat(editorial, ", ");
				strcat(editorial, libros[i].nombre);
				strcat(editorial, ", ");
				strcat(editorial, libros[i].autor);
				strcat(editorial, ", ");
				strcat(editorial, libros[i].editorial);
				strcat(editorial, "\n");
			}
		}
		result = editorial;
	}
	else
	{		
		strcpy(libro_autor," ");
		for (int i = 0; i < 5; i++)
		{
			if(strncmp(libros[i].autor,*argp, tam)==0)
			{
				strcat(libro_autor, libros[i].id);
				strcat(libro_autor, ", ");
				strcat(libro_autor, libros[i].areaConocimiento);
				strcat(libro_autor, ", ");
				strcat(libro_autor, libros[i].nombre);
				strcat(libro_autor, ", ");
				strcat(libro_autor, libros[i].autor);
				strcat(libro_autor, ", ");
				strcat(libro_autor, libros[i].editorial);
				strcat(libro_autor, "\n");
			}
		}
		result = libro_autor;
	}
	return &result;
}

/**
 * Consulta un préstamo registrado en el servidor.
 * @param argp ID del libro que se prestó.
 * @param rqstp Solicitud de servicio.
 * @return Struct con la información del préstamo.
*/
datos_prestamo *
consultarprestamo_1_svc(char **argp, struct svc_req *rqstp)
{
	static datos_prestamo  result;

	strcpy(result.idLibro," ");
    int pos = consultaVector_prestamos(*argp);
    printf("\n --------Invocando a Consultar Préstamo\n");
    if (pos!=-1)
	{
        result = vectorDePrestamos[pos];
        printf("\n --------Préstamo Encontrado\n");
    }

	return &result;
}

/**
 * Elimina un préstamo del servidor al hacer la devolución de un libro. Registra una multa si se devolvió con retraso.
 * @param argp Struct con la información del préstamo.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si se pudo realizar la devolución, FALSE en caso contrario.
*/
bool_t *
realizardevolucion_1_svc(datos_prestamo *argp, struct svc_req *rqstp)
{
	static bool_t  result;

	result = FALSE;
	char * id_libro = (*argp).idLibro;
    printf("\n --------Invocando a Realizar Devolución\n");
	datos_fecha fecha_prestamo = { argp->diaPrestamo, argp->mesPrestamo, argp->anioPrestamo };
	datos_fecha fecha_plazo_max = { argp->diaPlazoMax, argp->mesPlazoMax, argp->anioPlazoMax };
	datos_fecha fecha_devolucion_realizada = { argp->diaDevolucion, argp->mesDevolucion, argp->anioDevolucion};
	if(numDias(fecha_prestamo, fecha_devolucion_realizada)>7)
	{
		int multa = 0;
		multa = calcularMulta(fecha_plazo_max, fecha_devolucion_realizada);
		vectorDeMultas[posMulta].idMulta[0] = (posMulta + 1) + '0';
		strcpy(vectorDeMultas[posMulta].usuario, argp->usuario);
		strcpy(vectorDeMultas[posMulta].idLibro, argp->idLibro);
		vectorDeMultas[posMulta].diaPrestamo = argp->diaPrestamo;
		vectorDeMultas[posMulta].mesPrestamo = argp->mesPrestamo;
		vectorDeMultas[posMulta].anioPrestamo = argp->anioPrestamo;
		vectorDeMultas[posMulta].diaDevolucion = argp->diaDevolucion;
		vectorDeMultas[posMulta].mesDevolucion = argp->mesDevolucion;
		vectorDeMultas[posMulta].anioDevolucion = argp->anioDevolucion;
		vectorDeMultas[posMulta].diasRetraso = numDias(fecha_plazo_max, fecha_devolucion_realizada);
		vectorDeMultas[posMulta].valorMulta = multa;
		posMulta++;
	}
    
	int pos = consultaVector_prestamos(id_libro);
	if (pos!=-1)
	{
        strcpy(vectorDePrestamos[pos].usuario, "");
		strcpy(vectorDePrestamos[pos].idLibro, "");
		vectorDePrestamos[pos].diaPrestamo = 0;
		vectorDePrestamos[pos].mesPrestamo = 0;
		vectorDePrestamos[pos].anioPrestamo = 0;
		vectorDePrestamos[pos].diaDevolucion = 0;
		vectorDePrestamos[pos].mesDevolucion = 0;
		vectorDePrestamos[pos].anioDevolucion = 0;
		posPrestamo --;
		result = TRUE;
        printf("\n --------Devolución Realizada\n");
    }

	return &result;
}

/**
 * Consulta una multa registrada en el servidor.
 * @param argp ID de la multa
 * @param rqstp Solicitud de servicio.
 * @return Struct con la información de la multa.
*/
datos_multa *
consultarmulta_1_svc(char **argp, struct svc_req *rqstp)
{
	static datos_multa  result;

	int pos = consultaVector_multas(*argp);
    printf("\nLa posición es: %d", pos);
	printf("\n --------Invocando a Consultar Multa\n");
    
	if (pos!=-1)
	{
		result = vectorDeMultas[pos];
        printf("\n --------Multa Encontrada\n");		
	}
	else{
		strcpy(result.idLibro, "");
	}

	return &result;
}

/**
 * Consulta el número de multas de un usuario o si tiene alguna que sea mayor a $20.000
 * @param argp Nombre de usuario.
 * @param rqstp Solicitud de servicio
 * @return Número de multas de un usuario, o si tiene una multa mayor a 20.000 se retorna el valor de esa multa.
*/
int *
consultarmultasusuario_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	printf("\n --------Invocando a Consultar Multas Usuario");
	numMultas = 0;
	int i;
	for (i=0 ;  i<posMulta ; i++)
	{
		printf("\nIteró %d veces", i+1);
		if (strcmp(vectorDeMultas[i].usuario, *argp)==0)
		{
			printf("\nEntró al if de comparación del usuario");
            if(vectorDeMultas[i].valorMulta>20000)
			{
				printf("\nEntró al if de comparación de multa mayor a 20000");
				numMultas = vectorDeMultas[i].valorMulta;
				break;
			}
			numMultas++;
		}
	}
	result = numMultas;

	return &result;
}

/**
 * Consulta id, usuario e id de las multas registradas en el servidor.
 * @param argp Vacío.
 * @param rqstp Solicitud de servicio.
 * @return Cadena con el id, usuario e id del libro de cada multa.
*/
char **
imprimirmultas_1_svc(void *argp, struct svc_req *rqstp)
{
	static char * result;

	printf("\n --------Invocando a Imprimir Multas\n");
	strcpy(info,"");
	if(posMulta>0)
	{
		int i;
		for (i=0 ;  i<posMulta ; i++)
		{
			strcat(info, "\n");
			strcat(info, vectorDeMultas[i].idMulta);
			strcat(info, " - ");
			strcat(info, vectorDeMultas[i].usuario);
			strcat(info, " - ");
			strcat(info, vectorDeMultas[i].idLibro);
			strcat(info, "\n");
		}
	}
	result = info;

	return &result;
}

/**
 * Elimina una multa registrada en el servidor.
 * @param argp ID de la multa.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si se canceló la multa, FALSE en caso contrario.
*/
bool_t *
cancelarmulta_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t  result;
	
	result = FALSE;
	printf("\n --------Invocando a Cancelar Multa\n");
	int pos = consultaVector_multas(*argp);
	if (pos!=-1)
	{
		strcpy(vectorDeMultas[pos].idMulta, "");
        strcpy(vectorDeMultas[pos].usuario, "");
		strcpy(vectorDeMultas[pos].idLibro, "");
		vectorDeMultas[pos].diaPrestamo = 0;
		vectorDeMultas[pos].mesPrestamo = 0;
		vectorDeMultas[pos].anioPrestamo = 0;
		vectorDeMultas[pos].diaDevolucion = 0;
		vectorDeMultas[pos].mesDevolucion = 0;
		vectorDeMultas[pos].anioDevolucion = 0;
		vectorDeMultas[pos].valorMulta = 0;
		posMulta--;
		result = TRUE;
        printf("\n --------Multa Cancelada\n");
    }

	return &result;
}

/**
 * Verifica si la fecha en la que ingresa un usuario coincide con la fecha de plazo máximo de un préstamo realizado.
 * @param argp Struct con la información de inicio de sesión, la cual contiene la fecha de ingreso del usuario
 * @param rqstp Solicitud de servicio
 * @return Cadena con un mensaje indicando que ya es el día de devolución de un libro.
*/
char **
esdiadevolucion_1_svc(datos_credencial *argp, struct svc_req *rqstp)
{
	static char * result;
	printf("\n --------Invocando a Es día de Devolución\n");
	
	strcpy(info_dev,"");
	for (int i = 0; i < 5; i++)
	{
		if (strcmp(argp->usuario,vectorDePrestamos[i].usuario)==0)
		{
			if ((argp->diaIngreso == vectorDePrestamos[i].diaPlazoMax) && (argp->mesIngreso == vectorDePrestamos[i].mesPlazoMax) && (argp->anioIngreso == vectorDePrestamos[i].anioPlazoMax))
			{
				strcat(info_dev,"\nFecha de devolución alcanzada\n**ID del libro a entregar: ");
				strcat(info_dev,vectorDePrestamos[i].idLibro);
				strcat(info_dev,"\n");
			}
		}
		
	}
	result = info_dev;

	return &result;
}

/**
 * Cuenta el número de años bisiestos antes de un determinada fecha.
 * @param fecha Fecha dada.
 * @return Número de años bisiestos hasta la fecha dada.
*/
int contarAniosBisiestos(datos_fecha fecha)
{
    int anios = fecha.anio;

    if (fecha.mes <= 2)
	{
        anios--;
    }

    return anios / 4 - anios / 100 + anios / 400;
}

/**
 * Calcula el número de días que hay entre dos fechas.
 * @param fecha1 Fecha inicial.
 * @param fecha2 Fecha final.
 * @return Número de días que hay entre la fecha inicial y la fecha final
*/
int numDias(datos_fecha fecha1, datos_fecha fecha2)
{
    long int n1 = fecha1.anio * 365 + fecha1.dia;
 
    for (int i = 0; i < fecha1.mes - 1; i++)
	{
        n1 += mesDias[i];
    }

    n1 += contarAniosBisiestos(fecha1);
  
    long int n2 = fecha2.anio * 365 + fecha2.dia;
    for (int i = 0; i < fecha2.mes - 1; i++)
	{
        n2 += mesDias[i];
    }
    n2 += contarAniosBisiestos(fecha2);

    return (n2 - n1);
}

/**
 * Consulta un usuario por su nombre de usuario.
 * @param p_username Nombre de usuario
 * @return Posición en el vector de usuarios o -1 si no se encontró el usuario.
*/
int consultaVector_username(char* p_username)
{
	int resultado=-1;
	int i ;
	for (i=0 ;  i<5 ; i++)
	{
		if (strcmp(vectorDeUsuarios[i].usuario,p_username)==0)
		{
            if(strcmp(vectorDeUsuarios[i].usuario,"")==0)
			{
                resultado=-1;
            }
			resultado=i;
			break;
		}	
	}
	return resultado;
}

/**
 * Valida que al iniciar sesión, el usuario y clave sean de un usuario en el arreglo de usuarios.
 * @param p_credenciales Apuntador a un struct con la información de las credenciales de inicio de sesión.
 * @return Posición en el vector de usuarios o -1 si no se encontró el usuario.
*/
int consultaVector_credenciales(datos_credencial *p_credenciales)
{
	int resultado=-1;
	int i ;
	for (i=0 ;  i<5 ; i++)
	{
		if ((strcmp(vectorDeUsuarios[i].usuario,(*p_credenciales).usuario)==0) && (strcmp(vectorDeUsuarios[i].clave,(*p_credenciales).clave)==0))
		{
            if(strcmp(vectorDeUsuarios[i].usuario,"")==0)
			{
                resultado=-1;
            }
			resultado=i;
			break;
		}	
	}
	return resultado;
}

/**
 * Consulta la posición de un préstamo en el arreglo de prestamos.
 * que se prestó. Si no se encuntra un préstamo con ese libro retorna -1.
 * @param p_id_libro ID del libro que se prestó.
 * @return Posición en el vector de préstamos o -1 si no se encontró el préstamo.
*/
int consultaVector_prestamos(char* p_id_libro)
{
	int resultado=-1;
	int i ;
	for (i=0 ;  i<5 ; i++)
	{
		if (strcmp(vectorDePrestamos[i].idLibro,p_id_libro)==0)
		{
            if(strcmp(vectorDePrestamos[i].idLibro,"")==0)
			{
                resultado=-1;
            }
			resultado=i;
			break;
		}	
	}
	return resultado;
}

/**
 * Calcula la multa a pagar tomando en cuanta el número de días que se retrasó el usuario en
 * entregar el libro.
 * @param fecha_plazo_max Fecha del plazo máximo que tenía el usuario para devolver el libro
 * @param fecha_devolucion Fecha en la que el usuario devolvió el libro
 * @return El valor de la multa.
*/
int calcularMulta(datos_fecha fecha_plazo_max, datos_fecha fecha_devolucion)
{
	int multa=0;
	int dias_atraso = numDias(fecha_plazo_max, fecha_devolucion);
	if(dias_atraso<4)
	{
		multa = 10000;
	}
	else if( (dias_atraso>=4) && (dias_atraso<=8) )
	{
		multa = 30000 + ( 1000*(dias_atraso - 3) );
	}
	else if( dias_atraso>8 )
	{
		multa = 2000 * dias_atraso;
	}
	return multa;
}

/**
 * Consulta la posición de una multa en el arreglo de multas.
 * @param p_id_multa El id de la multa a buscar.
 * @return Posición en el vector de multas o -1 si no se encontró la multa.
*/
int consultaVector_multas(char* p_id_multa)
{
	int posicion=-1;
	int i ;
	for (i=0 ;  i<posMulta ; i++)
	{
		if (strcmp(vectorDeMultas[i].idMulta, p_id_multa)==0)
		{
			posicion=i;
			break;
		}	
	}
	return posicion;
}