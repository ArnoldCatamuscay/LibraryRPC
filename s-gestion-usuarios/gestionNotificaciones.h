/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _GESTIONNOTIFICACIONES_H_RPCGEN
#define _GESTIONNOTIFICACIONES_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define gestion_notificaciones 0x20000022
#define gestion_notificaciones_version 1

#if defined(__STDC__) || defined(__cplusplus)
#define enviarNotificacionAcceso 1
extern  bool_t * enviarnotificacionacceso_1(char **, CLIENT *);
extern  bool_t * enviarnotificacionacceso_1_svc(char **, struct svc_req *);
extern int gestion_notificaciones_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define enviarNotificacionAcceso 1
extern  bool_t * enviarnotificacionacceso_1();
extern  bool_t * enviarnotificacionacceso_1_svc();
extern int gestion_notificaciones_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_GESTIONNOTIFICACIONES_H_RPCGEN */
