/**
  * Integrantes:
  *   Osman David Jim�nez Guti�rrez
  *   Manuel Alejandro Vergara D�az
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
  error.printError( );
  //printTokens( );
}

string getNextToken( ) {
  if( !tokens.empty( ) ) {
    Token curToken = tokens.front( );
    row = curToken.r; col = curToken.c;
    tokens.pop_front( );
    if( curToken.tk != "" )
      return curToken.tk;
    return curToken.lx;
  }
  return TOKEN_EOF;
}

void match( string expectedToken ) {
//  cout << "match ( " << token << ", " << expectedToken << " ) = ";
  if( token == expectedToken ) {
//    cout << " true\n";
    token = getNextToken( );
    return ;
  }
//  cout << " false\n";
//  cout << "Error match\n";
  exit( 0 );
}

void parser( ) {
  token = getNextToken( );
  PROGRAMA( );
  if( token != TOKEN_EOF ) {
//    cout << "->Error\n";
  }
  else
    cout << "El analisis sintactico ha finalizado exitosamente.\n";
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
