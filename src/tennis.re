type player =
  | PlayerOne
  | PlayerTwo;

type point =
  | Love
  | Fifteen
  | Thirty;

type pointsData = {
  playerOne: point,
  playerTwo: point,
};

type fortyData = {
  player,
  otherPlayerPoint: point,
};

type score =
  | Points(pointsData)
  | Forty(fortyData)
  | Deuce
  | Advantage(player)
  | Game(player);

let scoreWhenDeuce: player => score = winner => Advantage(winner);

let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) => advantagedPlayer == winner ? Game(winner) : Deuce;

let other = player =>
  switch (player) {
  | PlayerOne => PlayerTwo
  | PlayerTwo => PlayerOne
  };

let incrementPoint: point => option(point) =
  point =>
    switch (point) {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };

let scoreWhenForty: (fortyData, player) => score =
  (current, winner) =>
    current.player == winner ?
      Game(winner) :
      (
        switch (incrementPoint(current.otherPlayerPoint)) {
        | Some(p) => Forty({...current, otherPlayerPoint: p})
        | None => Deuce
        }
      );

let pointTo = (player, point, current) =>
  switch (player) {
  | PlayerOne => {...current, playerOne: point}
  | PlayerTwo => {...current, playerTwo: point}
  };

let pointFor = (player, current) =>
  switch (player) {
  | PlayerOne => current.playerOne
  | PlayerTwo => current.playerTwo
  };

let scoreWhenPoints = (current, winner) =>
  switch (current |> pointFor(winner) |> incrementPoint) {
  | Some(np) => Points(pointTo(winner, np, current))
  | None => Forty({player: winner, otherPlayerPoint: current |> pointFor(other(winner))})
  };

let scoreWhenGame = winner => Game(winner);

let score = (current, winner) =>
  switch (current) {
  | Points(p) => scoreWhenPoints(p, winner)
  | Forty(f) => scoreWhenForty(f, winner)
  | Deuce => scoreWhenDeuce(winner)
  | Advantage(a) => scoreWhenAdvantage(a, winner)
  | Game(g) => scoreWhenGame(g)
  };

let newGame = Points({playerOne: Love, playerTwo: Love});

let string_of_player = player =>
  switch (player) {
  | PlayerOne => "Player One"
  | PlayerTwo => "Player Two"
  };

let string_of_point = point =>
  switch (point) {
  | Love => "0"
  | Fifteen => "15"
  | Thirty => "30"
  };

print_string(string_of_player(PlayerOne));

print_string(string_of_point(Love));

let string_of_score = score =>
  switch (score) {
  | Points(p) =>
    string_of_player(PlayerOne)
    ++ " : "
    ++ string_of_point(p.playerOne)
    ++ ", "
    ++ string_of_player(PlayerTwo)
    ++ " : "
    ++ string_of_point(p.playerTwo)
  | Forty(f) =>
    string_of_player(f.player) ++ " has " ++ "40 and otherPlayer has " ++ string_of_point(f.otherPlayerPoint)
  | Deuce => "40 - 40"
  | Advantage(a) => "Advantage for " ++ string_of_player(a)
  | Game(g) => "Game for " ++ string_of_player(g)
  };
