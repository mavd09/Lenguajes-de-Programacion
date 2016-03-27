///imprimir
IMPRIMIR -> imprimir tk_par_izq EXPRESION1 OTRO tk_par_der tk_pyc
OTRO     -> tk_coma EXPRESION1 OTRO
OTRO     -> eps
/// leer
LEER     -> leer tk_par_izq ID tk_par_der tk_pyc
/// asignaciones
ASIGNACION -> ID tk_asig EXPRESION1 tk_pyc
ID         -> id ID*
ID*        -> tk_punto id ID*
ID*        -> eps
/// llamado de funciones
LLAMAR -> ID tk_par_izq L tk_par_der tk_pyc
L      -> id L*
L      -> eps
L*     -> tk_coma id L*
L*     -> eps
///Condicionales ---> Pendiente
CONDICIONAL  -> si tk_par_izq EXPRESION1 tk_par_der entonces BLOQUE CONDICIONAL*
CONDICIONAL* -> fin_si
CONDICIONAL* -> si_no BLOQUE fin_si
/// seleccion multiple
SELECCION  -> seleccionar tk_par_izq ID tk_par_der entre SELECCION* defecto tk_dosp fin_seleccionar
SELECCION* -> caso EXPRESION1 tk_dosp BLOQUE SELECCION*
/// CICLO MIENTRAS
MIENTRAS -> mientras tk_par_izq EXPRESION1 tk_par_der hacer BLOQUE fin_mientras
/// CICLO HACER-MIENTRAS
HACER -> hacer BLOQUE mientras tk_par_izq EXPRESION1 tk_par_der tk_pyc
/// CICLO PARA
PARA  -> para tk_par_izq PARA1 tk_pyc PARA2 tk_pyc PARA3 tk_par_der hacer BLOQUE fin_para
PARA1 -> TIPODATO id tk_asig EXPRESION1
PARA1 -> id tk_asig EXPRESION1
PARA2 -> EXPRESION1
PARA2 -> eps
PARA3 -> PARA2
/// Bloque

/// Declaracion de TDA
TDA  -> estructura id VARIABLE TDA* fin_estructura
TDA* -> VARIABLE TDA*
TDA* -> eps
/// creación variables
VARIABLE  -> TIPODATO B A
VARIABLE  -> id id tk_pyc
  A       -> tk_coma B A
  A       -> tk_pyc
  B       -> id C
  C       -> eps
  C       -> tk_asig EXPRESION1
TIPODATO  -> entero
TIPODATO  -> real
TIPODATO  -> caracter
TIPODATO  -> cadena
/// expresion
EXPRESION1  -> T EXPRESION1*
EXPRESION1* -> tk_y T EXPRESION1*
EXPRESION1* -> tk_O T EXPRESION1*
EXPRESION1* -> eps
EXPRESION1  -> EXPRESSION2
EXPRESION2  -> T EXPRESSION2* tk_igual T
EXPRESION2* -> tk_igual T EXPRESION2*
EXPRESION2* -> tk_dif T EXPRESSION2*
EXPRESION2* -> eps
EXPRESION2  -> EXPRESION3
EXPRESION3  -> T EXPRESSION3*
EXPRESION3* -> tk_menor T EXPRESSION3*
EXPRESION3* -> tk_mayor T EXPRESSION3*
EXPRESION3* -> tk_menor_igual T EXPRESSION3*
EXPRESION3* -> tk_mayor_igual T EXPRESSION3*
EXPRESION3* -> eps
EXPRESION3  -> EXPRESION4
EXPRESION4  -> T EXPRESSION4*
EXPRESION4* -> tk_mas T EXPRESSION4*
EXPRESION4* -> tk_menos T EXPRESSION4*
EXPRESION4* -> eps
EXPRESION4  -> EXPRESION5
EXPRESION5  -> T EXPRESSION5*
EXPRESION5* -> tk_mult T EXPRESSION5*
EXPRESION5* -> tk_mod T EXPRESSION5*
EXPRESION5* -> tk_div T EXPRESSION5*
EXPRESION5* -> eps
T           -> tk_par_izq EXPRESION1 tk_par_der
T           -> tk_entero
T           -> tk_real
T           -> tk_caracter
T           -> tk_cadena
T           -> ID
T           -> True
T           -> False
T           -> tk_menos T
T           -> tk_mas T
T           -> tk_neg T
///constantes
CONSTANTE -> tk_entero | tk_real | tk_caracter | tk:
/**/
