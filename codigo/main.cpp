#include "main.h"

using namespace std; 

//*****************************VARIABLES GLOBALES*****************************//

static istream *iss = 0;	// Input Stream (clase para manejo de los flujos de entrada)
static ostream *oss = 0;	// Output Stream (clase para manejo de los flujos de salida)
static fstream ifs; 		// Input File Stream (derivada de la clase ifstream que deriva de istream para el manejo de archivos)
static fstream ofs;		// Output File Stream (derivada de la clase ofstream que deriva de ostream para el manejo de archivos)

static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{1, "f", "function", NULL, opt_function, OPT_DEFAULT},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, },
};

enum functions {z, expz, conjugar, inversa, logaritmo, seno, pow2};
static functions chosen_function = z;

// **********************************MAIN**********************************//

int main(int argc, char * const argv[]){
	image input_image;

	cmdline cmdl(options);	       // Objeto con parametro tipo option_t (struct) declarado globalmente. Ver línea 51 main.cc
	cmdl.parse(argc, argv);        // Metodo de parseo de la clase cmdline

	if(!read_pgm(iss, input_image)){    // Se lee la imagen de intrada
		cerr<<"Fallo en el archivo"<<endl;
		return 1;
	}
	
  // Se declara la imagen de salida a partir de las dimenciones de la imagen de entrada
	image output_image(input_image.get_max_dim(),input_image.get_max_dim(),input_image.get_greyscale());

  // Switch que, a partir del parámetro ingresado por consola, define que función implementar mediante puntero a función
	switch(chosen_function){  
		case z:                  
			input_image.print_image(oss);
			return 0;
		break;
		case expz: 
      map_image(input_image, output_image, &complejo::exponencial);
    break;
		case conjugar:
			map_image(input_image, output_image, &complejo::conjugar);
			break;
		case inversa:
			map_image(input_image, output_image, &complejo::inversa);
		break;
		case logaritmo:
			map_image(input_image, output_image, &complejo::logaritmo);
		break;
		case seno:
			map_image(input_image, output_image, &complejo::seno);
		break;
		case pow2:
			map_image(input_image, output_image, &complejo::pow2);
		break;
		default: 
			cerr<< "Error en seleccion de funcion" << endl;
			return 1;
	}

  // Se imprime la imagen de salida
  oss << output_image;

	return 0;
}

//************************FUNCIONES DE CMDLINE************************//

static void opt_input(string const &arg){

	if (arg == "-") {
		iss = &cin;		// Se establece la entrada estandar cin como flujo de entrada
	}
	else {
		ifs.open(arg.c_str(), ios::in);
		iss = &ifs;
	}
	// Verificamos que el stream este OK.
	if (!iss->good()) {
		cerr << "No se puede abrir el archivo de entrada: "<< arg<< "."<< endl;
		exit(1);
	}
}

static void opt_output(string const &arg){

	if (arg == "-") {	// Si el input es -,
		oss = &cout;	// se establece la salida estandar cout como flujo de salida
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
	}
	if (!oss->good()) {
		cerr << "No se puede abrir el archivo de salida: "<< arg<< "."<< endl;
		exit(1);
	}
}

static void opt_function(string const &arg){

  if (arg == FUNCTION_Z || arg == "-") {chosen_function = z;} // Se establece la función estandar z

  else if (arg == FUNCTION_EXPZ) {chosen_function = expz; }
  else if (arg == FUNCTION_CONJUGAR) {chosen_function = conjugar; }
  else if (arg == FUNCTION_INVERSA) {chosen_function = inversa; } 
  else if (arg == FUNCTION_LOGARITMO) {chosen_function = logaritmo; }
  else if (arg == FUNCTION_SENO) {chosen_function = seno; }
  else if (arg == FUNCTION_POW) {chosen_function = pow2; }
  else {
    cerr << "Funcion invalida" << endl;
    exit(1);
  }
}

static void opt_help(string const &arg){  // La opción -h imprime el formato de ejecución
  cout << "cmdline -f 'function' [-i file] [-o file]" << endl;
  cout << "funciones: z, exp(z), conjugar, inversa, log(z), sin(z), pow(z) (recordar usar comillas) " << endl;
	exit(0);
}

//***********************METODOS DE CMDLINE***********************//

cmdline::cmdline(){}

cmdline::cmdline(option_t *table) : option_table(table){}

