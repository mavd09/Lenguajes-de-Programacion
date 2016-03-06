#include <bits/stdc++.h>

#define PB      push_back

using namespace std;

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
    if( r )
      cout << ">>> Error lexico (linea: " << r << ", posicion: " << c << ")\n";
  }
};

int row, col, p;
string program;
vector< Token > tokens;
Error error;
bool aborted;
vector< string > reservedWords;
map< string, string > tokenName;
string validSymbols;

void initialize( );
bool isLetter( char c ) { return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ); }
bool isDigit( char c ) { return ( '0' <= c && c <= '9' ); }
bool isWhiteSpace( char c ) { return ( c == '\n' || c == ' ' || c == '\t' ); }
bool isSymbol( char c ) { return ( validSymbols.find( c ) != string::npos ); }
bool isValidCharacter( char c ) { return ( isLetter( c ) || isDigit( c ) || isWhiteSpace( c ) /*|| isSymbol( c )/**/ || c == '_'/**/ ); }

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

void ignoreAll( ) {
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
    if( c == '\'' && ( isLetter( program[ p ] ) || isDigit( program[ p ] ) ) &&
        program[ p+1 ] == '\'' ) {
      curToken.addChar( program[ p ] );
      curToken.addChar( program[ p+1 ] );
      curToken.setTk( "tk_caracter" );
      col += 2; p += 2;
    }
    else if( c == '"' ) { /// REVISAR!!!!!!!!!
      int i = p;
      while( program[ i ] != 0 && program[ i ] != '"' && program[ i ] != '\n' ) { /// *
        if( !isValidCharacter( program[ i ] ) ) break; /// *
        i++;
      }
      if( program[ i ] == '"' ) {
        while( program[ p ] != '"' ) {
          curToken.addChar( program[ p ] );
          if( program[ p ] == '\n' )
            row++, col = 1;
          else col++;
          p++;
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
  ignoreAll( );
  while( program[ p ] != 0 ) {
    Token nxtToken = nextToken( );
    if( !aborted ) tokens.PB( nxtToken );
    else break;
    ignoreAll( );
  }
}

void printTokens( ) {
  for( int i = 0; i < int( tokens.size( ) ); i++ )
    tokens[ i ].printToken( );
  error.printError( );
}

int main( ) {

  #ifdef LOCAL
    freopen( "input", "r", stdin );
    freopen( "output", "w", stdout );
  #endif // LOCAL

  initialize( );

  string line;
  while( getline( cin, line ) ) {
    program += ( line );
    program += ( '\n' );
  }
  program += "\0\0\0\0";

  splitInput( );
  printTokens( );

  return 0;
}

void initialize( ) {
  reservedWords = { "booleano", "caracter", "entero", "real", "cadena", "leer", "seleccionar",
                    "imprimir", "si", "entonces", "fin_si", "si_no", "mientras", "verdadero",
                    "hacer", "fin_mientras", "para", "fin_para", "entre", "caso", "falso",
                    "romper", "defecto", "fin_seleccionar", "estructura", "fin_estructura",
                    "retornar", "fin_funcion", "funcion_principal", "fin_principal", "funcion" };
  sort( reservedWords.begin( ), reservedWords.end( ) );
  tokenName = { { "+", "tk_mas" }, { "-", "tk_menos" }, { "*", "tk_mult" }, { "/", "tk_div" },
                { "%", "tk_mod" }, { "=", "tk_asig" }, { "<", "tk_menor" }, { ">", "tk_mayor" },
                { "<=", "tk_menor_igual" }, { ">=", "tk_mayor_igual" }, { "==", "tk_igual" }, { "&&", "tk_y" },
                { "||", "tk_o" }, { "!=", "tk_dif" }, { "!", "tk_neg" }, { ":", "tk_dosp" },
                { "'", "tk_comilla_sen" }, { "\"", "tk_comilla_dob" }, { ";", "tk_pyc" }, { ",", "tk_coma" },
                { "(", "tk_par_izq" }, { ")", "tk_par_der" }, { ".", "tk_punto" } };
  validSymbols = "+-*/%=<>&|!:'\";,().";
}
