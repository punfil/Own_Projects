import Foundation

class Board {
  var width: Int
  var height: Int
  var board = [[Character]]() 
  init(){
    self.width = 3
    self.height = 3
    for i in 0...2{
      self.board.append([])
      for _ in 0...2{
        self.board[i].append("_")
      }
    }
  }
  func set_tile(_ x: Int, _ y: Int, _ c: Character){
    self.board[x][y] = c
  }
  func get_tile(_ x: Int, _ y: Int)->Character{
    return board[x][y]
  }
  func make_move(_ x: Int,_ y: Int, _ player_sign: Character){
    self.board[x][y] = player_sign
  }
  func check_free(_ x: Int, _ y: Int)->Bool{
    return self.board[x][y] == "_"
  }
  func get_width()->Int{
    return self.width
  }
  func get_height()->Int{
    return self.height
  }
  func check_winning()->Character{
    //xxx
    for i in 0...2{
      let winning_inside = self.board[i][0]
      if (winning_inside == "_"){
        continue
      }
      for j in 0...2{
        if (self.board[i][j] != winning_inside){
          break;
        }
        if (j==2){
          return winning_inside
        }
        
      }
    }
    //przekatna lewa dol prawa gorna
    var winning_inside = self.board[0][2]
    for i in 0...2{
      if (self.board[i][2-i] != winning_inside || winning_inside == "_"){
        break;
      }
      if (i==2){
        return winning_inside
      }
    }

    //przekatna lewa gora prawa dol
    winning_inside = self.board[0][0]
    for i in 0...2{
      if (self.board[i][i] != winning_inside || winning_inside == "_"){
        break;
      }
      if (i==2){
        return winning_inside
      }
    }
    //x
    //x
    //x
    for j in 0...2{
      winning_inside = self.board[0][j]
      for i in 0...2{
        if (self.board[i][j] != winning_inside || winning_inside == "_") {
          break
        }
        if (i==2){
          return winning_inside
        }
      }
    }
    return "_"
  }
}
func copy_board(_ b: Board)->Board{
  let new_board = Board()
  for i in 0...2{
    for j in 0...2{
      new_board.set_tile(i, j, b.get_tile(i, j))
    }
  }
  return new_board
}

class Player {
  var sign: Character = "x"
  var board: Board;
  init(_ sign: Character, _ board: Board){
    self.sign = sign
    self.board = board
  }
  func get_move() -> (Int, Int) {
    print("Now choose your move! Enter x")
    let x: String? = readLine()
    print("Now y")
    let y: String? = readLine()
    return (Array(x!)[0].wholeNumberValue!, Array(y!)[0].wholeNumberValue!)
  }
  func make_move(_ x: Int, _ y: Int){
    self.board.make_move(x, y, self.sign)
  }
  func get_sign()->Character{
    return self.sign
  }
  func get_board()->Board{
    return self.board
  }
  func set_level(_ level: Int){
  }
}

class AIPlayer : Player {
  var level: Int
  var change_level: Int
  
  override init(_ sign: Character, _ board: Board){
    self.level = 1
    self.change_level = self.level
    super.init(sign, board)
  }
  func return_random_move()-> (Int, Int) {
    while true{
      let x: Int = Int.random(in: 0..<board.get_width())
      let y: Int = Int.random(in: 0..<board.get_height())
      if (board.check_free(x, y) == true){
        return (x, y)
      }
    }
  }
  override func get_move()->(Int, Int){
    if (self.change_level == 1 || self.level == 1){
      //Easy
      if (self.level == 2){
        self.change_level = 2
      }
      let position = self.return_random_move()
      return (position.0, position.1)
    }
    else{
      if (self.level == 2){
        self.change_level = 1
      }
      let opponents_sign: Character = "x"
      for i in (0..<self.board.get_width()){
        for j in (0..<self.board.get_height()){
          if (self.board.check_free(i, j) == true){
            let copy = copy_board(self.board)
            copy.make_move(i, j, self.sign)
            if (copy.check_winning() == self.sign){
              return (i, j)
            }
          }
        }
      }
      for i in (0..<self.board.get_width()){
        for j in (0..<self.board.get_height()){
          if (self.board.check_free(i, j) == true){
            let copy = copy_board(self.board)
            copy.make_move(i, j, opponents_sign)
            if (copy.check_winning() == opponents_sign){
              return (i, j)
            }
          }
        }
      } 
      //Try to take center and cornerns
      if (self.board.get_tile(1, 1) == "_"){
        return (1, 1)
      }
      else if (self.board.get_tile(0, 2) == "_"){
        return (0,2)
      }
      else if (self.board.get_tile(2, 0) == "_"){
        return (2, 0)
      }
      else if (self.board.get_tile(0, 0) == "_"){
        return (0, 0)
      }
      else if (self.board.get_tile(2, 2) == "_"){
        return (2, 2)
      }
      let position = self.return_random_move()
      return (position.0, position.1)
    }
  }
  override func set_level(_ level: Int){
    self.level = level
    self.change_level = level
  }
}

class Game{
  var board: Board
  var players: [Player] = [Player]()
  var current_player: Character
  init(){
    self.board = Board()
    self.players.append(Player("x", self.board))
    self.players.append(AIPlayer("o", self.board))
    self.current_player = "x"
  }
  func change_current_player(){
    if (self.current_player == "x"){
      self.current_player = "o"
    }
    else{
      self.current_player = "x"
    }
  }
  func return_player_with_sign(_ sign: Character)->Player{
    if (self.players[0].get_sign() == sign){
      return self.players[0]
    }
    else{
      return self.players[1]
    }
  }
  func display_menu(){
    print("Please enter difficulty level from 1-3")
    let x: String? = readLine()
    let diff: Int = Array(x!)[0].wholeNumberValue!
    self.players[1].set_level(diff)
    print("You play as x, AI plays as o")
  } 
  func display_board(){
    for i in 0..<self.board.get_width(){
      for j in 0..<self.board.get_height(){
        print(self.board.get_tile(i, j), terminator: "")
      }
      print("")
    }
    for _ in 0..<self.board.get_width(){
        print("##", terminator: "")
    }
    print("")
  }
  func check_draw()->Bool{
    for i in 0..<self.board.get_width(){
      for j in 0..<self.board.get_height(){
        if (self.board.get_tile(i, j) == "_"){
          return false
        }
      }
    }
    return true
  }
  func play_one_game(){
    self.display_menu()
    self.display_board()
    var winner: Character = "_"
    while (self.check_draw() == false){
      let player = self.return_player_with_sign(self.current_player)
      let position = player.get_move()
      player.make_move(position.0, position.1)
      self.change_current_player()
      self.display_board()
      winner = self.board.check_winning()
      if (winner != "_"){
        break;
      }
    }
    if (winner=="_"){
      print("The end of the game! It's a draw!")
    }
    else{
      print("The end of the game! Wins "+String(winner))  
    }
    
  }
}
func play_tictactoe(){
  var quit = false
  print("Welcome to 184657 Panfil Wojciech's TicTacToe!")
  while (quit == false){
    print("Enter p if you want to play or q if want to exit")
    let input = readLine()
    if (Array(input!)[0] == "p"){
      let my_game: Game = Game()
      my_game.play_one_game()
    }
    else{
      quit=true
    }
  }
  print("Bye!")
}

play_tictactoe()
