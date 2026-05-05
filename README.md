# ♟️ Chess Engine — C++ Terminal Game

A terminal-based chess engine written in C++ featuring a fully functional AI opponent powered by the **Minimax algorithm with Alpha-Beta pruning**. This is my **fourth C++ project** and by far the most complex thing I've built — a complete chess implementation with legal move generation, special move rules, and a bot that can realistically challenge human players.

---

## 📸 Preview

```
8 r n b q k b n r
7 p p p p p p p p
6 . . . . . . . .
5 . . . . . . . .
4 . . . . . . . .
3 . . . . . . . .
2 P P P P P P P P
1 R N B Q K B N R
  a b c d e f g h

Col from which you are moving: e
Row from which you are moving: 2
Col to which you are moving: e
Row to which you are moving: 4

8 r n b q k b n r
7 p p p p p p p p
6 . . . . . . . .
5 . . . . . . . .
4 . . . . P . . .
3 . . . . . . . .
2 P P P P . P P P
1 R N B Q K B N R
  a b c d e f g h

score: 15

Bot played from col: e
Bot played from row: 7
Bot played to col: e
Bot played to row: 5
```

---

## ⚙️ Features

- **Full legal move generation** — every piece type follows correct chess rules, including edge cases
- **Special moves** — castling (kingside and queenside for both sides), en passant, and pawn promotion
- **Check & checkmate detection** — the engine correctly identifies when a king is in check, and ends the game on checkmate or stalemate
- **Minimax with Alpha-Beta pruning** — the AI searches 5 moves deep and picks the best move based on board evaluation
- **Piece-square tables** — the evaluation function uses position bonuses so the bot understands piece activity, not just material count
- **Input validation** — handles invalid inputs and illegal moves without crashing
- **Board score display** — shows a numerical evaluation after every move so you can see who's winning

---

## 🤖 How the Bot AI Works

The bot uses the **Minimax algorithm** — a recursive decision tree search that simulates all possible moves for both sides up to a set depth, then picks the move that leads to the best outcome assuming the opponent also plays optimally.

**Alpha-Beta pruning** is layered on top to cut off branches of the search tree that can't possibly affect the result. This significantly reduces the number of positions the engine needs to evaluate, making depth-5 search practical.

Board positions are scored using an **evaluation function** that considers:

| Factor | Description |
|--------|-------------|
| Material | Each piece type has a fixed value (pawn = 100, knight = 320, bishop = 330, rook = 500, queen = 900) |
| Position | Piece-square tables reward pieces for being on strong squares (e.g. knights in the center, rooks on open files) |
| Checkmate | A detected checkmate returns ±30,000, overriding all material considerations |
| Stalemate | Draws return 0 — the bot avoids them when winning |

The bot always plays black. White is always the human player.

---

## ♟️ Chess Rules Implemented

This engine handles the full ruleset of chess including the tricky edge cases:

**Standard moves** — all six piece types (pawn, knight, bishop, rook, queen, king) with correct movement and capture rules.

**Castling** — both kingside and queenside for white and black. The engine checks that the king hasn't moved, the relevant rook hasn't moved, the squares between are empty, and none of the squares the king passes through are under attack.

**En passant** — tracked via a column index that stores where a pawn just made a double move. The capture is only valid on the very next turn.

**Pawn promotion** — pawns that reach the back rank automatically promote to a queen.

**Check filtering** — every generated move is tested against a copy of the board. Moves that leave the king in check are discarded before they're added to the legal move list.

---

## 🛠️ Tech Stack

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![Terminal](https://img.shields.io/badge/Console-App-black?style=for-the-badge)

- **Language:** C++
- **Compiler:** g++ / GCC
- **Libraries:** `<iostream>`, `<vector>`, `<ctime>`

---

## 🚀 How to Run

### Requirements
- g++ compiler installed ([MinGW](https://www.mingw-w64.org/) on Windows, or built-in on Linux/macOS)

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/YOUR_USERNAME/Cpp-Chess-Engine.git

# 2. Navigate into the folder
cd Cpp-Chess-Engine

# 3. Compile the source file
g++ main.cpp -o chess

# 4. Run the game
./chess       # Linux / macOS
chess.exe     # Windows
```

### How to Play

- You play as **White**. The bot plays as **Black**.
- On your turn, enter the **column** (a–h) and **row** (1–8) of the piece you want to move, then the destination square.
- The board is printed after every move, along with a numerical score (positive = White is winning, negative = Black is winning).
- The game ends automatically on checkmate or stalemate.

---

## 🧠 What I Learned

This was my fourth C++ project and the biggest leap in complexity yet. Here's what I took away:

**Minimax & recursion** — understanding how a recursive search tree works was the core challenge of this project. Minimax had to not just find moves, but simulate an entire game tree and correctly alternate between maximizing and minimizing — getting that logic right took real thought and debugging.

**Alpha-Beta pruning** — once the basic Minimax worked, adding pruning was a meaningful step up. Understanding *why* certain branches can be safely skipped, and implementing the alpha and beta bounds correctly, taught me a lot about optimization thinking.

**Representing game state** — the board is a simple `int[8][8]` where positive numbers are white pieces and negative numbers are black pieces. Keeping all the extra state needed for special moves (castling rights, en passant column) in a separate `PieceInformation` struct and passing copies through the search tree cleanly was an important design decision.

**Legal move generation** — generating *pseudo-legal* moves first, then filtering out the ones that leave the king in check, was the cleanest approach I found. Each piece type has its own function, which kept things organized.

**Planning before coding** — the chess ruleset is complicated enough that jumping straight into code would have been a disaster. I thought through all the edge cases — what information do I need to track? what does each function need to know? — before writing a single line. That habit from my previous projects paid off here more than ever.

---

## 🧩 Challenges

**Chess logic** — chess has more edge cases than it looks. Castling alone requires checking six separate conditions per side. En passant only lasts one turn. Pawn promotion happens at a specific row. Getting all of these right and making sure they interacted correctly with the check detection took patience and a lot of debugging.

**Debugging the search** — when the bot made an obviously bad move, it wasn't always clear whether the bug was in move generation, evaluation, or the search itself. Learning to isolate which layer of the engine was misbehaving — and building confidence that each piece was working independently before combining everything — was genuinely difficult.

**Understanding Minimax** — the algorithm is simple to describe but tricky to implement correctly. The recursive calls need to correctly flip between maximizing and minimizing, the base cases need to handle checkmate and stalemate differently (checkmate is not just "no legal moves"), and Alpha-Beta pruning adds another layer of state that needs to be managed carefully across every call.

The project was definitely more time-consuming than my previous ones — but not overwhelmingly hard. More long and methodical than difficult, if that makes sense.

---

## 🔮 Future Plans

- [ ] Add a graphical interface (SFML or SDL2)
- [ ] Add selectable difficulty levels (search depth 1–5)
- [ ] Add move history / notation display (algebraic notation)
- [ ] Add a draw by repetition / fifty-move rule
- [ ] Add underpromotion options (promote to rook, bishop, or knight)
- [ ] Implement iterative deepening for more consistent response times
- [ ] Add an opening book for stronger early play

---

## 📄 License

This project is open source under the [MIT License](LICENSE).

---

*Fourth project. Recursive search trees, a working chess engine, and a bot that can beat my friends. Still learning. ♟️*
