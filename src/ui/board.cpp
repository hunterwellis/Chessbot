#include "board.hpp"
#include <QPainter>
#include <QResizeEvent>
#include <cwchar>
#include <QPixmap>
#include <QDebug>
#include <qcolor.h>

Board::Board(QWidget *parent) : QWidget(parent) {
    initPieces();
    setMinimumSize(BOARD_SIZE, BOARD_SIZE);
}

// draw position
void Board::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    int squareWidth = width() / 8;
    int squareHeight = height() / 8;

    // draw board
    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 8; ++col){
            bool isDark = (row + col) % 2;
            painter.setBrush(isDark ? darkColor : lightColor);
            painter.drawRect(col * squareWidth, row * squareHeight, squareWidth, squareHeight);
        }
    }

    // draw pieces
    for (const auto& piece : pieces){
        int pieceWidth = squareWidth;
        int pieceHeight = squareHeight;

        int x = piece.col * squareWidth + (squareWidth - pieceWidth) / 2;
        int y = piece.row * squareHeight + (squareHeight - pieceHeight) / 2;

        QRect target(x, y, pieceWidth, pieceHeight);
        painter.drawPixmap(target, piece.pixmap.scaled(pieceWidth, pieceHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

// load assets and place peices
void Board::initPieces(){
    int state [64] = { 0 };

    int black_back_rank [8] = { Piece::Black | Piece::Rook, Piece::Black | Piece::Knight, Piece::Black | Piece::Bishop, Piece::Black | Piece::Queen, Piece::Black | Piece::King, Piece::Black | Piece::Bishop, Piece::Black | Piece::Knight , Piece::Black | Piece::Rook };
    int black_pawns [8];
    std::fill(black_pawns, black_pawns + 8, Piece::Black | Piece::Pawn);
    int white_back_rank [8] = { Piece::White | Piece::Rook, Piece::White | Piece::Knight, Piece::White | Piece::Bishop, Piece::White | Piece::Queen, Piece::White | Piece::King, Piece::White | Piece::Bishop, Piece::White | Piece::Knight , Piece::White | Piece::Rook };
    int white_pawns [8];
    std::fill(white_pawns, white_pawns + 8, Piece::White | Piece::Pawn);

    for (int i = 0; i < 8; ++i){
        state[i] = black_back_rank[i];
        state[8 + i] = black_pawns[i];
        state[48 + i] = white_pawns[i];
        state[56 + i] = white_back_rank[i];
    }
    updateBoard(state);
}

void Board::fen(){


}

void Board::updateBoard(int *state){
   for (int i = 0; i < 64; ++i){
        int piece = state[i];
        int type = piece & Piece::PieceMask;
        int color = piece & Piece::ColorMask;

        QString imgPath = "";

        if (color == Piece::Black){
            imgPath += ":/assets/b";
        }
        else if (color == Piece::White){
            imgPath += ":/assets/w";
        }
        else{
            continue;
        }

        switch (type){
            case Piece::Pawn:
                imgPath += "Pawn.png"; break;
            case Piece::Knight:
                imgPath += "Knight.png"; break;
            case Piece::Bishop:
                imgPath += "Bishop.png"; break;
            case Piece::Rook:
                imgPath += "Rook.png"; break;
            case Piece::Queen:
                imgPath += "Queen.png"; break;
            case Piece::King:
                imgPath += "King.png"; break;
        }

        QPixmap pixmap(imgPath);
        if (!pixmap.isNull()) {
            int row = i / 8;
            int col = i % 8;
            pieces.push_back({pixmap, row, col});
        }
   } 
   update();
}
