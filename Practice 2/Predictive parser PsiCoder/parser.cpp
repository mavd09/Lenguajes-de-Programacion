/**
  * Integrantes:
  *   Osman David Jiménez Gutiérrez
  *   Manuel Alejandro Vergara Díaz
  **/

#include <bits/stdc++.h>

#define PB          push_back
#define PF          push_front
#define MP          make_pair
#define FI          first
#define SE          second

using namespace std;

typedef pair< int, string > pis;

struct Token {
  string tk, lx;
  int r, c;
  Token( ) : tk( "" ), lx( "" ), r( 0 ), c( 0 ) { }
  Token( int r, int c ) : tk( "" ), lx( "" ), r( r ), c( c ) { }
  Token( string tk ) : tk( tk ), lx( tk ), r( 0 ), c( 0 ) { }
  void add_char( char c ) {
    lx += c;
  }
  void set_tk( string tk ) {
    this->tk = tk;
  }
  void set_lx( string lx ) {
    this->lx = lx;
  }
  void print_token( ) {
    cout << "<";
    if( tk != "" )
      cout << tk << ",";
    if( lx != "" )
      cout << lx << ",";
    cout << r << "," << c << ">\n";
  }
  string get_value( ) {
    if( tk != "" )
      return tk;
    return lx;
  }
}token;

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

string EPS = "epsilon";
Token TOKEN_EOF( "EOF" );

int row, col, p;
string program;
deque< string > derivation;
deque< Token > tokens;
Error error;
bool aborted;
vector< string > reserved_words, grammar;
map< string, string > token_name;
string valid_symbols;

map< string, vector< vector< string > > > rules;
set< string > terminals, non_terminals;
map< string, set< string > > firsts, follows;
map< string, vector< set< string > > > predictions;
map< string, pis > token_symbol;

void initialize( );
void print_firsts( );
void print_follows( );
void print_predictions( );
void print_rule( );
bool is_letter( char c ) { return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ); }
bool is_digit( char c ) { return ( '0' <= c && c <= '9' ); }
bool is_white_space( char c ) { return ( c == '\n' || c == ' ' || c == '\t' || c == '\r' ); }
bool is_symbol( char c ) { return ( valid_symbols.find( c ) != string::npos ); }
bool is_valid_character( char c ) { return ( is_letter( c ) || is_digit( c ) || ( is_white_space( c ) && c != '\n' ) || c == '_' ); }

bool ignore_white_space( ) {
  bool flag = false;
  while( is_white_space( program[ p ] ) ) {
    if( program[ p ] == '\n' )
      row++, col = 1;
    else col++;
    flag = true;
    p++;
  }
  return flag;
}

