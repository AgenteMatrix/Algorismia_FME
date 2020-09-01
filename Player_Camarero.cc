#include "Player.hh"

#define PLAYER_NAME CAAAMAREROO

struct PLAYER_NAME : public Player {

  static Player* factory () {
    return new PLAYER_NAME;
  }
  
  using VE = vector<int>;
  using PE = pair<int, int>;
  using PPE = pair<int,PE>;
  using PEP = pair<int, Pos>;
  using VB = vector<bool>;
  using MB = vector<VB>;
  using VPE = vector<PE>;
  using MPE = vector<VPE>;

  VE D, W, T;
  int n, m, t;
  vector<PE> VTreasures;
  vector<PE> VConquer;
  vector<PE> VSeek_and_Destroy;
  set<int> usat_d;
  MB B;

  void attack(){
    for(int l = 0; l < n; ++l){
       int id = D[l];
       Pos p = unit(id).pos;
       if(B[p.i][p.j]) {
        bool safe = false;
        for(int k = 0; k < 8 and not safe; ++k) {
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type == Cave and cell(p2).type == Outside and cell(p2).id == -1 and not B[p2.i][p2.j]) {
            command(id, Dir(k));
            usat_d.insert(id);
            safe = true;
          }
        }
       }
       else {
        int a = 0;
        int b = 0;
        bool enemic = false;
         for(int k = 0; k < 8; ++k){
           Pos p2 = p + Dir(k);
           if(pos_ok(p2)){
             int id2 = cell(p2).id; 
             if(id2 != -1 and unit(id2).player != me()) {
              enemic = true;
              if(unit(id2).type == Dwarf) {
                if(2 > a) {
                  a = 2;
                  b = k;
                }
              }
              if(unit(id2).type == Wizard) {
                if(3 > a) {
                  a = 3;
                  b = k;
                }
              }
              if(unit(id2).type == Orc) {
                if(1 > a) {
                  a = 1;
                  b = k;
                }
              }
            }
          }
        }
        if(enemic) {
          command(id, Dir(b));
          usat_d.insert(id);
        }
      }
    }
  }

  void treasures(){
    vector<vector<PPE>> dist(rows(),vector<PPE>(cols(),{1e9, {-1,-1}}));
    priority_queue <PEP> Q;
    for(int k = 0; k < n; ++k){
      set<int>::iterator it = usat_d.find(D[k]);
      if(it == usat_d.end()) {
        Pos p = unit(D[k]).pos;
        PPE a = {0,{D[k],-1}};
        dist[p.i][p.j] = a;
        Q.push({0,p});
      }
    }
    bool no_interesa = false;
    while(not Q.empty() and not no_interesa){
      PEP a = Q.top(); Q.pop();
      int d = -a.first;
      if(d > (200 - round())) no_interesa = true; 
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(cell(p).treasure) VTreasures.push_back(dist[p.i][p.j].second);
        for(int k = 0; k < 8; ++k){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j]){
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PPE trio;
              trio.first = d2;
              trio.second = dist[p.i][p.j].second;
              if(trio.second.second == -1) trio.second.second = k;
              dist[p2.i][p2.j] = trio;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  }

  void conquer(){
    vector<vector<PPE>> dist(rows(),vector<PPE>(cols(),{1e9,{-1,-1}}));
    priority_queue <PEP> Q;
    for(int k = 0; k < n; ++k){
      set<int>::iterator it = usat_d.find(D[k]);
      if(it == usat_d.end()){
        Pos p = unit(D[k]).pos;
        PPE a = {0,{D[k],-1}};
        dist[p.i][p.j] = a;
        Q.push({0,p});
      }
    }
    bool no_interesa = false;
    while(not Q.empty() and not no_interesa){
      PEP a = Q.top(); Q.pop();
      int d = -a.first;
      if(d > (200 - round())) no_interesa = true; 
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(cell(p).owner != me() and cell(p).type != Outside) VConquer.push_back(dist[p.i][p.j].second);
        for(int k = 0; k < 8; ++k){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j]){
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PPE trio;
              trio.first = d2;
              trio.second = dist[p.i][p.j].second;
              if(trio.second.second == -1) trio.second.second = k;
              dist[p2.i][p2.j] = trio;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  }

  void  Seek_and_DestroyI(){
    vector<vector<PPE>> dist(rows(),vector<PPE>(cols(),{1e9,{-1,-1}}));
    priority_queue <PEP> Q;
    for(int k = 0; k < n; ++k){
      set<int>::iterator it = usat_d.find(D[k]);
      if(it == usat_d.end()){
        Pos p = unit(D[k]).pos;
        PPE a = {0,{D[k],-1}};
        dist[p.i][p.j] = a;
        Q.push({0,p});
      }
    }
    bool no_interesa = false;
    while(not Q.empty() and not no_interesa){
      PEP a = Q.top(); Q.pop();
      int d = -a.first;
      if(d > (200 - round())) no_interesa = true; 
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(cell(p).id != -1 and (unit(cell(p).id).type == Dwarf or unit(cell(p).id).type == Wizard) and unit(cell(p).id).player != me() and (d > 0 or cell(p).id == -1)) {
          VSeek_and_Destroy.push_back(dist[p.i][p.j].second);
        }
        for(int k = 0; k < 8; ++k){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j]){
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PPE trio;
              trio.first = d2;
              trio.second = dist[p.i][p.j].second;
              if(trio.second.second == -1) trio.second.second = k;
              dist[p2.i][p2.j] = trio;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  }

   void Seek_and_DestroyII(int id){
    MPE dist(rows(), VPE(cols(),{1e9,-1}));
    priority_queue <PEP> Q;
    Pos p = unit(id).pos;
    PE a = {0,-1};
    dist[p.i][p.j] = a;
    Q.push({0,p});
    while(not Q.empty()){
      PEP a = Q.top(); Q.pop();
      int d = -a.first; 
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(cell(p).id != -1 and (unit(cell(p).id).type == Dwarf or unit(cell(p).id).type == Wizard) and unit(cell(p).id).player != me() and (d > 0 or cell(p).id == -1)) {
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(id, Dir(dist[p.i][p.j].second));
            usat_d.insert(id);
          }
        }
        for(int k = 0; k < 8; ++k){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j]){
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PE par;
              par.first = d2;
              par.second = dist[p.i][p.j].second;
              if(par.second == -1) par.second = k;
              dist[p2.i][p2.j] = par;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  }

   void Do_Not_Disturb(int id){
    MPE dist(rows(),vector<PE>(cols(),{1e9,-1}));
    priority_queue <PEP> Q;
    Pos p = unit(id).pos;
    if(cell(p).type == Outside) return;
    PE a = {0,-1};
    dist[p.i][p.j] = a;
    Q.push({0,p});
    while(not Q.empty()){
      PEP a = Q.top(); Q.pop();
      int d = -a.first;
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(pos_ok(p) and cell(p).type == Outside) {
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(id, Dir(dist[p.i][p.j].second)); 
            usat_d.insert(id); 
          }
        }
        for(int k = 0; k < 8; ++k){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j] and (d > 0 or cell(p2).id == -1)){
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PE par;
              par.first = d2;
              par.second = dist[p.i][p.j].second;
              if(par.second == -1) par.second = k;
              dist[p2.i][p2.j] = par;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  } 

  void observe() {
    for(int k = 0; k < n; ++k) {
      int id = D[k];
      Pos p = unit(id).pos;
      for(int l = 0; l < 8; ++l) {
        Pos p2 = p + Dir(l);
        if(pos_ok(p2) and cell(p2).id != -1 and unit(cell(p2).id).player == me() and unit(cell(p2).id).type == Wizard) {
          Support(id);
        }
      }
    }
  }

  void Support(int id){
    MPE dist(rows(), VPE(cols(),{1e9,-1}));
    priority_queue <PEP> Q;
    Pos p = unit(id).pos;
    PE a = {0,-1};
    dist[p.i][p.j] = a;
    Q.push({0,p});
    while(not Q.empty()){
      PEP a = Q.top(); Q.pop();
      int d = -a.first; 
      Pos p = a.second;
      if(d == dist[p.i][p.j].first){
        if(cell(p).id != -1 and (unit(cell(p).id).type == Dwarf or unit(cell(p).id).type == Wizard) and unit(cell(p).id).player != me()) {
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(id, Dir(dist[p.i][p.j].second));
            usat_d.insert(id);
          }
        }
        for(int k = 0; k < 8; k += 2){
          Pos p2 = p + Dir(k);
          if(pos_ok(p2) and cell(p2).type != Granite and cell(p2).type != Abyss and not B[p2.i][p2.j] and (d > 0 or cell(p2).id == -1)) {
            int c = 1;
            if(cell(p2).type == Rock) c += cell(p2).turns;
            int d2 = d + c;
            if(d2 < dist[p2.i][p2.j].first){
              PE par;
              par.first = d2;
              par.second = dist[p.i][p.j].second;
              if(par.second == -1) par.second = k;
              dist[p2.i][p2.j] = par;
              Q.push({-d2,p2});
            }
          }
        }
      }
    }
  }

  void Unite(int id) {
    Pos p = unit(id).pos;
    set<Pos> exp;
    exp.insert(p);
    queue<PEP> Q;
    Q.push({-1,p});
    while(not Q.empty()){
      PEP a = Q.front(); Q.pop();
      p = a.second;
      int d = a.first;
      for(int k = 0; k < 8; k++){
        Pos p2 = p + Dir(k);
        set<Pos>::iterator it = exp.find(p2);
        if(it == exp.end()){
          exp.insert(p2);
          if(pos_ok(p2) and cell(p2).type != Abyss and cell(p2).type != Granite and cell(p2).type != Rock and not B[p2.i][p2.j]){
            if(cell(p2).id != -1 and unit(cell(p2).id).player == me() and unit(cell(p2).id).type == Dwarf){
              if(d == -1) return command(id, Dir(k));
              return command(id, Dir(d));
            }
            else{
              if(d == -1) Q.push({k,p2});
              else Q.push({d,p2});
            }
          }
        }
      }
    }
  }

  void move_dwarves(){
    if(status(me()) < 0.9) {
      if(round() < 5) {
        attack();
        for(int k = 0; k < n; ++k) {
          set<int>::iterator it = usat_d.find(D[k]);
          if(it == usat_d.end()) {
            Unite(D[k]);
          }
        }

      }
      else if(round() < 180) {
        attack();
        observe();
        treasures();
        for(int k = 0; k < (int) VTreasures.size(); ++k){
          int id = VTreasures[k].first;
          int dir = VTreasures[k].second;
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(id,Dir(dir));
            usat_d.insert(id);
          }
        }
        Seek_and_DestroyI();
        for(int k = 0; k < (int) VSeek_and_Destroy.size(); ++k) {
          int id = VSeek_and_Destroy[k].first;
          int dir = VSeek_and_Destroy[k].second;
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(id,Dir(dir));
            usat_d.insert(id);
          }
        }
        for(int k = 0; k < n; ++k) {
          set<int>::iterator it = usat_d.find(D[k]);
          if(it == usat_d.end()) Seek_and_DestroyII(D[k]);
        }
        conquer();
        for(int k = 0; k < (int) VConquer.size(); ++k){
          int id = VConquer[k].first;
          int dir = VConquer[k].second;
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            if(cell(unit(id).pos + Dir(dir)).id != -1 and unit(cell(unit(id).pos + Dir(dir)).id).player == me() and unit(cell(unit(id).pos + Dir(dir)).id).type == Wizard) {
              int r = random(0, 7);
              if(pos_ok(unit(id).pos + Dir(r)) and cell(unit(id).pos + Dir(r)).type != Abyss and cell(unit(id).pos + Dir(r)).type != Granite) {
              command(id, Dir(random(0, 7)));
              usat_d.insert(id);
              }
            }
            else {
              command(id,Dir(dir));
              usat_d.insert(id);
            }
          }
        }
        for(int k = 0; k < n; ++k){
          int id = D[k];
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            Do_Not_Disturb(D[k]);
          }
        }
        for(int k = 0; k < n; ++k){
          int id = D[k];
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            command(D[k], Dir(random(0,7)));
            usat_d.insert(id);
          }
        }
      }
      else {
        attack();
        observe();
        conquer();
        for(int k = 0; k < (int) VConquer.size(); ++k){
          int id = VConquer[k].first;
          int dir = VConquer[k].second;
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            if(cell(unit(id).pos + Dir(dir)).id != -1 and unit(cell(unit(id).pos + Dir(dir)).id).player == me() and unit(cell(unit(id).pos + Dir(dir)).id).type == Wizard) {
              int r = random(0, 7);
              if(pos_ok(unit(id).pos + Dir(r)) and cell(unit(id).pos + Dir(r)).type != Abyss and cell(unit(id).pos + Dir(r)).type != Granite) {
              command(id, Dir(random(0, 7)));
              usat_d.insert(id);
              }
            }
            else {
              command(id,Dir(dir));
              usat_d.insert(id);
            }
          }
        }
        for(int k = 0; k < n; ++k) {
          set<int>::iterator it = usat_d.find(D[k]);
          if(it == usat_d.end()) Seek_and_DestroyII(D[k]);
        }
        for(int k = 0; k < n; ++k){
          int id = D[k];
          set<int>::iterator it = usat_d.find(id);
          if(it == usat_d.end()){
            Do_Not_Disturb(D[k]);
          }
        }
      }
    }
    else {
      attack();
      conquer();
      for(int k = 0; k < (int) VConquer.size(); ++k){
        int id = VConquer[k].first;
        int dir = VConquer[k].second;
        set<int>::iterator it = usat_d.find(id);
        if(it == usat_d.end()){
          if(cell(unit(id).pos + Dir(dir)).id != -1 and unit(cell(unit(id).pos + Dir(dir)).id).player == me() and unit(cell(unit(id).pos + Dir(dir)).id).type == Wizard) {
            int r = random(0, 7);
            if(pos_ok(unit(id).pos + Dir(r)) and cell(unit(id).pos + Dir(r)).type != Abyss and cell(unit(id).pos + Dir(r)).type != Granite) {
            command(id, Dir(random(0, 7)));
            usat_d.insert(id);
            }
          }
          else {
            command(id,Dir(dir));
            usat_d.insert(id);
          }
        }
      }
      Seek_and_DestroyI();
      for(int k = 0; k < (int) VSeek_and_Destroy.size(); ++k) {
        int id = VSeek_and_Destroy[k].first;
        int dir = VSeek_and_Destroy[k].second;
        set<int>::iterator it = usat_d.find(id);
        if(it == usat_d.end()){
          command(id,Dir(dir));
          usat_d.insert(id);
        }
      }
    }
  }

  void BFS_wizards(int id){
    Pos p = unit(id).pos;
    set<Pos> exp;
    exp.insert(p);
    queue<PEP> Q;
    Q.push({-1,p});
    while(not Q.empty()){
      PEP a = Q.front(); Q.pop();
      p = a.second;
      int d = a.first;
      for(int k = 0; k < 4; k++){
        Pos p2 = p + Dir(2*k);
        set<Pos>::iterator it = exp.find(p2);
        if(it == exp.end()){
          exp.insert(p2);
          if(pos_ok(p2) and cell(p2).type != Abyss and cell(p2).type != Granite and cell(p2).type != Rock and not B[p2.i][p2.j]){
            if(cell(p2).id != -1 and unit(cell(p2).id).player == me() and unit(cell(p2).id).type == Dwarf){
              if(d == -1) return command(id, Dir(2*k));
              return command(id, Dir(d));
            }
            else{
              if(d == -1) Q.push({2*k,p2});
              else Q.push({d,p2});
            }
          }
        }
      }
    }
  }

  virtual void play () {
    usat_d = set<int> ();
    VTreasures = vector<PE> (0);
    VConquer = vector<PE> (0);
    VSeek_and_Destroy = vector<PE> (0);
    D = dwarves(me());
    n = D.size();
    W = wizards(me());
    m = W.size();
    T = trolls();
    t = T.size();
    B = MB(rows(), VB(cols(), false));
    Pos pb = unit(balrog_id()).pos;
    for(int k = pb.i - 4; k < pb.i + 4; ++k) {
      for(int l = pb.j - 4; l < pb.j + 4; ++l) {
        if(pos_ok({k,l})) B[k][l] = true;
      }
    }
    for(int k = 0; k < t; ++k) {
      Pos pt = unit(T[k]).pos;
      for(int i = pt.i - 1; i <= pt.i + 1; ++i) {
        for(int j = pt.j - 1; j <= pt.j + 1; ++j) {
          if(pos_ok({i,j})) B[i][j] = true;
        }
      }
    }
    move_dwarves();
    for(int k = 0; k < m; ++k) BFS_wizards(W[k]);
  }
};

RegisterPlayer(PLAYER_NAME);