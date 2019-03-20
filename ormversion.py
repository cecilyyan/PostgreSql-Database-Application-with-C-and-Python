from sqlalchemy import *
db = create_engine('postgresql://postgres:passw0rd@localhost:5432/ACC_BBALL')
db.echo = False  # Try changing this to True and see what happens
metadata = MetaData(db)
players = Table('player', metadata, autoload=True)
teams = Table('team', metadata, autoload=True)
states = Table('state', metadata, autoload=True)
colors = Table('color', metadata, autoload=True)

def run(stmt):
        rs = stmt.execute()
        for row in rs:
                print row

def query1(use_mpg, min_mpg, max_mpg,
            use_ppg, min_ppg, max_ppg,
            use_rpg, min_rpg, max_rpg,
            use_apg, min_apg, max_apg,
            use_spg, min_spg, max_spg,
            use_bpg, min_bpg, max_bpg):

        print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
        if use_mpg==1:
                s = players.select((players.c.mpg <= max_mpg) & (players.c.mpg >= min_mpg))
                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
                run(s)

        if use_ppg==1:
                s = players.select((players.c.ppg <= max_ppg) & (players.c.ppg >= min_ppg))

                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
                run(s)
        if use_rpg==1:
                s = players.select((players.c.rpg <= max_rpg) & (players.c.rpg >= min_rpg))
                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")

                run(s)

        if use_apg==1:
                s = players.select((players.c.apg <= max_apg) & (players.c.apg >= min_apg))
                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
                
                run(s)

        if use_spg==1:
                s = players.select((players.c.spg <= max_spg) & (players.c.spg >= min_spg))
                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
                
                run(s)

        if use_bpg==1:
                s = players.select((players.c.bpg <= max_bpg) & (players.c.bpg >= min_bpg))
                #print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
                run(s)



query1(1, 35, 40, 0, 1, 8, 0, 1, 5, 0, 1, 3, 0, 0.1, 0.5, 1, 0.4, 0.6)



def query2(team_color):
      s = select([teams.c.name], (teams.c.color_id == colors.c.color_id)&(colors.c.name==team_color))
      print("NAME")
      run(s)

#query2("Red")



def query3(team_name):
        s = select([players.c.first_name, players.c.last_name], (players.c.team_id==teams.c.team_id)&(teams.c.name==team_name)).order_by(desc(players.c.ppg))
        print("FIRST_NAME LAST_NAME")
        run(s)
#query3("Duke")


def query4(team_state, team_color):
        s = select([players.c.first_name, players.c.last_name, players.c.uniform_num], (players.c.team_id==teams.c.team_id)&(teams.c.state_id==states.c.state_id)&(teams.c.color_id==colors.c.color_id)&(states.c.name==team_state)&(colors.c.name==team_color))
        print("FIRST_NAME LAST_NAME UNIFORM_ID")
        run(s)

#query4("NC", "Red")


def query5(num_wins):
        s = select([players.c.first_name, players.c.last_name, teams.c.name, teams.c.wins], (players.c.team_id==teams.c.team_id)&(teams.c.wins > num_wins))
        print("FIRST_NAME LAST_NAME NAME WINS")
        run(s)
#query5(12)
