#include "query_funcs.h"
#include <string>
using namespace std;

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  work W(*C);
  std::string insert_player = "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG,RPG, APG, SPG, BPG) VALUES (" +to_string(team_id)+", "+to_string(jersey_num)+", '"+W.esc(first_name)+"', '"+W.esc(last_name)+"', "+to_string(mpg)+", "+to_string(ppg)+", "+to_string(rpg)+", "+to_string(apg)+", "+to_string(spg)+", "+to_string(bpg)+");";
  W.exec(insert_player);
  W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
  work W(*C);
  std::string insert_team = "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES( '"+W.esc(name)+"', "+to_string(state_id)+", "+to_string(color_id)+", "+to_string(wins)+", "+to_string(losses)+");";
  W.exec(insert_team);
  W.commit();
}


void add_state(connection *C, string name)
{

  work W(*C);
  std::string insert_state = "INSERT INTO STATE(NAME) VALUES( '"+W.esc(name)+"' );";
  W.exec(insert_state);
  W.commit();
}


void add_color(connection *C, string name)
{
  work W(*C);
  std::string insert_color = "INSERT INTO COLOR(NAME) VALUES( '"+W.esc(name)+"' );";
  W.exec(insert_color);
  W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{


  std::ostringstream strs1;
  strs1 << min_spg;
  std::string min_spg_str = strs1.str();

  std::ostringstream strs2;
  strs2 << max_spg;
  std::string max_spg_str = strs2.str();

  std::ostringstream strs3;
  strs3 << min_bpg;
  std::string min_bpg_str = strs3.str();

  std::ostringstream strs4;
  strs4 << max_bpg;
  std::string max_bpg_str = strs4.str();
  
  if(use_mpg==0 && use_ppg==0 && use_rpg==0 && use_apg==0 && use_spg==0 && use_bpg==0){
    cout<<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
    return;
  }
  std::string sql;
  sql = "SELECT * FROM PLAYER";
  int first = 0;
  if(use_mpg==1){
    if(first == 0){
      first = 1;
      sql = sql + " WHERE ";
    }else{
      sql = sql + " AND ";
    }
  sql = sql + "MPG >= " + to_string(min_mpg) +" AND MPG <= "+to_string(max_mpg);
  }

  if(use_ppg ==1 ){
    if(first == 0){
      first = 1;
      sql = sql+ " WHERE ";
    }else{
      sql = sql+ " AND ";
    }
    sql = sql + "PPG >=  " +to_string(min_ppg) +" AND PPG <=  "+to_string(max_ppg);
  }


  if(use_rpg ==1 ){
    if(first == 0){
      first = 1;
      sql = sql+ " WHERE ";
    }else{
      sql = sql+ " AND ";
    }
    sql= sql + "RPG >=  " +to_string(min_rpg) +" AND RPG <= "+to_string(max_rpg);
   
  }


  if(use_apg ==1 ){
    if(first == 0){
      first = 1;
      sql = sql+ " WHERE ";
    }else{
      sql = sql+ " AND ";
    }
    sql= sql + "APG >= " +to_string(min_apg) +" AND APG <= "+to_string(max_apg);
   
  }


  if(use_spg ==1 ){
    if(first == 0){
      first = 1;
      sql = sql+ " WHERE ";
    }else{
      sql = sql+ " AND ";
    }
    sql= sql + "SPG >= " +min_spg_str +" AND SPG <= "+max_spg_str;
  }

  
  if(use_bpg ==1 ){
    if(first == 0){
      first = 1;
      sql = sql+ " WHERE ";
    }else{
      sql = sql+ " AND ";
    }
    sql= sql + "BPG >= " +min_bpg_str +" AND BPG <= "+max_bpg_str;
  }


  sql = sql + ";";
  //cout << sql << endl;
  
  
  
  
  nontransaction N(*C);
 
  result R( N.exec( sql ));
  cout<<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<int>() << " " << c[1].as<int>() <<" " << c[2].as<int>() << " " << c[3].as<string>() << " " << c[4].as<string>() << " " <<c[5].as<int>() << " " << c[6].as<int>() << " " << c[7].as<int>() <<" " <<c[8].as<int>() << " "<< c[9].as<string>() << " " << c[10].as<string>() <<endl;
    }

   
}


void query2(connection *C, string team_color)
{
  std::string sql;
  sql = "SELECT TEAM.NAME FROM TEAM, COLOR  WHERE TEAM.COLOR_ID=COLOR.COLOR_ID AND COLOR.NAME='"+team_color+"' ;";
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( sql ));
  cout<<"NAME"<<endl;
  for(result::const_iterator c = R.begin(); c != R.end(); ++c){
    cout<< c[0].as<string>() <<endl;
  }
  
}


void query3(connection *C, string team_name)
{
  std::string sql;
  sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER,TEAM WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.NAME='"+team_name+"' ORDER BY PLAYER.PPG DESC;";
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( sql ));
  cout<<"FIRST_NAME" << " " << "LAST_NAME"<<endl;
  for(result::const_iterator c = R.begin(); c != R.end(); ++c){
    cout<< c[0].as<string>() << " " << c[1].as<string>() << endl;
  }
}


void query4(connection *C, string team_state, string team_color)
{
  std::string sql;
  sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME,PLAYER.UNIFORM_NUM FROM PLAYER,TEAM,STATE,COLOR WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.STATE_ID=STATE.STATE_ID AND TEAM.COLOR_ID=COLOR.COLOR_ID AND STATE.NAME='"+team_state+"' AND COLOR.NAME='"+team_color+"' ;";
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( sql ));
  cout<<"FIRST_NAME" << " " << "LAST_NAME" << " " << "UNIFORM_ID"<<endl;
  for(result::const_iterator c = R.begin(); c != R.end(); ++c){
    cout<< c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<int>() << endl;
  }
  
}


void query5(connection *C, int num_wins)
{
  std::string sql;
  sql = "SELECT PLAYER.FIRST_NAME,PLAYER.LAST_NAME,TEAM.NAME,TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.WINS > "+to_string(num_wins)+";";
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( sql ));
  cout<<"FIRST_NAME" << " " << "LAST_NAME" << " " << "NAME" << " " <<"WINS" << " " <<endl;
  for(result::const_iterator c = R.begin(); c != R.end(); ++c){
    cout<< c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() << " " <<c[3].as<int>() << endl;
  }
}
