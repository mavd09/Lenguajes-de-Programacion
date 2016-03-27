/// funcion principal
MAIN_FUNCTION -> FUNCTIONS_ADT funcion_principal BLOCK_INSTRUCTIONS fin_principal FUNCTIONS_ADT
FUNCTIONS_ADT -> FUNCTION_DECLARATION FUNCTIONS_ADT
FUNCTIONS_ADT -> ADT FUNCTIONS_ADT
FUNCTIONS_ADT -> epsilon
/// parametros funcion
PARAMETERS -> DATATYPE id END_PARAMETER
END_PARAMETER  -> tk_coma DATATYPE id END_PARAMETER
END_PARAMETER  -> epsilon
///declaracion de funciones
FUNCTION_DECLARATION -> funcion DATATYPE id tk_par_izq PARAMETERS tk_par_der hacer BLOCK_INSTRUCTIONS retornar EXPRESSION1 tk_pyc fin_funcion
///imprimir
PRINT -> imprimir tk_par_izq EXPRESSION1 MORE tk_par_der tk_pyc
MORE  -> tk_coma EXPRESSION1 MORE
MORE  -> epsilon
/// leer
READ -> leer tk_par_izq ID tk_par_der tk_pyc
/// asignaciones
VALUE_ASSIGNMENT -> ID tk_asig EXPRESSION1 tk_pyc
/// id
ID  -> id ID*
ID* -> tk_punto id ID*
ID* -> epsilon
/// llamado de funciones
CALL_FUNCTION -> ID tk_par_izq PARAMETERS tk_par_der tk_pyc
PARAMETERS    -> id PARAMETERS*
PARAMETERS    -> epsilon
PARAMETERS*   -> tk_coma id PARAMETERS*
PARAMETERS*   -> epsilon
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
LOOP_FOR2 -> epsilon
LOOP_FOR3 -> LOOP_FOR2
/// Bloque
BLOCK_INSTRUCTIONS -> PRINT BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> READ BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> VALUE_ASSIGNMENT BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> CALL_FUNCTION BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> IF_CONDITIONAL BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> SWITCH_CASE BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> LOOP_WHILE BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> LOOP_FOR BLOCK_INSTRUCTIONS
BLOCK_INSTRUCTIONS -> VARIABLE_DECLARATION BLOCK_INSTRUCTIONS
/// Declaracion de TDA
ADT  -> estructura id VARIABLE_DECLARATION ADT* fin_estructura
ADT* -> VARIABLE_DECLARATION ADT*
ADT* -> epsilon
/// creaci�n variables
VARIABLE_DECLARATION  -> DATATYPE VAR END_VAR_DECLARATION
END_VAR_DECLARATION  -> tk_coma VAR END_VAR_DECLARATION
END_VAR_DECLARATION -> tk_pyc
VAR    -> id ASSIGN_VALUE
ASSIGN_VALUE -> epsilon
ASSIGN_VALUE -> tk_asig EXPRESSION1
DATATYPE  -> entero
DATATYPE  -> real
DATATYPE  -> caracter
DATATYPE  -> cadena
DATATYPE  -> id
/// expresion
EXPRESSION1  -> TERM EXPRESSION1*
EXPRESSION1* -> tk_y TERM EXPRESSION1*
EXPRESSION1* -> tk_O TERM EXPRESSION1*
EXPRESSION1* -> epsilon
EXPRESSION1  -> EXPRESSION2
EXPRESSION2  -> TERM EXPRESSION2* tk_igual TERM
EXPRESSION2* -> tk_igual TERM EXPRESSION2*
EXPRESSION2* -> tk_dif TERM EXPRESSION2*
EXPRESSION2* -> epsilon
EXPRESSION2  -> EXPRESSION3
EXPRESSION3  -> TERM EXPRESSION3*
EXPRESSION3* -> tk_menor TERM EXPRESSION3*
EXPRESSION3* -> tk_mayor TERM EXPRESSION3*
EXPRESSION3* -> tk_menor_igual TERM EXPRESSION3*
EXPRESSION3* -> tk_mayor_igual TERM EXPRESSION3*
EXPRESSION3* -> epsilon
EXPRESSION3  -> EXPRESSION4
EXPRESSION4  -> TERM EXPRESSION4*
EXPRESSION4* -> tk_mas TERM EXPRESSION4*
EXPRESSION4* -> tk_menos TERM EXPRESSION4*
EXPRESSION4* -> epsilon
EXPRESSION4  -> EXPRESSION5
EXPRESSION5  -> TERM EXPRESSION5*
EXPRESSION5* -> tk_mult TERM EXPRESSION5*
EXPRESSION5* -> tk_mod TERM EXPRESSION5*
EXPRESSION5* -> tk_div TERM EXPRESSION5*
EXPRESSION5* -> epsilon
TERM           -> tk_par_izq EXPRESSION1 tk_par_der
TERM           -> CONSTANT
TERM           -> ID
TERM           -> verdadero
TERM           -> false
TERM           -> tk_menos TERM
TERM           -> tk_mas TERM
TERM           -> tk_neg TERM
///constantes
CONSTANT -> tk_entero
CONSTANT -> tk_real
CONSTANT -> tk_caracter
CONSTANT -> tk_cadena
/**/
