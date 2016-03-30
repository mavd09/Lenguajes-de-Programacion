PROGRAMA -> DECLARACIONES_GLOBALES FUNCION_PRINCIPAL DECLARACIONES_GLOBALES

DECLARACIONES_GLOBALES -> DECLARACION_VARIABLES_GLOBALES DECLARACIONES_GLOBALES
DECLARACIONES_GLOBALES -> DECLARACION_FUNCION DECLARACIONES_GLOBALES
DECLARACIONES_GLOBALES -> DECLARACION_ESTRUCTURA DECLARACIONES_GLOBALES
DECLARACIONES_GLOBALES -> epsilon

FUNCION_PRINCIPAL -> funcion_principal BLOQUE_INSTRUCCIONES fin_principal


DECLARACION_FUNCION -> funcion TIPO_DATO id tk_par_izq PARAMETROS_FUNCION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_funcion

PARAMETROS_FUNCION -> TIPO_DATO id MAS_PARAMETROS_FUNCION
PARAMETROS_FUNCION -> epsilon
MAS_PARAMETROS_FUNCION -> tk_coma TIPO_DATO id MAS_PARAMETROS_FUNCION
MAS_PARAMETROS_FUNCION -> epsilon

BLOQUE_INSTRUCCIONES_FUNCION -> LEER BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> IMPRIMIR BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> DECLARACION_O_ASIGNACION_O_LLAMADO BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> CONDICIONAL_SI_FUNCION BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> SELECCIONAR_FUNCION BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_HACER_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_PARA_FUNCION BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> RETORNAR BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> ROMPER BLOQUE_INSTRUCCIONES_FUNCION
BLOQUE_INSTRUCCIONES_FUNCION -> epsilon

DECLARACION_ESTRUCTURA -> estructura id DATOS_ESTRUCTURA fin_estructura
DATOS_ESTRUCTURA -> TIPO_DATO DECLARACION_VARIABLE tk_pyc COMPLEMENTO_DATOS_ESTRUCTURA
COMPLEMENTO_DATOS_ESTRUCTURA -> TIPO_DATO DECLARACION_VARIABLE tk_pyc COMPLEMENTO_DATOS_ESTRUCTURA
COMPLEMENTO_DATOS_ESTRUCTURA -> epsilon

DECLARACION_VARIABLES_GLOBALES -> TIPO_DATO DECLARACION_VARIABLE tk_pyc

CONDICIONAL_SI_FUNCION -> si tk_par_izq EXPRESION tk_par_der entonces BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_CONDICIONAL_SI_FUNCION
COMPLEMENTO_CONDICIONAL_SI_FUNCION -> si_no BLOQUE_INSTRUCCIONES_FUNCION fin_si
COMPLEMENTO_CONDICIONAL_SI_FUNCION -> fin_si

SELECCIONAR_FUNCION -> seleccionar tk_par_izq EXPRESION tk_par_der entre BLOQUE_SELECCIONAR_FUNCION fin_seleccionar
BLOQUE_SELECCIONAR_FUNCION -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION
BLOQUE_SELECCIONAR_FUNCION -> defecto tk_dosp BLOQUE_INSTRUCCIONES_FUNCION
COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION
COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> defecto tk_dosp BLOQUE_INSTRUCCIONES_FUNCION
COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> epsilon

CICLO_MIENTRAS_FUNCION -> mientras tk_par_izq EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_mientras

CICLO_HACER_MIENTRAS_FUNCION -> hacer BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION
BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION
BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> mientras tk_par_izq EXPRESION tk_par_der AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION

AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> hacer BLOQUE_INSTRUCCIONES_FUNCION fin_mientras BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION
AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> tk_pyc

COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> LEER
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> IMPRIMIR
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> DECLARACION_O_ASIGNACION_O_LLAMADO
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CONDICIONAL_SI_FUNCION
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> SELECCIONAR_FUNCION
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CICLO_HACER_MIENTRAS_FUNCION
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CICLO_PARA_FUNCION
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> RETORNAR
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> ROMPER

