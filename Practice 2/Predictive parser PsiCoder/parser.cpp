/**
  * Integrantes:
  *   Osman David Jiménez Gutiérrez
  *   Manuel Alejandro Vergara Díaz
  **/

#include <bits/stdc++.h>

#define PB      push_back

using namespace std;

const string TOKEN_EOF = "$";

struct Token {
  string tk, lx;
  int r, c;
  Token( ) : tk( "" ), lx( "" ), r( 0 ), c( 0 ) { }
  Token( int r, int c ) : tk( "" ), lx( "" ), r( r ), c( c ) { }
  void addChar( char c ) {
    lx += c;
  }
  void setTk( string tk ) {
    this->tk = tk;
  }
  void setLx( string lx ) {
    this->lx = lx;
  }
  void printToken( ) {
    cout << "<";
    if( tk != "" )
      cout << tk << ",";
    if( lx != "" )
      cout << lx << ",";
    cout << r << "," << c << ">\n";
  }
};

struct Error {
  int r, c;
  Error( ) : r( 0 ), c( 0 ) { }
  Error( int r, int c ) : r( r ), c( c ) { }
  void printError( ) {
    if( r ) {
      cout << ">>> Error lexico(linea: " << r << ", posicion: " << c << ")\n";
      exit( 0 );
    }
  }
};

int row, col, p;
string program, token;
deque< Token > tokens;
Error error;
bool aborted;
vector< string > reservedWords;
map< string, string > tokenName;
string validSymbols;

void ASIGNACION_VARIABLE( );
void AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
void AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
void BLOQUE_INSTRUCCIONES( );
void BLOQUE_INSTRUCCIONES_FUNCION( );
void BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
void BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
void BLOQUE_SELECCIONAR( );
void BLOQUE_SELECCIONAR_FUNCION( );
void CICLO_HACER_MIENTRAS( );
void CICLO_HACER_MIENTRAS_FUNCION( );
void CICLO_MIENTRAS( );
void CICLO_MIENTRAS_FUNCION( );
void CICLO_PARA( );
void CICLO_PARA_1( );
void CICLO_PARA_FUNCION( );
void CICLO_PARA_FUNCION_1( );
void COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
void COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
void COMPLEMENTO_BLOQUE_SELECCIONAR( );
void COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION( );
void COMPLEMENTO_CICLO_PARA_1( );
void COMPLEMENTO_CICLO_PARA_FUNCION_1( );
void COMPLEMENTO_CONDICIONAL_SI( );
void COMPLEMENTO_CONDICIONAL_SI_FUNCION( );
void COMPLEMENTO_DATOS_ESTRUCTURA( );
void COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO( );
void COMPLEMENTO_EXPRESION( );
void COMPLEMENTO_EXPRESION_1_2( );
void COMPLEMENTO_IDENTIFICADOR( );
void COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA( );
void COMPLEMENTO_IDENTIFICADOR_O_LLAMADO( );
void CONDICIONAL_SI( );
void CONDICIONAL_SI_FUNCION( );
void CONSTANTE( );
void DATOS_ESTRUCTURA( );
void DECLARACIONES_GLOBALES( );
void DECLARACION_ESTRUCTURA( );
void DECLARACION_FUNCION( );
void DECLARACION_O_ASIGNACION_O_LLAMADO( );
void DECLARACION_VARIABLE( );
void DECLARACION_VARIABLES_GLOBALES( );
void EXPRESION( );
void EXPRESION_1( );
void EXPRESION_2( );
void EXPRESION_3( );
void EXPRESION_4( );
void FUNCION_PRINCIPAL( );
void IDENTIFICADOR( );
void IDENTIFICADOR_DATO_ESTRUCTURA( );
void IDENTIFICADOR_O_LLAMADO( );
void IMPRIMIR( );
void LEER( );
void LLAMADO_FUNCION( );
void MAS_DECLARACION_VARIABLE( );
void MAS_PARAMETROS_FUNCION( );
void MAS_PARAMETROS_IMPRIMIR( );
void MAS_PARAMETROS_LLAMADO_FUNCION( );
void PARAMETROS_FUNCION( );
void PARAMETROS_IMPRIMIR( );
void PARAMETROS_LLAMADO_FUNCION( );
void PROGRAMA( );
void RETORNAR( );
void ROMPER( );
void SELECCIONAR( );
void SELECCIONAR_FUNCION( );
void TERMINO( );
void TIPO_DATO( );
void TIPO_DATO_PRIMITIVO( );
void VARIABLE( );
void initialize( );
bool isLetter( char c ) { return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ); }
bool isDigit( char c ) { return ( '0' <= c && c <= '9' ); }
bool isWhiteSpace( char c ) { return ( c == '\n' || c == ' ' || c == '\t' || c == '\r' ); }
bool isSymbol( char c ) { return ( validSymbols.find( c ) != string::npos ); }
bool isValidCharacter( char c ) { return ( isLetter( c ) || isDigit( c ) || ( isWhiteSpace( c ) && c != '\n' ) || c == '_' ); }

