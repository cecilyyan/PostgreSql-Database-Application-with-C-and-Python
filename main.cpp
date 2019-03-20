#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "exerciser.h"
#include <sstream>
#include <string>
using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  //create table
  string creat_player =  "CREATE TABLE PLAYER("
          "PLAYER_ID SERIAL PRIMARY KEY     NOT NULL,"
          "TEAM_ID INT NOT NULL,"
          "UNIFORM_NUM INT NOT NULL,"
          "FIRST_NAME          VARCHAR(50)    NOT NULL,"
          "LAST_NAME           VARCHAR(50)    NOT NULL,"
          "MPG            INT     NOT NULL,"
          "PPG            INT     NOT NULL,"
          "RPG            INT     NOT NULL,"
          "APG            INT     NOT NULL,"
          "SPG            NUMERIC(2,1),"            
          "BPG            NUMERIC(2,1));";
  string drop_player = "DROP TABLE IF EXISTS PLAYER;";

  
  string creat_team = "CREATE TABLE TEAM("
           "TEAM_ID    SERIAL PRIMARY KEY    NOT NULL,"
           "NAME       VARCHAR(100)          NOT NULL,"
           "STATE_ID   INT                NOT NULL,"
           "COLOR_ID   INT                NOT NULL,"
           "WINS       INT                NOT NULL,"
    "LOSSES     INT                NOT NULL);";
  string drop_team = "DROP TABLE IF EXISTS TEAM;";

  
  string creat_state = "CREATE TABLE STATE("
            "STATE_ID  SERIAL PRIMARY KEY,"
            "NAME      VARCHAR(50) )";

  string drop_state = "DROP TABLE IF EXISTS STATE;";

  
  string creat_color = "CREATE TABLE COLOR("
            "COLOR_ID  SERIAL   PRIMARY KEY,"
    "NAME      VARCHAR(100)  NOT NULL );";

  string drop_color = "DROP TABLE IF EXISTS COLOR;";

    
  work W(*C);
  W.exec(drop_player);
  W.exec(creat_player);
  //  W.commit();


  W.exec(drop_team);
  W.exec(creat_team);
  // W.commit();


  W.exec(drop_state);
  W.exec(creat_state);
  // W.commit();


  W.exec(drop_color);
  W.exec(creat_color);
  W.commit();

  
  std::ifstream player_file("player.txt");
  std::string player_line;
  while(std::getline(player_file, player_line)){
    work W(*C);
    std::istringstream iss(player_line);
    int player_id;
    int team_id;
    int uni_num;
    std::string firstname;
    std::string lastname;
    int mpg;
    int ppg;
    int rpg;
    int apg;
    double spg;
    double bpg;
    if(!(iss >> player_id >> team_id >> uni_num >> firstname >> lastname >> mpg >> ppg >> rpg >> apg >> spg >> bpg)){break;}
    std::string insert_player = "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG,RPG, APG, SPG, BPG) VALUES (" +to_string(team_id)+", "+to_string(uni_num)+", '"+W.esc(firstname)+"', '"+W.esc(lastname)+"', "+to_string(mpg)+", "+to_string(ppg)+", "+to_string(rpg)+", "+to_string(apg)+", "+to_string(spg)+", "+to_string(bpg)+");";
    W.exec(insert_player);
    W.commit();
    
  }

  std::ifstream team_file("team.txt");
  std::string team_line;
  while(std::getline(team_file, team_line)){
    work W(*C);
    std::istringstream iss(team_line);
    int team_id;
    std::string name;
    int state_id;
    int color_id;
    int wins;
    int losses;

    if(!(iss >> team_id >> name >> state_id >> color_id >> wins >> losses)){break;}

    std::string insert_team = "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES( '"+W.esc(name)+"', "+to_string(state_id)+", "+to_string(color_id)+", "+to_string(wins)+", "+to_string(losses)+");";
    W.exec(insert_team);
    W.commit();

  }

  std::ifstream state_file("state.txt");
  std::string state_line;
  while(std::getline(state_file, state_line)){
    work W(*C);
    std::istringstream iss(state_line);
    int state_id;
    string name;
    
    if(!(iss >> state_id >> name)){break;}
    std::string insert_state = "INSERT INTO STATE(NAME) VALUES( '"+W.esc(name)+"' );";
    W.exec(insert_state);
    W.commit();
  }

  std::ifstream color_file("color.txt");
  std::string color_line;
  while(std::getline(color_file, color_line)){
    work W(*C);
    std::istringstream iss(color_line);
    int color_id;
    string name;

    if(!(iss >> color_id >> name)){break;}
    std::string insert_color = "INSERT INTO COLOR(NAME) VALUES( '"+W.esc(name)+"' );";
    W.exec(insert_color);
    W.commit();
  }
  
    
  

  

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files


      exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


