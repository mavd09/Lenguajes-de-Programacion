///imprimir
PRINT -> imprimir tk_par_izq EXPRESSION1 MORE tk_par_der tk_pyc
MORE  -> tk_coma EXPRESSION1 MORE
MORE  -> epsilion
/// leer
READ -> leer tk_par_izq ID tk_par_der tk_pyc
/// asignaciones
VALUE_ASSIGNMENT -> ID tk_asig EXPRESSION1 tk_pyc
/// id
ID  -> id ID*
ID* -> tk_punto id ID*
ID* -> epsilion
/// llamado de funciones
CALL_FUNCTION -> ID tk_par_izq PARAMETERS tk_par_der tk_pyc
PARAMETERS    -> id PARAMETERS*
PARAMETERS    -> epsilion
PARAMETERS*   -> tk_coma id PARAMETERS*
PARAMETERS*   -> epsilion
///Condicionales ---> Pendiente
IF_CONDITIONAL  -> si tk_par_izq EXPRESSION1 tk_par_der entonces BLOCK_INSTRUCTIONS IF_CONDITIONAL*
IF_CONDITIONAL* -> fin_si
IF_CONDITIONAL* -> si_no BLOCK_INSTRUCTIONS fin_si
/// seleccion multiple
SWITCH_CASE  -> seleccionar tk_par_izq ID tk_par_der entre SWITCH_CASE* defecto tk_dosp fin_seleccionar
SWITCH_CASE* -> caso EXPRESSION1 tk_dosp BLOCK_INSTRUCTIONS SWITCH_CASE*
/// CICLO MIENTRAS
LOOP_WHILE -> mientras tk_par_izq EXPRESSION1 tk_par_der hacer BLOCK_INSTRUCTIONS fin_mientras
/// CICLO LOOP_HACER-MIENTRAS
LOOP_DO_WHILE -> hacer BLOCK_INSTRUCTIONS mientras tk_par_izq EXPRESSION1 tk_par_der tk_pyc
/// CICLO LOOP_FOR
LOOP_FOR  -> para tk_par_izq LOOP_FOR1 tk_pyc LOOP_FOR2 tk_pyc LOOP_FOR3 tk_par_der hacer BLOCK_INSTRUCTIONS fin_para
LOOP_FOR1 -> DATATYPE id tk_asig EXPRESSION1
LOOP_FOR1 -> id tk_asig EXPRESSION1
LOOP_FOR2 -> EXPRESSION1
LOOP_FOR2 -> epsilion
LOOP_FOR3 -> LOOP_FOR2
/// Bloque

/// Declaracion de TDA
TDA  -> estructura id VARIABLE TDA* fin_estructura
TDA* -> VARIABLE TDA*
TDA* -> epsilion
/// creaci�n variables
VARIABLE  -> DATATYPE B A
VARIABLE  -> id id tk_pyc
  A       -> tk_coma B A
  A       -> tk_pyc
  B       -> id C
  C       -> epsilion
  C       -> tk_asig EXPRESSION1
DATATYPE  -> entero
DATATYPE  -> real
DATATYPE  -> caracter
DATATYPE  -> cadena
/// expresion
EXPRESSION1  -> TERM EXPRESSION1*
EXPRESSION1* -> tk_y TERM EXPRESSION1*
EXPRESSION1* -> tk_O TERM EXPRESSION1*
EXPRESSION1* -> epsilion
EXPRESSION1  -> EXPRESSION2
EXPRESSION2  -> TERM EXPRESSION2* tk_igual TERM
EXPRESSION2* -> tk_igual TERM EXPRESSION2*
EXPRESSION2* -> tk_dif TERM EXPRESSION2*
EXPRESSION2* -> epsilion
EXPRESSION2  -> EXPRESSION3
EXPRESSION3  -> TERM EXPRESSION3*
EXPRESSION3* -> tk_menor TERM EXPRESSION3*
EXPRESSION3* -> tk_mayor TERM EXPRESSION3*
EXPRESSION3* -> tk_menor_igual TERM EXPRESSION3*
EXPRESSION3* -> tk_mayor_igual TERM EXPRESSION3*
EXPRESSION3* -> epsilion
EXPRESSION3  -> EXPRESSION4
EXPRESSION4  -> TERM EXPRESSION4*
EXPRESSION4* -> tk_mas TERM EXPRESSION4*
EXPRESSION4* -> tk_menos TERM EXPRESSION4*
EXPRESSION4* -> epsilion
EXPRESSION4  -> EXPRESSION5
EXPRESSION5  -> TERM EXPRESSION5*
EXPRESSION5* -> tk_mult TERM EXPRESSION5*
EXPRESSION5* -> tk_mod TERM EXPRESSION5*
EXPRESSION5* -> tk_div TERM EXPRESSION5*
EXPRESSION5* -> epsilion
TERM           -> tk_par_izq EXPRESSION1 tk_par_der
TERM           -> CONSTANT
TERM           -> ID
TERM           -> True
TERM           -> False
TERM           -> tk_menos TERM
TERM           -> tk_mas TERM
TERM           -> tk_neg TERM
///constantes
CONSTANT -> tk_entero | tk_real | tk_caracter | tk_cadena:
/**/
