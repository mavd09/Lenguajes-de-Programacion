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

const int MAX = int( 1e7 );
const int MOD = int( 1e9+7 );
const int oo  = INT_MAX;

const string EPS = "epsilon";

map< string, vector< vector< string > > > rules;
set< string > terminals, non_terminals;

map< string, set< string > > firsts, follows;
map< string, vector< set< string > > > predictions;

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

void generateFunctions( ) {
  for( auto& non_terminal : non_terminals ) {
    cout << "void " << non_terminal << "( ) {\n";
    for( int i = 0; i < int( rules[ non_terminal ].size( ) ); i++ ) {
      bool fst = true;
      cout << "\tif(";
      for( auto& p : predictions[ non_terminal ][ i ] ) {
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
    cout << "\tcout << \"Error " << non_terminal << " \\n\";\n";
    cout << "\texit( 0 );\n";
    cout << "}\n";
  }
}

int main( ) {

  #ifdef LOCAL
    freopen( "Grammar PsiCoder.cpp", "r", stdin );
    freopen( "C:\\Users\\Alejandro\\Documents\\Universidad\\Lenguajes de Programaci\ón\\Practice 2\\Predictive parser PsiCoder\\parser.cpp", "w", stdout );
  #endif // LOCAL

  ios_base::sync_with_stdio( 0 );
  cin.tie( 0 ); cout.tie( 0 );

  read_grammar( );

  compute_firsts( );
  compute_follows( );
  compute_predictions( );

  //print_firsts( );
  //print_follows( );
  //print_predictions( );

  ifstream fin( "C:\\Users\\Alejandro\\Documents\\Universidad\\Lenguajes de Programaci\ón\\Practice 2\\Predictive parser PsiCoder\\template.cpp" );
  string line;
  for( int l = 1; getline( fin, line ); l++ ) {
    if( l == 60 ) {
      for( auto& non_terminal : non_terminals ) {
        cout << "void " << non_terminal << "( );\n";
      }
    }
    cout << line << "\n";
  }
  fin.close( );

  generateFunctions( );

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
