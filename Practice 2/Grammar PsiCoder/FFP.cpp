#include <bits/stdc++.h>

#define PB          push_back
#define PF          push_front
#define MP          make_pair
#define FI          first
#define SE          second

using namespace std;

typedef long long           ll;
typedef unsigned long long  ull;
typedef long double         lf;
typedef pair< int, int >    pii;
typedef vector< int >       vi;
typedef vector< vi >        vvi;
typedef pair< int, string > is;

const int MAX = int( 1e7 );
const int MOD = int( 1e9+7 );
const int oo  = INT_MAX;

const string EPS = "epsilon";

map< string, vector< vector< string > > > rules;
set< string > terminals, non_terminals;

map< string, set< string > > firsts, follows;
map< string, vector< set< string > > > predictions;

map< string, is > token_symbol;

void initialize( );
void print_firsts( );
void print_follows( );
void print_predictions( );
void print_rule( );

void classify_symbol( string current ) {
  if( 'a' <= current[ 0 ] && current[ 0 ] <= 'z' )
    terminals.insert( current );
  else
    non_terminals.insert( current );
}

void read_grammar( ) {
  bool initial_symbol = true;
  string rule, lft, rght, garbage;
  while( getline( cin, rule ) ) {
    if( int( rule.size( ) ) == 0 || rule[ 0 ] == '/' )
      continue;
    stringstream ss_in( rule );
    vector< string > current_rule;
    ss_in >> lft >> garbage;
    classify_symbol( lft );
    if( initial_symbol ) {
      follows[ lft ].insert( "$" );
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

void generate_functions( ) {
  for( auto& non_terminal : non_terminals ) {
    set< is > expected_tokens;
    cout << "void " << non_terminal << "( ) {\n";
    for( int i = 0; i < int( rules[ non_terminal ].size( ) ); i++ ) {
      bool fst = true;
      cout << "\tif(";
      for( auto& p : predictions[ non_terminal ][ i ] ) {
        expected_tokens.insert( token_symbol[ p ] );
        if( !fst )
          cout << " ||";
        cout << " token == \"" << p << "\"";
        fst = false;
      }
      cout << " ) {\n";
      for( auto& s : rules[ non_terminal ][ i ] ) {
        if( s == EPS )
          continue;
        if( terminals.count( s ) ) {
          cout << "\t\tmatch( \"" << s << "\" );\n";
        }
        else {
          cout << "\t\t" << s << "( );\n";
        }
      }
      if( int( rules[ non_terminal ][ i ].size( ) ) == 1 && rules[ non_terminal ][ i ][ 0 ] == EPS ) {
      }
      cout << "\t\treturn ;\n";
      cout << "\t}\n";
    }
    cout << "\tcout << \"<\" << row << \",\" << col << \"> Error sintactico: se encontro: \\\"\" << token << \"\\\"; se esperaba:";
    bool fst = true;
    for( auto& e : expected_tokens ) {
      if( !fst )
        cout << ",";
      else
        fst = false;
      cout << " \\\"" << e.SE << "\\\"";
    }
    cout << ".\\n\";\n";
//    cout << "\tcout << \"Error " << non_terminal << " \\n\";\n";
    cout << "\texit( 0 );\n";
    cout << "}\n";
  }
}

string verify_predictions( ) {
  for( auto& non_terminal : non_terminals ) {
    set< string > all;
    for( auto& v : predictions[ non_terminal ] ) {
      for( auto& s : v ){
        if( all.count( s ) ) return "bad";
        all.insert( s );
      }
    }
  }
  return "good";
}

int main( ) {

  #ifdef LOCAL
    freopen( "Grammar PsiCoder.cpp", "r", stdin );
    freopen( "C:\\Users\\Alejandro\\Documents\\Universidad\\Lenguajes de Programaci\�n\\Practice 2\\Predictive parser PsiCoder\\parser.cpp", "w", stdout );
  #endif // LOCAL

  ios_base::sync_with_stdio( 0 );
  cin.tie( 0 ); cout.tie( 0 );

  initialize( );

  read_grammar( );

  compute_firsts( );
  compute_follows( );
  compute_predictions( );

  cerr << "The prediction's set is " << verify_predictions( ) << "\n";

  //print_firsts( );
  //print_follows( );
  //print_predictions( );

  ifstream fin( "C:\\Users\\Alejandro\\Documents\\Universidad\\Lenguajes de Programaci\�n\\Practice 2\\Predictive parser PsiCoder\\template.cpp" );
  string line;
  for( int l = 1; getline( fin, line ); l++ ) {
    if( line == "void initialize( );" ) {
      for( auto& non_terminal : non_terminals ) {
        cout << "void " << non_terminal << "( );\n";
      }
    }
    cout << line << "\n";
  }
  fin.close( );

  generate_functions( );

  return 0;
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

void initialize( ) {
  token_symbol["tk_mas"] = is(0, "+");
  token_symbol["tk_menos"] = is(1, "-");
  token_symbol["tk_mult"] = is(2, "*");
  token_symbol["tk_div"] = is(3, "/");
  token_symbol["tk_mod"] = is(4, "%");
  token_symbol["tk_asig"] = is(5, "=");
  token_symbol["tk_menor"] = is(6, "<");
  token_symbol["tk_mayor"] = is(7, ">");
  token_symbol["tk_menor_igual"] = is(8, "<=");
  token_symbol["tk_mayor_igual"] = is(9, ">=");
  token_symbol["tk_igual"] = is(10, "==");
  token_symbol["tk_y"] = is(11, "&&");
  token_symbol["tk_o"] = is(12, "||");
  token_symbol["tk_dif"] = is(13, "!=");
  token_symbol["tk_neg"] = is(14, "!");
  token_symbol["tk_dosp"] = is(15, ":");
  token_symbol["tk_comilla_sen"] = is(16, "'");
  token_symbol["tk_comilla_dob"] = is(17, "\"");
  token_symbol["tk_pyc"] = is(18, ";");
  token_symbol["tk_coma"] = is(19, ",");
  token_symbol["tk_punto"] = is(20, ".");
  token_symbol["tk_par_izq"] = is(21, "(");
  token_symbol["tk_par_der"] = is(22, ")");
  token_symbol["id"] = is(23, "identificador");
  token_symbol["tk_entero"] = is(24, "valor_entero");
  token_symbol["tk_real"] = is(25, "valor_real");
  token_symbol["tk_caracter"] = is(26, "valor_caracter");
  token_symbol["tk_cadena"] = is(27, "valor_cadena");
  token_symbol["funcion_principal"] = is(28, "funcion_principal");
  token_symbol["fin_principal"] = is(29, "fin_principal");
  token_symbol["leer"] = is(30, "leer");
  token_symbol["imprimir"] = is(31, "imprimir");
  token_symbol["booleano"] = is(32, "booleano");
  token_symbol["caracter"] = is(33, "caracter");
  token_symbol["entero"] = is(34, "entero");
  token_symbol["real"] = is(35, "real");
  token_symbol["cadena"] = is(36, "cadena");
  token_symbol["si"] = is(37, "si");
  token_symbol["entonces"] = is(38, "entonces");
  token_symbol["fin_si"] = is(39, "fin_si");
  token_symbol["si_no"] = is(40, "si_no");
  token_symbol["mientras"] = is(41, "mientras");
  token_symbol["hacer"] = is(42, "hacer");
  token_symbol["fin_mientras"] = is(43, "fin_mientras");
  token_symbol["para"] = is(44, "para");
  token_symbol["fin_para"] = is(45, "fin_para");
  token_symbol["seleccionar"] = is(46, "seleccionar");
  token_symbol["entre"] = is(47, "entre");
  token_symbol["caso"] = is(48, "caso");
  token_symbol["romper"] = is(49, "romper");
  token_symbol["defecto"] = is(50, "defecto");
  token_symbol["fin_seleccionar"] = is(51, "fin_seleccionar");
  token_symbol["estructura"] = is(52, "estructura");
  token_symbol["fin_estructura"] = is(53, "fin_estructura");
  token_symbol["funcion"] = is(54, "funcion");
  token_symbol["fin_funcion"] = is(55, "fin_funcion");
  token_symbol["retornar"] = is(56, "retornar");
  token_symbol["falso"] = is(57, "falso");
  token_symbol["verdadero"] = is(58, "verdadero");
  token_symbol["$"] = is(59, "EOF");
}