CICLO_PARA_FUNCION  -> para tk_par_izq CICLO_PARA_FUNCION_1 tk_pyc EXPRESION tk_pyc EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_para
CICLO_PARA_FUNCION_1 -> TIPO_DATO_PRIMITIVO id tk_asig EXPRESION
CICLO_PARA_FUNCION_1 -> id COMPLEMENTO_CICLO_PARA_FUNCION_1
COMPLEMENTO_CICLO_PARA_FUNCION_1 -> id tk_asig EXPRESION
COMPLEMENTO_CICLO_PARA_FUNCION_1 -> tk_asig EXPRESION

ROMPER -> romper tk_pyc

RETORNAR -> retornar EXPRESION tk_pyc

DECLARACION_O_ASIGNACION_O_LLAMADO -> TIPO_DATO_PRIMITIVO DECLARACION_VARIABLE tk_pyc
DECLARACION_O_ASIGNACION_O_LLAMADO -> id COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO tk_pyc
COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> IDENTIFICADOR_DATO_ESTRUCTURA ASIGNACION_VARIABLE
COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> ASIGNACION_VARIABLE
COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> LLAMADO_FUNCION
COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> DECLARACION_VARIABLE

IMPRIMIR -> imprimir tk_par_izq PARAMETROS_IMPRIMIR tk_par_der tk_pyc
PARAMETROS_IMPRIMIR -> EXPRESION MAS_PARAMETROS_IMPRIMIR
MAS_PARAMETROS_IMPRIMIR -> tk_coma EXPRESION MAS_PARAMETROS_IMPRIMIR
MAS_PARAMETROS_IMPRIMIR -> epsilon

LEER -> leer tk_par_izq IDENTIFICADOR tk_par_der tk_pyc


DECLARACION_VARIABLE -> VARIABLE MAS_DECLARACION_VARIABLE
MAS_DECLARACION_VARIABLE -> tk_coma VARIABLE MAS_DECLARACION_VARIABLE
MAS_DECLARACION_VARIABLE -> epsilon

VARIABLE -> id ASIGNACION_VARIABLE

ASIGNACION_VARIABLE -> tk_asig EXPRESION
ASIGNACION_VARIABLE -> epsilon

LLAMADO_FUNCION -> tk_par_izq PARAMETROS_LLAMADO_FUNCION tk_par_der
PARAMETROS_LLAMADO_FUNCION -> EXPRESION MAS_PARAMETROS_LLAMADO_FUNCION
PARAMETROS_LLAMADO_FUNCION -> epsilon
MAS_PARAMETROS_LLAMADO_FUNCION -> tk_coma EXPRESION MAS_PARAMETROS_LLAMADO_FUNCION
MAS_PARAMETROS_LLAMADO_FUNCION -> epsilon

IDENTIFICADOR -> id COMPLEMENTO_IDENTIFICADOR
COMPLEMENTO_IDENTIFICADOR -> tk_punto id COMPLEMENTO_IDENTIFICADOR
COMPLEMENTO_IDENTIFICADOR -> epsilon

IDENTIFICADOR_DATO_ESTRUCTURA -> tk_punto id COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA
COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA -> tk_punto id COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA
COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA -> epsilon

TIPO_DATO  -> TIPO_DATO_PRIMITIVO
TIPO_DATO  -> id
TIPO_DATO_PRIMITIVO -> entero
TIPO_DATO_PRIMITIVO -> real
TIPO_DATO_PRIMITIVO -> caracter
TIPO_DATO_PRIMITIVO -> cadena
TIPO_DATO_PRIMITIVO -> booleano

BLOQUE_INSTRUCCIONES -> LEER BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> IMPRIMIR BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> DECLARACION_O_ASIGNACION_O_LLAMADO BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> CONDICIONAL_SI BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> SELECCIONAR BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> CICLO_MIENTRAS BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> CICLO_HACER_MIENTRAS BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> CICLO_PARA BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> ROMPER BLOQUE_INSTRUCCIONES
BLOQUE_INSTRUCCIONES -> epsilon

CONDICIONAL_SI -> si tk_par_izq EXPRESION tk_par_der entonces BLOQUE_INSTRUCCIONES COMPLEMENTO_CONDICIONAL_SI
COMPLEMENTO_CONDICIONAL_SI -> si_no BLOQUE_INSTRUCCIONES fin_si
COMPLEMENTO_CONDICIONAL_SI -> fin_si