bool ignore_comments( ) {
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

void ignore_garbage( ) {
  bool found;
  do {
    found = false;
    found |= ignore_white_space( );
    found |= ignore_comments( );
  } while( found );
}

Token next_token( ) {
  Token curToken( row, col );
  char c = program[ p ];
  p++; col++;
  curToken.add_char( c );
  if( is_letter( c ) ) {
    c = program[ p ];
    while( is_letter( c ) || is_digit( c ) || c == '_' ) {
      p++; col++;
      curToken.add_char( c );
      c = program[ p ];
    }
    if( !binary_search( reserved_words.begin( ), reserved_words.end( ), curToken.lx ) )
      curToken.set_tk( "id" );
  }
  else if( is_digit( c ) /*|| c == '-'*/ ) {
    if( c == '-' && !is_digit( program[ p ] ) ) {
        curToken.set_tk( token_name[ curToken.lx ] );
        curToken.set_lx( "" );
    }
    else {
      c = program[ p ];
      bool pnt = false;
      while( is_digit( c ) || ( c == '.' && !pnt && is_digit( program[ p+1 ] ) ) ) {
        p++; col++;
        pnt |= ( c == '.' );
        curToken.add_char( c );
        c = program[ p ];
      }
      if( pnt ) curToken.set_tk( "tk_real" );
      else curToken.set_tk( "tk_entero" );
    }
  }
  else if( is_symbol( c ) ) {
    if( c == '\'' && is_valid_character( program[ p ] ) && program[ p+1 ] == '\'' ) {
      curToken.add_char( program[ p ] );
      curToken.add_char( program[ p+1 ] );
      curToken.set_tk( "tk_caracter" );
      col += 2; p += 2;
    }
    else if( c == '"' ) {
      int i = p;
      while( program[ i ] != 0 && is_valid_character( program[ i ] ) )
        i++;
      if( program[ i ] == '"' ) {
        while( program[ p ] != '"' ) {
          curToken.add_char( program[ p ] );
          p++; col++;
        }
        curToken.add_char( program[ p ] );
        curToken.set_tk( "tk_cadena" );
        p++; col++;
      }
      else {
        curToken.set_tk( token_name[ curToken.lx ] );
        curToken.set_lx( "" );
      }
    }
    else {
      string tmp = string( 1, c )+string( 1, program[ p ] );
      if( token_name.count( tmp ) ) {
        curToken.add_char( program[ p ] );
        curToken.set_tk( token_name[ curToken.lx ] );
        curToken.set_lx( "" );
        p++; col++;
      }
      else if( token_name.count( curToken.lx ) ) {
        curToken.set_tk( token_name[ curToken.lx ] );
        curToken.set_lx( "" );
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

void split_input( ) {
  p = 0;
  row = col = 1;
  ignore_garbage( );
  while( program[ p ] != '\0' ) {
    Token nxtToken = next_token( );
    if( !aborted ) tokens.PB( nxtToken );
    else break;
    ignore_garbage( );
  }
}

void print_tokens( ) {
  for( int i = 0; i < int( tokens.size( ) ); i++ )
    tokens[ i ].print_token( );
  error.printError( );
}

void classify_symbol( string current ) {
  if( 'a' <= current[ 0 ] && current[ 0 ] <= 'z' )
    terminals.insert( current );
  else
    non_terminals.insert( current );
}

void read_grammar( ) {
  bool initial_symbol = true;
  string rule, lft, rght, garbage;
  for( int i = 0; i < int( grammar.size( ) ); i++ ) {
    rule = grammar[ i ];
    stringstream ss_in( rule );
    vector< string > current_rule;
    ss_in >> lft >> garbage;
    classify_symbol( lft );
    if( initial_symbol ) {
      derivation.push_back( lft );
      follows[ lft ].insert( TOKEN_EOF.get_value( ) );
      initial_symbol = false;
    }
    while( ss_in >> rght ) {
      current_rule.PB( rght );
      classify_symbol( rght );
    }
    if( current_rule.back( ) != EPS )
      current_rule.PB( EPS );
    rules[ lft ].PB( current_rule );
  }
}

bool add_symbol( set< string >& set_non_terminal, string symbol ) {
  if( set_non_terminal.count( symbol ) )
    return false;
  set_non_terminal.insert( symbol );
  return true;
}

bool add_symbols_of( set< string >& set_non_terminal_1, set< string >& set_non_terminal_2, bool with_eps = false ) {
  bool changes = false;
  for( auto& symbol : set_non_terminal_2 ) {
    if( !with_eps && symbol == EPS )
      continue;
    changes |= add_symbol( set_non_terminal_1, symbol );
  }
  return changes;
}

set< string > compute_firsts_from( vector< string >& rule, int from = 0 ) {
  set< string > new_firsts;
  if( from+1 == int( rule.size( ) ) && rule[ from ] == EPS ) {
    add_symbol( new_firsts, EPS );
  }
  else {
    for( int i = from; i < int( rule.size( ) ); i++ ) {
      if( terminals.count( rule[ i ] ) ) {
        add_symbol( new_firsts, rule[ i ] );
        break;
      }
      else {
        bool contain_eps = firsts[ rule[ i ] ].count( EPS );
        add_symbols_of( new_firsts, firsts[ rule[ i ] ] );
        if( contain_eps ) {
          if( i+1 == int( rule.size( ) ) )
            add_symbol( new_firsts, EPS );
        }
        else
          break;
      }
    }
  }
  return new_firsts;
}

bool add_firsts_from( set< string >& firsts_non_terminal, vector< string > rule, int from = 0 ) {
  bool changes = false;
  set< string > new_firsts = compute_firsts_from( rule, from );
  changes |= add_symbols_of( firsts_non_terminal, new_firsts, true );
  return changes;
}

bool add_follows_from( set< string >& follows_non_terminal, string non_terminal, vector< string >& rule, int from = 0 ) {
  bool changes = false;
  set< string > new_firsts = compute_firsts_from( rule, from );
  bool contain_eps = new_firsts.count( EPS );
  changes |= add_symbols_of( follows_non_terminal, new_firsts );
  if( contain_eps )
    changes |= add_symbols_of( follows_non_terminal, follows[ non_terminal ] );
  return changes;
}

void compute_firsts( ) {
  bool changes;
  do {
    changes = false;
    for( auto& non_terminal : non_terminals )
      for( auto& rule : rules[ non_terminal ] )
        changes |= add_firsts_from( firsts[ non_terminal ], rule );
  } while( changes );
}

void compute_follows( ) {
  bool changes;
  do {
    changes = false;
    for( auto& non_terminal_1 : non_terminals )
      for( auto& non_terminal_2 : non_terminals )
        for( auto& rule : rules[ non_terminal_2 ] )
          for( int i = 0; i < int( rule.size( ) ); i++ )
            if( rule[ i ] == non_terminal_1 )
              changes |= add_follows_from( follows[ non_terminal_1 ], non_terminal_2, rule, i+1 );
  } while( changes );
}

void compute_predictions( ) {
  for( auto& non_terminal : non_terminals ) {
    for( auto& rule : rules[ non_terminal ] ) {
      set< string > current_firsts = compute_firsts_from( rule );
      set< string > current_predictions;
      bool contain_eps = current_firsts.count( EPS );
      add_symbols_of( current_predictions, current_firsts );
      if( contain_eps )
        add_symbols_of( current_predictions, follows[ non_terminal ] );
      predictions[ non_terminal ].PB( current_predictions );
    }
  }
}

void verify_predictions( ) {
  for( auto& non_terminal : non_terminals ) {
    set< string > all;
    for( auto& v : predictions[ non_terminal ] ) {
      for( auto& s : v ) {
        if( all.count( s ) ) {
          cerr << "The prediction's set is bad\n";
          return ;
        }
        all.insert( s );
      }
    }
  }
  cerr << "The prediction's set is good\n";
}

void lexer( ) {
  string line;
  while( getline( cin, line ) ) {
    program += ( line );
    program += ( '\n' );
  }
  program += "\0\0\0\0";
  split_input( );
  error.printError( );
  //print_tokens( );
}

Token get_next_token( ) {
  Token ret = TOKEN_EOF;
  ret.r = row; ret.c = col;
  if( !tokens.empty( ) ) {
    ret = tokens.front( );
    tokens.pop_front( );
  }
  return ret;
}

void match( ) {
  if( int( derivation.size( ) ) == 0 ) {
    cout << "No se puede hacer emparejamiento\n";
    exit( 0 );
  }
  string expected_token = derivation.front( );
  derivation.pop_front( );
  if( expected_token == EPS || expected_token == "" )
    return ;
  if( token.get_value( ) == expected_token ) {
    token = get_next_token( );
    return ;
  }
  if( expected_token == "funcion_principal" ) {
    cout << "Error sintactico: falta funcion_principal\n";
  }
  else {
    cout << "<" << token.r << "," << token.c << "> ";
    cout << "Error sintactico: se encontro: \"" << token.lx << "\"; ";
    cout << "se esperaba: \"" << expected_token << "\".\n";
  }
  exit( 0 );
}

void replace_in_derivation( ) {
  while( int( derivation.size( ) ) > 0 ) {
    string current_symbol = derivation.front( );
    if( current_symbol == EPS || terminals.count( current_symbol ) ) {
      match( );
      continue;
    }
    else {
      bool found_rule = false;
      for( int i = 0; i < int( rules[ current_symbol ].size( ) ); i++ ) {
        bool found_symbol = false;
        for( auto& p : predictions[ current_symbol ][ i ] ) {
          if( p == token.get_value( ) ) {
            derivation.pop_front( );
            for( int j = int( rules[ current_symbol ][ i ].size( ) )-1; j >= 0; j-- )
              derivation.push_front( rules[ current_symbol ][ i ][ j ] );
            found_symbol = true;
            break;
          }
        }
        found_rule |= found_symbol;
        if( found_rule )
          break;
      }
      if( !found_rule )
        break;
    }
  }
  vector< string > d;
  for( auto& e : derivation )
    if( e != EPS )
      d.PB( e );
  set< string > expected_tokens = compute_firsts_from( d );
  set< pis > sorted_expected_tokens;
  for( auto& e : expected_tokens )
    if( token_symbol.count( e ) )
      sorted_expected_tokens.insert( token_symbol[ e ] );
  if( int( sorted_expected_tokens.size( ) ) == 0 ) {
    cout << "El analisis sintactico ha finalizado exitosamente.\n";
    exit( 0 );
  }
  if( expected_tokens.count( "funcion_principal" ) ) {
    cout << "Error sintactico: falta funcion_principal\n";
  }
  else {
    cout << "<" << token.r << "," << token.c << "> ";
    cout << "Error sintactico: se encontro: \"" << token.lx << "\"; ";
    cout << "se esperaba:";
    bool fst = true;
    for( auto& e : sorted_expected_tokens ) {
      if( !fst )
        cout << ",";
      else
        fst = false;
      cout << " \"" << e.SE << "\"";
    }
    cout << ".\n";
  }
  exit( 0 );
}

void parser( ) {

  read_grammar( );

  compute_firsts( );
  compute_follows( );
  compute_predictions( );

  token = get_next_token( );
  replace_in_derivation( );

}

int main( ) {

  #ifdef LOCAL
    freopen( "input", "r", stdin );
    freopen( "output", "w", stdout );
  #endif // LOCAL

  initialize( );

  lexer( );
  parser( );

  return 0;
}

void initialize( ) {

  reserved_words.PB("booleano"); reserved_words.PB("caracter"); reserved_words.PB("entero"); reserved_words.PB("real"); reserved_words.PB("cadena");
  reserved_words.PB("leer"); reserved_words.PB("seleccionar"); reserved_words.PB("imprimir"); reserved_words.PB("si"); reserved_words.PB("entonces");
  reserved_words.PB("fin_si"); reserved_words.PB("si_no"); reserved_words.PB("mientras"); reserved_words.PB("verdadero"); reserved_words.PB("hacer");
  reserved_words.PB("fin_mientras"); reserved_words.PB("para"); reserved_words.PB("fin_para"); reserved_words.PB("entre"); reserved_words.PB("caso");
  reserved_words.PB("falso"); reserved_words.PB("romper"); reserved_words.PB("defecto"); reserved_words.PB("fin_seleccionar"); reserved_words.PB("estructura");
  reserved_words.PB("fin_estructura"); reserved_words.PB("retornar"); reserved_words.PB("fin_funcion"); reserved_words.PB("funcion_principal"); reserved_words.PB("funcion");
  reserved_words.PB("fin_principal");

  sort( reserved_words.begin( ), reserved_words.end( ) );

  token_name["+"] = "tk_mas"; token_name["-"] = "tk_menos"; token_name["*"] = "tk_mult"; token_name["/"] = "tk_div";
  token_name["%"] = "tk_mod"; token_name["="] = "tk_asig"; token_name["<"] = "tk_menor"; token_name[">"] = "tk_mayor";
  token_name["<="] = "tk_menor_igual"; token_name["||"] = "tk_o"; token_name[">="] = "tk_mayor_igual"; token_name["=="] = "tk_igual";
  token_name["&&"] = "tk_y"; token_name["!="] = "tk_dif"; token_name["!"] = "tk_neg"; token_name[":"] = "tk_dosp";
  token_name["'"] = "tk_comilla_sen"; token_name["\""] = "tk_comilla_dob"; token_name[";"] = "tk_pyc"; token_name[","] = "tk_coma";
  token_name["("] = "tk_par_izq"; token_name[")"] = "tk_par_der"; token_name["."] = "tk_punto";

  valid_symbols = "+-*/%=<>&|!:'\";,().";

  token_symbol["tk_mas"] = pis(0, "+"); token_symbol["tk_menos"] = pis(1, "-"); token_symbol["tk_mult"] = pis(2, "*");
  token_symbol["tk_div"] = pis(3, "/"); token_symbol["tk_mod"] = pis(4, "%"); token_symbol["tk_asig"] = pis(5, "=");
  token_symbol["tk_menor"] = pis(6, "<"); token_symbol["tk_mayor"] = pis(7, ">"); token_symbol["tk_menor_igual"] = pis(8, "<=");
  token_symbol["tk_mayor_igual"] = pis(9, ">="); token_symbol["tk_igual"] = pis(10, "=="); token_symbol["tk_y"] = pis(11, "&&");
  token_symbol["tk_o"] = pis(12, "||"); token_symbol["tk_dif"] = pis(13, "!="); token_symbol["tk_neg"] = pis(14, "!");
  token_symbol["tk_dosp"] = pis(15, ":"); token_symbol["tk_comilla_sen"] = pis(16, "'"); token_symbol["tk_comilla_dob"] = pis(17, "\"");
  token_symbol["tk_pyc"] = pis(18, ";"); token_symbol["tk_coma"] = pis(19, ","); token_symbol["tk_punto"] = pis(20, ".");
  token_symbol["tk_par_izq"] = pis(21, "("); token_symbol["tk_par_der"] = pis(22, ")"); token_symbol["id"] = pis(23, "identificador");
  token_symbol["tk_entero"] = pis(24, "valor_entero"); token_symbol["tk_real"] = pis(25, "valor_real"); token_symbol["tk_caracter"] = pis(26, "valor_caracter");
  token_symbol["tk_cadena"] = pis(27, "valor_cadena"); token_symbol["funcion_principal"] = pis(28, "funcion_principal"); token_symbol["fin_principal"] = pis(29, "fin_principal");
  token_symbol["leer"] = pis(30, "leer"); token_symbol["imprimir"] = pis(31, "imprimir"); token_symbol["booleano"] = pis(32, "booleano");
  token_symbol["caracter"] = pis(33, "caracter"); token_symbol["entero"] = pis(34, "entero"); token_symbol["real"] = pis(35, "real");
  token_symbol["cadena"] = pis(36, "cadena"); token_symbol["si"] = pis(37, "si"); token_symbol["entonces"] = pis(38, "entonces");
  token_symbol["fin_si"] = pis(39, "fin_si"); token_symbol["si_no"] = pis(40, "si_no"); token_symbol["mientras"] = pis(41, "mientras");
  token_symbol["hacer"] = pis(42, "hacer"); token_symbol["fin_mientras"] = pis(43, "fin_mientras"); token_symbol["para"] = pis(44, "para");
  token_symbol["fin_para"] = pis(45, "fin_para"); token_symbol["seleccionar"] = pis(46, "seleccionar"); token_symbol["entre"] = pis(47, "entre");
  token_symbol["caso"] = pis(48, "caso"); token_symbol["romper"] = pis(49, "romper"); token_symbol["defecto"] = pis(50, "defecto");
  token_symbol["fin_seleccionar"] = pis(51, "fin_seleccionar"); token_symbol["estructura"] = pis(52, "estructura"); token_symbol["fin_estructura"] = pis(53, "fin_estructura");
  token_symbol["funcion"] = pis(54, "funcion"); token_symbol["fin_funcion"] = pis(55, "fin_funcion"); token_symbol["retornar"] = pis(56, "retornar");
  token_symbol["falso"] = pis(57, "falso"); token_symbol["verdadero"] = pis(58, "verdadero"); token_symbol["EOF"] = pis(59, "EOF");

  grammar.PB( "PROGRAMA -> DECLARACIONES_GLOBALES FUNCION_PRINCIPAL DECLARACIONES_GLOBALES" );
  grammar.PB( "DECLARACIONES_GLOBALES -> DECLARACION_VARIABLES_GLOBALES DECLARACIONES_GLOBALES" );
  grammar.PB( "DECLARACIONES_GLOBALES -> DECLARACION_FUNCION DECLARACIONES_GLOBALES" );
  grammar.PB( "DECLARACIONES_GLOBALES -> DECLARACION_ESTRUCTURA DECLARACIONES_GLOBALES" );
  grammar.PB( "DECLARACIONES_GLOBALES -> epsilon" );
  grammar.PB( "FUNCION_PRINCIPAL -> funcion_principal BLOQUE_INSTRUCCIONES fin_principal" );
  grammar.PB( "DECLARACION_FUNCION -> funcion TIPO_DATO id tk_par_izq PARAMETROS_FUNCION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_funcion" );
  grammar.PB( "PARAMETROS_FUNCION -> TIPO_DATO id MAS_PARAMETROS_FUNCION" );
  grammar.PB( "PARAMETROS_FUNCION -> epsilon" );
  grammar.PB( "MAS_PARAMETROS_FUNCION -> tk_coma TIPO_DATO id MAS_PARAMETROS_FUNCION" );
  grammar.PB( "MAS_PARAMETROS_FUNCION -> epsilon" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> LEER BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> IMPRIMIR BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> DECLARACION_O_ASIGNACION_O_LLAMADO BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> CONDICIONAL_SI_FUNCION BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> SELECCIONAR_FUNCION BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_HACER_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> CICLO_PARA_FUNCION BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> RETORNAR BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> ROMPER BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_FUNCION -> epsilon" );
  grammar.PB( "DECLARACION_ESTRUCTURA -> estructura id DATOS_ESTRUCTURA fin_estructura" );
  grammar.PB( "DATOS_ESTRUCTURA -> TIPO_DATO DECLARACION_VARIABLE tk_pyc COMPLEMENTO_DATOS_ESTRUCTURA" );
  grammar.PB( "COMPLEMENTO_DATOS_ESTRUCTURA -> TIPO_DATO DECLARACION_VARIABLE tk_pyc COMPLEMENTO_DATOS_ESTRUCTURA" );
  grammar.PB( "COMPLEMENTO_DATOS_ESTRUCTURA -> epsilon" );
  grammar.PB( "DECLARACION_VARIABLES_GLOBALES -> TIPO_DATO DECLARACION_VARIABLE tk_pyc" );
  grammar.PB( "CONDICIONAL_SI_FUNCION -> si tk_par_izq EXPRESION tk_par_der entonces BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_CONDICIONAL_SI_FUNCION" );
  grammar.PB( "COMPLEMENTO_CONDICIONAL_SI_FUNCION -> si_no BLOQUE_INSTRUCCIONES_FUNCION fin_si" );
  grammar.PB( "COMPLEMENTO_CONDICIONAL_SI_FUNCION -> fin_si" );
  grammar.PB( "SELECCIONAR_FUNCION -> seleccionar tk_par_izq EXPRESION tk_par_der entre BLOQUE_SELECCIONAR_FUNCION fin_seleccionar" );
  grammar.PB( "BLOQUE_SELECCIONAR_FUNCION -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION" );
  grammar.PB( "BLOQUE_SELECCIONAR_FUNCION -> defecto tk_dosp BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES_FUNCION COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> defecto tk_dosp BLOQUE_INSTRUCCIONES_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR_FUNCION -> epsilon" );
  grammar.PB( "CICLO_MIENTRAS_FUNCION -> mientras tk_par_izq EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_mientras" );
  grammar.PB( "CICLO_HACER_MIENTRAS_FUNCION -> hacer BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> mientras tk_par_izq EXPRESION tk_par_der AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION" );
  grammar.PB( "AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> hacer BLOQUE_INSTRUCCIONES_FUNCION fin_mientras BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION" );
  grammar.PB( "AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> tk_pyc" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> LEER" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> IMPRIMIR" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> DECLARACION_O_ASIGNACION_O_LLAMADO" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CONDICIONAL_SI_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> SELECCIONAR_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CICLO_HACER_MIENTRAS_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> CICLO_PARA_FUNCION" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> RETORNAR" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS_FUNCION -> ROMPER" );
  grammar.PB( "CICLO_PARA_FUNCION  -> para tk_par_izq CICLO_PARA_FUNCION_1 tk_pyc EXPRESION tk_pyc EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES_FUNCION fin_para" );
  grammar.PB( "CICLO_PARA_FUNCION_1 -> TIPO_DATO_PRIMITIVO id tk_asig EXPRESION" );
  grammar.PB( "CICLO_PARA_FUNCION_1 -> id COMPLEMENTO_CICLO_PARA_FUNCION_1" );
  grammar.PB( "COMPLEMENTO_CICLO_PARA_FUNCION_1 -> id tk_asig EXPRESION" );
  grammar.PB( "COMPLEMENTO_CICLO_PARA_FUNCION_1 -> tk_asig EXPRESION" );
  grammar.PB( "ROMPER -> romper tk_pyc" );
  grammar.PB( "RETORNAR -> retornar EXPRESION tk_pyc" );
  grammar.PB( "DECLARACION_O_ASIGNACION_O_LLAMADO -> TIPO_DATO_PRIMITIVO DECLARACION_VARIABLE tk_pyc" );
  grammar.PB( "DECLARACION_O_ASIGNACION_O_LLAMADO -> id COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO tk_pyc" );
  grammar.PB( "COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> IDENTIFICADOR_DATO_ESTRUCTURA ASIGNACION_VARIABLE" );
  grammar.PB( "COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> ASIGNACION_VARIABLE" );
  grammar.PB( "COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> LLAMADO_FUNCION" );
  grammar.PB( "COMPLEMENTO_DECLARACION_O_ASIGNACION_O_LLAMADO -> DECLARACION_VARIABLE" );
  grammar.PB( "IMPRIMIR -> imprimir tk_par_izq PARAMETROS_IMPRIMIR tk_par_der tk_pyc" );
  grammar.PB( "PARAMETROS_IMPRIMIR -> EXPRESION MAS_PARAMETROS_IMPRIMIR" );
  grammar.PB( "MAS_PARAMETROS_IMPRIMIR -> tk_coma EXPRESION MAS_PARAMETROS_IMPRIMIR" );
  grammar.PB( "MAS_PARAMETROS_IMPRIMIR -> epsilon" );
  grammar.PB( "LEER -> leer tk_par_izq IDENTIFICADOR tk_par_der tk_pyc" );
  grammar.PB( "DECLARACION_VARIABLE -> VARIABLE MAS_DECLARACION_VARIABLE" );
  grammar.PB( "MAS_DECLARACION_VARIABLE -> tk_coma VARIABLE MAS_DECLARACION_VARIABLE" );
  grammar.PB( "MAS_DECLARACION_VARIABLE -> epsilon" );
  grammar.PB( "VARIABLE -> id ASIGNACION_VARIABLE" );
  grammar.PB( "ASIGNACION_VARIABLE -> tk_asig EXPRESION" );
  grammar.PB( "ASIGNACION_VARIABLE -> epsilon" );
  grammar.PB( "LLAMADO_FUNCION -> tk_par_izq PARAMETROS_LLAMADO_FUNCION tk_par_der" );
  grammar.PB( "PARAMETROS_LLAMADO_FUNCION -> EXPRESION MAS_PARAMETROS_LLAMADO_FUNCION" );
  grammar.PB( "PARAMETROS_LLAMADO_FUNCION -> epsilon" );
  grammar.PB( "MAS_PARAMETROS_LLAMADO_FUNCION -> tk_coma EXPRESION MAS_PARAMETROS_LLAMADO_FUNCION" );
  grammar.PB( "MAS_PARAMETROS_LLAMADO_FUNCION -> epsilon" );
  grammar.PB( "IDENTIFICADOR -> id COMPLEMENTO_IDENTIFICADOR" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR -> tk_punto id COMPLEMENTO_IDENTIFICADOR" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR -> epsilon" );
  grammar.PB( "IDENTIFICADOR_DATO_ESTRUCTURA -> tk_punto id COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA -> tk_punto id COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR_DATO_ESTRUCTURA -> epsilon" );
  grammar.PB( "TIPO_DATO  -> TIPO_DATO_PRIMITIVO" );
  grammar.PB( "TIPO_DATO  -> id" );
  grammar.PB( "TIPO_DATO_PRIMITIVO -> entero" );
  grammar.PB( "TIPO_DATO_PRIMITIVO -> real" );
  grammar.PB( "TIPO_DATO_PRIMITIVO -> caracter" );
  grammar.PB( "TIPO_DATO_PRIMITIVO -> cadena" );
  grammar.PB( "TIPO_DATO_PRIMITIVO -> booleano" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> LEER BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> IMPRIMIR BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> DECLARACION_O_ASIGNACION_O_LLAMADO BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> CONDICIONAL_SI BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> SELECCIONAR BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> CICLO_MIENTRAS BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> CICLO_HACER_MIENTRAS BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> CICLO_PARA BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> ROMPER BLOQUE_INSTRUCCIONES" );
  grammar.PB( "BLOQUE_INSTRUCCIONES -> epsilon" );
  grammar.PB( "CONDICIONAL_SI -> si tk_par_izq EXPRESION tk_par_der entonces BLOQUE_INSTRUCCIONES COMPLEMENTO_CONDICIONAL_SI" );
  grammar.PB( "COMPLEMENTO_CONDICIONAL_SI -> si_no BLOQUE_INSTRUCCIONES fin_si" );
  grammar.PB( "COMPLEMENTO_CONDICIONAL_SI -> fin_si" );
  grammar.PB( "SELECCIONAR -> seleccionar tk_par_izq EXPRESION tk_par_der entre BLOQUE_SELECCIONAR fin_seleccionar" );
  grammar.PB( "BLOQUE_SELECCIONAR -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES COMPLEMENTO_BLOQUE_SELECCIONAR" );
  grammar.PB( "BLOQUE_SELECCIONAR -> defecto tk_dosp BLOQUE_INSTRUCCIONES" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR -> caso EXPRESION tk_dosp BLOQUE_INSTRUCCIONES COMPLEMENTO_BLOQUE_SELECCIONAR" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR -> defecto tk_dosp BLOQUE_INSTRUCCIONES" );
  grammar.PB( "COMPLEMENTO_BLOQUE_SELECCIONAR -> epsilon" );
  grammar.PB( "CICLO_MIENTRAS -> mientras tk_par_izq EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES fin_mientras" );
  grammar.PB( "CICLO_HACER_MIENTRAS -> hacer BLOQUE_INSTRUCCIONES_HACER_MIENTRAS" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS BLOQUE_INSTRUCCIONES_HACER_MIENTRAS" );
  grammar.PB( "BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> mientras tk_par_izq EXPRESION tk_par_der AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS" );
  grammar.PB( "AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> hacer BLOQUE_INSTRUCCIONES fin_mientras BLOQUE_INSTRUCCIONES_HACER_MIENTRAS" );
  grammar.PB( "AUXILIAR_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> tk_pyc" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> LEER" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> IMPRIMIR" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> DECLARACION_O_ASIGNACION_O_LLAMADO" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CONDICIONAL_SI" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> SELECCIONAR" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CICLO_HACER_MIENTRAS" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> CICLO_PARA" );
  grammar.PB( "COMPLEMENTO_BLOQUE_INSTRUCCIONES_HACER_MIENTRAS -> ROMPER" );
  grammar.PB( "CICLO_PARA  -> para tk_par_izq CICLO_PARA_1 tk_pyc EXPRESION tk_pyc EXPRESION tk_par_der hacer BLOQUE_INSTRUCCIONES fin_para" );
  grammar.PB( "CICLO_PARA_1 -> TIPO_DATO_PRIMITIVO id tk_asig EXPRESION" );
  grammar.PB( "CICLO_PARA_1 -> id COMPLEMENTO_CICLO_PARA_1" );
  grammar.PB( "COMPLEMENTO_CICLO_PARA_1 -> id tk_asig EXPRESION" );
  grammar.PB( "COMPLEMENTO_CICLO_PARA_1 -> tk_asig EXPRESION" );
  grammar.PB( "EXPRESION  -> TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION -> tk_y TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION -> tk_o TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION -> EXPRESION_1" );
  grammar.PB( "EXPRESION_1 -> tk_igual TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_1 -> tk_dif TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_1 -> EXPRESION_2" );
  grammar.PB( "EXPRESION_2 -> tk_menor TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_2 -> tk_mayor TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_2 -> tk_menor_igual TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_2 -> tk_mayor_igual TERMINO COMPLEMENTO_EXPRESION_1_2" );
  grammar.PB( "EXPRESION_2 -> EXPRESION_3" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_y TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_o TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_mas TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_menos TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_mult TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_mod TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> tk_div TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "COMPLEMENTO_EXPRESION_1_2 -> epsilon" );
  grammar.PB( "EXPRESION_3 -> tk_mas TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "EXPRESION_3 -> tk_menos TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "EXPRESION_3 -> EXPRESION_4" );
  grammar.PB( "EXPRESION_4 -> tk_mult TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "EXPRESION_4 -> tk_mod TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "EXPRESION_4 -> tk_div TERMINO COMPLEMENTO_EXPRESION" );
  grammar.PB( "EXPRESION_4 -> epsilon" );
  grammar.PB( "TERMINO -> tk_par_izq EXPRESION tk_par_der" );
  grammar.PB( "TERMINO -> CONSTANTE" );
  grammar.PB( "TERMINO -> IDENTIFICADOR_O_LLAMADO" );
  grammar.PB( "TERMINO -> verdadero" );
  grammar.PB( "TERMINO -> falso" );
  grammar.PB( "TERMINO -> tk_menos TERMINO" );
  grammar.PB( "TERMINO -> tk_mas TERMINO" );
  grammar.PB( "TERMINO -> tk_neg TERMINO" );
  grammar.PB( "IDENTIFICADOR_O_LLAMADO -> id COMPLEMENTO_IDENTIFICADOR_O_LLAMADO" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> LLAMADO_FUNCION" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> IDENTIFICADOR_DATO_ESTRUCTURA" );
  grammar.PB( "COMPLEMENTO_IDENTIFICADOR_O_LLAMADO -> epsilon" );
  grammar.PB( "CONSTANTE -> tk_entero" );
  grammar.PB( "CONSTANTE -> tk_real" );
  grammar.PB( "CONSTANTE -> tk_caracter" );
  grammar.PB( "CONSTANTE -> tk_cadena" );

}

void print_firsts( ) {
  cout << "*FIRST\n";
  for( auto& non_terminal : non_terminals ) {
    cout << non_terminal << " = { ";
    bool ok = false;
    for( auto& symbol : firsts[ non_terminal ] ) {
      if( ok )
        cout << ", ";
      cout << symbol;
      ok = true;
    }
    cout << " }\n";
  }
}

void print_follows( ) {
  cout << "*FOLLOWS\n";
  for( auto& non_terminal : non_terminals ) {
    cout << non_terminal << " = { ";
    bool ok = false;
    for( auto& symbol : follows[ non_terminal ] ) {
      if( ok )
        cout << ", ";
      cout << symbol;
      ok = true;
    }
    cout << " }\n";
  }
}

void print_rule( string non_terminal, vector< string > rule ) {
  cout << non_terminal << " ->";
  for( auto& symbol : rule )
    cout << " " << symbol;
  cout << "\n";
}

void print_predictions( ) {
  cout << "*PREDICTIONS\n";
  for( auto& non_terminal : non_terminals ) {
    for( int i = 0; i < int( rules[ non_terminal ].size( ) ); i++ ) {
      auto& rule = rules[ non_terminal ][ i ];
      print_rule( non_terminal, rule );
      cout << "\t{ ";
      bool ok = false;
      for( auto& symbol : predictions[ non_terminal ][ i ] ) {
        if( ok )
          cout << ", ";
        cout << symbol;
        ok = true;
      }
      cout << " }\n";
    }
  }
}