void cmdline::parse(int argc, char * const argv[]) {
#define END_OF_OPTIONS(p)       \
	((p)->short_name == 0   \
	 && (p)->long_name == 0 \
	 && (p)->parse == 0)

	for (option_t *op = option_table; !END_OF_OPTIONS(op); ++op)
		op->flags &= ~OPT_SEEN;

	for (int i = 1; i < argc; ++i) {

		if (argv[i][0] != '-') {
			cerr << "Argumento inválido." << argv[i] << endl;
			exit(1);
		}

		if (argv[i][1] == '-'
		    && argv[i][2] == 0)
			break;

		if (argv[i][1] == '-')
			i += do_long_opt(&argv[i][2], argv[i + 1]);
		else
			i += do_short_opt(&argv[i][1], argv[i + 1]);
	}

	for (option_t *op = option_table; !END_OF_OPTIONS(op); ++op) {
#define OPTION_NAME(op) \
	(op->short_name ? op->short_name : op->long_name)
		if (op->flags & OPT_SEEN)
			continue;
		if (op->flags & OPT_MANDATORY) {
			cerr << "Opción "<< "-"<< OPTION_NAME(op)<< " es obligatoria."<< "\n";
			exit(1);
		}
		if (op->def_value == 0)
			continue;
		op->parse(string(op->def_value));
	}
}

int cmdline::do_long_opt(const char *opt, const char *arg) {
	// Se recorre la tabla de opciones, y se busca la entrada larga que se corresponda 
  // con la opción de línea de comandos. De no encontrarse, se indica el error correspondiente.
	
	for (option_t *op = option_table; op->long_name != 0; ++op) {
		if (string(opt) == string(op->long_name)) {
			// Se marca esta opción como usada en forma explícita, para evitar tener que inicializarla con el valor por defecto.

			op->flags |= OPT_SEEN;

			if (op->has_arg) {
				if (arg == 0) { // Verificamos que se provea un argumento
					cerr << "La opción requiere el argumento: "<< "--"<< opt<< "\n";
					exit(1);
				}
				op->parse(string(arg));
				return 1;
			} else {
				op->parse(string("")); // Opción sin argumento.
				return 0;
			}
		}
	}
	// Error: opción no reconocida.
	cerr << "Opción desconocida: "<< "--"<< opt<< "."<< endl;
	exit(1);
	return -1;
}

int cmdline::do_short_opt(const char *opt, const char *arg) {
	option_t *op;

	for (op = option_table; op->short_name != 0; ++op) {

		if (string(opt) == string(op->short_name)) {
			op->flags |= OPT_SEEN;
			if (op->has_arg) {
				if (arg == 0) {	
					cerr << "La opción requiere el argumento: "<< "-"<< opt<< "\n";
					exit(1);
				}
				op->parse(string(arg));
				return 1;
			} else {
				op->parse(string(""));
				return 0;
			}
		}
	}
	cerr << "Opción desconocida: "<< "-"<< opt << "." << endl;
	exit(1);
	return -1;
}


// *******************************FUNCIONES**********************************//


void map_image(image & original, image & destino, complejo(complejo::*function_pointer)(void)){

  int pos[2];
  int aux_color;
  double max = original.get_max_dim();
  complejo aux;

  double paso= 2/(max-1);  // Determina el paso que debe haber debido al salto de una posicion para que en los limites se encutren los unos
  double aux_real=-1;
  double aux_imag=1;

  // Se recorre la matriz de complejos para transformar cada uno de los puntos.
  // Los indices de la matriz de complejos coinciden con los de la matriz destino, por lo tanto 
  // alcanza con recorrer solo una de las dos.
  for(int i=0 ; i < max ; i++){

    for (int j = 0 ; j < max ; j++)
    {

      // Se crea un complejo a el cual se le va a realizar la transformacion
      complejo aux (aux_real,aux_imag);

      // Se transforma el complejo
      aux = (aux.*function_pointer)();

      // Se corrobora que el valor c a buscar este dentro de el semiplano que conforman los puntos (-1+i), (-1-i), (1-i) y (1+i)
      // sino lo esta, no se hace nada, ya que la matriz de la imagen destino se encuentra rellena de ceros (negro)

      if(abs(aux.get_real()) <= 1 && abs(aux.get_img()) <= 1){

        search(pos, aux, max);
        aux_color = original.get_matrix_value(pos[1],pos[0]);
        destino.set_matrix_value(i,j,aux_color); // Se guarda el valor del pixel en la imagen destino

      }
      aux_real=aux_real+paso; // Se ajusta el valor para la proxima posicion
    }
    aux_real=-1;  // Se reinicia el valor del x ya que recorre por filas
    aux_imag=aux_imag-paso; // Se ajusta el valor para la proxima posicion

  }
  
} 


void search(int * pos,const complejo c, const double max){
  double paso=2/(max-1);  // Determina el paso que debe haber debido al salto de una posicion para que en los limites se encutren los unos
  double cte = 1;
  int i=0 , j=0;

  i = round((c.get_real()+cte)/paso);
  
  j = round((cte-c.get_img())/paso);

  if (j>=max)
    j=max-1;
  if (i>=max)
    i=max-1;

  
  pos[0]=i;
  pos[1]=j;

  return;
}