SELECCIONAR -> seleccionar tk_par_izq EXPRESION tk_par_der entre BLOQUE_INSTRUCCIONES fin_seleccionar
BLOQUE_SELECCIONAR -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES COMPLEMENTO_BLOQUE_SELECCIONAR
BLOQUE_SELECCIONAR -> defecto tk_dosp BLOQUE_INSTRUCCIONES
COMPLEMENTO_BLOQUE_SELECCIONAR -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES COMPLEMENTO_BLOQUE_SELECCIONAR
COMPLEMENTO_BLOQUE_SELECCIONAR -> defecto tk_dosp BLOQUE_INSTRUCCIONES
COMPLEMENTO_BLOQUE_SELECCIONAR -> epsilon

CICLO_MIENTRAS -> mientras tk_par_izq EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES fin_mientras

CICLO_HACER_MIENTRAS -> hacer BLOQUE_INSTRUCCIONES_HACER_MIENTRAS
BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS BLOQUE_INSTRUCCIONES_HACER_MIENTRAS
BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> mientras tk_par_izq EXPRESION tk_par_der AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS


AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> hacer BLOQUE_INSTRUCCIONES fin_mientras BLOQUE_INSTRUCCIONES_HACER_MIENTRAS
AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> tk_pyc

COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> LEER
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> IMPRIMIR
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> DECLARACION_O_ASIGNACION_O_LLAMADO
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CONDICIONAL_SI
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> SELECCIONAR
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CICLO_HACER_MIENTRAS
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CICLO_PARA
COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> ROMPER

CICLO_PARA  -> para tk_par_izq CICLO_PARA_1 tk_pyc EXPRESION tk_pyc EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES fin_para
CICLO_PARA_1 -> TIPO_DATO_PRIMITIVO id tk_asig EXPRESION
CICLO_PARA_1 -> id COMPLEMENTO_CICLO_PARA_1
COMPLEMENTO_CICLO_PARA_1 -> id tk_asig EXPRESION
COMPLEMENTO_CICLO_PARA_1 -> tk_asig EXPRESION

EXPRESION  -> TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION -> tk_y TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION -> tk_o TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION -> EXPRESION_1
EXPRESION_1 -> tk_igual TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_1 -> tk_dif TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_1 -> EXPRESION_2
EXPRESION_2 -> tk_menor TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_2 -> tk_mayor TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_2 -> tk_menor_igual TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_2 -> tk_mayor_igual TERMINO COMPLEMENTO_EXPRESION_1_2
EXPRESION_2 -> EXPRESION_3
COMPLEMENTO_EXPRESION_1_2 -> tk_y TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_o TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_mas TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_menos TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_mult TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_mod TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> tk_div TERMINO COMPLEMENTO_EXPRESION
COMPLEMENTO_EXPRESION_1_2 -> epsilon
EXPRESION_3 -> tk_mas TERMINO COMPLEMENTO_EXPRESION
EXPRESION_3 -> tk_menos TERMINO COMPLEMENTO_EXPRESION
EXPRESION_3 -> EXPRESION_4
EXPRESION_4 -> tk_mult TERMINO COMPLEMENTO_EXPRESION
EXPRESION_4 -> tk_mod TERMINO COMPLEMENTO_EXPRESION
EXPRESION_4 -> tk_div TERMINO COMPLEMENTO_EXPRESION
EXPRESION_4 -> epsilon
TERMINO -> tk_par_izq EXPRESION tk_par_der
TERMINO -> CONSTANTE
TERMINO -> IDENTIFICADOR_O_LLAMADO
TERMINO -> verdadero
TERMINO -> falso
TERMINO -> tk_menos TERMINO
TERMINO -> tk_mas TERMINO
TERMINO -> tk_neg TERMINO

IDENTIFICADOR_O_LLAMADO -> id COMPLEMENTO_IDENTIFICADOR_O_LLAMADO
COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> LLAMADO_FUNCION
COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> IDENTIFICADOR_DATO_ESTRUCTURA
COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> epsilon

CONSTANTE -> tk_entero
CONSTANTE -> tk_real
CONSTANTE -> tk_caracter
CONSTANTE -> tk_cadena