bool ignoreWhiteSpace( ) {
  bool flag = false;
  while( isWhiteSpace( program[ p ] ) ) {
    if( program[ p ] == '\n' )
      row++, col = 1;
    else col++;
    flag = true;
    p++;
  }
  return flag;
}

bool ignoreComments( ) {
  bool flag = false;
  if( program[ p ] == '/' && program[ p+1 ] == '/' ) {
    while( program[ p ] != '\n' ) {
      p++; col++;
    }
    flag = true;
  }
  else if( program[ p ] == '/' && program[ p+1 ] == '*' ) {
    col += 2;
    p += 2;
    while( program[ p ] != 0 && !( program[ p ] == '*' && program[ p+1 ] == '/' ) ) {
      if( program[ p ] == '\n' )
        row++, col = 1;
      else col++;
      p++;
    }
    if( program[ p ] != 0 )
      p += 2, col += 2;
    flag = true;
  }
  return flag;
}

void ignoreGarbage( ) {
  bool found;
  do {
    found = false;
    found |= ignoreWhiteSpace( );
    found |= ignoreComments( );
  } while( found );
}

Token nextToken( ) {
  Token curToken( row, col );
  char c = program[ p ];
  p++; col++;
  curToken.addChar( c );
  if( isLetter( c ) ) {
    c = program[ p ];
    while( isLetter( c ) || isDigit( c ) || c == '_' ) {
      p++; col++;
      curToken.addChar( c );
      c = program[ p ];
    }
    if( !binary_search( reservedWords.begin( ), reservedWords.end( ), curToken.lx ) )
      curToken.setTk( "id" );
  }
  else if( isDigit( c ) /*|| c == '-'*/ ) {
    if( c == '-' && !isDigit( program[ p ] ) ) {
        curToken.setTk( tokenName[ curToken.lx ] );
        curToken.setLx( "" );
    }
    else {
      c = program[ p ];
      bool pnt = false;
      while( isDigit( c ) || ( c == '.' && !pnt && isDigit( program[ p+1 ] ) ) ) {
        p++; col++;
        pnt |= ( c == '.' );
        curToken.addChar( c );
        c = program[ p ];
      }
      if( pnt ) curToken.setTk( "tk_real" );
      else curToken.setTk( "tk_entero" );
    }
  }
  else if( isSymbol( c ) ) {
    if( c == '\'' && isValidCharacter( program[ p ] ) && program[ p+1 ] == '\'' ) {
      curToken.addChar( program[ p ] );
      curToken.addChar( program[ p+1 ] );
      curToken.setTk( "tk_caracter" );
      col += 2; p += 2;
    }
    else if( c == '"' ) {
      int i = p;
      while( program[ i ] != 0 && isValidCharacter( program[ i ] ) )
        i++;
      if( program[ i ] == '"' ) {
        while( program[ p ] != '"' ) {
          curToken.addChar( program[ p ] );
          p++; col++;
        }
        curToken.addChar( program[ p ] );
        curToken.setTk( "tk_cadena" );
        p++; col++;
      }
      else {
        curToken.setTk( tokenName[ curToken.lx ] );
        curToken.setLx( "" );
      }
    }
    else {
      string tmp = string( 1, c )+string( 1, program[ p ] );
      if( tokenName.count( tmp ) ) {
        curToken.addChar( program[ p ] );
        curToken.setTk( tokenName[ curToken.lx ] );
        curToken.setLx( "" );
        p++; col++;
      }
      else if( tokenName.count( curToken.lx ) ) {
        curToken.setTk( tokenName[ curToken.lx ] );
        curToken.setLx( "" );
      }
      else {
        error = Error( row, col-1 );
        aborted = true;
      }
    }
  }
  else {
    error = Error( row, col-1 );
    aborted = true;
  }
  return curToken;
}

void splitInput( ) {
  p = 0;
  row = col = 1;
  ignoreGarbage( );
  while( program[ p ] != '\0' ) {
    Token nxtToken = nextToken( );
    if( !aborted ) tokens.PB( nxtToken );
    else break;
    ignoreGarbage( );
  }
}

void printTokens( ) {
  for( int i = 0; i < int( tokens.size( ) ); i++ )
    tokens[ i ].printToken( );
  error.printError( );
}

void lexer( ) {
  initialize( );
  string line;
  while( getline( cin, line ) ) {
    program += ( line );
    program += ( '\n' );
  }
  program += "\0\0\0\0";
  splitInput( );
  //printTokens( );
}

