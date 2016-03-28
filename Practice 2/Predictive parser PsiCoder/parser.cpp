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

void A( );
void ADT( );
void ADT_( );
void ASSIGN_VALUE( );
void A_( );
void BLOCK_INSTRUCTIONS( );
void CALL_FUNCTION( );
void CONSTANT( );
void DATATYPE( );
void END_PARAMETER( );
void END_VAR_DECLARATION( );
void EXPRESSION1( );
void EXPRESSION1_( );
void EXPRESSION2_( );
void EXPRESSION3_( );
void EXPRESSION4_( );
void EXPRESSION5_( );
void FUNCTIONS_ADT( );
void FUNCTION_DECLARATION( );
void ID( );
void ID2( );
void ID2_( );
void ID_( );
void IF_CONDITIONAL( );
void IF_CONDITIONAL_( );
void LOOP_DO_WHILE( );
void LOOP_FOR( );
void LOOP_FOR1( );
void LOOP_FOR1_( );
void LOOP_FOR2( );
void LOOP_FOR3( );
void LOOP_WHILE( );
void MAIN_FUNCTION( );
void MORE( );
void PARAMETERS( );
void PARAMETERS_( );
void PRIMITIVE_DATATYPE( );
void PRINT( );
void READ( );
void SWITCH_CASE( );
void SWITCH_CASE_( );
void TERM( );
void VALUE_ASSIGNMENT( );
void VAR( );
void VARIABLE_DECLARATION( );
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
  MAIN_FUNCTION( );
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
void A( ) {
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		PRIMITIVE_DATATYPE( );
		VARIABLE_DECLARATION( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		A_( );
		return ;
	}
	cout << "Error A \n";
	exit( 0 );
}
void ADT( ) {
	if( token == "estructura" ) {
		match( "estructura" );
		match( "id" );
		DATATYPE( );
		VARIABLE_DECLARATION( );
		ADT_( );
		match( "fin_estructura" );
		return ;
	}
	cout << "Error ADT \n";
	exit( 0 );
}
void ADT_( ) {
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DATATYPE( );
		VARIABLE_DECLARATION( );
		ADT_( );
		return ;
	}
	if( token == "fin_estructura" ) {
		return ;
	}
	cout << "Error ADT_ \n";
	exit( 0 );
}
void ASSIGN_VALUE( ) {
	if( token == "tk_coma" || token == "tk_pyc" ) {
		return ;
	}
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESSION1( );
		return ;
	}
	cout << "Error ASSIGN_VALUE \n";
	exit( 0 );
}
void A_( ) {
	if( token == "tk_punto" ) {
		ID2( );
		VALUE_ASSIGNMENT( );
		return ;
	}
	if( token == "tk_asig" ) {
		VALUE_ASSIGNMENT( );
		return ;
	}
	if( token == "tk_par_izq" ) {
		CALL_FUNCTION( );
		return ;
	}
	if( token == "id" ) {
		VARIABLE_DECLARATION( );
		return ;
	}
	cout << "Error A_ \n";
	exit( 0 );
}
void BLOCK_INSTRUCTIONS( ) {
	if( token == "imprimir" ) {
		PRINT( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "leer" ) {
		READ( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		A( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "si" ) {
		IF_CONDITIONAL( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "seleccionar" ) {
		SWITCH_CASE( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "mientras" ) {
		LOOP_WHILE( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "para" ) {
		LOOP_FOR( );
		BLOCK_INSTRUCTIONS( );
		return ;
	}
	if( token == "caso" || token == "fin_mientras" || token == "fin_para" || token == "fin_principal" || token == "fin_si" || token == "mientras" || token == "retornar" || token == "si_no" ) {
		return ;
	}
	cout << "Error BLOCK_INSTRUCTIONS \n";
	exit( 0 );
}
void CALL_FUNCTION( ) {
	if( token == "tk_par_izq" ) {
		match( "tk_par_izq" );
		PARAMETERS( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error CALL_FUNCTION \n";
	exit( 0 );
}
void CONSTANT( ) {
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
	cout << "Error CONSTANT \n";
	exit( 0 );
}
void DATATYPE( ) {
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		PRIMITIVE_DATATYPE( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		return ;
	}
	cout << "Error DATATYPE \n";
	exit( 0 );
}
void END_PARAMETER( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		DATATYPE( );
		match( "id" );
		END_PARAMETER( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error END_PARAMETER \n";
	exit( 0 );
}
void END_VAR_DECLARATION( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		VAR( );
		END_VAR_DECLARATION( );
		return ;
	}
	if( token == "tk_pyc" ) {
		match( "tk_pyc" );
		return ;
	}
	cout << "Error END_VAR_DECLARATION \n";
	exit( 0 );
}
void EXPRESSION1( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	cout << "Error EXPRESSION1 \n";
	exit( 0 );
}
void EXPRESSION1_( ) {
	if( token == "tk_y" ) {
		match( "tk_y" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_o" ) {
		match( "tk_o" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dif" || token == "tk_div" || token == "tk_dosp" || token == "tk_igual" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESSION2_( );
		return ;
	}
	cout << "Error EXPRESSION1_ \n";
	exit( 0 );
}
void EXPRESSION2_( ) {
	if( token == "tk_igual" ) {
		match( "tk_igual" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_dif" ) {
		match( "tk_dif" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESSION3_( );
		return ;
	}
	cout << "Error EXPRESSION2_ \n";
	exit( 0 );
}
void EXPRESSION3_( ) {
	if( token == "tk_menor" ) {
		match( "tk_menor" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_mayor" ) {
		match( "tk_mayor" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_menor_igual" ) {
		match( "tk_menor_igual" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_mayor_igual" ) {
		match( "tk_mayor_igual" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mas" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESSION4_( );
		return ;
	}
	cout << "Error EXPRESSION3_ \n";
	exit( 0 );
}
void EXPRESSION4_( ) {
	if( token == "tk_mas" ) {
		match( "tk_mas" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_menos" ) {
		match( "tk_menos" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_div" || token == "tk_dosp" || token == "tk_mod" || token == "tk_mult" || token == "tk_par_der" || token == "tk_pyc" ) {
		EXPRESSION5_( );
		return ;
	}
	cout << "Error EXPRESSION4_ \n";
	exit( 0 );
}
void EXPRESSION5_( ) {
	if( token == "tk_mult" ) {
		match( "tk_mult" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_mod" ) {
		match( "tk_mod" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_div" ) {
		match( "tk_div" );
		TERM( );
		EXPRESSION1_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dosp" || token == "tk_par_der" || token == "tk_pyc" ) {
		return ;
	}
	cout << "Error EXPRESSION5_ \n";
	exit( 0 );
}
void FUNCTIONS_ADT( ) {
	if( token == "funcion" ) {
		FUNCTION_DECLARATION( );
		FUNCTIONS_ADT( );
		return ;
	}
	if( token == "estructura" ) {
		ADT( );
		FUNCTIONS_ADT( );
		return ;
	}
	if( token == "$" || token == "funcion_principal" ) {
		return ;
	}
	cout << "Error FUNCTIONS_ADT \n";
	exit( 0 );
}
void FUNCTION_DECLARATION( ) {
	if( token == "funcion" ) {
		match( "funcion" );
		DATATYPE( );
		match( "id" );
		match( "tk_par_izq" );
		PARAMETERS( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOCK_INSTRUCTIONS( );
		match( "retornar" );
		EXPRESSION1( );
		match( "tk_pyc" );
		match( "fin_funcion" );
		return ;
	}
	cout << "Error FUNCTION_DECLARATION \n";
	exit( 0 );
}
void ID( ) {
	if( token == "id" ) {
		match( "id" );
		ID_( );
		return ;
	}
	cout << "Error ID \n";
	exit( 0 );
}
void ID2( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		ID2_( );
		return ;
	}
	cout << "Error ID2 \n";
	exit( 0 );
}
void ID2_( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		ID2_( );
		return ;
	}
	if( token == "tk_asig" ) {
		return ;
	}
	cout << "Error ID2_ \n";
	exit( 0 );
}
void ID_( ) {
	if( token == "tk_punto" ) {
		match( "tk_punto" );
		match( "id" );
		ID_( );
		return ;
	}
	if( token == "tk_coma" || token == "tk_dif" || token == "tk_div" || token == "tk_dosp" || token == "tk_igual" || token == "tk_mas" || token == "tk_mayor" || token == "tk_mayor_igual" || token == "tk_menor" || token == "tk_menor_igual" || token == "tk_menos" || token == "tk_mod" || token == "tk_mult" || token == "tk_o" || token == "tk_par_der" || token == "tk_pyc" || token == "tk_y" ) {
		return ;
	}
	cout << "Error ID_ \n";
	exit( 0 );
}
void IF_CONDITIONAL( ) {
	if( token == "si" ) {
		match( "si" );
		match( "tk_par_izq" );
		EXPRESSION1( );
		match( "tk_par_der" );
		match( "entonces" );
		BLOCK_INSTRUCTIONS( );
		IF_CONDITIONAL_( );
		return ;
	}
	cout << "Error IF_CONDITIONAL \n";
	exit( 0 );
}
void IF_CONDITIONAL_( ) {
	if( token == "fin_si" ) {
		match( "fin_si" );
		return ;
	}
	if( token == "si_no" ) {
		match( "si_no" );
		BLOCK_INSTRUCTIONS( );
		match( "fin_si" );
		return ;
	}
	cout << "Error IF_CONDITIONAL_ \n";
	exit( 0 );
}
void LOOP_DO_WHILE( ) {
	if( token == "hacer" ) {
		match( "hacer" );
		BLOCK_INSTRUCTIONS( );
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESSION1( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error LOOP_DO_WHILE \n";
	exit( 0 );
}
void LOOP_FOR( ) {
	if( token == "para" ) {
		match( "para" );
		match( "tk_par_izq" );
		LOOP_FOR1( );
		match( "tk_pyc" );
		LOOP_FOR2( );
		match( "tk_pyc" );
		LOOP_FOR3( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOCK_INSTRUCTIONS( );
		match( "fin_para" );
		return ;
	}
	cout << "Error LOOP_FOR \n";
	exit( 0 );
}
void LOOP_FOR1( ) {
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "real" ) {
		PRIMITIVE_DATATYPE( );
		match( "id" );
		match( "tk_asig" );
		EXPRESSION1( );
		return ;
	}
	if( token == "id" ) {
		match( "id" );
		LOOP_FOR1_( );
		return ;
	}
	cout << "Error LOOP_FOR1 \n";
	exit( 0 );
}
void LOOP_FOR1_( ) {
	if( token == "id" ) {
		match( "id" );
		match( "tk_asig" );
		EXPRESSION1( );
		return ;
	}
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESSION1( );
		return ;
	}
	cout << "Error LOOP_FOR1_ \n";
	exit( 0 );
}
void LOOP_FOR2( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		EXPRESSION1( );
		return ;
	}
	if( token == "tk_par_der" || token == "tk_pyc" ) {
		return ;
	}
	cout << "Error LOOP_FOR2 \n";
	exit( 0 );
}
void LOOP_FOR3( ) {
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_der" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		LOOP_FOR2( );
		return ;
	}
	cout << "Error LOOP_FOR3 \n";
	exit( 0 );
}
void LOOP_WHILE( ) {
	if( token == "mientras" ) {
		match( "mientras" );
		match( "tk_par_izq" );
		EXPRESSION1( );
		match( "tk_par_der" );
		match( "hacer" );
		BLOCK_INSTRUCTIONS( );
		match( "fin_mientras" );
		return ;
	}
	cout << "Error LOOP_WHILE \n";
	exit( 0 );
}
void MAIN_FUNCTION( ) {
	if( token == "estructura" || token == "funcion" || token == "funcion_principal" ) {
		FUNCTIONS_ADT( );
		match( "funcion_principal" );
		BLOCK_INSTRUCTIONS( );
		match( "fin_principal" );
		FUNCTIONS_ADT( );
		return ;
	}
	cout << "Error MAIN_FUNCTION \n";
	exit( 0 );
}
void MORE( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		EXPRESSION1( );
		MORE( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error MORE \n";
	exit( 0 );
}
void PARAMETERS( ) {
	if( token == "cadena" || token == "caracter" || token == "entero" || token == "id" || token == "real" ) {
		DATATYPE( );
		match( "id" );
		END_PARAMETER( );
		return ;
	}
	if( token == "falso" || token == "id" || token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_mas" || token == "tk_menos" || token == "tk_neg" || token == "tk_par_izq" || token == "tk_real" || token == "verdadero" ) {
		EXPRESSION1( );
		PARAMETERS_( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error PARAMETERS \n";
	exit( 0 );
}
void PARAMETERS_( ) {
	if( token == "tk_coma" ) {
		match( "tk_coma" );
		EXPRESSION1( );
		PARAMETERS_( );
		return ;
	}
	if( token == "tk_par_der" ) {
		return ;
	}
	cout << "Error PARAMETERS_ \n";
	exit( 0 );
}
void PRIMITIVE_DATATYPE( ) {
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
	cout << "Error PRIMITIVE_DATATYPE \n";
	exit( 0 );
}
void PRINT( ) {
	if( token == "imprimir" ) {
		match( "imprimir" );
		match( "tk_par_izq" );
		EXPRESSION1( );
		MORE( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error PRINT \n";
	exit( 0 );
}
void READ( ) {
	if( token == "leer" ) {
		match( "leer" );
		match( "tk_par_izq" );
		ID( );
		match( "tk_par_der" );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error READ \n";
	exit( 0 );
}
void SWITCH_CASE( ) {
	if( token == "seleccionar" ) {
		match( "seleccionar" );
		match( "tk_par_izq" );
		ID( );
		match( "tk_par_der" );
		match( "entre" );
		SWITCH_CASE_( );
		match( "defecto" );
		match( "tk_dosp" );
		match( "fin_seleccionar" );
		return ;
	}
	cout << "Error SWITCH_CASE \n";
	exit( 0 );
}
void SWITCH_CASE_( ) {
	if( token == "caso" ) {
		match( "caso" );
		EXPRESSION1( );
		match( "tk_dosp" );
		BLOCK_INSTRUCTIONS( );
		SWITCH_CASE_( );
		return ;
	}
	cout << "Error SWITCH_CASE_ \n";
	exit( 0 );
}
void TERM( ) {
	if( token == "tk_par_izq" ) {
		match( "tk_par_izq" );
		EXPRESSION1( );
		match( "tk_par_der" );
		return ;
	}
	if( token == "tk_cadena" || token == "tk_caracter" || token == "tk_entero" || token == "tk_real" ) {
		CONSTANT( );
		return ;
	}
	if( token == "id" ) {
		ID( );
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
		TERM( );
		return ;
	}
	if( token == "tk_mas" ) {
		match( "tk_mas" );
		TERM( );
		return ;
	}
	if( token == "tk_neg" ) {
		match( "tk_neg" );
		TERM( );
		return ;
	}
	cout << "Error TERM \n";
	exit( 0 );
}
void VALUE_ASSIGNMENT( ) {
	if( token == "tk_asig" ) {
		match( "tk_asig" );
		EXPRESSION1( );
		match( "tk_pyc" );
		return ;
	}
	cout << "Error VALUE_ASSIGNMENT \n";
	exit( 0 );
}
void VAR( ) {
	if( token == "id" ) {
		match( "id" );
		ASSIGN_VALUE( );
		return ;
	}
	cout << "Error VAR \n";
	exit( 0 );
}
void VARIABLE_DECLARATION( ) {
	if( token == "id" ) {
		VAR( );
		END_VAR_DECLARATION( );
		return ;
	}
	cout << "Error VARIABLE_DECLARATION \n";
	exit( 0 );
}
