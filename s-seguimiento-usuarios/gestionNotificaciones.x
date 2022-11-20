/*Declaración de datos a transferir entre el servidor-gestion-pacientes(role cliente) y el servidor seguimiento pacientes*/
/*Definición de las operaciones que se pueden realizar*/
program gestion_notificaciones{

version gestion_notificaciones_version{
    bool enviarNotificacionAcceso(string mensaje)=1;
    }=1;
}=0x20000022;
