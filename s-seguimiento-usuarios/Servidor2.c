/*-------------------------------------------------------------------------
|
|								   C Module
|
|--------------------------------------------------------------------------
|
| Filename   : Servidor2.c
| Description: Archivo fuente de un servidor el cual contiene la   
|	       	   implementación de una funcion que recibe una notificación
|			   proveniente otro servidor.
|
|--------------------------------------------------------------------------
|
|				Copyright (c) 2022, Universidad del Cauca
|			   				All rights reserved
|
|------------------------------------------------------------------------*/
#include "gestionNotificaciones.h"

/**
 * Imprime una notificación.
 * @param argp Cadena con la información a imprimir.
 * @param rqstp Solicitud de servicio.
 * @return TRUE si la cadena no está vacía, FALSE en caso contrario.
*/
bool_t *
enviarnotificacionacceso_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t  result;

	printf("\n --------Invocando a Enviar Notificacion\n");
	if(strcmp(*argp,"")!=0)
	{
        printf("\n%s\n",*argp);
        result = TRUE;
    }
	else{
        result = FALSE;
    }

	return &result;
}
