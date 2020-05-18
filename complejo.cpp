#include <iostream>
#include "complejo.h"
#include <math.h>

using namespace std;

// Constructor por defecto
complejo::complejo(){ 
	real = 0;
	img = 0;
}

// Constructor normal, primer argumento es real, segundo es img.
complejo::complejo(const double arg_real, const double arg_img){
	real = arg_real;
	img = arg_img;
}

// Constructor por copia, simplemente copia los dos valores de un complejo a otro
complejo::complejo(const complejo & arg_complejo){
	real = arg_complejo.real;
	img = arg_complejo.img;
}

// Destructor
complejo::~complejo(){}

// Setters
void complejo::set_real(const double arg_real){
	real = arg_real;	
}

void complejo::set_img(const double arg_img){
	img = arg_img;
}

// Getters
double complejo::get_real(void) const{
	return real;
}

double complejo::get_img(void) const{
	return img;
}

double complejo::get_modulo(){
	return sqrt((this->real)*(this->real) + (this->img)*(this->img));
}

double complejo::get_angulo(){
	return atan((this->img)/(this->real));
}

// Printer
void complejo::print_complejo(){
	cout <<"(" << real << "," << img << ")";
}

// CONJUGAR
complejo complejo::conjugar (){
	complejo aux;
	aux.real = this->real;
	aux.img = -(this->img);
	return aux;
}

// Operador SUMA
complejo complejo::operator + (const complejo & complejo_a_sumar){
	complejo aux;
	aux.real = (this->real) + complejo_a_sumar.real;
	aux.img = (this->img) + complejo_a_sumar.img;
	return aux;
}

complejo complejo::operator + (const double & double_a_sumar){
	complejo aux;
	aux.real = (this->real) + double_a_sumar;
	aux.img = (this->img);
	return aux;
}

// Operador RESTA
complejo complejo::operator - (const complejo & complejo_a_restar){
	complejo aux;
	aux.real = (this->real) - complejo_a_restar.real;
	aux.img = (this->img) - complejo_a_restar.img;
	return aux;
}

// Operador RESTA
complejo complejo::operator - (const double & double_a_restar){
	complejo aux;
	aux.real = (this->real) - double_a_restar;
	aux.img = (this->img);
	return aux;
}

// Operador MULTIPLICACION
complejo complejo::operator * (const complejo & A){
	complejo aux;
	aux.real = (this->real * A.real) - (this->img * A.img);
	aux.img = (this->real * A.img) + (this->img * A.real);
	return aux;
}

complejo complejo::operator * (const double & A){
	complejo aux;
	aux.real = (this->real * A);
	aux.img = (this->img * A);
	return aux;
}

// Operador DIVISION
complejo complejo::operator / (const complejo & divisor){
	complejo aux;
	double a = this->real;
	double b = this->img;
	double c = divisor.real;
	double d = divisor.img;

	aux.real = (a*c)/(c*c + d*d) + (b*d)/(c*c + d*d);
	aux.img = (b*c)/(c*c + d*d) - (a*d)/(c*c + d*d);
	return aux;
}

complejo complejo::operator / (const double & divisor){
	complejo aux;
	aux.real = this->real / divisor;
	aux.img = this->img / divisor;
	return aux;
}

// Operador IGUAL
complejo & complejo::operator = (const complejo & complejo_a_igualar){
	real=complejo_a_igualar.real;
	img=complejo_a_igualar.img;
	return *this;
}

 // Funciones

complejo complejo::exponencial(){

  complejo aux;

  double modulo = exp(this->real);
  double angulo = this->img;

  aux.real = modulo*cos(angulo);
  aux.img = modulo*sin(angulo);

  return aux;
}



// Matrix
/*
complejo ** complejo::complex_matrix(double max){
	cout<<"Entro";
	complejo ** matrix;
	
	matrix = new complejo*[(int)max];

 	for (int i = 0; i < max; i++){  // Crea la matriz de enteros y los llena con ceros
      matrix[i] = new complejo[(int)max];   // Hay que tener en cuenta q la matriz va a ser cuadrada
  	}                               // Por eso se pide dos veces de dimension "max"

	double paso=2/(max-1);
	double aux_real=-1;
	double aux_imag=1;
	cout << aux_real << endl;
  	for (int i = 0; i < max; i++){    // raws// Rellena la matris con color negro 
    	for (int j = 0; j < max; j++){  // co
    		matrix[i][j]=complejo(aux_real,aux_imag);
    		aux_real=aux_real+paso;
      	}
      	aux_real=-1;
      	aux_imag=aux_imag-paso;
      
    }

    return matrix;
}
*/























