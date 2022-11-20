/*Declaracion de datos a transferir entre el cliente y el servidor gestion usuarios*/
/*Declaracion de constantes*/
const MAXNOM = 30;
const MAXDAT = 20;
const MAXID = 10;
/*Declaracion de la estructura que permite almacenar los datos de una fecha*/
struct datos_fecha {
    int dia;
    int mes;
    int anio;
};
/*Declaracion de la estructura que permite almacenar los datos del usuario*/
struct datos_usuario{
    char nombreCompleto[MAXNOM];
    char ocupacion[MAXDAT];
    char usuario[MAXDAT];
    char clave[MAXDAT];
};
/*Declaración de la estructura que permite almacenar los datos de las credenciales de acceso de los usuarios*/
struct datos_credencial{
    char usuario[MAXDAT];
    char clave[MAXDAT];
    int diaIngreso;
    int mesIngreso;
    int anioIngreso;
};
/*Declaración de la estructura que permite almacenar los datos de un prestamo*/
struct datos_prestamo {
    char usuario[MAXDAT];
    char idLibro[MAXID];
    int diaPrestamo;
    int mesPrestamo;
    int anioPrestamo;
    int diaPlazoMax;
    int mesPlazoMax;
    int anioPlazoMax;
    int diaDevolucion;
    int mesDevolucion;
    int anioDevolucion;
};
/*Declaración de la estructura que permite almacenar los datos de un libro*/
struct datos_libro {
    char id[MAXID];
    char areaConocimiento[MAXDAT];
    char nombre[MAXNOM];
    char autor[MAXNOM];
    char editorial[MAXDAT];
};
/*Declaración de la estructura que permite almacenar los datos de un libro*/
struct datos_multa {
    char idMulta[MAXID];
    char usuario[MAXDAT];
    char idLibro[MAXID];
    int diaPrestamo;
    int mesPrestamo;
    int anioPrestamo;
    int diaDevolucion;
    int mesDevolucion;
    int anioDevolucion;
    int diasRetraso;
    int valorMulta;
};
/*Definicion de las operaciones que se pueden realizar*/
program gestion_biblioteca{
    version gestion_biblioteca_version{
        bool abrirSesion(datos_credencial objCredencial)=1;
        bool registrarUsuario(datos_usuario objUsuario)=2;
        bool registrarPrestamo(datos_prestamo objPrestamo)=3;
        datos_usuario consultarUsuario(string usuario)=4;
        string consultarLibros(string tipoBusqueda)=5;
        datos_prestamo consultarPrestamo(string idLibro)=6;
        bool realizarDevolucion(datos_prestamo objPrestamo)=7;
        datos_multa consultarMulta(string idMulta)=8;
        int consultarMultasUsuario(string usuario)=9;
        string imprimirMultas()=10;
        bool cancelarMulta(string idMulta)=11;
        string esDiaDevolucion(datos_credencial objCredencial)=12;
    }=1;
}=0x20000021;