string getNextToken( ) {
  if( !tokens.empty( ) ) {
    Token curToken = tokens.front( );
    tokens.pop_front( );
    if( curToken.tk != "" )
      return curToken.tk;
    return curToken.lx;
  }
  return TOKEN_EOF;
}

void match( string expectedToken ) {
  cout << "match ( " << token << ", " << expectedToken << " ) = ";
  if( token == expectedToken ) {
    cout << " true\n";
    token = getNextToken( );
    return ;
  }
  cout << " false\n";
  cout << "Error match\n";
  exit( 0 );
}

void parser( ) {
  token = getNextToken( );
  PROGRAMA( );
  if( token != TOKEN_EOF ) {
    cout << "->Error\n";
  }
  else {
    cout << "El analisis sintactico ha finalizado exitosamente.\n";
  }
}

int main( ) {

  #ifdef LOCAL
    freopen( "input", "r", stdin );
    freopen( "output", "w", stdout );
  #endif // LOCAL

  lexer( );
  parser( );

  return 0;
}

void initialize( ) {

  reservedWords.PB("booleano"); reservedWords.PB("caracter"); reservedWords.PB("entero"); reservedWords.PB("real"); reservedWords.PB("cadena");
  reservedWords.PB("leer"); reservedWords.PB("seleccionar"); reservedWords.PB("imprimir"); reservedWords.PB("si"); reservedWords.PB("entonces");
  reservedWords.PB("fin_si"); reservedWords.PB("si_no"); reservedWords.PB("mientras"); reservedWords.PB("verdadero"); reservedWords.PB("hacer");
  reservedWords.PB("fin_mientras"); reservedWords.PB("para"); reservedWords.PB("fin_para"); reservedWords.PB("entre"); reservedWords.PB("caso");
  reservedWords.PB("falso"); reservedWords.PB("romper"); reservedWords.PB("defecto"); reservedWords.PB("fin_seleccionar"); reservedWords.PB("estructura");
  reservedWords.PB("fin_estructura"); reservedWords.PB("retornar"); reservedWords.PB("fin_funcion"); reservedWords.PB("funcion_principal"); reservedWords.PB("funcion");
  reservedWords.PB("fin_principal");

  sort( reservedWords.begin( ), reservedWords.end( ) );

  tokenName["+"] = "tk_mas"; tokenName["-"] = "tk_menos"; tokenName["*"] = "tk_mult"; tokenName["/"] = "tk_div";
  tokenName["%"] = "tk_mod"; tokenName["="] = "tk_asig"; tokenName["<"] = "tk_menor"; tokenName[">"] = "tk_mayor";
  tokenName["<="] = "tk_menor_igual"; tokenName["||"] = "tk_o"; tokenName[">="] = "tk_mayor_igual"; tokenName["=="] = "tk_igual";
  tokenName["&&"] = "tk_y"; tokenName["!="] = "tk_dif"; tokenName["!"] = "tk_neg"; tokenName[":"] = "tk_dosp";
  tokenName["'"] = "tk_comilla_sen"; tokenName["\""] = "tk_comilla_dob"; tokenName[";"] = "tk_pyc"; tokenName[","] = "tk_coma";
  tokenName["("] = "tk_par_izq"; tokenName[")"] = "tk_par_der"; tokenName["."] = "tk_punto";

  validSymbols = "+-*/%=<>&|!:'\";,().";
}
void ASIGNACION_VARIABLE( ) {
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_pyc" ) {
		return ;
	}
	cout << "Error ASIGNACION_VARIABLE \n";
	exit( 0 );
}
void AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( ) {
	if( token == "hacer" ) {
		match( "hacer" );
		BLOQUE_INSTRUCCIONES( );
		match( "fin_mientras" );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
		return ;
	}
	if( token == "tk_pyc" ) {
		match( "tk_pyc" );
		return ;
	}
	cout << "Error AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS \n";
	exit( 0 );
}
void AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( ) {
	if( token == "hacer" ) {
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		match( "fin_mientras" );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
		return ;
	}
	if( token == "tk_pyc" ) {
		match( "tk_pyc" );
		return ;
	}
	cout << "Error AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION \n";
	exit( 0 );
}
void BLOQUE_INSTRUCCIONES( ) {
	if( token == "leer" ) {
		LEER( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "imprimir" ) {
		IMPRIMIR( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DECLARACION_O_ASIGNACION_O_LLAMADO( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "si" ) {
		CONDICIONAL_SI( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "seleccionar" ) {
		SELECCIONAR( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "mientras" ) {
		CICLO_MIENTRAS( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "hacer" ) {
		CICLO_HACER_MIENTRAS( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "para" ) {
		CICLO_PARA( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "romper" ) {
		ROMPER( );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "caso" || token == "defecto" || token == "fin_mientras" || token == "fin_para" || token == "fin_principal" || token == "fin_seleccionar" || token == "fin_si" || token == "si_no" ) {
		return ;
	}
	cout << "Error BLOQUE_INSTRUCCIONES \n";
	exit( 0 );
}
void BLOQUE_INSTRUCCIONES_FUNCION( ) {
	if( token == "leer" ) {
		LEER( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "imprimir" ) {
		IMPRIMIR( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DECLARACION_O_ASIGNACION_O_LLAMADO( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "si" ) {
		CONDICIONAL_SI_FUNCION( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "seleccionar" ) {
		SELECCIONAR_FUNCION( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "mientras" ) {
		CICLO_MIENTRAS_FUNCION( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "hacer" ) {
		CICLO_HACER_MIENTRAS_FUNCION( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "para" ) {
		CICLO_PARA_FUNCION( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "retornar" ) {
		RETORNAR( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "romper" ) {
		ROMPER( );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "caso" || token == "defecto" || token == "fin_funcion" || token == "fin_mientras" || token == "fin_para" || token == "fin_seleccionar" || token == "fin_si" || token == "si_no" ) {
		return ;
	}
	cout << "Error BLOQUE_INSTRUCCIONES_FUNCION \n";
	exit( 0 );
}
void BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "hacer" || token == "id" || token == "imprimir" || token == "leer" || token == "para" || token == "real" || token == "romper" || token == "seleccionar" || token == "si" ) {
		COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
		return ;
	}
	if( token == "mientras" ) {
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
		return ;
	}
	cout << "Error BLOQUE_INSTRUCCIONES_HACER_MIENTRAS \n";
	exit( 0 );
}
void BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "hacer" || token == "id" || token == "imprimir" || token == "leer" || token == "para" || token == "real" || token == "retornar" || token == "romper" || token == "seleccionar" || token == "si" ) {
		COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
		return ;
	}
	if( token == "mientras" ) {
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
		return ;
	}
	cout << "Error BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION \n";
	exit( 0 );
}
void BLOQUE_SELECCIONAR( ) {
	if( token == "caso" ) {
		match( "caso" );
		EXPRESION( );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES( );
		COMPLEMENTO_BLOQUE_SELECCIONAR( );
		return ;
	}
	if( token == "defecto" ) {
		match( "defecto" );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	cout << "Error BLOQUE_SELECCIONAR \n";
	exit( 0 );
}
void BLOQUE_SELECCIONAR_FUNCION( ) {
	if( token == "caso" ) {
		match( "caso" );
		EXPRESION( );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION( );
		return ;
	}
	if( token == "defecto" ) {
		match( "defecto" );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	cout << "Error BLOQUE_SELECCIONAR_FUNCION \n";
	exit( 0 );
}
void CICLO_HACER_MIENTRAS( ) {
	if( token == "hacer" ) {
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( );
		return ;
	}
	cout << "Error CICLO_HACER_MIENTRAS \n";
	exit( 0 );
}
void CICLO_HACER_MIENTRAS_FUNCION( ) {
	if( token == "hacer" ) {
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( );
		return ;
	}
	cout << "Error CICLO_HACER_MIENTRAS_FUNCION \n";
	exit( 0 );
}
void CICLO_MIENTRAS( ) {
	if( token == "mientras" ) {
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOQUE_INSTRUCCIONES( );
		match( "fin_mientras" );
		return ;
	}
	cout << "Error CICLO_MIENTRAS \n";
	exit( 0 );
}
void CICLO_MIENTRAS_FUNCION( ) {
	if( token == "mientras" ) {
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		match( "fin_mientras" );
		return ;
	}
	cout << "Error CICLO_MIENTRAS_FUNCION \n";
	exit( 0 );
}
void CICLO_PARA( ) {
	if( token == "para" ) {
		match( "para" );
		match( "tk_par_izq" );
		CICLO_PARA_1( );
		match( "tk_pyc" );
		EXPRESION( );
		match( "tk_pyc" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOQUE_INSTRUCCIONES( );
		match( "fin_para" );
		return ;
	}
	cout << "Error CICLO_PARA \n";
	exit( 0 );
}
void CICLO_PARA_1( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		TIPO_DATO_PRIMITIVO( );
		match( "id" );
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		COMPLEMENTO_CICLO_PARA_1( );
		return ;
	}
	cout << "Error CICLO_PARA_1 \n";
	exit( 0 );
}
void CICLO_PARA_FUNCION( ) {
	if( token == "para" ) {
		match( "para" );
		match( "tk_par_izq" );
		CICLO_PARA_FUNCION_1( );
		match( "tk_pyc" );
		EXPRESION( );
		match( "tk_pyc" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		match( "fin_para" );
		return ;
	}
	cout << "Error CICLO_PARA_FUNCION \n";
	exit( 0 );
}
void CICLO_PARA_FUNCION_1( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		TIPO_DATO_PRIMITIVO( );
		match( "id" );
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		COMPLEMENTO_CICLO_PARA_FUNCION_1( );
		return ;
	}
	cout << "Error CICLO_PARA_FUNCION_1 \n";
	exit( 0 );
}
void COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS( ) {
	if( token == "leer" ) {
		LEER( );
		return ;
	}
	if( token == "imprimir" ) {
		IMPRIMIR( );
		return ;
	}
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DECLARACION_O_ASIGNACION_O_LLAMADO( );
		return ;
	}
	if( token == "si" ) {
		CONDICIONAL_SI( );
		return ;
	}
	if( token == "seleccionar" ) {
		SELECCIONAR( );
		return ;
	}
	if( token == "hacer" ) {
		CICLO_HACER_MIENTRAS( );
		return ;
	}
	if( token == "para" ) {
		CICLO_PARA( );
		return ;
	}
	if( token == "romper" ) {
		ROMPER( );
		return ;
	}
	cout << "Error COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS \n";
	exit( 0 );
}
void COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION( ) {
	if( token == "leer" ) {
		LEER( );
		return ;
	}
	if( token == "imprimir" ) {
		IMPRIMIR( );
		return ;
	}
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DECLARACION_O_ASIGNACION_O_LLAMADO( );
		return ;
	}
	if( token == "si" ) {
		CONDICIONAL_SI_FUNCION( );
		return ;
	}
	if( token == "seleccionar" ) {
		SELECCIONAR_FUNCION( );
		return ;
	}
	if( token == "hacer" ) {
		CICLO_HACER_MIENTRAS_FUNCION( );
		return ;
	}
	if( token == "para" ) {
		CICLO_PARA_FUNCION( );
		return ;
	}
	if( token == "retornar" ) {
		RETORNAR( );
		return ;
	}
	if( token == "romper" ) {
		ROMPER( );
		return ;
	}
	cout << "Error COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION \n";
	exit( 0 );
}
void COMPLEMENTO_BLOQUE_SELECCIONAR( ) {
	if( token == "caso" ) {
		match( "caso" );
		EXPRESION( );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES( );
		COMPLEMENTO_BLOQUE_SELECCIONAR( );
		return ;
	}
	if( token == "defecto" ) {
		match( "defecto" );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES( );
		return ;
	}
	if( token == "fin_seleccionar" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_BLOQUE_SELECCIONAR \n";
	exit( 0 );
}
void COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION( ) {
	if( token == "caso" ) {
		match( "caso" );
		EXPRESION( );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION( );
		return ;
	}
	if( token == "defecto" ) {
		match( "defecto" );
		match( "tk_dosp" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		return ;
	}
	if( token == "fin_seleccionar" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION \n";
	exit( 0 );
}
void COMPLEMENTO_CICLO_PARA_1( ) {
	if( token == "id" ) {
		match( "id" );
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	cout << "Error COMPLEMENTO_CICLO_PARA_1 \n";
	exit( 0 );
}
void COMPLEMENTO_CICLO_PARA_FUNCION_1( ) {
	if( token == "id" ) {
		match( "id" );
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESION( );
		return ;
	}
	cout << "Error COMPLEMENTO_CICLO_PARA_FUNCION_1 \n";
	exit( 0 );
}
void COMPLEMENTO_CONDICIONAL_SI( ) {
	if( token == "si_no" ) {
		match( "si_no" );
		BLOQUE_INSTRUCCIONES( );
		match( "fin_si" );
		return ;
	}
	if( token == "fin_si" ) {
		match( "fin_si" );
		return ;
	}
	cout << "Error COMPLEMENTO_CONDICIONAL_SI \n";
	exit( 0 );
}
void COMPLEMENTO_CONDICIONAL_SI_FUNCION( ) {
	if( token == "si_no" ) {
		match( "si_no" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		match( "fin_si" );
		return ;
	}
	if( token == "fin_si" ) {
		match( "fin_si" );
		return ;
	}
	cout << "Error COMPLEMENTO_CONDICIONAL_SI_FUNCION \n";
	exit( 0 );
}
void COMPLEMENTO_DATOS_ESTRUCTURA( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		TIPO_DATO( );
		DECLARACION_VARIABLE( );
		match( "tk_pyc" );
		COMPLEMENTO_DATOS_ESTRUCTURA( );
		return ;
	}
	if( token == "fin_estructura" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_DATOS_ESTRUCTURA \n";
	exit( 0 );
}
void COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO( ) {
	if( token == "tk_punto" ) {
		IDENTIFICADOR_DATO_ESTRUCTURA( );
		ASIGNACION_VARIABLE( );
		return ;
	}
	if( token == "tk_asig" || token == "tk_pyc" ) {
		ASIGNACION_VARIABLE( );
		return ;
	}
	if( token == "tk_par_izq" ) {
		LLAMADO_FUNCION( );
		return ;
	}
	if( token == "id" ) {
		DECLARACION_VARIABLE( );
		return ;
	}
	cout << "Error COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO \n";
	exit( 0 );
}
void COMPLEMENTO_EXPRESION( ) {
	if( token == "tk_y" ) {
		match( "tk_y" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_o" ) {
		match( "tk_o" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dif" || token == "tk_div" || token == "tk_dosp" || token == "tk_igual" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESION_1( );
		return ;
	}
	cout << "Error COMPLEMENTO_EXPRESION \n";
	exit( 0 );
}
void COMPLEMENTO_EXPRESION_1_2( ) {
	if( token == "tk_y" ) {
		match( "tk_y" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_o" ) {
		match( "tk_o" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_mas" ) {
		match( "tk_mas" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_menos" ) {
		match( "tk_menos" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_mult" ) {
		match( "tk_mult" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_mod" ) {
		match( "tk_mod" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_div" ) {
		match( "tk_div" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dosp" || token == "tk_par_der" || token == "tk_pyc" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_EXPRESION_1_2 \n";
	exit( 0 );
}
void COMPLEMENTO_IDENTIFICADOR( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		COMPLEMENTO_IDENTIFICADOR( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_IDENTIFICADOR \n";
	exit( 0 );
}
void COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA( );
		return ;
	}
	if( token == "tk_asig" || token == "tk_coma" || token == "tk_dif" || token == "tk_div" || token == "tk_dosp" || token == "tk_igual" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_o" || token == "tk_par_der" || token == "tk_pyc" || token == "tk_y" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA \n";
	exit( 0 );
}
void COMPLEMENTO_IDENTIFICADOR_O_LLAMADO( ) {
	if( token == "tk_par_izq" ) {
		LLAMADO_FUNCION( );
		return ;
	}
	if( token == "tk_punto" ) {
		IDENTIFICADOR_DATO_ESTRUCTURA( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dif" || token == "tk_div" || token == "tk_dosp" || token == "tk_igual" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_o" || token == "tk_par_der" || token == "tk_pyc" || token == "tk_y" ) {
		return ;
	}
	cout << "Error COMPLEMENTO_IDENTIFICADOR_O_LLAMADO \n";
	exit( 0 );
}
void CONDICIONAL_SI( ) {
	if( token == "si" ) {
		match( "si" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "entonces" );
		BLOQUE_INSTRUCCIONES( );
		COMPLEMENTO_CONDICIONAL_SI( );
		return ;
	}
	cout << "Error CONDICIONAL_SI \n";
	exit( 0 );
}
void CONDICIONAL_SI_FUNCION( ) {
	if( token == "si" ) {
		match( "si" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "entonces" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		COMPLEMENTO_CONDICIONAL_SI_FUNCION( );
		return ;
	}
	cout << "Error CONDICIONAL_SI_FUNCION \n";
	exit( 0 );
}
void CONSTANTE( ) {
	if( token == "tk_entero" ) {
		match( "tk_entero" );
		return ;
	}
	if( token == "tk_real" ) {
		match( "tk_real" );
		return ;
	}
	if( token == "tk_caracter" ) {
		match( "tk_caracter" );
		return ;
	}
	if( token == "tk_cadena" ) {
		match( "tk_cadena" );
		return ;
	}
	cout << "Error CONSTANTE \n";
	exit( 0 );
}
void DATOS_ESTRUCTURA( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		TIPO_DATO( );
		DECLARACION_VARIABLE( );
		match( "tk_pyc" );
		COMPLEMENTO_DATOS_ESTRUCTURA( );
		return ;
	}
	cout << "Error DATOS_ESTRUCTURA \n";
	exit( 0 );
}
void DECLARACIONES_GLOBALES( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DECLARACION_VARIABLES_GLOBALES( );
		DECLARACIONES_GLOBALES( );
		return ;
	}
	if( token == "funcion" ) {
		DECLARACION_FUNCION( );
		DECLARACIONES_GLOBALES( );
		return ;
	}
	if( token == "estructura" ) {
		DECLARACION_ESTRUCTURA( );
		DECLARACIONES_GLOBALES( );
		return ;
	}
	if( token == "$" || token == "funcion_principal" ) {
		return ;
	}
	cout << "Error DECLARACIONES_GLOBALES \n";
	exit( 0 );
}
void DECLARACION_ESTRUCTURA( ) {
	if( token == "estructura" ) {
		match( "estructura" );
		match( "id" );
		DATOS_ESTRUCTURA( );
		match( "fin_estructura" );
		return ;
	}
	cout << "Error DECLARACION_ESTRUCTURA \n";
	exit( 0 );
}
void DECLARACION_FUNCION( ) {
	if( token == "funcion" ) {
		match( "funcion" );
		TIPO_DATO( );
		match( "id" );
		match( "tk_par_izq" );
		PARAMETROS_FUNCION( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOQUE_INSTRUCCIONES_FUNCION( );
		match( "fin_funcion" );
		return ;
	}
	cout << "Error DECLARACION_FUNCION \n";
	exit( 0 );
}
void DECLARACION_O_ASIGNACION_O_LLAMADO( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		TIPO_DATO_PRIMITIVO( );
		DECLARACION_VARIABLE( );
		match( "tk_pyc" );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO( );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error DECLARACION_O_ASIGNACION_O_LLAMADO \n";
	exit( 0 );
}
void DECLARACION_VARIABLE( ) {
	if( token == "id" ) {
		VARIABLE( );
		MAS_DECLARACION_VARIABLE( );
		return ;
	}
	cout << "Error DECLARACION_VARIABLE \n";
	exit( 0 );
}
void DECLARACION_VARIABLES_GLOBALES( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		TIPO_DATO( );
		DECLARACION_VARIABLE( );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error DECLARACION_VARIABLES_GLOBALES \n";
	exit( 0 );
}
void EXPRESION( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	cout << "Error EXPRESION \n";
	exit( 0 );
}
void EXPRESION_1( ) {
	if( token == "tk_igual" ) {
		match( "tk_igual" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_dif" ) {
		match( "tk_dif" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESION_2( );
		return ;
	}
	cout << "Error EXPRESION_1 \n";
	exit( 0 );
}
void EXPRESION_2( ) {
	if( token == "tk_menor" ) {
		match( "tk_menor" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_mayor" ) {
		match( "tk_mayor" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_menor_igual" ) {
		match( "tk_menor_igual" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_mayor_igual" ) {
		match( "tk_mayor_igual" );
		TERMINO( );
		COMPLEMENTO_EXPRESION_1_2( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mas" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESION_3( );
		return ;
	}
	cout << "Error EXPRESION_2 \n";
	exit( 0 );
}
void EXPRESION_3( ) {
	if( token == "tk_mas" ) {
		match( "tk_mas" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_menos" ) {
		match( "tk_menos" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESION_4( );
		return ;
	}
	cout << "Error EXPRESION_3 \n";
	exit( 0 );
}
void EXPRESION_4( ) {
	if( token == "tk_mult" ) {
		match( "tk_mult" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_mod" ) {
		match( "tk_mod" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_div" ) {
		match( "tk_div" );
		TERMINO( );
		COMPLEMENTO_EXPRESION( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dosp" || token == "tk_par_der" || token == "tk_pyc" ) {
		return ;
	}
	cout << "Error EXPRESION_4 \n";
	exit( 0 );
}
void FUNCION_PRINCIPAL( ) {
	if( token == "funcion_principal" ) {
		match( "funcion_principal" );
		BLOQUE_INSTRUCCIONES( );
		match( "fin_principal" );
		return ;
	}
	cout << "Error FUNCION_PRINCIPAL \n";
	exit( 0 );
}
void IDENTIFICADOR( ) {
	if( token == "id" ) {
		match( "id" );
		COMPLEMENTO_IDENTIFICADOR( );
		return ;
	}
	cout << "Error IDENTIFICADOR \n";
	exit( 0 );
}
void IDENTIFICADOR_DATO_ESTRUCTURA( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA( );
		return ;
	}
	cout << "Error IDENTIFICADOR_DATO_ESTRUCTURA \n";
	exit( 0 );
}
void IDENTIFICADOR_O_LLAMADO( ) {
	if( token == "id" ) {
		match( "id" );
		COMPLEMENTO_IDENTIFICADOR_O_LLAMADO( );
		return ;
	}
	cout << "Error IDENTIFICADOR_O_LLAMADO \n";
	exit( 0 );
}
void IMPRIMIR( ) {
	if( token == "imprimir" ) {
		match( "imprimir" );
		match( "tk_par_izq" );
		PARAMETROS_IMPRIMIR( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error IMPRIMIR \n";
	exit( 0 );
}
void LEER( ) {
	if( token == "leer" ) {
		match( "leer" );
		match( "tk_par_izq" );
		IDENTIFICADOR( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error LEER \n";
	exit( 0 );
}
void LLAMADO_FUNCION( ) {
	if( token == "tk_par_izq" ) {
		match( "tk_par_izq" );
		PARAMETROS_LLAMADO_FUNCION( );
		match( "tk_par_der" );
		return ;
	}
	cout << "Error LLAMADO_FUNCION \n";
	exit( 0 );
}
void MAS_DECLARACION_VARIABLE( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		VARIABLE( );
		MAS_DECLARACION_VARIABLE( );
		return ;
	}
	if( token == "tk_pyc" ) {
		return ;
	}
	cout << "Error MAS_DECLARACION_VARIABLE \n";
	exit( 0 );
}
void MAS_PARAMETROS_FUNCION( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		TIPO_DATO( );
		match( "id" );
		MAS_PARAMETROS_FUNCION( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error MAS_PARAMETROS_FUNCION \n";
	exit( 0 );
}
void MAS_PARAMETROS_IMPRIMIR( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		EXPRESION( );
		MAS_PARAMETROS_IMPRIMIR( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error MAS_PARAMETROS_IMPRIMIR \n";
	exit( 0 );
}
void MAS_PARAMETROS_LLAMADO_FUNCION( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		EXPRESION( );
		MAS_PARAMETROS_LLAMADO_FUNCION( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error MAS_PARAMETROS_LLAMADO_FUNCION \n";
	exit( 0 );
}
void PARAMETROS_FUNCION( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		TIPO_DATO( );
		match( "id" );
		MAS_PARAMETROS_FUNCION( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error PARAMETROS_FUNCION \n";
	exit( 0 );
}
void PARAMETROS_IMPRIMIR( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		EXPRESION( );
		MAS_PARAMETROS_IMPRIMIR( );
		return ;
	}
	cout << "Error PARAMETROS_IMPRIMIR \n";
	exit( 0 );
}
void PARAMETROS_LLAMADO_FUNCION( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		EXPRESION( );
		MAS_PARAMETROS_LLAMADO_FUNCION( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error PARAMETROS_LLAMADO_FUNCION \n";
	exit( 0 );
}
void PROGRAMA( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "estructura" || token == "funcion" || token == "funcion_principal" || token == "id" || token == "real" ) {
		DECLARACIONES_GLOBALES( );
		FUNCION_PRINCIPAL( );
		DECLARACIONES_GLOBALES( );
		return ;
	}
	cout << "Error PROGRAMA \n";
	exit( 0 );
}
void RETORNAR( ) {
	if( token == "retornar" ) {
		match( "retornar" );
		EXPRESION( );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error RETORNAR \n";
	exit( 0 );
}
void ROMPER( ) {
	if( token == "romper" ) {
		match( "romper" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error ROMPER \n";
	exit( 0 );
}
void SELECCIONAR( ) {
	if( token == "seleccionar" ) {
		match( "seleccionar" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "entre" );
		BLOQUE_SELECCIONAR( );
		match( "fin_seleccionar" );
		return ;
	}
	cout << "Error SELECCIONAR \n";
	exit( 0 );
}
void SELECCIONAR_FUNCION( ) {
	if( token == "seleccionar" ) {
		match( "seleccionar" );
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		match( "entre" );
		BLOQUE_SELECCIONAR_FUNCION( );
		match( "fin_seleccionar" );
		return ;
	}
	cout << "Error SELECCIONAR_FUNCION \n";
	exit( 0 );
}
void TERMINO( ) {
	if( token == "tk_par_izq" ) {
		match( "tk_par_izq" );
		EXPRESION( );
		match( "tk_par_der" );
		return ;
	}
	if( token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_real" ) {
		CONSTANTE( );
		return ;
	}
	if( token == "id" ) {
		IDENTIFICADOR_O_LLAMADO( );
		return ;
	}
	if( token == "verdadero" ) {
		match( "verdadero" );
		return ;
	}
	if( token == "falso" ) {
		match( "falso" );
		return ;
	}
	if( token == "tk_menos" ) {
		match( "tk_menos" );
		TERMINO( );
		return ;
	}
	if( token == "tk_mas" ) {
		match( "tk_mas" );
		TERMINO( );
		return ;
	}
	if( token == "tk_neg" ) {
		match( "tk_neg" );
		TERMINO( );
		return ;
	}
	cout << "Error TERMINO \n";
	exit( 0 );
}
void TIPO_DATO( ) {
	if( token == "booleano" || token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		TIPO_DATO_PRIMITIVO( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		return ;
	}
	cout << "Error TIPO_DATO \n";
	exit( 0 );
}
void TIPO_DATO_PRIMITIVO( ) {
	if( token == "entero" ) {
		match( "entero" );
		return ;
	}
	if( token == "real" ) {
		match( "real" );
		return ;
	}
	if( token == "caracter" ) {
		match( "caracter" );
		return ;
	}
	if( token == "cadena" ) {
		match( "cadena" );
		return ;
	}
	if( token == "booleano" ) {
		match( "booleano" );
		return ;
	}
	cout << "Error TIPO_DATO_PRIMITIVO \n";
	exit( 0 );
}
void VARIABLE( ) {
	if( token == "id" ) {
		match( "id" );
		ASIGNACION_VARIABLE( );
		return ;
	}
	cout << "Error VARIABLE \n";
	exit( 0 );
}
